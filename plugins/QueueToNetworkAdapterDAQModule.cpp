/**
 * 
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "networkqueue/QueueToNetworkAdapterDAQModule.hpp"

#include <ers/ers.h>

#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "networkqueue/Serialization.hpp"
#include "networkqueue/nq/Structs.hpp"
#include "networkqueue/nq/Nljs.hpp"

#include "networkqueue/fsd/QToNMaker.hpp"

namespace dunedaq {

QueueToNetworkAdapterDAQModule::QueueToNetworkAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&QueueToNetworkAdapterDAQModule::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{

  register_command("conf",      &QueueToNetworkAdapterDAQModule::do_configure);
  register_command("start",     &QueueToNetworkAdapterDAQModule::do_start);
  register_command("stop",      &QueueToNetworkAdapterDAQModule::do_stop);
}

void
QueueToNetworkAdapterDAQModule::init(const data_t& init_data)
{
  auto mod_init_data=init_data.get<appfwk::cmd::ModInit>();
  // TODO: Should get these values safely via codegen/schema
  std::string msg_type_name=init_data.at("msg_type");
  serialization::SerializationType stype=serialization::fromString(init_data.value("serialization_type", "json"));
  impl_=QueueToNetworkBaseMaker(msg_type_name, stype, mod_init_data);
  if(impl_.get()==nullptr){
    throw std::runtime_error("No QToN for requested msg_type");
  }
}

void
QueueToNetworkAdapterDAQModule::do_configure(const data_t& config_data)
{
  auto cfg = config_data.get<networkqueue::nq::QueueToNetworkAdapterConf>();
  output_=dunedaq::ipm::makeIPMSender("ZmqSender");
  ERS_INFO("Output is " << output_.get());
  output_->connect_for_sends(cfg.connection_info);
}

void
QueueToNetworkAdapterDAQModule::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

void
QueueToNetworkAdapterDAQModule::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

void
QueueToNetworkAdapterDAQModule::do_work(std::atomic<bool>& running_flag)
{
  while (running_flag.load()) {
    // TODO: Proper handling of "stop"
    std::vector<uint8_t> s;
    try{
      s=impl_->get();
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    }

    try{
      output_->send(s.data(), s.size(), std::chrono::milliseconds(100));
    } catch(const dunedaq::ipm::SendTimeoutExpired& e) {
      continue;
    }
  }
}

DEFINE_DUNE_DAQ_MODULE(QueueToNetworkAdapterDAQModule)
} // namespace dunedaq::ipm


