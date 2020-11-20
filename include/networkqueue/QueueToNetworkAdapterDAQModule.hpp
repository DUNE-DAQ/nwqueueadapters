/**
 * @file QueueToNetworkAdapter.hpp
 *
 * Based on VectorIntIPMSenderDAQModule from IPM
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_QUEUETONETWORKADAPTERDAQMODULE_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_QUEUETONETWORKADAPTERDAQMODULE_HPP_

#include "appfwk/DAQModule.hpp"
#include "appfwk/DAQSource.hpp"
#include "appfwk/ThreadHelper.hpp"
#include "ipm/Sender.hpp"

#include <ers/Issue.h>

#include <future>
#include <memory>
#include <string>
#include <vector>

#include "appfwk/cmd/Nljs.hpp"

#include "networkqueue/Serialization.hpp"
#include "networkqueue/NetworkObjectSender.hpp"

namespace dunedaq {
  
class QueueToNetworkBase
{
public:
  virtual void get() = 0;
};

template<typename MsgType>
class QueueToNetworkImpl : public QueueToNetworkBase
{
public:
  QueueToNetworkImpl(const appfwk::cmd::ModInit& mod_init_data,
                     const dunedaq::networkqueue::nos::Conf& sender_conf)
    : sender_(sender_conf)
  {
    for (const auto& qi : mod_init_data.qinfos) {
      if (qi.name == "input") {
        inputQueue_.reset(new appfwk::DAQSource<MsgType>(qi.inst));
      }
    }
  }
  
  virtual void get()
  {
    MsgType msg;
    inputQueue_->pop(msg, std::chrono::milliseconds(10));
    sender_.send(msg, std::chrono::milliseconds(10));
  }
  
private:
  std::unique_ptr<appfwk::DAQSource<MsgType>> inputQueue_;
  NetworkObjectSender<MsgType> sender_;
};
  
/**
 * @brief QueueToNetworkAdapterDAQModule creates vectors of ints and sends them
 * downstream
 */
class QueueToNetworkAdapterDAQModule : public appfwk::DAQModule
{
public:
  /**
   * @brief QueueToNetworkAdapterDAQModule Constructor
   * @param name Instance name for this QueueToNetworkAdapterDAQModule instance
   */
  explicit QueueToNetworkAdapterDAQModule(const std::string& name);

  QueueToNetworkAdapterDAQModule(const QueueToNetworkAdapterDAQModule&) =
    delete; ///< QueueToNetworkAdapterDAQModule is not copy-constructible
  QueueToNetworkAdapterDAQModule& operator=(const QueueToNetworkAdapterDAQModule&) =
    delete; ///< QueueToNetworkAdapterDAQModule is not copy-assignable
  QueueToNetworkAdapterDAQModule(QueueToNetworkAdapterDAQModule&&) =
    delete; ///< QueueToNetworkAdapterDAQModule is not move-constructible
  QueueToNetworkAdapterDAQModule& operator=(QueueToNetworkAdapterDAQModule&&) =
    delete; ///< QueueToNetworkAdapterDAQModule is not move-assignable

  void init(const data_t& ) override;

private:
  // Commands
  void do_configure(const data_t& );
  void do_start(const data_t& );
  void do_stop(const data_t& );

  // Threading
  void do_work(std::atomic<bool>& running_flag);
  appfwk::ThreadHelper thread_;

  std::unique_ptr<QueueToNetworkBase> impl_;
};

} // namespace dunedaq


#endif // NETWORKQUEUE_INCLUDE_NETWORKQUEUE_QUEUETONETWORKADAPTERDAQMODULE_HPP_
