cmake_minimum_required(VERSION 3.12)
project(networkqueue VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(appfwk REQUIRED)
find_package(ipm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(serialization REQUIRED)

##############################################################################
# Main library

# We don't have a real library, but we want to create a
# networkqueue::networkqueue target for dependents to be able to
# depend on
daq_add_library(LINK_LIBRARIES ipm::ipm serialization::serialization)

##############################################################################

# Test stuff
daq_add_plugin( FakeSerializableDataConsumerDAQModule duneDAQModule TEST LINK_LIBRARIES networkqueue appfwk::appfwk )
daq_add_plugin( FakeSerializableDataProducerDAQModule duneDAQModule TEST LINK_LIBRARIES networkqueue appfwk::appfwk )

daq_add_plugin( QueueToNetworkAdapterDAQModule duneDAQModule LINK_LIBRARIES networkqueue appfwk::appfwk ipm::ipm)
daq_add_plugin( NetworkToQueueAdapterDAQModule duneDAQModule LINK_LIBRARIES networkqueue appfwk::appfwk ipm::ipm)

daq_add_plugin( FakeData duneNetworkQueue TEST LINK_LIBRARIES networkqueue appfwk::appfwk ipm::ipm)

##############################################################################

daq_install()
