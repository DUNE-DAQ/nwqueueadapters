/**
 * @file QueueToNetwork.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/QueueToNetwork.hpp"

#include "appfwk/DAQModuleHelper.hpp"
#include "nwqueueadapters/Issues.hpp"
#include "nwqueueadapters/networkobjectsender/Nljs.hpp"
#include "nwqueueadapters/queuetonetwork/Nljs.hpp"
#include "serialization/Serialization.hpp"

#include <ers/Severity.hpp>
#include <ers/ers.hpp>

#include <chrono>
#include <string>
#include <vector>

namespace dunedaq::nwqueueadapters {

QueueToNetwork::QueueToNetwork(const std::string& name)
  : appfwk::DAQModule(name)
  , m_thread(std::bind(&QueueToNetwork::do_work, this, std::placeholders::_1))
  , m_impl(nullptr)
{

  register_command("conf", &QueueToNetwork::do_configure);
  register_command("scrap", &QueueToNetwork::do_scrap);
}

void
QueueToNetwork::init(const data_t& init_data)
{
  m_queue_instance = appfwk::queue_inst(init_data, "input");
}

void
QueueToNetwork::do_configure(const data_t& config_data)
{
  auto conf = config_data.get<dunedaq::nwqueueadapters::queuetonetwork::Conf>();
  m_message_type_name = conf.msg_type;

  {
    std::lock_guard<std::mutex> _(m_opmon_mutex);
    m_opmon_info.popped_count = 0;
    m_opmon_info.sent_count = 0;
    m_opmon_info.send_failed_count = 0;
  }

  try {
    m_impl = makeQueueToNetworkBase(conf.msg_module_name, m_message_type_name, m_queue_instance, conf.sender_config);
  } catch (NoQueueToNetworkImpl& e) {
    throw CannotConfigure(ERS_HERE, e);
  }
  m_thread.start_working_thread("qton");
}

void
QueueToNetwork::do_scrap(const data_t& /*args*/)
{
  m_thread.stop_working_thread();
  m_impl.reset(nullptr); // call dtor of implementation
}

void
QueueToNetwork::do_work(std::atomic<bool>& running_flag)
{
  while (running_flag.load()) {
    // TODO 2021-05-21 Philip Rodrigues rodriges@fnal.gov: Proper handling of "stop"
    try {
      m_impl->get();
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    } catch (const dunedaq::ipm::SendTimeoutExpired& e) {
      ers::warning(QueueToNetworkSendTimeout(ERS_HERE, m_message_type_name, m_queue_instance, e));
      
      std::lock_guard<std::mutex> _(m_opmon_mutex);
      ++m_opmon_info.popped_count;
      ++m_opmon_info.send_failed_count;

      continue;
    }
    std::lock_guard<std::mutex> _(m_opmon_mutex);
    ++m_opmon_info.popped_count;
    ++m_opmon_info.sent_count;
  }
}

void QueueToNetwork::get_info(opmonlib::InfoCollector& ci, int)
{
  std::lock_guard<std::mutex> _(m_opmon_mutex);
  ci.add(m_opmon_info);
}

DEFINE_DUNE_DAQ_MODULE(QueueToNetwork)
} // namespace dunedaq::nwqueueadapters
