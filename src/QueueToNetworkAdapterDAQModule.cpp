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

#include "networkqueue/nq/Structs.hpp"
#include "networkqueue/nq/Nljs.hpp"

// TODO: Fix paths
#include "/home/rodrigues/dune/daq/appfwk-buildtools-nov-2020-release/testrel/sourcecode/networkqueue/test/src/networkqueue/fsd/Structs.hpp"
#include "/home/rodrigues/dune/daq/appfwk-buildtools-nov-2020-release/testrel/sourcecode/networkqueue/test/src/networkqueue/fsd/Nljs.hpp"


namespace dunedaq {

QueueToNetworkAdapterDAQModule::QueueToNetworkAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&QueueToNetworkAdapterDAQModule::do_work, this, std::placeholders::_1))
  , inputQueue_(nullptr)
{

  register_command("conf",      &QueueToNetworkAdapterDAQModule::do_configure);
  register_command("start",     &QueueToNetworkAdapterDAQModule::do_start);
  register_command("stop",      &QueueToNetworkAdapterDAQModule::do_stop);
}

void
QueueToNetworkAdapterDAQModule::init(const data_t& init_data)
{
  auto mod_init_data=init_data.get<appfwk::cmd::ModInit>();
  // TODO: This line to be replaced with fancy codegen bit
  impl_.reset(new QueueToNetworkImpl<dunedaq::networkqueue::fsd::FakeData>());
  impl_->init(mod_init_data);
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
  using json=nlohmann::json;
  
  while (running_flag.load()) {
    // TODO: Proper handling of "stop"
    try{
      json::string_t s=impl_->get();
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    }
    ERS_DEBUG(0, "Sending " << s);
    try{
      output_->send(s.c_str(), s.size(), std::chrono::milliseconds(100));
    } catch(const dunedaq::ipm::SendTimeoutExpired& e) {
      continue;
    }
     
  }
}

} // namespace dunedaq::ipm


