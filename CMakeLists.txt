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
daq_add_plugin( FakeDataConsumer duneDAQModule SCHEMA TEST LINK_LIBRARIES networkqueue )
daq_add_plugin( FakeDataProducer duneDAQModule SCHEMA TEST LINK_LIBRARIES networkqueue )

daq_add_plugin( QueueToNetworkAdapterDAQModule duneDAQModule SCHEMA LINK_LIBRARIES networkqueue)
daq_add_plugin( NetworkToQueueAdapterDAQModule duneDAQModule SCHEMA LINK_LIBRARIES networkqueue )

daq_add_plugin( FakeData duneNetworkQueue TEST LINK_LIBRARIES networkqueue )

##############################################################################

daq_install()
