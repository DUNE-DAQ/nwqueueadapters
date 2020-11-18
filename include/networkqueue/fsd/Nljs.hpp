/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::fsd to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_FSD_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_FSD_NLJS_HPP


#include "networkqueue/fsd/Structs.hpp"


#include <nlohmann/json.hpp>

namespace dunedaq::networkqueue::fsd {

    using data_t = nlohmann::json;


    
    inline void to_json(data_t& j, const AnotherFakeData& obj) {
        j["fake_count"] = obj.fake_count;
        j["fake_timestamp"] = obj.fake_timestamp;
    }
    
    inline void from_json(const data_t& j, AnotherFakeData& obj) {
        if (j.contains("fake_count"))
            j.at("fake_count").get_to(obj.fake_count);    
        if (j.contains("fake_timestamp"))
            j.at("fake_timestamp").get_to(obj.fake_timestamp);    
    }
    
    inline void to_json(data_t& j, const FakeData& obj) {
        j["fake_count"] = obj.fake_count;
    }
    
    inline void from_json(const data_t& j, FakeData& obj) {
        if (j.contains("fake_count"))
            j.at("fake_count").get_to(obj.fake_count);    
    }
    
    // fixme: add support for MessagePack serializers (at least)

} // namespace dunedaq::networkqueue::fsd

#endif // DUNEDAQ_NETWORKQUEUE_FSD_NLJS_HPP