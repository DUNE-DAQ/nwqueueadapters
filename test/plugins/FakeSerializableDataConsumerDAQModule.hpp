/**
 * @file FakeSerializableDataConsumerDAQModule.hpp
 *
 * FakeSerializableDataConsumerDAQModule is a simple DAQModule implementation that receives FakeData objects produced by
 * FakeSerializableDataProducerDAQModule. It is intended for testing NetworkToQueueAdapterDAQModule
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef APPFWK_TEST_FAKEDATACONSUMERDAQMODULE_HPP_
#define APPFWK_TEST_FAKEDATACONSUMERDAQMODULE_HPP_

#include "appfwk/DAQModule.hpp"
#include "appfwk/DAQSource.hpp"
#include "appfwk/ThreadHelper.hpp"

// Our command data structures
#include "networkqueue/fakeserializabledataconsumerdaqmodule/Nljs.hpp"
#include "networkqueue/fsd/Structs.hpp"

#include <ers/Issue.h>

#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace networkqueue {

class FakeSerializableDataConsumerDAQModule : public dunedaq::appfwk::DAQModule
{
public:
  /**
   * @brief FakeSerializableDataConsumerDAQModule Constructor
   * @param name Instance name for this FakeSerializableDataConsumerDAQModule instance
   */
  explicit FakeSerializableDataConsumerDAQModule(const std::string& name);

  FakeSerializableDataConsumerDAQModule(const FakeSerializableDataConsumerDAQModule&) =
    delete; ///< FakeSerializableDataConsumerDAQModule is not copy-constructible
  FakeSerializableDataConsumerDAQModule& operator=(const FakeSerializableDataConsumerDAQModule&) =
    delete; ///< FakeSerializableDataConsumerDAQModule is not copy-assignable
  FakeSerializableDataConsumerDAQModule(FakeSerializableDataConsumerDAQModule&&) =
    delete; ///< FakeSerializableDataConsumerDAQModule is not move-constructible
  FakeSerializableDataConsumerDAQModule& operator=(FakeSerializableDataConsumerDAQModule&&) =
    delete; ///< FakeSerializableDataConsumerDAQModule is not move-assignable

  void init(const nlohmann::json& ) override;

private:
  // Commands
  void do_configure(const data_t& data);
  void do_start(const data_t& data);
  void do_stop(const data_t& data);

  // Threading
  void do_work(std::atomic<bool>& running_flag);
  dunedaq::appfwk::ThreadHelper thread_;

  // Configuration (for validation)
  fakeserializabledataconsumerdaqmodule::Conf cfg_;

  std::chrono::milliseconds queueTimeout_;
  std::unique_ptr<dunedaq::appfwk::DAQSource<fsd::FakeData>> inputQueue_;
};

} // namespace appfwk
ERS_DECLARE_ISSUE_BASE(networkqueue,
                       ConsumerProgressUpdate,
                       appfwk::GeneralDAQModuleIssue,
                       message,
                       ((std::string)name),
                       ((std::string)message))
} // namespace dunedaq

#endif // APPFWK_TEST_FAKEDATACONSUMERDAQMODULE_HPP_
