/**
 * @file NetworkQueueMacros.hpp
 *
 * Macros to make a type usable with the networkqueue modules
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKQUEUEMACROS_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKQUEUEMACROS_HPP_

#include "networkqueue/NetworkToQueue.hpp"
#include "networkqueue/QueueToNetwork.hpp"

#define DEFINE_DUNE_NETWORKQUEUE(klass)                                                                                \
  DEFINE_DUNE_QUEUE_TO_NETWORK(klass)                                                                                  \
  DEFINE_DUNE_NETWORK_TO_QUEUE(klass)

#endif
