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
#include <msgpack.hpp>
#include <networkqueue/msgpack_json.hpp>

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


// MsgPack serialization/deserialization functions
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {


template<>
struct convert<dunedaq::networkqueue::nq::NetworkToQueueAdapterConf> {
    msgpack::object const& operator()(msgpack::object const& o, dunedaq::networkqueue::nq::NetworkToQueueAdapterConf& v) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 2) throw msgpack::type_error();
        v.queue_timeout_ms = o.via.array.ptr[0].as<dunedaq::networkqueue::nq::Duration>();
        v.connection_info = o.via.array.ptr[1].as<dunedaq::networkqueue::nq::Data>();
        return o;
    }
};

template<>
struct pack<dunedaq::networkqueue::nq::NetworkToQueueAdapterConf> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, dunedaq::networkqueue::nq::NetworkToQueueAdapterConf const& v) const {
        // packing member variables as an array.
        o.pack_array(2);
        o.pack(v.queue_timeout_ms);
        o.pack(v.connection_info);
        return o;
    }
};


template<>
struct convert<dunedaq::networkqueue::nq::QueueToNetworkAdapterConf> {
    msgpack::object const& operator()(msgpack::object const& o, dunedaq::networkqueue::nq::QueueToNetworkAdapterConf& v) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 2) throw msgpack::type_error();
        v.queue_timeout_ms = o.via.array.ptr[0].as<dunedaq::networkqueue::nq::Duration>();
        v.connection_info = o.via.array.ptr[1].as<dunedaq::networkqueue::nq::Data>();
        return o;
    }
};

template<>
struct pack<dunedaq::networkqueue::nq::QueueToNetworkAdapterConf> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, dunedaq::networkqueue::nq::QueueToNetworkAdapterConf const& v) const {
        // packing member variables as an array.
        o.pack_array(2);
        o.pack(v.queue_timeout_ms);
        o.pack(v.connection_info);
        return o;
    }
};


} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#endif // DUNEDAQ_NETWORKQUEUE_NQ_NLJS_HPP