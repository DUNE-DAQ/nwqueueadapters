/**
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/NetworkToQueue.hpp"

#include "nwqueueadapters/networkobjectreceiver/Nljs.hpp"
#include "nwqueueadapters/networktoqueue/Nljs.hpp"

#include "appfwk/DAQModuleHelper.hpp"
#include "logging/Logging.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace dunedaq::nwqueueadapters {

NetworkToQueue::NetworkToQueue(const std::string& name)
  : appfwk::DAQModule(name)
  , m_thread(std::bind(&NetworkToQueue::do_work, this, std::placeholders::_1))
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

  try {
    m_impl = makeNetworkToQueueBase(conf.msg_module_name, conf.msg_type, m_queue_instance, conf.receiver_config);
  } catch (NoNetworkToQueueImpl& e) {
    throw CannotConfigure(ERS_HERE, e);
  }
  m_thread.start_working_thread();
}

void
NetworkToQueue::do_scrap(const data_t& /*args*/)
{
  m_thread.stop_working_thread();
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
      ers::warning(NetworkToQueuePushTimeout(ERS_HERE, m_message_type_name, m_queue_instance, e));
      continue;
    }
  }
  TLOG() << "Did " << recv_counter << " receives";
}

DEFINE_DUNE_DAQ_MODULE(NetworkToQueue)
} // namespace dunedaq::nwqueueadapters
