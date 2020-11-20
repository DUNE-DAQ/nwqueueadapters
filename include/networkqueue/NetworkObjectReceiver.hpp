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

namespace dunedaq {

  template<class T>
  class NetworkObjectReceiver
  {
  public:
    NetworkObjectReceiver(const serialization::SerializationType stype)
      : receiver_(dunedaq::ipm::makeIPMReceiver("ZmqReceiver"))
      , stype_(stype)
    {
      receiver_->connect_for_receives({ {"connection_string", "tcp://127.0.0.1:12345"} });
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
