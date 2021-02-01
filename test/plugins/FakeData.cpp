#include "networkqueue/QueueToNetworkAdapterDAQModule.hpp"
#include "networkqueue/NetworkToQueueAdapterDAQModule.hpp"

#include "networkqueue/fsd/Nljs.hpp"
#include "networkqueue/fsd/MsgP.hpp"

DEFINE_DUNE_QUEUE_TO_NETWORK(dunedaq::networkqueue::fsd::FakeData)
DEFINE_DUNE_NETWORK_TO_QUEUE(dunedaq::networkqueue::fsd::FakeData)
