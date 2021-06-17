// The schema used by NetworkObjectReceiver

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local s = moo.oschema.schema("dunedaq.nwqueueadapters.networkobjectreceiver");

// Object structure used by NetworkObjectReceiver
local nor = {

  ipmtype: s.string("IPMPluginType", doc="IPM plugin type"),
  
  address: s.string("Address", doc="Address to receive from"),

  subscription: s.string("Subscription", doc="Subscription string"),
  
  conf: s.record("Conf",  [
    s.field("ipm_plugin_type", self.ipmtype, "ZmqSender",
      doc="IPM plugin type"),
    s.field("address", self.address, "inproc://default",
      doc="Address to receive from"),
    s.field("subscription", self.subscription, "",
      doc="Topic to subscribe to. Empty topic subscribes to everything")
   ], doc="NetworkObjectReceiver Configuration"),
  
};

moo.oschema.sort_select(nor)

