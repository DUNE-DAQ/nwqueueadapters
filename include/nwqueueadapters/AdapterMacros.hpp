/**
 * @file AdapterMacros.hpp
 *
 * Macros to make a type usable with the nwqueueadapters modules
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_ADAPTERMACROS_HPP_
#define NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_ADAPTERMACROS_HPP_

#include "nwqueueadapters/NetworkToQueue.hpp"
#include "nwqueueadapters/QueueToNetwork.hpp"

#define DEFINE_DUNE_NWQUEUEADAPTERS(...)                                                                               \
  DEFINE_DUNE_QUEUE_TO_NETWORK(__VA_ARGS__)                                                                            \
  DEFINE_DUNE_NETWORK_TO_QUEUE(__VA_ARGS__)

#endif
