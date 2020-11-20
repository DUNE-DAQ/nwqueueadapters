/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::nos.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_NOS_STRUCTS_HPP
#define DUNEDAQ_NETWORKQUEUE_NOS_STRUCTS_HPP

#include <cstdint>

#include <string>

namespace dunedaq::networkqueue::nos {

    // @brief Address to send to
    using Address = std::string;

    // @brief String describing serialization type
    using SerializationString = std::string;

    // @brief IPM plugin type
    using IPMPluginType = std::string;

    // @brief NetworkObjectSender Configuration
    struct Conf {

        // @brief Serialization type
        SerializationString stype;

        // @brief IPM plugin type
        IPMPluginType ipm_plugin_type;

        // @brief Address to send to
        Address address;
    };

} // namespace dunedaq::networkqueue::nos

#endif // DUNEDAQ_NETWORKQUEUE_NOS_STRUCTS_HPP