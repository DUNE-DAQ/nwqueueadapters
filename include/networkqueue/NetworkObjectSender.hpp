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

#include "networkqueue/nos/Structs.hpp"
#include "networkqueue/nos/Nljs.hpp"

namespace dunedaq {

  template<class T>
  class NetworkObjectSender
  {
  public:
    NetworkObjectSender(const dunedaq::networkqueue::nos::Conf& conf)
      : sender_(dunedaq::ipm::makeIPMSender(conf.ipm_plugin_type))
      , stype_(dunedaq::serialization::fromString(conf.stype))
    {
      // TODO: We should get a moo.any object from the conf and just pass it straight through
      sender_->connect_for_sends({ {"connection_string", conf.address} });
    }

    void send(const T& obj, const dunedaq::ipm::Sender::duration_type& timeout)
    {
      nlohmann::json j = obj;
      auto s=serialization::serialize(j, stype_);
      sender_->send(s.data(), s.size(), timeout);
    }
    
  protected:
    std::shared_ptr<ipm::Sender> sender_;
    serialization::SerializationType stype_;
  };
}

#endif
