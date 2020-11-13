/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::nq.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_NQ_STRUCTS_HPP
#define DUNEDAQ_NETWORKQUEUE_NQ_STRUCTS_HPP

#include <cstdint>

#include <nlohmann/json.hpp>

namespace dunedaq::networkqueue::nq {

    // @brief An opaque object holding lower layer substructure
    using Data = nlohmann::json;

    // @brief A duration in milliseconds
    using Duration = int32_t;


    // @brief NetworkToQueueAdapterDAQModule Configuration
    struct NetworkToQueueAdapterConf {

        // @brief Milliseconds to wait on queue before timing out
        Duration queue_timeout_ms;

        // @brief Connection info for the IPM socket
        Data connection_info;
    };

    // @brief QueueToNetworkAdapterDAQModule Configuration
    struct QueueToNetworkAdapterConf {

        // @brief Milliseconds to wait on queue before timing out
        Duration queue_timeout_ms;

        // @brief Connection info for the IPM socket
        Data connection_info;
    };

} // namespace dunedaq::networkqueue::nq

#endif // DUNEDAQ_NETWORKQUEUE_NQ_STRUCTS_HPP