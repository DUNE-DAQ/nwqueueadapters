/**
 * @file network_object_send_receive.cxx
 * 
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/NetworkObjectReceiver.hpp"
#include "nwqueueadapters/NetworkObjectSender.hpp"
// clang-format off
#include <string> // NOLINT This is here to workaround moo issue #12
#include "nwqueueadapters/fsd/MsgP.hpp" // NOLINT
#include "nwqueueadapters/fsd/Nljs.hpp" // NOLINT
#include "nwqueueadapters/fsd/Structs.hpp" // NOLINT
// clang-format on
#include "logging/Logging.hpp" // NOLINT

#include <iostream>

int
main()
{
  using FakeData = dunedaq::nwqueueadapters::fsd::FakeData;

  dunedaq::nwqueueadapters::networkobjectsender::Conf sender_conf;
  sender_conf.ipm_plugin_type = "ZmqSender";
  sender_conf.stype = "json";
  sender_conf.address = "inproc://foo";

  dunedaq::nwqueueadapters::networkobjectreceiver::Conf receiver_conf;
  receiver_conf.ipm_plugin_type = "ZmqReceiver";
  receiver_conf.address = "inproc://foo";

  dunedaq::nwqueueadapters::NetworkObjectSender<FakeData> sender(sender_conf);
  dunedaq::nwqueueadapters::NetworkObjectReceiver<FakeData> receiver(receiver_conf);

  FakeData fd;
  fd.fake_count = 25;

  sender.send(fd, std::chrono::milliseconds(2));
  FakeData fd_recv = receiver.recv(std::chrono::milliseconds(2));
  TLOG() << "Sent: " << fd.fake_count << ". Received: " << fd_recv.fake_count << std::endl;
}
