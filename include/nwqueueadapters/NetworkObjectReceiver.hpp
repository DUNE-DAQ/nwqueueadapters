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

#include "ipm/PluginInfo.hpp"
#include "ipm/Subscriber.hpp"
#include "networkmanager/NetworkManager.hpp"
#include "nwqueueadapters/networkobjectreceiver/Nljs.hpp"
#include "nwqueueadapters/networkobjectreceiver/Structs.hpp"
#include "serialization/Serialization.hpp"

#include "ipm/Receiver.hpp"

#include <memory> // for shared_ptr
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
  {
    auto is_subscriber = networkmanager::NetworkManager::get().is_pubsub_connection(conf.name);
    auto plugin_type =
      ipm::get_recommended_plugin_name(is_subscriber ? ipm::IpmPluginType::Subscriber : ipm::IpmPluginType::Receiver);
    m_receiver = dunedaq::ipm::make_ipm_receiver(plugin_type);

    nlohmann::json config_json;
    config_json["connection_string"] = networkmanager::NetworkManager::get().get_connection_string(conf.name);
    m_receiver->connect_for_receives(config_json);

    auto m_subscriber = std::dynamic_pointer_cast<ipm::Subscriber>(m_receiver);
    if (m_subscriber) {
      for (auto topic : conf.subscriptions) {
        m_subscriber->subscribe(topic);
      }
    }
  }

  T recv(const dunedaq::ipm::Receiver::duration_t& timeout)
  {
    dunedaq::ipm::Receiver::Response recvd = m_receiver->receive(timeout);
    return serialization::deserialize<T>(recvd.data);
  }

protected:
  std::shared_ptr<ipm::Receiver> m_receiver;
};
} // namespace dunedaq::nwqueueadapters

#endif // NWQUEUEADAPTERS_INCLUDE_NWQUEUEADAPTERS_NETWORKOBJECTRECEIVER_HPP_
