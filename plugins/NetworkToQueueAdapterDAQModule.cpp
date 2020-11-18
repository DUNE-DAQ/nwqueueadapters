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
#include <thread>
#include <utility>
#include <vector>

#include "appfwk/cmd/Nljs.hpp"

#include "networkqueue/nq/Structs.hpp"
#include "networkqueue/nq/Nljs.hpp"

#include "networkqueue/fsd/NToQMaker.hpp"

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
  impl_=NetworkToQueueBaseMaker(msg_type_name, mod_init_data);
  if(impl_.get()==nullptr){
    throw std::runtime_error("No NToQ for requested msg_type");
  }
}

void
NetworkToQueueAdapterDAQModule::do_configure(const data_t& config_data)
{
  auto cfg = config_data.get<networkqueue::nq::NetworkToQueueAdapterConf>();
  input_=dunedaq::ipm::makeIPMReceiver("ZmqReceiver");
  input_->connect_for_receives(cfg.connection_info);
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
    dunedaq::ipm::Receiver::Response recvd;
    try{
      recvd = input_->receive(std::chrono::milliseconds(10));
      ++recv_counter;
    }
    catch(ipm::ReceiveTimeoutExpired&){
      // It's not a problem if the receive times out
      continue;
    }
    // push() might throw if the queue is full. We leave the exception
    // to propagate up so someone else can DTRT
    impl_->push(recvd.data);
  }
  ERS_INFO("Did " << recv_counter << " receives");
}

DEFINE_DUNE_DAQ_MODULE(NetworkToQueueAdapterDAQModule)
} // namespace dunedaq

