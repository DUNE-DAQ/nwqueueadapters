/**
 * @file FakeSerializableDataConsumerDAQModule.hpp
 *
 * FakeSerializableDataConsumerDAQModule is a simple DAQModule implementation that receives vectors of integers produced by
 * FakeSerializableDataProducerDAQModule and checks that they are consistent.
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
#include "networkqueue/fsdc/Structs.hpp"
#include "networkqueue/fsd/Structs.hpp"

#include <ers/Issue.h>

#include <future>
#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace networkqueue {
/**
 * @brief FakeSerializableDataConsumerDAQModule creates vectors of ints and sends them
 * downstream
 */
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
  fsdc::Conf cfg_;
  // size_t nIntsPerVector_ = 999;
  // int starting_int_ = -999;
  // int ending_int_ = -999;
  std::chrono::milliseconds queueTimeout_;
  std::unique_ptr<dunedaq::appfwk::DAQSource<fsd::FakeData>> inputQueue_;
};

} // namespace appfwk
ERS_DECLARE_ISSUE_BASE(networkqueue,
                       ConsumerErrorDetected,
                       appfwk::GeneralDAQModuleIssue,

                       "Error in received vector " << counter << " at position " << position << ": Expected "
                                                   << expected << ", received " << received,
                       ((std::string)name),
                       ((int)counter)((int)position)((int)expected)((int)received))
ERS_DECLARE_ISSUE_BASE(networkqueue,
                       ConsumerProgressUpdate,
                       appfwk::GeneralDAQModuleIssue,
                       message,
                       ((std::string)name),
                       ((std::string)message))
} // namespace dunedaq

#endif // APPFWK_TEST_FAKEDATACONSUMERDAQMODULE_HPP_
