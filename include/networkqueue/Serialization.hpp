/**
 * @file Serialization.hpp
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_SERIALIZATIONTYPE_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_SERIALIZATIONTYPE_HPP_

#include <nlohmann/json.hpp>

#include <string>

namespace dunedaq {

  namespace serialization {

    /**
     * @brief Serialization methods that are available
     */
    enum SerializationType
    {
      JSON,
      MsgPack,
      CBOR
    };

    /**
     * @brief Convert string to SerializationType
     */
    SerializationType fromString(const std::string s)
    {
      if(s=="json")    return JSON;
      if(s=="msgpack") return MsgPack;
      if(s=="cbor")    return CBOR;
      throw std::runtime_error("Unknown serialization type");
    }

    /**
     * @brief Serialize json object @p j using serialization method @p stype
     */
    std::vector<uint8_t> serialize(const nlohmann::json& j, SerializationType stype)
    {
      switch(stype){
      case JSON:
        {
          nlohmann::json::string_t s=j.dump();
          std::vector<uint8_t> ret(s.begin(), s.end());
          return ret;
        }
      case MsgPack:
        return nlohmann::json::to_msgpack(j);
      case CBOR:
        return nlohmann::json::to_cbor(j);
      default:
        throw std::runtime_error("Unknown serialization type");
      }
    }

    /**
     * @brief Deserialize vector of bytes @p v into an instance of class @p T
     */
    template<class T>
    T deserialize(const std::vector<char>& v, SerializationType stype)
    {
      using json=nlohmann::json;
      json j;
      switch(stype){
      case JSON:
        j=json::parse(v);
        break;
      case MsgPack:
        j=json::from_msgpack(v);
        break;
      case CBOR:
        j=json::from_cbor(v);
        break;
      default:
        throw std::runtime_error("Unknown serialization type");
      }
      return j.get<T>();
    }

  } // namespace serialization
  
  } // namespace dunedaq
#endif
