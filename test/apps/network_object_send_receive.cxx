#include "networkqueue/NetworkObjectSender.hpp"
#include "networkqueue/NetworkObjectReceiver.hpp"
#include "networkqueue/fsd/Structs.hpp"
#include "networkqueue/fsd/Nljs.hpp"

#include <iostream>

int main()
{
  using FakeData=dunedaq::networkqueue::fsd::FakeData;
  dunedaq::NetworkObjectSender<FakeData> sender(dunedaq::serialization::CBOR);
  dunedaq::NetworkObjectReceiver<FakeData> receiver(dunedaq::serialization::CBOR);

  FakeData fd;
  fd.fake_count=25;
  
  sender.send(fd, std::chrono::milliseconds(2));
  FakeData fd_recv=receiver.recv(std::chrono::milliseconds(2));
  std::cout << "Sent: " << fd.fake_count << ". Received: " <<  fd_recv.fake_count << std::endl;
}
