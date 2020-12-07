/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for schema in
 * namespace dunedaq::networkqueue::fsdc to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_FSDC_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_FSDC_NLJS_HPP


#include "networkqueue/fsdc/Structs.hpp"



#include <nlohmann/json.hpp>
#include <msgpack.hpp>
#include <networkqueue/msgpack_json.hpp>

namespace dunedaq::networkqueue::fsdc {

    using data_t = nlohmann::json;


    inline void to_json(data_t& j, const Conf& obj) {
        j["nIntsPerVector"] = obj.nIntsPerVector;
        j["starting_int"] = obj.starting_int;
        j["ending_int"] = obj.ending_int;
        j["queue_timeout_ms"] = obj.queue_timeout_ms;
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
    }

} // namespace dunedaq::networkqueue::fsdc


// MsgPack serialization/deserialization functions
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {


template<>
struct convert<dunedaq::networkqueue::fsdc::Conf> {
    msgpack::object const& operator()(msgpack::object const& o, dunedaq::networkqueue::fsdc::Conf& v) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 4) throw msgpack::type_error();
        v.nIntsPerVector = o.via.array.ptr[0].as<dunedaq::networkqueue::fsdc::Size>();
        v.starting_int = o.via.array.ptr[1].as<dunedaq::networkqueue::fsdc::Count>();
        v.ending_int = o.via.array.ptr[2].as<dunedaq::networkqueue::fsdc::Count>();
        v.queue_timeout_ms = o.via.array.ptr[3].as<dunedaq::networkqueue::fsdc::Count>();
        return o;
    }
};

template<>
struct pack<dunedaq::networkqueue::fsdc::Conf> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, dunedaq::networkqueue::fsdc::Conf const& v) const {
        // packing member variables as an array.
        o.pack_array(4);
        o.pack(v.nIntsPerVector);
        o.pack(v.starting_int);
        o.pack(v.ending_int);
        o.pack(v.queue_timeout_ms);
        return o;
    }
};


} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#endif // DUNEDAQ_NETWORKQUEUE_FSDC_NLJS_HPP