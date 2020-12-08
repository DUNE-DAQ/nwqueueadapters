/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for schema in
 * namespace dunedaq::networkqueue::nq to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_NQ_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_NQ_NLJS_HPP


#include "networkqueue/nq/Structs.hpp"



#include <nlohmann/json.hpp>

namespace dunedaq::networkqueue::nq {

    using data_t = nlohmann::json;


    inline void to_json(data_t& j, const NetworkToQueueAdapterConf& obj) {
        j["queue_timeout_ms"] = obj.queue_timeout_ms;
        j["connection_info"] = obj.connection_info;
    }

    inline void from_json(const data_t& j, NetworkToQueueAdapterConf& obj) {
        if (j.contains("queue_timeout_ms"))
            j.at("queue_timeout_ms").get_to(obj.queue_timeout_ms);
        obj.connection_info = j.at("connection_info");
    }
    inline void to_json(data_t& j, const QueueToNetworkAdapterConf& obj) {
        j["queue_timeout_ms"] = obj.queue_timeout_ms;
        j["connection_info"] = obj.connection_info;
    }

    inline void from_json(const data_t& j, QueueToNetworkAdapterConf& obj) {
        if (j.contains("queue_timeout_ms"))
            j.at("queue_timeout_ms").get_to(obj.queue_timeout_ms);
        obj.connection_info = j.at("connection_info");
    }

} // namespace dunedaq::networkqueue::nq


#endif // DUNEDAQ_NETWORKQUEUE_NQ_NLJS_HPP