/**
 * 
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <chrono>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "appfwk/cmd/Nljs.hpp"

#include "networkqueue/nq/Structs.hpp"
#include "networkqueue/nq/Nljs.hpp"

namespace dunedaq {

template<class T>
NetworkToQueueAdapterDAQModule<T>::NetworkToQueueAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&NetworkToQueueAdapterDAQModule::do_work, this, std::placeholders::_1))
  , outputQueue_(nullptr)
{
  register_command("conf",      &NetworkToQueueAdapterDAQModule::do_configure);
  register_command("start",     &NetworkToQueueAdapterDAQModule::do_start);
  register_command("stop",      &NetworkToQueueAdapterDAQModule::do_stop);
}

template<class T>
void
NetworkToQueueAdapterDAQModule<T>::init(const data_t& init_data)
{
  auto ini = init_data.get<appfwk::cmd::ModInit>();
  for (const auto& qi : ini.qinfos) {
    if (qi.name == "output") {
      outputQueue_.reset(new appfwk::DAQSink<T>(qi.inst));
    }
  }
}

template<class T>
void
NetworkToQueueAdapterDAQModule<T>::do_configure(const data_t& config_data)
{
  auto cfg = config_data.get<networkqueue::nq::NetworkToQueueAdapterConf>();
  input_=dunedaq::ipm::makeIPMReceiver("ZmqReceiver");
  input_->connect_for_receives(cfg.connection_info);
}

template<class T>
void
NetworkToQueueAdapterDAQModule<T>::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

template<class T>
void
NetworkToQueueAdapterDAQModule<T>::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

template<class T>
void
NetworkToQueueAdapterDAQModule<T>::do_work(std::atomic<bool>& running_flag)
{
  static int recv_counter=0;
  using json=nlohmann::json;
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
    std::string d(&recvd.data[0], recvd.data.size());
    ERS_DEBUG(0, "Received data of size " << recvd.data.size() << " with value: \"" << d << "\"");
    json j=json::parse(recvd.data);
    ERS_DEBUG(0, "Got " << j.dump());
    T t=j.get<T>();

    // push() might throw if the queue is full. We leave the exception
    // to propagate up so someone else can DTRT
    outputQueue_->push(std::move(t), std::chrono::milliseconds(10));
  }
  ERS_INFO("Did " << recv_counter << " receives");
}

} // namespace dunedaq

// This will be done in the codegen'ed instantiation that knows the type T
//DEFINE_DUNE_DAQ_MODULE(dunedaq::ipm::NetworkToQueueAdapterDAQModule)
