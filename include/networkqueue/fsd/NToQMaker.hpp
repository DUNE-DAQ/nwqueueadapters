/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 */


#include "networkqueue/fsd/Structs.hpp"
#include "networkqueue/fsd/Nljs.hpp"

#include "appfwk/cmd/Structs.hpp"
#include "appfwk/cmd/Nljs.hpp"

#include "networkqueue/NetworkToQueueAdapterDAQModule.hpp"
#include "networkqueue/Serialization.hpp"
#include "networkqueue/NetworkObjectReceiver.hpp"

namespace dunedaq{

std::unique_ptr<NetworkToQueueBase> NetworkToQueueBaseMaker(std::string msg_type_name, const appfwk::cmd::ModInit& mod_init_data, const dunedaq::networkqueue::nor::Conf& receiver_conf)
{
    

  if(msg_type_name=="FakeData"){
    return std::make_unique<NetworkToQueueImpl<dunedaq::networkqueue::fsd::FakeData>>(mod_init_data, receiver_conf);
  }
    

  if(msg_type_name=="AnotherFakeData"){
    return std::make_unique<NetworkToQueueImpl<dunedaq::networkqueue::fsd::AnotherFakeData>>(mod_init_data, receiver_conf);
  }
  return nullptr;
}

} // namespace dunedaq