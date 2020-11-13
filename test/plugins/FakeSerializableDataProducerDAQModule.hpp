/**
 * @file FakeSerializableDataProducerDAQModule.hpp
 *
 * FakeSerializableDataProducerDAQModule creates vectors of integers of a given length, starting with the given start integer and
 * counting up to the given ending integer. Its current position is persisted between generated vectors, so if the
 * parameters are chosen correctly, the generated vectors will "walk" through the valid range.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef APPFWK_TEST_FAKEDATAPRODUCERDAQMODULE_HPP_
#define APPFWK_TEST_FAKEDATAPRODUCERDAQMODULE_HPP_

#include "appfwk/DAQModule.hpp"
#include "appfwk/DAQSink.hpp"
#include "appfwk/ThreadHelper.hpp"

// Our command structures.  
#include "networkqueue/fsdp/Structs.hpp"
#include "networkqueue/fsd/Structs.hpp"

#include <future>
#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace networkqueue {
/**
 * @brief FakeSerializableDataProducerDAQModule creates vectors of ints and sends them
 * downstream
 */
class FakeSerializableDataProducerDAQModule : public ::dunedaq::appfwk::DAQModule
{
public:
  /**
   * @brief FakeSerializableDataProducerDAQModule Constructor
   * @param name Instance name for this FakeSerializableDataProducerDAQModule instance
   */
  explicit FakeSerializableDataProducerDAQModule(const std::string& name);

  FakeSerializableDataProducerDAQModule(const FakeSerializableDataProducerDAQModule&) =
    delete; ///< FakeSerializableDataProducerDAQModule is not copy-constructible
  FakeSerializableDataProducerDAQModule& operator=(const FakeSerializableDataProducerDAQModule&) =
    delete; ///< FakeSerializableDataProducerDAQModule is not copy-assignable
  FakeSerializableDataProducerDAQModule(FakeSerializableDataProducerDAQModule&&) =
    delete; ///< FakeSerializableDataProducerDAQModule is not move-constructible
  FakeSerializableDataProducerDAQModule& operator=(FakeSerializableDataProducerDAQModule&&) =
    delete; ///< FakeSerializableDataProducerDAQModule is not move-assignable

  void init(const nlohmann::json& ) override;

private:
  // Commands
  void do_configure(const data_t& data);
  void do_start(const data_t& data);
  void do_stop(const data_t& data);

  // Threading
  dunedaq::appfwk::ThreadHelper thread_;
  void do_work(std::atomic<bool>& running_flag);

  // Configuration
  std::unique_ptr<dunedaq::appfwk::DAQSink<fsd::FakeData>> outputQueue_;
  std::chrono::milliseconds queueTimeout_;

  fsdp::Conf cfg_;

  // size_t nIntsPerVector_ = 999;
  // int starting_int_ = -999;
  // int ending_int_ = -999;
  // size_t wait_between_sends_ms_ = 999;
};

} // namespace networkqueue
ERS_DECLARE_ISSUE_BASE(networkqueue,
                       ProducerProgressUpdate,
                       appfwk::GeneralDAQModuleIssue,
                       message,
                       ((std::string)name),
                       ((std::string)message))
} // namespace dunedaq

#endif // APPFWK_TEST_FAKEDATAPRODUCERDAQMODULE_HPP_
