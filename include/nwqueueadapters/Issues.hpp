/**
 * @file Issues.hpp
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_ISSUES_HPP_
#define NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_ISSUES_HPP_

#include "ers/Issue.hpp"

#include <string>

namespace dunedaq {
// clang-format off

ERS_DECLARE_ISSUE(nwqueueadapters,                        // namespace
                  CannotConfigure,                        // issue name
                  "Failed to configure",                  // message
                  )                                       // (no attributes)

ERS_DECLARE_ISSUE(nwqueueadapters,                        // namespace
                  NetworkToQueuePushTimeout,              // issue name
                  "Push timed out: Message of type " << t // message
                  << " to queue " << q,
                  ((std::string)t)((std::string)q))       // attributes

ERS_DECLARE_ISSUE(nwqueueadapters,                                        // namespace
                  NoNetworkToQueueImpl,                                   // issue name

                  "Cannot create NetworkToQueueImpl for class " << klass  // message
                  << " in plugin library " << library
                  << " for queue instance " << queue_instance,

                  ((std::string)klass)                                    // attributes
                  ((std::string)library)
                  ((std::string)queue_instance))

ERS_DECLARE_ISSUE(nwqueueadapters,                        // namespace
                  QueueToNetworkSendTimeout,              // issue name
                  "Send timed out: Message of type " << t // message
                  << " from queue " << q,
                  ((std::string)t)((std::string)q))       // attributes

ERS_DECLARE_ISSUE(nwqueueadapters,                                        // namespace
                  NoQueueToNetworkImpl,                                   // issue name

                  "Cannot create QueueToNetworkImpl for class " << klass  // message
                  << " in plugin library " << library
                  << " for queue instance " << queue_instance,

                  ((std::string)klass)                                    // attributes
                  ((std::string)library)
                  ((std::string)queue_instance))

// clang-format on
} // namespace dunedaq

#endif // NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_ISSUES_HPP_
