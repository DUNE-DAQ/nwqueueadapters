/**
 * @file FakeDataConsumer.cxx FakeDataConsumer class
 * implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "FakeDataConsumer.hpp"

#include "appfwk/cmd/Nljs.hpp"
#include "appfwk/DAQModuleHelper.hpp"

#include "nwqueueadapters/fakedataconsumer/Nljs.hpp"
#include "nwqueueadapters/fsd/MsgP.hpp"
#include "nwqueueadapters/fsd/Nljs.hpp"

#include <logging/Logging.hpp>

/**
 * @brief Name used by TRACE TLOG calls from this source file
 */
#define TRACE_NAME "FakeDataConsumer" // NOLINT

#include <chrono>
#include <string>
#include <vector>

namespace dunedaq::nwqueueadapters {

FakeDataConsumer::FakeDataConsumer(const std::string& name)
  : DAQModule(name)
  , thread_(std::bind(&FakeDataConsumer::do_work, this, std::placeholders::_1))
  , inputQueue_(nullptr)
{
  register_command("conf", &FakeDataConsumer::do_configure);
  register_command("start", &FakeDataConsumer::do_start);
  register_command("stop", &FakeDataConsumer::do_stop);
}

void
FakeDataConsumer::init(const nlohmann::json& init_data)
{
  inputQueue_.reset(new dunedaq::appfwk::DAQSource<fsd::FakeData>(appfwk::queue_inst(init_data, "input")));
}

void
FakeDataConsumer::do_configure(const data_t& data)
{
  cfg_ = data.get<fakedataconsumer::Conf>();

  queueTimeout_ = std::chrono::milliseconds(cfg_.queue_timeout_ms);
}

void
FakeDataConsumer::do_start(const data_t& /*data*/)
{
  thread_.start_working_thread();
}

void
FakeDataConsumer::do_stop(const data_t& /*data*/)
{
  thread_.stop_working_thread();
}

void
FakeDataConsumer::do_work(std::atomic<bool>& running_flag)
{
  TLOG() << "FDC: do_work";
  int counter = 0;
  int fail_count = 0;
  int timeout_count = 0;
  fsd::FakeData fake_data;
  int prev_fake_count = -1;
  std::ostringstream oss;

  while (running_flag.load()) {
    try {
      inputQueue_->pop(fake_data, queueTimeout_);
    } catch (const dunedaq::appfwk::QueueTimeoutExpired& excpt) {
      ++timeout_count;
      continue;
    }

    if (prev_fake_count != -1 && (fake_data.fake_count != prev_fake_count + 1)) {
      TLOG() << "Got fake_count " << fake_data.fake_count << " when expecting " << (prev_fake_count + 1);
      ++fail_count;
    }
    counter++;
  }

  oss << ": Processed " << counter << " objects with " << fail_count << " failures and " << timeout_count
      << " timeouts";
  ers::info(ConsumerProgressUpdate(ERS_HERE, get_name(), oss.str()));
}

} // namespace dunedaq::nwqueueadapters

DEFINE_DUNE_DAQ_MODULE(dunedaq::nwqueueadapters::FakeDataConsumer)

// Local Variables:
// c-basic-offset: 2
// End:
