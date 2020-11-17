/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::fsdp.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_FSDP_STRUCTS_HPP
#define DUNEDAQ_NETWORKQUEUE_FSDP_STRUCTS_HPP

#include <cstdint>


namespace dunedaq::networkqueue::fsdp {

    // @brief A count of very many things
    using Size = uint64_t; // NOLINT


    // @brief A count of not too many things
    using Count = int32_t;


    // @brief Fake Data Producer DAQ Module Configuration
    struct Conf {

        // @brief Number of numbers
        Size nIntsPerVector;

        // @brief Number to start with
        Count starting_int;

        // @brief Number to end with
        Count ending_int;

        // @brief Milliseconds to wait on queue before timing out
        Count queue_timeout_ms;

        // @brief Millisecs to wait between sending
        Count wait_between_sends_ms;
    };

} // namespace dunedaq::networkqueue::fsdp

#endif // DUNEDAQ_NETWORKQUEUE_FSDP_STRUCTS_HPP