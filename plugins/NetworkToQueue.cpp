/**
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/NetworkToQueue.hpp"

#include "appfwk/DAQModuleHelper.hpp"
#include "logging/Logging.hpp"
#include "nwqueueadapters/networkobjectreceiver/Nljs.hpp"
#include "nwqueueadapters/networktoqueue/Nljs.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace dunedaq::nwqueueadapters {

NetworkToQueue::NetworkToQueue(const std::string& name)
  : appfwk::DAQModule(name)
  , m_thread(std::bind(&NetworkToQueue::do_work, this, std::placeholders::_1))
  , m_is_subscriber_type(false)
  , m_impl(nullptr)
{
  register_command("conf", &NetworkToQueue::do_configure);
  register_command("scrap", &NetworkToQueue::do_scrap);
}

void
NetworkToQueue::init(const data_t& init_data)
{
  m_queue_instance = appfwk::queue_inst(init_data, "output");
}

void
NetworkToQueue::do_configure(const data_t& config_data)
{
  auto conf = config_data.get<dunedaq::nwqueueadapters::networktoqueue::Conf>();
  m_message_type_name = conf.msg_type;
  // Hacky string comparison to determine whether the receiver is a
  // "subscriber" type, so dropping messages is OK
  m_is_subscriber_type = networkmanager::NetworkManager::get().is_subscriber(conf.receiver_config.name);

  {
    std::lock_guard<std::mutex> _(m_opmon_mutex);
    m_opmon_info.received_count = 0;
    m_opmon_info.pushed_count = 0;
    m_opmon_info.push_failed_count = 0;
  }

  try {
    m_impl = makeNetworkToQueueBase(conf.msg_module_name, conf.msg_type, m_queue_instance, conf.receiver_config);
  } catch (NoNetworkToQueueImpl& e) {
    throw CannotConfigure(ERS_HERE, e);
  }
  m_thread.start_working_thread("ntoq");
}

void
NetworkToQueue::do_scrap(const data_t& /*args*/)
{
  m_thread.stop_working_thread();
  m_impl.reset(nullptr); // call dtor of implementation
}

void
NetworkToQueue::do_work(std::atomic<bool>& running_flag)
{
  static int recv_counter = 0;
  while (running_flag.load()) {
    try {
      m_impl->push();
      ++recv_counter;
    } catch (ipm::ReceiveTimeoutExpired&) {
      // It's not a problem if the receive times out
      continue;
    } catch (const dunedaq::appfwk::QueueTimeoutExpired& e) {

      auto issue = NetworkToQueuePushTimeout(ERS_HERE, m_message_type_name, m_queue_instance, e);
      // If we're connected to a "subscriber" type socket, it's ~OK to drop
      // messages
      if (m_is_subscriber_type) {
        TLOG_DEBUG(1) << get_name() << ": Push of message type " << m_message_type_name << " to queue "
                      << m_queue_instance
                      << " timed out. Ignoring because incoming socket is "
                         "subscriber type";
      } else {
        ers::warning(issue);
      }

      std::lock_guard<std::mutex> _(m_opmon_mutex);
      ++m_opmon_info.received_count;
      ++m_opmon_info.push_failed_count;

      continue;
    }
    std::lock_guard<std::mutex> _(m_opmon_mutex);
    ++m_opmon_info.received_count;
    ++m_opmon_info.pushed_count;

  }
  TLOG() << "Did " << recv_counter << " receives";
}

void NetworkToQueue::get_info(opmonlib::InfoCollector& ci, int)
{
  std::lock_guard<std::mutex> _(m_opmon_mutex);
  ci.add(m_opmon_info);
}

DEFINE_DUNE_DAQ_MODULE(NetworkToQueue)
} // namespace dunedaq::nwqueueadapters
