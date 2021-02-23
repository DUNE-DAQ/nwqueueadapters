/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for shema in 
 * namespace dunedaq::nwqueueadapters::fsd to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NWQUEUEADAPTERS_FSD_NLJS_HPP
#define DUNEDAQ_NWQUEUEADAPTERS_FSD_NLJS_HPP

// My structs
#include "nwqueueadapters/fsd/Structs.hpp"


#include <nlohmann/json.hpp>

namespace dunedaq::nwqueueadapters::fsd {

    using data_t = nlohmann::json;    NLOHMANN_JSON_SERIALIZE_ENUM( Fakeness, {
            { dunedaq::nwqueueadapters::fsd::Fakeness::Unknown, "Unknown" },
            { dunedaq::nwqueueadapters::fsd::Fakeness::Fake, "Fake" },
            { dunedaq::nwqueueadapters::fsd::Fakeness::SuperFake, "SuperFake" },
        })

    
    inline void to_json(data_t& j, const FakeData& obj) {
        j["fake_count"] = obj.fake_count;
    }
    
    inline void from_json(const data_t& j, FakeData& obj) {
        if (j.contains("fake_count"))
            j.at("fake_count").get_to(obj.fake_count);    
    }
    
    inline void to_json(data_t& j, const AnotherFakeData& obj) {
        j["fake_count"] = obj.fake_count;
        j["fake_timestamp"] = obj.fake_timestamp;
        j["fake_datas"] = obj.fake_datas;
        j["fakeness"] = obj.fakeness;
    }
    
    inline void from_json(const data_t& j, AnotherFakeData& obj) {
        if (j.contains("fake_count"))
            j.at("fake_count").get_to(obj.fake_count);    
        if (j.contains("fake_timestamp"))
            j.at("fake_timestamp").get_to(obj.fake_timestamp);    
        if (j.contains("fake_datas"))
            j.at("fake_datas").get_to(obj.fake_datas);    
        if (j.contains("fakeness"))
            j.at("fakeness").get_to(obj.fakeness);    
    }
    
} // namespace dunedaq::nwqueueadapters::fsd

#endif // DUNEDAQ_NWQUEUEADAPTERS_FSD_NLJS_HPP