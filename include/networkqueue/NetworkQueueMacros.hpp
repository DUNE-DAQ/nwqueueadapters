#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKQUEUEMACROS_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKQUEUEMACROS_HPP_

#include "networkqueue/QueueToNetworkAdapterDAQModule.hpp"
#include "networkqueue/NetworkToQueueAdapterDAQModule.hpp"

#define DEFINE_DUNE_NETWORKQUEUE(klass)  \
  DEFINE_DUNE_QUEUE_TO_NETWORK(klass)    \
  DEFINE_DUNE_NETWORK_TO_QUEUE(klass)    \

#endif
