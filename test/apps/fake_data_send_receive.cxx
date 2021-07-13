/**
 * @file fake_data_send_receive.cxx
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/fakedataconsumer/Nljs.hpp"
#include "nwqueueadapters/fakedataproducer/Nljs.hpp"
#include "nwqueueadapters/networktoqueue/Nljs.hpp"
#include "nwqueueadapters/queuetonetwork/Nljs.hpp"

#include "appfwk/DAQModule.hpp"
#include "appfwk/QueueRegistry.hpp"
#include "appfwk/app/Nljs.hpp"
#include "ipm/ZmqContext.hpp"

#include "logging/Logging.hpp" // NOLINT

#include <iostream>
#include <map>
#include <memory>
#include <string>

using namespace dunedaq::appfwk;
using namespace dunedaq::nwqueueadapters;

int
main()
{
  TLOG() << "Creating Module instances...";
  std::shared_ptr<DAQModule> fake_data_producer = make_module("FakeDataProducer", "producer");
  std::shared_ptr<DAQModule> fake_data_sender = make_module("QueueToNetwork", "sender");
  std::shared_ptr<DAQModule> fake_data_receiver = make_module("NetworkToQueue", "receiver");
  std::shared_ptr<DAQModule> fake_data_consumer = make_module("FakeDataConsumer", "consumer");

  std::map<std::string, QueueConfig> queue_map;
  queue_map["producer_output"] = QueueConfig{ QueueConfig::kStdDeQueue, 10 };
  queue_map["consumer_input"] = QueueConfig{ QueueConfig::kStdDeQueue, 10 };
  QueueRegistry::get().configure(queue_map);

  // Init
  TLOG() << "Calling init on modules...";
  app::ModInit producer_init_data;
  app::QueueInfo producer_output_queue_info{ "producer_output", "output", "out" };
  producer_init_data.qinfos.push_back(producer_output_queue_info);
  nlohmann::json producer_init_json;
  to_json(producer_init_json, producer_init_data);
  fake_data_producer->init(producer_init_json);

  app::ModInit sender_init_data;
  app::QueueInfo sender_output_queue_info{ "producer_output", "input", "in" };
  sender_init_data.qinfos.push_back(sender_output_queue_info);
  nlohmann::json sender_init_json;
  to_json(sender_init_json, sender_init_data);
  fake_data_sender->init(sender_init_json);

  app::ModInit receiver_init_data;
  app::QueueInfo receiver_output_queue_info{ "consumer_input", "output", "out" };
  receiver_init_data.qinfos.push_back(receiver_output_queue_info);
  nlohmann::json receiver_init_json;
  to_json(receiver_init_json, receiver_init_data);
  fake_data_receiver->init(receiver_init_json);

  app::ModInit consumer_init_data;
  app::QueueInfo consumer_output_queue_info{ "consumer_input", "input", "in" };
  consumer_init_data.qinfos.push_back(consumer_output_queue_info);
  nlohmann::json consumer_init_json;
  to_json(consumer_init_json, consumer_init_data);
  fake_data_consumer->init(consumer_init_json);

  // Conf
  TLOG() << "Calling conf on modules...";
  fakedataproducer::Conf producer_config;
  nlohmann::json producer_conf_json;
  to_json(producer_conf_json, producer_config);
  fake_data_producer->execute_command("conf", producer_conf_json);

  queuetonetwork::Conf sender_config;
  sender_config.msg_type = "dunedaq::nwqueueadapters::fsd::FakeData";
  sender_config.msg_module_name = "FakeData";
  nlohmann::json sender_conf_json;
  to_json(sender_conf_json, sender_config);
  fake_data_sender->execute_command("conf", sender_conf_json);

  networktoqueue::Conf receiver_config;
  receiver_config.msg_type = "dunedaq::nwqueueadapters::fsd::FakeData";
  receiver_config.msg_module_name = "FakeData";
  nlohmann::json receiver_conf_json;
  to_json(receiver_conf_json, receiver_config);
  fake_data_receiver->execute_command("conf", receiver_conf_json);

  fakedataconsumer::Conf consumer_config;
  nlohmann::json consumer_conf_json;
  to_json(consumer_conf_json, consumer_config);
  fake_data_consumer->execute_command("conf", consumer_conf_json);

  // Start
  TLOG() << "Calling start on modules...";
  fake_data_consumer->execute_command("start");
  fake_data_producer->execute_command("start");

  TLOG() << "Waiting for 10 seconds...";
  sleep(10);

  // Stop
  TLOG() << "Calling stop on modules...";
  fake_data_producer->execute_command("stop");
  fake_data_consumer->execute_command("stop");

  // Scrap
  TLOG() << "Calling scrap on modules...";
  fake_data_sender->execute_command("scrap");
  fake_data_receiver->execute_command("scrap");

  TLOG() << "Test complete";
}
