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
#include <msgpack.hpp>

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
      CBOR,
      UBJSON,
      BSON
    };

    /**
     * @brief Convert string to SerializationType
     */
    SerializationType fromString(const std::string s)
    {
      if(s=="json")    return JSON;
      if(s=="msgpack") return MsgPack;
      if(s=="cbor")    return CBOR;
      if(s=="ubjson")  return UBJSON;
      if(s=="bson")    return BSON;
      throw std::runtime_error("Unknown serialization type");
    }

    /**
     * @brief Serialize object @p obj using serialization method @p stype
     */
    template<class T>
    std::vector<uint8_t> serialize(const T& obj, SerializationType stype)
    {
      switch(stype){
      case JSON:
        {
          nlohmann::json j = obj;
          nlohmann::json::string_t s=j.dump();
          std::vector<uint8_t> ret(s.begin(), s.end());
          return ret;
        }
      case MsgPack:
        {
          // Feels like there should be a nicer way to do this...
          msgpack::sbuffer buf;
          msgpack::pack(buf, obj);
          std::vector<uint8_t> v(buf.data(), buf.data()+buf.size());
          return v;
        }
      case CBOR:
        {
          nlohmann::json j = obj;
          return nlohmann::json::to_cbor(j);
        }
      case UBJSON:
        {
          nlohmann::json j = obj;
          return nlohmann::json::to_ubjson(j);
        }
      case BSON:
        {
          nlohmann::json j = obj;
          return nlohmann::json::to_bson(j);
        }
      default:
        throw std::runtime_error("Unknown serialization type");
      }
    }

    /**
     * @brief Deserialize vector of bytes @p v into an instance of class @p T
     */
    template<class T, typename CharType=unsigned char>
    T deserialize(const std::vector<CharType>& v, SerializationType stype)
    {
      using json=nlohmann::json;

      switch(stype){
      case JSON:
        {
          json j=json::parse(v);
          return j.get<T>();
        }
      case MsgPack:
        {
          msgpack::object_handle oh =
            msgpack::unpack((char*)v.data(), v.size());
          msgpack::object obj = oh.get();
          return obj.as<T>();
        }
      case CBOR:
        {
          json j=json::from_cbor(v);
          return j.get<T>();
        }
      case UBJSON:
        {
          json j=json::from_ubjson(v);
          return j.get<T>();
        }
      case BSON:
        {
          json j=json::from_bson(v);
          return j.get<T>();
        }
      default:
        throw std::runtime_error("Unknown serialization type");
      }

    }

  } // namespace serialization
  
  } // namespace dunedaq
#endif
