/**
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/NetworkToQueue.hpp"

#include <chrono>
#include <string>
#include <vector>

#include "appfwk/DAQModuleHelper.hpp"

#include "serialization/networkobjectreceiver/Nljs.hpp"
#include "nwqueueadapters/networktoqueue/Nljs.hpp"

namespace dunedaq::nwqueueadapters {

NetworkToQueue::NetworkToQueue(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&NetworkToQueue::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{
  register_command("conf", &NetworkToQueue::do_configure);
  register_command("start", &NetworkToQueue::do_start);
  register_command("stop", &NetworkToQueue::do_stop);
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
  auto receiver_conf = conf.receiver_config.get<dunedaq::serialization::networkobjectreceiver::Conf>();

  impl_ = makeNetworkToQueueBase(conf.msg_module_name, conf.msg_type, queue_instance_, receiver_conf);
  if (impl_.get() == nullptr) {
    throw std::runtime_error("No NToQ for requested msg_type");
  }
}

void
NetworkToQueue::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

void
NetworkToQueue::do_stop(const data_t& /*args*/)
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
    }
  }
  ERS_INFO("Did " << recv_counter << " receives");
}

DEFINE_DUNE_DAQ_MODULE(NetworkToQueue)
} // namespace dunedaq::nwqueueadapters
