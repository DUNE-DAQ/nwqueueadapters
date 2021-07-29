/**
 * @file NetworkObjectSender.hpp
 *
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTSENDER_HPP_
#define NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTSENDER_HPP_

#include "nwqueueadapters/networkobjectsender/Nljs.hpp"
#include "nwqueueadapters/networkobjectsender/Structs.hpp"
#include "serialization/Serialization.hpp"

#include "ipm/Sender.hpp"

#include <memory> // for shared_ptr

namespace dunedaq::nwqueueadapters {

/**
 * @brief NetworkObjectSender sends objects over IPM connections
 *
 * NetworkObjectSender and its counterpart NetworkObjectReceiver
 * provide a convenient interface to object serialization and
 * sending and receiving over network connections. Any class which
 * can be converted to/from an @c nlohmann::json object can be used;
 * in particular, all classes generated with moo schema are suitable
 * for use with NetworkObjectSender/Receiver
 *
 * Typical usage:
 *
 * @code
 * NetworkObjectSender<MyClass> sender(conf_object);
 * MyClass m;
 * // Set some fields of m...
 * sender.send(m, std::chrono::milliseconds(2));
 * @endcode
 *
 */
template<class T>
class NetworkObjectSender
{
public:
  explicit NetworkObjectSender(const dunedaq::nwqueueadapters::networkobjectsender::Conf& conf)
    : m_conf(conf)
    , m_sender(dunedaq::ipm::make_ipm_sender(conf.ipm_plugin_type))
    , m_stype(dunedaq::serialization::from_string(conf.stype))
  {
    m_sender->connect_for_sends({ { "connection_string", conf.address } });
  }
  
  /**
   * @brief Send a control message consisting of a single byte @p c
   */
  void send_control_message(char c, const dunedaq::ipm::Sender::duration_t& timeout)
  {
    std::vector<char> bytes{c};
    m_sender->send(bytes.data(), bytes.size(), timeout, m_conf.topic);
  }
  
  /**
   * @brief Send object @p obj with timeout @p timeout
   */
  void send(const T& obj, const dunedaq::ipm::Sender::duration_t& timeout)
  {
    auto bytes = serialization::serialize(obj, m_stype);
    m_sender->send(bytes.data(), bytes.size(), timeout, m_conf.topic);
  }

protected:
  dunedaq::nwqueueadapters::networkobjectsender::Conf m_conf;
  std::shared_ptr<ipm::Sender> m_sender;
  serialization::SerializationType m_stype;
};
} // namespace dunedaq::nwqueueadapters

#endif // NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTSENDER_HPP_
