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
  , thread_(std::bind(&NetworkToQueue::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{
  register_command("conf", &NetworkToQueue::do_configure);
  register_command("scrap", &NetworkToQueue::do_scrap);
}

void
NetworkToQueue::init(const data_t& init_data)
{
  queue_instance_ = dunedaq::appfwk::queue_index(init_data, {"output"})["output"].inst;
}

void
NetworkToQueue::do_configure(const data_t& config_data)
{
  auto conf = config_data.get<dunedaq::nwqueueadapters::networktoqueue::Conf>();
  auto receiver_conf = conf.receiver_config.get<dunedaq::nwqueueadapters::networkobjectreceiver::Conf>();
  message_type_name_=conf.msg_type;

  impl_ = makeNetworkToQueueBase(conf.msg_module_name, conf.msg_type, queue_instance_, receiver_conf);
  if (impl_.get() == nullptr) {
    throw std::runtime_error("No NToQ for requested msg_type");
  }
  thread_.start_working_thread();
}

void
NetworkToQueue::do_scrap(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

void
NetworkToQueue::do_work(std::atomic<bool>& running_flag)
{
  static int recv_counter = 0;
  while (running_flag.load()) {
    try {
      impl_->push();
      ++recv_counter;
    } catch (ipm::ReceiveTimeoutExpired&) {
      // It's not a problem if the receive times out
      continue;
    } catch (const dunedaq::appfwk::QueueTimeoutExpired& e) {
      ers::warning(NetworkToQueuePushTimeout(ERS_HERE, message_type_name_, queue_instance_, e));
      continue;
    }
  }
  TLOG() <<"Did " << recv_counter << " receives";
}

DEFINE_DUNE_DAQ_MODULE(NetworkToQueue)
} // namespace dunedaq::nwqueueadapters
