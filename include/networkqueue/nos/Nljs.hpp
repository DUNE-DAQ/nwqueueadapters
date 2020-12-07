/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for schema in
 * namespace dunedaq::networkqueue::nos to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_NOS_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_NOS_NLJS_HPP


#include "networkqueue/nos/Structs.hpp"



#include <nlohmann/json.hpp>
#include <msgpack.hpp>
#include <networkqueue/msgpack_json.hpp>

namespace dunedaq::networkqueue::nos {

    using data_t = nlohmann::json;


    inline void to_json(data_t& j, const Conf& obj) {
        j["stype"] = obj.stype;
        j["ipm_plugin_type"] = obj.ipm_plugin_type;
        j["address"] = obj.address;
    }

    inline void from_json(const data_t& j, Conf& obj) {
        if (j.contains("stype"))
            j.at("stype").get_to(obj.stype);
        if (j.contains("ipm_plugin_type"))
            j.at("ipm_plugin_type").get_to(obj.ipm_plugin_type);
        if (j.contains("address"))
            j.at("address").get_to(obj.address);
    }

} // namespace dunedaq::networkqueue::nos


// MsgPack serialization/deserialization functions
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {


template<>
struct convert<dunedaq::networkqueue::nos::Conf> {
    msgpack::object const& operator()(msgpack::object const& o, dunedaq::networkqueue::nos::Conf& v) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 3) throw msgpack::type_error();
        v.stype = o.via.array.ptr[0].as<dunedaq::networkqueue::nos::SerializationString>();
        v.ipm_plugin_type = o.via.array.ptr[1].as<dunedaq::networkqueue::nos::IPMPluginType>();
        v.address = o.via.array.ptr[2].as<dunedaq::networkqueue::nos::Address>();
        return o;
    }
};

template<>
struct pack<dunedaq::networkqueue::nos::Conf> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, dunedaq::networkqueue::nos::Conf const& v) const {
        // packing member variables as an array.
        o.pack_array(3);
        o.pack(v.stype);
        o.pack(v.ipm_plugin_type);
        o.pack(v.address);
        return o;
    }
};


} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#endif // DUNEDAQ_NETWORKQUEUE_NOS_NLJS_HPP