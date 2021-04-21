/**
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/QueueToNetwork.hpp"

#include <chrono>
#include <ers/Severity.hpp>
#include <ers/ers.hpp>
#include <string>
#include <vector>

#include "appfwk/DAQModuleHelper.hpp"

#include "nwqueueadapters/Issues.hpp"
#include "serialization/Serialization.hpp"

#include "nwqueueadapters/networkobjectsender/Nljs.hpp"
#include "nwqueueadapters/queuetonetwork/Nljs.hpp"

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
  m_queue_instance = dunedaq::appfwk::queue_index(init_data, {"input"})["input"].inst;
}

void
QueueToNetwork::do_configure(const data_t& config_data)
{
  auto conf = config_data.get<dunedaq::nwqueueadapters::queuetonetwork::Conf>();
  m_message_type_name=conf.msg_type;

  try{
    m_impl = makeQueueToNetworkBase(conf.msg_module_name, m_message_type_name, m_queue_instance, conf.sender_config);
  } catch(NoQueueToNetworkImpl& e) {
    throw CannotConfigure(ERS_HERE, e);
  }
  m_thread.start_working_thread();
}

void
QueueToNetwork::do_scrap(const data_t& /*args*/)
{
  m_thread.stop_working_thread();
}

void
QueueToNetwork::do_work(std::atomic<bool>& running_flag)
{
  while (running_flag.load()) {
    // TODO: Proper handling of "stop"
    try {
      m_impl->get();
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    } catch (const dunedaq::ipm::SendTimeoutExpired& e) {
      ers::warning(QueueToNetworkSendTimeout(ERS_HERE, m_message_type_name, m_queue_instance, e));
      continue;
    }
  }
}

DEFINE_DUNE_DAQ_MODULE(QueueToNetwork)
} // namespace dunedaq::nwqueueadapters
