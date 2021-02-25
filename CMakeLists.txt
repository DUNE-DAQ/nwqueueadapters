cmake_minimum_required(VERSION 3.12)
project(nwqueueadapters VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(appfwk REQUIRED)
find_package(ipm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(serialization REQUIRED)

##############################################################################
# Schema

daq_codegen(*.jsonnet TEMPLATES Structs.hpp.j2 Nljs.hpp.j2 )

##############################################################################
# Main library

# We don't have a real library, but we want to create a
# nwqueueadapters::nwqueueadapters target for dependents to be able to
# depend on
daq_add_library(LINK_LIBRARIES ipm::ipm serialization::serialization)

##############################################################################

# Test stuff
daq_add_plugin( FakeDataConsumer duneDAQModule TEST LINK_LIBRARIES nwqueueadapters )
daq_add_plugin( FakeDataProducer duneDAQModule TEST LINK_LIBRARIES nwqueueadapters )

daq_add_plugin( QueueToNetwork duneDAQModule LINK_LIBRARIES nwqueueadapters)
daq_add_plugin( NetworkToQueue duneDAQModule LINK_LIBRARIES nwqueueadapters )

daq_add_plugin( FakeData duneNetworkQueue TEST LINK_LIBRARIES nwqueueadapters )

##############################################################################

daq_install()
