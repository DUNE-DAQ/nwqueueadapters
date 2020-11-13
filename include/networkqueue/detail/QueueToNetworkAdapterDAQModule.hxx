/**
 * 
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <ers/ers.h>

#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "appfwk/cmd/Nljs.hpp"

#include "networkqueue/nq/Structs.hpp"
#include "networkqueue/nq/Nljs.hpp"

namespace dunedaq {

template<class T>
QueueToNetworkAdapterDAQModule<T>::QueueToNetworkAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&QueueToNetworkAdapterDAQModule::do_work, this, std::placeholders::_1))
  , inputQueue_(nullptr)
{

  register_command("conf",      &QueueToNetworkAdapterDAQModule::do_configure);
  register_command("start",     &QueueToNetworkAdapterDAQModule::do_start);
  register_command("stop",      &QueueToNetworkAdapterDAQModule::do_stop);
}

template<class T>
void
QueueToNetworkAdapterDAQModule<T>::init(const data_t& init_data)
{
  auto ini = init_data.get<appfwk::cmd::ModInit>();
  for (const auto& qi : ini.qinfos) {
    if (qi.name == "input") {
      inputQueue_.reset(new appfwk::DAQSource<T>(qi.inst));
    }
  }
}

template<class T>
void
QueueToNetworkAdapterDAQModule<T>::do_configure(const data_t& config_data)
{
  auto cfg = config_data.get<networkqueue::nq::QueueToNetworkAdapterConf>();
  output_=dunedaq::ipm::makeIPMSender("ZmqSender");
  ERS_INFO("Output is " << output_.get());
  output_->connect_for_sends(cfg.connection_info);
}

template<class T>
void
QueueToNetworkAdapterDAQModule<T>::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

template<class T>
void
QueueToNetworkAdapterDAQModule<T>::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

template<class T>
void
QueueToNetworkAdapterDAQModule<T>::do_work(std::atomic<bool>& running_flag)
{
  using json=nlohmann::json;
  
  while (running_flag.load()) {
    T t;
    try {
      inputQueue_->pop(t, std::chrono::milliseconds(10));
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    }

    json j = t;
    json::string_t s=j.dump();
    ERS_DEBUG(0, "Sending " << s);
    try{
      output_->send(s.c_str(), s.size(), std::chrono::milliseconds(100));
    } catch(const dunedaq::ipm::SendTimeoutExpired& e) {
      continue;
    }
     
  }
}

} // namespace dunedaq::ipm


