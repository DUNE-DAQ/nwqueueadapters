cmake_minimum_required(VERSION 3.12)
project(networkqueue VERSION 0.0.0)

find_package(daq-cmake 1.0.0 REQUIRED)

daq_setup_environment()

find_package(appfwk REQUIRED)
find_package(ipm REQUIRED)
find_package(nlohmann_json REQUIRED)

##############################################################################
# Main library

# We don't have a real library, but we want to create a
# networkqueue::networkqueue target for dependents to be able to
# depend on
daq_add_library(dummy.cpp LINK_LIBRARIES ipm::ipm)

##############################################################################

# Test stuff
daq_add_plugin( FakeSerializableDataConsumerDAQModule duneDAQModule TEST LINK_LIBRARIES networkqueue appfwk::appfwk )
daq_add_plugin( FakeSerializableDataProducerDAQModule duneDAQModule TEST LINK_LIBRARIES networkqueue appfwk::appfwk )

daq_add_plugin( QueueToNetworkAdapterDAQModuleOfFakeData duneDAQModule TEST LINK_LIBRARIES networkqueue appfwk::appfwk ipm::ipm)
daq_add_plugin( NetworkToQueueAdapterDAQModuleOfFakeData duneDAQModule TEST LINK_LIBRARIES networkqueue appfwk::appfwk ipm::ipm)

##############################################################################

daq_install()
