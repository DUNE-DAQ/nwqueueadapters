/**
 * @file NetworkObjectReceiver.hpp
 *
 * Based on VectorIntIPMReceiverDAQModule from IPM
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKOBJECTRECEIVER_HPP_
#define NETWORKQUEUE_INCLUDE_NETWORKQUEUE_NETWORKOBJECTRECEIVER_HPP_

#include "ipm/Receiver.hpp"
#include "networkqueue/Serialization.hpp"

#include "networkqueue/nor/Structs.hpp"
#include "networkqueue/nor/Nljs.hpp"

namespace dunedaq {

  template<class T>
  class NetworkObjectReceiver
  {
  public:
    NetworkObjectReceiver(const dunedaq::networkqueue::nor::Conf& conf)
      : receiver_(dunedaq::ipm::makeIPMReceiver(conf.ipm_plugin_type))
      , stype_(dunedaq::serialization::fromString(conf.stype))
    {
      // TODO: We should get a moo.any object from the conf and just pass it straight through
      receiver_->connect_for_receives({ {"connection_string", conf.address} });
    }

    T recv(const dunedaq::ipm::Receiver::duration_type& timeout)
    {
      dunedaq::ipm::Receiver::Response recvd=receiver_->receive(timeout);
      return serialization::deserialize<T>(recvd.data, stype_);
    }
    
  protected:
    std::shared_ptr<ipm::Receiver> receiver_;
    serialization::SerializationType stype_;
  };
}

#endif
