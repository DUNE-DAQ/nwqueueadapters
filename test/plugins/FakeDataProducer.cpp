/**
 * @file FakeDataProducer.cc FakeDataProducer class
 * implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "FakeDataProducer.hpp"

#include "nwqueueadapters/fakedataproducer/Nljs.hpp"
#include "nwqueueadapters/fsd/MsgP.hpp"
#include "nwqueueadapters/fsd/Nljs.hpp"

#include "appfwk/cmd/Nljs.hpp"
#include "appfwk/DAQModuleHelper.hpp"
#include "logging/Logging.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace dunedaq {
namespace nwqueueadapters {

FakeDataProducer::FakeDataProducer(const std::string& name)
  : DAQModule(name)
  , thread_(std::bind(&FakeDataProducer::do_work, this, std::placeholders::_1))
  , outputQueue_(nullptr)
  , queueTimeout_(100)
{
  register_command("conf", &FakeDataProducer::do_configure);
  register_command("start", &FakeDataProducer::do_start);
  register_command("stop", &FakeDataProducer::do_stop);
}

void
FakeDataProducer::init(const nlohmann::json& init_data)
{
  auto qindex=appfwk::queue_index(init_data, {"output"});
  outputQueue_.reset(new dunedaq::appfwk::DAQSink<fsd::FakeData>(qindex["output"].inst));
}

void
FakeDataProducer::do_configure(const data_t& data)
{
  cfg_ = data.get<fakedataproducer::Conf>();

  queueTimeout_ = std::chrono::milliseconds(cfg_.queue_timeout_ms);
}

void
FakeDataProducer::do_start(const data_t& /*data*/)
{
  thread_.start_working_thread();
}

void
FakeDataProducer::do_stop(const data_t& /*data*/)
{
  thread_.stop_working_thread();
}

void
FakeDataProducer::do_work(std::atomic<bool>& running_flag)
{
  TLOG() << "FDP: do_work";
  int current_int = cfg_.starting_int;
  size_t counter = 0;
  std::ostringstream oss;

  while (running_flag.load()) {
    TLOG_DEBUG(1) << get_name() << ": Creating output vector";
    fsd::FakeData output{ current_int++ }; // NOLINT
    oss << "Produced vector " << counter << " with contents " << current_int;
    ers::debug(ProducerProgressUpdate(ERS_HERE, get_name(), oss.str()));
    oss.str("");

    TLOG_DEBUG(1) << get_name() << ": Pushing vector into outputQueue";
    TLOG() << "FDP \"" << get_name() << "\" push " << counter;
    try {
      outputQueue_->push(std::move(output), queueTimeout_);
    } catch (const dunedaq::appfwk::QueueTimeoutExpired& ex) {
      ers::warning(ex);
    }

    TLOG_DEBUG(1) << get_name() << ": Start of sleep between sends";
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg_.wait_between_sends_ms));
    TLOG_DEBUG(1) << get_name() << ": End of do_work loop";
    counter++;
  }
}

} // namespace nwqueueadapters
} // namespace dunedaq

DEFINE_DUNE_DAQ_MODULE(dunedaq::nwqueueadapters::FakeDataProducer)

// Local Variables:
// c-basic-offset: 2
// End:
