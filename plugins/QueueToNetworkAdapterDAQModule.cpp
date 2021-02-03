/**
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "networkqueue/QueueToNetworkAdapterDAQModule.hpp"

#include <ers/ers.h>

#include <chrono>
#include <string>
#include <vector>

#include "serialization/Serialization.hpp"

// #include "networkqueue/fsd/QToNMaker.hpp"

namespace dunedaq {

QueueToNetworkAdapterDAQModule::QueueToNetworkAdapterDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&QueueToNetworkAdapterDAQModule::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{

  register_command("conf", &QueueToNetworkAdapterDAQModule::do_configure);
  register_command("start", &QueueToNetworkAdapterDAQModule::do_start);
  register_command("stop", &QueueToNetworkAdapterDAQModule::do_stop);
}

void
QueueToNetworkAdapterDAQModule::init(const data_t& init_data)
{
  auto mod_init_data = init_data.get<appfwk::cmd::ModInit>();
  // TODO: Should get these values safely via codegen/schema
  std::string msg_module_name = init_data.at("msg_module_name");
  std::string msg_type_name = init_data.at("msg_type");
  auto sender_conf = init_data.at("sender_config").get<dunedaq::serialization::networkobjectsender::Conf>();

  impl_ = makeQueueToNetworkBase(msg_module_name, msg_type_name, mod_init_data, sender_conf);
  if (impl_.get() == nullptr) {
    throw std::runtime_error("No QToN for requested msg_type");
  }
}

void
QueueToNetworkAdapterDAQModule::do_configure(const data_t& /* config_data */)
{}

void
QueueToNetworkAdapterDAQModule::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

void
QueueToNetworkAdapterDAQModule::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

void
QueueToNetworkAdapterDAQModule::do_work(std::atomic<bool>& running_flag)
{
  while (running_flag.load()) {
    // TODO: Proper handling of "stop"
    try {
      impl_->get();
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    } catch (const dunedaq::ipm::SendTimeoutExpired&) {
      continue;
    }
  }
}

DEFINE_DUNE_DAQ_MODULE(QueueToNetworkAdapterDAQModule)
} // namespace dunedaq
