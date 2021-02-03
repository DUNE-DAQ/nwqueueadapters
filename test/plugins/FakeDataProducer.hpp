/**
 * @file FakeDataProducer.hpp
 *
 * FakeDataProducer, based on
 * FakeDataProducerDAQModule from appfwk, pushes
 * networkqueue::fsd::FakeData objects onto a queue. It exists to
 * allow testing of QueueToNetworkAdapterDAQModule, which takes the
 * FakeData items off the queue and serializes them onto the network
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
#include "networkqueue/fakedataproducer/Structs.hpp"
#include "networkqueue/fsd/Structs.hpp"

#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace networkqueue {
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
  dunedaq::appfwk::ThreadHelper thread_;
  void do_work(std::atomic<bool>& running_flag);

  // Configuration
  std::unique_ptr<dunedaq::appfwk::DAQSink<fsd::FakeData>> outputQueue_;
  std::chrono::milliseconds queueTimeout_;

  fakedataproducer::Conf cfg_;
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
