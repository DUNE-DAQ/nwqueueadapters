/**
 * @file FakeDataConsumer.hpp
 *
 * FakeDataConsumer is a simple DAQModule implementation that receives FakeData objects produced by
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
#include "networkqueue/fakedataconsumer/Nljs.hpp"
#include "networkqueue/fsd/Structs.hpp"

#include <ers/Issue.h>

#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace networkqueue {

class FakeDataConsumer : public dunedaq::appfwk::DAQModule
{
public:
  /**
   * @brief FakeDataConsumer Constructor
   * @param name Instance name for this FakeDataConsumer instance
   */
  explicit FakeDataConsumer(const std::string& name);

  FakeDataConsumer(const FakeDataConsumer&) = delete;            ///< FakeDataConsumer is not copy-constructible
  FakeDataConsumer& operator=(const FakeDataConsumer&) = delete; ///< FakeDataConsumer is not copy-assignable
  FakeDataConsumer(FakeDataConsumer&&) = delete;                 ///< FakeDataConsumer is not move-constructible
  FakeDataConsumer& operator=(FakeDataConsumer&&) = delete;      ///< FakeDataConsumer is not move-assignable

  void init(const nlohmann::json&) override;

private:
  // Commands
  void do_configure(const data_t& data);
  void do_start(const data_t& data);
  void do_stop(const data_t& data);

  // Threading
  void do_work(std::atomic<bool>& running_flag);
  dunedaq::appfwk::ThreadHelper thread_;

  // Configuration (for validation)
  fakedataconsumer::Conf cfg_;

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
