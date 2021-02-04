/**
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "networkqueue/NetworkToQueueAdapterDAQModule.hpp"

#include <chrono>
#include <string>
#include <vector>

#include "appfwk/DAQModuleHelper.hpp"

#include "serialization/networkobjectreceiver/Nljs.hpp"
#include "networkqueue/networktoqueueadapterdaqmodule/Nljs.hpp"

namespace dunedaq {

NetworkToQueueAdapterDAQModule::NetworkToQueueAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&NetworkToQueueAdapterDAQModule::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{
  register_command("conf", &NetworkToQueueAdapterDAQModule::do_configure);
  register_command("start", &NetworkToQueueAdapterDAQModule::do_start);
  register_command("stop", &NetworkToQueueAdapterDAQModule::do_stop);
}

void
NetworkToQueueAdapterDAQModule::init(const data_t& init_data)
{
  queue_instance_ = dunedaq::appfwk::queue_index(init_data, {"output"})["output"].inst;
}

void
NetworkToQueueAdapterDAQModule::do_configure(const data_t& config_data)
{
  auto conf = config_data.get<dunedaq::networkqueue::networktoqueueadapterdaqmodule::Conf>();
  auto receiver_conf = conf.receiver_config.get<dunedaq::serialization::networkobjectreceiver::Conf>();

  impl_ = makeNetworkToQueueBase(conf.msg_module_name, conf.msg_type, queue_instance_, receiver_conf);
  if (impl_.get() == nullptr) {
    throw std::runtime_error("No NToQ for requested msg_type");
  }
}

void
NetworkToQueueAdapterDAQModule::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

void
NetworkToQueueAdapterDAQModule::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

void
NetworkToQueueAdapterDAQModule::do_work(std::atomic<bool>& running_flag)
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

DEFINE_DUNE_DAQ_MODULE(NetworkToQueueAdapterDAQModule)
} // namespace dunedaq
