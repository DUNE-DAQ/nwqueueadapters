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

#include <cetlib/BasicPluginFactory.h>
#include <cetlib/compiler_macros.h>

#include <memory>
#include <string>
#include <vector>

#include "appfwk/cmd/Nljs.hpp"

#include "serialization/NetworkObjectReceiver.hpp"
#include "serialization/Serialization.hpp"

#ifndef EXTERN_C_FUNC_DECLARE_START
#define EXTERN_C_FUNC_DECLARE_START                                                                                    \
  extern "C"                                                                                                           \
  {
#endif

/**
 * @brief Declare the function that will be called by the plugin loader
 * @param klass Class for which a NetworkToQueue module will be used
 */
#define DEFINE_DUNE_NETWORK_TO_QUEUE(klass)                                                                            \
  EXTERN_C_FUNC_DECLARE_START                                                                                          \
  std::unique_ptr<dunedaq::NetworkToQueueBase> makeNToQ(                                                               \
    std::string const& plugin_name,                                                                                    \
    const dunedaq::appfwk::cmd::ModInit& mod_init_data,                                                                \
    const dunedaq::serialization::networkobjectreceiver::Conf& receiver_conf)                                          \
  {                                                                                                                    \
    if (plugin_name == #klass)                                                                                         \
      return std::make_unique<dunedaq::NetworkToQueueImpl<klass>>(mod_init_data, receiver_conf);                       \
    else                                                                                                               \
      return nullptr;                                                                                                  \
  }                                                                                                                    \
  }

namespace dunedaq {

class NetworkToQueueBase
{
public:
  virtual void push() = 0;
};

template<typename T>
class NetworkToQueueImpl : public NetworkToQueueBase
{
public:
  NetworkToQueueImpl(const appfwk::cmd::ModInit& mod_init_data,
                     const dunedaq::serialization::networkobjectreceiver::Conf& receiver_conf)
    : receiver_(receiver_conf)
  {
    for (const auto& qi : mod_init_data.qinfos) {
      if (qi.name == "output") {
        outputQueue_.reset(new appfwk::DAQSink<T>(qi.inst));
      }
    }
  }

  virtual void push()
  {
    outputQueue_->push(receiver_.recv(std::chrono::milliseconds(10)), std::chrono::milliseconds(10));
  }

private:
  std::unique_ptr<appfwk::DAQSink<T>> outputQueue_;
  NetworkObjectReceiver<T> receiver_;
};

/**
 * @brief NetworkToQueueAdapterDAQModule connects an incoming IPM
 * receiver to an app framework queue, transparently to users of the
 * queue
 *
 * This class is limited to types that are generated by schema in this
 * package. For reasons of plugin implementation, the type of objects
 * passed on the input queue is not specified as a template parameter,
 * but as the configuration parameter @c msg_type, which must be
 * specified in the init command of the module
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

  void init(const data_t&) override;

private:
  // Commands
  void do_configure(const data_t&);
  void do_start(const data_t&);
  void do_stop(const data_t&);

  // Threading
  appfwk::ThreadHelper thread_;
  void do_work(std::atomic<bool>& running_flag);

  std::unique_ptr<NetworkToQueueBase> impl_;
};

std::unique_ptr<NetworkToQueueBase>
makeNetworkToQueueBase(std::string const& module_name,
                       std::string const& plugin_name,
                       const appfwk::cmd::ModInit& mod_init_data,
                       const dunedaq::serialization::networkobjectreceiver::Conf& sender_conf)
{
  static cet::BasicPluginFactory bpf("duneNetworkQueue", "makeNToQ");
  return bpf.makePlugin<std::unique_ptr<NetworkToQueueBase>>(module_name, plugin_name, mod_init_data, sender_conf);
}

} // namespace dunedaq

#endif // NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKTOQUEUEADAPTERDAQMODULE_HPP_
