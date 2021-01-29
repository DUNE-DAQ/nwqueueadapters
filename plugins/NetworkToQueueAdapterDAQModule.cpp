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

#include "appfwk/cmd/Nljs.hpp"

// #include "networkqueue/fsd/NToQMaker.hpp"

namespace dunedaq {

NetworkToQueueAdapterDAQModule::NetworkToQueueAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&NetworkToQueueAdapterDAQModule::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{
  register_command("conf",      &NetworkToQueueAdapterDAQModule::do_configure);
  register_command("start",     &NetworkToQueueAdapterDAQModule::do_start);
  register_command("stop",      &NetworkToQueueAdapterDAQModule::do_stop);
}

void
NetworkToQueueAdapterDAQModule::init(const data_t& init_data)
{
  auto mod_init_data=init_data.get<appfwk::cmd::ModInit>();
  std::string msg_type_name=init_data.at("msg_type");
  auto receiver_conf=init_data.at("receiver_config").get<dunedaq::networkqueue::nor::Conf>();

  impl_=NetworkToQueueBaseMaker(msg_type_name, mod_init_data, receiver_conf);
  if(impl_.get()==nullptr){
    throw std::runtime_error("No NToQ for requested msg_type");
  }
}

void
NetworkToQueueAdapterDAQModule::do_configure(const data_t& /* config_data */)
{
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
  static int recv_counter=0;
  while (running_flag.load()) {
    try{
      impl_->push();
      ++recv_counter;
    }
    catch(ipm::ReceiveTimeoutExpired&){
      // It's not a problem if the receive times out
      continue;
    }
  }
  ERS_INFO("Did " << recv_counter << " receives");
}

DEFINE_DUNE_DAQ_MODULE(NetworkToQueueAdapterDAQModule)
} // namespace dunedaq

