cmake_minimum_required(VERSION 3.12)
project(nwqueueadapters VERSION 1.4.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(appfwk REQUIRED)
find_package(utilities REQUIRED)
find_package(networkmanager REQUIRED)
find_package(logging REQUIRED)
find_package(ipm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(serialization REQUIRED)
find_package(opmonlib REQUIRED)
find_package(ers REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

##############################################################################
# Schema

daq_codegen(
  fakedataconsumer.jsonnet
  fakedataproducer.jsonnet
  networkobjectreceiver.jsonnet
  networkobjectsender.jsonnet
  networktoqueueinfo.jsonnet
  networktoqueue.jsonnet
  queuetonetworkinfo.jsonnet
  queuetonetwork.jsonnet
  TEMPLATES Structs.hpp.j2 Nljs.hpp.j2
)

daq_codegen(
  fsd.jsonnet
  TEMPLATES Structs.hpp.j2 Nljs.hpp.j2 MsgP.hpp.j2 )

daq_codegen(
  networktoqueueinfo.jsonnet
  queuetonetworkinfo.jsonnet
  DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2
)

##############################################################################
# Main library

# We don't have a real library, but we want to create a
# nwqueueadapters::nwqueueadapters target for dependents to be able to
# depend on
daq_add_library(LINK_LIBRARIES logging::logging ipm::ipm serialization::serialization appfwk::appfwk networkmanager::networkmanager utilities::utilities)

##############################################################################

# Test stuff
daq_add_plugin( FakeDataConsumer duneDAQModule TEST LINK_LIBRARIES nwqueueadapters )
daq_add_plugin( FakeDataProducer duneDAQModule TEST LINK_LIBRARIES nwqueueadapters )

daq_add_plugin( QueueToNetwork duneDAQModule LINK_LIBRARIES nwqueueadapters)
daq_add_plugin( NetworkToQueue duneDAQModule LINK_LIBRARIES nwqueueadapters )

daq_add_plugin( FakeData duneNetworkQueue TEST LINK_LIBRARIES nwqueueadapters )

daq_add_application( fake_data_send_receive fake_data_send_receive.cxx TEST LINK_LIBRARIES nwqueueadapters )
daq_add_application( network_object_send_receive network_object_send_receive.cxx TEST LINK_LIBRARIES nwqueueadapters )
daq_add_application( serialization_speed serialization_speed.cxx TEST LINK_LIBRARIES nwqueueadapters )

##############################################################################

# Unit tests

daq_add_unit_test(NOR_NOS_test  LINK_LIBRARIES nwqueueadapters)

daq_install()
