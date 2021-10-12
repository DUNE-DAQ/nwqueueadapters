/**
 * @file serialization_speed.cxx
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/NetworkObjectReceiver.hpp"
#include "nwqueueadapters/NetworkObjectSender.hpp"
// clang-format off
#include <string> // NOLINT This is here to workaround moo issue #12
#include "nwqueueadapters/fsd/MsgP.hpp"    // NOLINT
#include "nwqueueadapters/fsd/Nljs.hpp"    // NOLINT
#include "nwqueueadapters/fsd/Structs.hpp" // NOLINT

// clang-format on

#include <chrono>
#include <iostream>
#include <thread>

#include "logging/Logging.hpp" // NOLINT

using namespace std::chrono_literals;

using FakeData = dunedaq::nwqueueadapters::fsd::FakeData;

void
sender_thread_fn(dunedaq::nwqueueadapters::networkobjectsender::Conf sender_conf, int n_messages)
{
  dunedaq::nwqueueadapters::NetworkObjectSender<FakeData> sender(sender_conf);

  for (int i = 0; i < n_messages; ++i) {
    FakeData fd;
    fd.fake_count = 25;

    sender.send(fd, std::chrono::milliseconds(1000000));
  }
}

void
receiver_thread_fn(dunedaq::nwqueueadapters::networkobjectreceiver::Conf receiver_conf, int n_messages)
{
  int total = 0;
  dunedaq::nwqueueadapters::NetworkObjectReceiver<FakeData> receiver(receiver_conf);
  for (int i = 0; i < n_messages; ++i) {
    FakeData fd_recv = receiver.recv(std::chrono::milliseconds(1000000));
    total += fd_recv.fake_count;
  }
  TLOG() << "Total:" << total << std::endl;
}

// Return the current steady clock in microseconds
inline uint64_t // NOLINT
now_us()
{
  using namespace std::chrono;
  // std::chrono is the worst
  return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

int
main()
{
  dunedaq::networkmanager::nwmgr::Connections testConfig;
  dunedaq::networkmanager::nwmgr::Connection testConn;
  testConn.name = "foo";
  testConn.address = "inproc://foo";
  testConn.type = dunedaq::networkmanager::nwmgr::Type::Receiver;
  testConfig.push_back(testConn);
  dunedaq::networkmanager::NetworkManager::get().configure(testConfig);

  dunedaq::nwqueueadapters::networkobjectsender::Conf sender_conf;
  sender_conf.stype = "msgpack";
  sender_conf.name = "foo";

  dunedaq::nwqueueadapters::networkobjectreceiver::Conf receiver_conf;
  receiver_conf.name = "foo";

  const int N = 1000000;
  uint64_t start_time = now_us(); // NOLINT
  std::thread sender_thread(sender_thread_fn, sender_conf, N);
  std::thread receiver_thread(receiver_thread_fn, receiver_conf, N);
  sender_thread.join();
  receiver_thread.join();
  uint64_t end_time = now_us(); // NOLINT
  double time_taken_s = 1e-6 * (end_time - start_time);
  double kHz = 1e-3 * N / time_taken_s;
  printf("Sent %d messages in %.3fs (%.1f kHz)\n", N, time_taken_s, kHz); // NOLINT
}
