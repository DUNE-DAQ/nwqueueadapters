/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for schema in
 * namespace dunedaq::networkqueue::fsdp to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_FSDP_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_FSDP_NLJS_HPP


#include "networkqueue/fsdp/Structs.hpp"



#include <nlohmann/json.hpp>
#include <msgpack.hpp>
#include <networkqueue/msgpack_json.hpp>

namespace dunedaq::networkqueue::fsdp {

    using data_t = nlohmann::json;


    inline void to_json(data_t& j, const Conf& obj) {
        j["nIntsPerVector"] = obj.nIntsPerVector;
        j["starting_int"] = obj.starting_int;
        j["ending_int"] = obj.ending_int;
        j["queue_timeout_ms"] = obj.queue_timeout_ms;
        j["wait_between_sends_ms"] = obj.wait_between_sends_ms;
    }

    inline void from_json(const data_t& j, Conf& obj) {
        if (j.contains("nIntsPerVector"))
            j.at("nIntsPerVector").get_to(obj.nIntsPerVector);
        if (j.contains("starting_int"))
            j.at("starting_int").get_to(obj.starting_int);
        if (j.contains("ending_int"))
            j.at("ending_int").get_to(obj.ending_int);
        if (j.contains("queue_timeout_ms"))
            j.at("queue_timeout_ms").get_to(obj.queue_timeout_ms);
        if (j.contains("wait_between_sends_ms"))
            j.at("wait_between_sends_ms").get_to(obj.wait_between_sends_ms);
    }

} // namespace dunedaq::networkqueue::fsdp


// MsgPack serialization/deserialization functions
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {


template<>
struct convert<dunedaq::networkqueue::fsdp::Conf> {
    msgpack::object const& operator()(msgpack::object const& o, dunedaq::networkqueue::fsdp::Conf& v) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 5) throw msgpack::type_error();
        v.nIntsPerVector = o.via.array.ptr[0].as<dunedaq::networkqueue::fsdp::Size>();
        v.starting_int = o.via.array.ptr[1].as<dunedaq::networkqueue::fsdp::Count>();
        v.ending_int = o.via.array.ptr[2].as<dunedaq::networkqueue::fsdp::Count>();
        v.queue_timeout_ms = o.via.array.ptr[3].as<dunedaq::networkqueue::fsdp::Count>();
        v.wait_between_sends_ms = o.via.array.ptr[4].as<dunedaq::networkqueue::fsdp::Count>();
        return o;
    }
};

template<>
struct pack<dunedaq::networkqueue::fsdp::Conf> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, dunedaq::networkqueue::fsdp::Conf const& v) const {
        // packing member variables as an array.
        o.pack_array(5);
        o.pack(v.nIntsPerVector);
        o.pack(v.starting_int);
        o.pack(v.ending_int);
        o.pack(v.queue_timeout_ms);
        o.pack(v.wait_between_sends_ms);
        return o;
    }
};


} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#endif // DUNEDAQ_NETWORKQUEUE_FSDP_NLJS_HPP