/**
 * @file NetworkObjectReceiver.hpp
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTRECEIVER_HPP_
#define NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTRECEIVER_HPP_

#include "ipm/Subscriber.hpp"
#include "nwqueueadapters/networkobjectreceiver/Nljs.hpp"
#include "nwqueueadapters/networkobjectreceiver/Structs.hpp"
#include "serialization/Serialization.hpp"

#include "ipm/Receiver.hpp"

#include <memory> // for shared_ptr
#include <variant>

namespace dunedaq::nwqueueadapters {

/**
 * @brief NetworkObjectReceiver receives objects over IPM connections
 *
 * NetworkObjectReceiver and its counterpart NetworkObjectSender
 * provide a convenient interface to object serialization/sending
 * and receiving/deserialization over network connections. Any class
 * which can be converted to/from an @c nlohmann::json object can be
 * used; in particular, all classes generated with moo schema are
 * suitable for use with NetworkObjectSender/Receiver
 *
 * Typical usage:
 *
 * @code
 * NetworkObjectReceiver<MyClass> receiver(conf_object);
 * MyClass m = receiver.recv(m, std::chrono::milliseconds(200));
 * @endcode
 *
 */
template<class T>
class NetworkObjectReceiver
{
public:
  explicit NetworkObjectReceiver(const dunedaq::nwqueueadapters::networkobjectreceiver::Conf& conf)
    : m_receiver(dunedaq::ipm::make_ipm_receiver(conf.ipm_plugin_type))
  {
    m_receiver->connect_for_receives({ { "connection_string", conf.address } });

    auto m_subscriber = std::dynamic_pointer_cast<ipm::Subscriber>(m_receiver);
    if (m_subscriber) {
      for (auto topic : conf.subscriptions) {
        m_subscriber->subscribe(topic);
      }
    }
  }

  /**
   * @brief Receive a control message or object
   *
   * Incoming messages may be "control" messages consisting of a
   * single byte, or serialized objects. In the former case, the
   * returned std::variant holds a char with the content of the
   * control message; in the latter case, the variant holds an object
   * of class T deserialized from the message
   **/
  std::variant<char, T> recv(const dunedaq::ipm::Receiver::duration_t& timeout)
  {
    dunedaq::ipm::Receiver::Response recvd = m_receiver->receive(timeout);
    // Messages consisting of a single byte are control messages; others are serialized objects
    if (recvd.data.size()==1) {
      return recvd.data[0];
    }
    else{
      return serialization::deserialize<T>(recvd.data);
    }
  }

protected:
  std::shared_ptr<ipm::Receiver> m_receiver;
};
} // namespace dunedaq::nwqueueadapters

#endif // NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTRECEIVER_HPP_
