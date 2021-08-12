/**
 * @file Serialization_test.cxx Serialization namespace Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/NetworkObjectReceiver.hpp"
#include "nwqueueadapters/NetworkObjectSender.hpp"
#include "serialization/Serialization.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE NOR_NOS_test // NOLINT

#include "boost/test/data/test_case.hpp"
#include "boost/test/unit_test.hpp"

#include <string>
#include <thread>
#include <vector>

// A type that's made serializable "intrusively", ie, by changing the type itself
struct MyTypeIntrusive
{
  int count;
  std::string name;
  std::vector<double> values;

  DUNE_DAQ_SERIALIZE(MyTypeIntrusive, count, name, values);
};

BOOST_AUTO_TEST_SUITE(NOR_NOS_test)

BOOST_DATA_TEST_CASE(NetworkObjectSenderReceiver, boost::unit_test::data::make({ "json", "msgpack" }))
{
  // This function is run in a loop with the two serialization
  // types. Sometimes we get back to the top of the loop before the
  // inproc connection is closed, and we get an "address already in
  // use" error. Hack around that by just sleeping here
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  dunedaq::nwqueueadapters::networkobjectsender::Conf sender_conf;
  sender_conf.ipm_plugin_type = "ZmqSender";
  sender_conf.stype = sample;
  sender_conf.address = "inproc://foo";

  dunedaq::nwqueueadapters::networkobjectreceiver::Conf receiver_conf;
  receiver_conf.ipm_plugin_type = "ZmqReceiver";
  receiver_conf.address = "inproc://foo";

  dunedaq::nwqueueadapters::NetworkObjectSender<MyTypeIntrusive> sender(sender_conf);
  dunedaq::nwqueueadapters::NetworkObjectReceiver<MyTypeIntrusive> receiver(receiver_conf);

  MyTypeIntrusive m;
  m.count = 3;
  m.name = "foo";
  m.values.push_back(3.1416);
  m.values.push_back(2.781);

  sender.send(m, std::chrono::milliseconds(2));
  MyTypeIntrusive m_recv = receiver.recv(std::chrono::milliseconds(2));

  BOOST_CHECK_EQUAL(m_recv.count, m.count);
  BOOST_CHECK_EQUAL(m_recv.name, m.name);
  BOOST_CHECK_EQUAL_COLLECTIONS(m_recv.values.begin(), m_recv.values.end(), m.values.begin(), m.values.end());
}

BOOST_AUTO_TEST_SUITE_END()
