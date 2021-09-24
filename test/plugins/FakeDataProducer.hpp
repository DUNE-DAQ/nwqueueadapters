/**
 * @file FakeDataProducer.hpp
 *
 * FakeDataProducer, based on
 * FakeDataProducerDAQModule from appfwk, pushes
 * nwqueueadapters::fsd::FakeData objects onto a queue. It exists to
 * allow testing of QueueToNetwork, which takes the
 * FakeData items off the queue and serializes them onto the network
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NWQUEUEADAPTERS_TEST_PLUGINS_FAKEDATAPRODUCER_HPP_
#define NWQUEUEADAPTERS_TEST_PLUGINS_FAKEDATAPRODUCER_HPP_

#include "appfwk/DAQModule.hpp"
#include "appfwk/DAQSink.hpp"
#include "toolbox/ThreadHelper.hpp"

// Our command structures.
#include "nwqueueadapters/fakedataproducer/Structs.hpp"
#include "nwqueueadapters/fsd/Structs.hpp"

#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace nwqueueadapters {
class FakeDataProducer : public ::dunedaq::appfwk::DAQModule
{
public:
  /**
   * @brief FakeDataProducer Constructor
   * @param name Instance name for this FakeDataProducer instance
   */
  explicit FakeDataProducer(const std::string& name);

  FakeDataProducer(const FakeDataProducer&) = delete;            ///< FakeDataProducer is not copy-constructible
  FakeDataProducer& operator=(const FakeDataProducer&) = delete; ///< FakeDataProducer is not copy-assignable
  FakeDataProducer(FakeDataProducer&&) = delete;                 ///< FakeDataProducer is not move-constructible
  FakeDataProducer& operator=(FakeDataProducer&&) = delete;      ///< FakeDataProducer is not move-assignable

  void init(const nlohmann::json&) override;

private:
  // Commands
  void do_configure(const data_t& data);
  void do_start(const data_t& data);
  void do_stop(const data_t& data);

  // Threading
  dunedaq::toolbox::ThreadHelper thread_;
  void do_work(std::atomic<bool>& running_flag);

  // Configuration
  std::unique_ptr<dunedaq::appfwk::DAQSink<fsd::FakeData>> outputQueue_;
  std::chrono::milliseconds queueTimeout_;

  fakedataproducer::Conf cfg_;
};

} // namespace nwqueueadapters
ERS_DECLARE_ISSUE_BASE(nwqueueadapters,
                       ProducerProgressUpdate,
                       appfwk::GeneralDAQModuleIssue,
                       message,
                       ((std::string)name),
                       ((std::string)message))
} // namespace dunedaq

#endif // NWQUEUEADAPTERS_TEST_PLUGINS_FAKEDATAPRODUCER_HPP_
