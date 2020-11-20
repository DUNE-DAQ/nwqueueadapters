/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::nor.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_NOR_STRUCTS_HPP
#define DUNEDAQ_NETWORKQUEUE_NOR_STRUCTS_HPP

#include <cstdint>

#include <string>

namespace dunedaq::networkqueue::nor {

    // @brief Address to receive from
    using Address = std::string;

    // @brief String describing serialization type
    using SerializationString = std::string;

    // @brief IPM plugin type
    using IPMPluginType = std::string;

    // @brief NetworkObjectReceiver Configuration
    struct Conf {

        // @brief Serialization type
        SerializationString stype;

        // @brief IPM plugin type
        IPMPluginType ipm_plugin_type;

        // @brief Address to receive from
        Address address;
    };

} // namespace dunedaq::networkqueue::nor

#endif // DUNEDAQ_NETWORKQUEUE_NOR_STRUCTS_HPP