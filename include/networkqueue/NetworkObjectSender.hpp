/**
 * @file NetworkObjectSender.hpp
 *
 * Based on VectorIntIPMSenderDAQModule from IPM
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKOBJECTSENDER_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKOBJECTSENDER_HPP_

#include "ipm/Sender.hpp"
#include "networkqueue/Serialization.hpp"

namespace dunedaq {

  template<class T>
  class NetworkObjectSender
  {
  public:
    NetworkObjectSender(const serialization::SerializationType stype)
      : sender_(nullptr)
      , stype_(stype)
    {
      sender_->connect_for_sends("tcp://127.0.0.1:12345");
    }

    void send(const T& obj, const std::chrono::duration& timeout)
    {
      nlohmann::json j = obj;
      auto s=serialization::serialize(j, stype_);
      sender_->send(s.data(), s.size(), timeout);
    }
    
  protected:
    std::unique_ptr<ipm::Sender> sender_;
    serialization::SerializationType stype_;
  };
}

#endif
