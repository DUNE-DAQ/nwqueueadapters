/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for schema in
 * namespace dunedaq::networkqueue::nor to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_NOR_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_NOR_NLJS_HPP


#include "networkqueue/nor/Structs.hpp"



#include <nlohmann/json.hpp>
#include <msgpack.hpp>
#include <networkqueue/msgpack_json.hpp>

namespace dunedaq::networkqueue::nor {

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

} // namespace dunedaq::networkqueue::nor


// MsgPack serialization/deserialization functions
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {


template<>
struct convert<dunedaq::networkqueue::nor::Conf> {
    msgpack::object const& operator()(msgpack::object const& o, dunedaq::networkqueue::nor::Conf& v) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 3) throw msgpack::type_error();
        v.stype = o.via.array.ptr[0].as<dunedaq::networkqueue::nor::SerializationString>();
        v.ipm_plugin_type = o.via.array.ptr[1].as<dunedaq::networkqueue::nor::IPMPluginType>();
        v.address = o.via.array.ptr[2].as<dunedaq::networkqueue::nor::Address>();
        return o;
    }
};

template<>
struct pack<dunedaq::networkqueue::nor::Conf> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, dunedaq::networkqueue::nor::Conf const& v) const {
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

#endif // DUNEDAQ_NETWORKQUEUE_NOR_NLJS_HPP