#include "networkqueue/NetworkObjectSender.hpp"
#include "networkqueue/NetworkObjectReceiver.hpp"
#include "networkqueue/fsd/Structs.hpp"
#include "networkqueue/fsd/Nljs.hpp"
#include "networkqueue/fsd/MsgPack.hpp"

#include <iostream>

int main()
{
  using FakeData=dunedaq::networkqueue::fsd::FakeData;

  dunedaq::networkqueue::nos::Conf sender_conf;
  sender_conf.ipm_plugin_type="ZmqSender";
  sender_conf.stype="cbor";
  sender_conf.address="inproc://foo";
  
  dunedaq::networkqueue::nor::Conf receiver_conf;
  receiver_conf.ipm_plugin_type="ZmqReceiver";
  receiver_conf.stype="cbor";
  receiver_conf.address="inproc://foo";
  
  dunedaq::NetworkObjectSender<FakeData> sender(sender_conf);
  dunedaq::NetworkObjectReceiver<FakeData> receiver(receiver_conf);

  FakeData fd;
  fd.fake_count=25;
  
  sender.send(fd, std::chrono::milliseconds(2));
  FakeData fd_recv=receiver.recv(std::chrono::milliseconds(2));
  std::cout << "Sent: " << fd.fake_count << ". Received: " <<  fd_recv.fake_count << std::endl;
}
