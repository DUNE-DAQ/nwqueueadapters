/**
 * @file VectorIntIPMReceiverDAQModule.hpp
 *
 * VectorIntIPMReceiverDAQModule receives vectors of integers from VectorIntIPMSenderDAQModule
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKTOQUEUEADAPTERDAQMODULE_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKTOQUEUEADAPTERDAQMODULE_HPP_

#include "appfwk/DAQModule.hpp"
#include "appfwk/DAQSink.hpp"
#include "appfwk/ThreadHelper.hpp"
#include "ipm/Receiver.hpp"

#include <future>
#include <memory>
#include <string>
#include <vector>

#include "appfwk/cmd/Nljs.hpp"

namespace dunedaq {

class NetworkToQueueBase
{
public:
  virtual void push(const std::vector<char>& msg) = 0;
};

template<typename T>
class NetworkToQueueImpl : public NetworkToQueueBase
{
public:
  NetworkToQueueImpl(const appfwk::cmd::ModInit& mod_init_data)
  {
    for (const auto& qi : mod_init_data.qinfos) {
      if (qi.name == "output") {
        outputQueue_.reset(new appfwk::DAQSink<T>(qi.inst));
      }
    }
  }
  
  virtual void push(const std::vector<char>& msg)
  {
    using json=nlohmann::json;
    json j=json::parse(msg);
    ERS_DEBUG(0, "Got " << j.dump());
    T t=j.get<T>();
    outputQueue_->push(t, std::chrono::milliseconds(10));
  }
  
private:
  std::unique_ptr<appfwk::DAQSink<T>> outputQueue_;
};

/**
 * @brief NetworkToQueueAdapterDAQModule creates vectors of ints and sends them
 * downstream
 */
class NetworkToQueueAdapterDAQModule : public appfwk::DAQModule
{
public:
  /**
   * @brief NetworkToQueueAdapterDAQModule Constructor
   * @param name Instance name for this NetworkToQueueAdapterDAQModule instance
   */
  explicit NetworkToQueueAdapterDAQModule(const std::string& name);

  NetworkToQueueAdapterDAQModule(const NetworkToQueueAdapterDAQModule&) =
    delete; ///< NetworkToQueueAdapterDAQModule is not copy-constructible
  NetworkToQueueAdapterDAQModule& operator=(const NetworkToQueueAdapterDAQModule&) =
    delete; ///< NetworkToQueueAdapterDAQModule is not copy-assignable
  NetworkToQueueAdapterDAQModule(NetworkToQueueAdapterDAQModule&&) =
    delete; ///< NetworkToQueueAdapterDAQModule is not move-constructible
  NetworkToQueueAdapterDAQModule& operator=(NetworkToQueueAdapterDAQModule&&) =
    delete; ///< NetworkToQueueAdapterDAQModule is not move-assignable

  void init(const data_t& ) override;

private:
  // Commands
  void do_configure(const data_t& );
  void do_start(const data_t& );
  void do_stop(const data_t& );

  // Threading
  appfwk::ThreadHelper thread_;
  void do_work(std::atomic<bool>& running_flag);

  // Configuration
  std::shared_ptr<ipm::Receiver> input_;
  std::unique_ptr<NetworkToQueueBase> impl_;

};

} // namespace dunedaq

#endif // NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKTOQUEUEADAPTERDAQMODULE_HPP_
