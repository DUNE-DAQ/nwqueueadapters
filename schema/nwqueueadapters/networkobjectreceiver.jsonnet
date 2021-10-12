// The schema used by NetworkObjectReceiver

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local s = moo.oschema.schema("dunedaq.nwqueueadapters.networkobjectreceiver");

// Object structure used by NetworkObjectReceiver
local nor = {

  connname: s.string("ConnectionName", doc="Connection name to send to"),

  topic: s.string("Topic", doc="A topic to subscribe to"),

  subscriptions: s.sequence("Subscriptions", self.topic, doc="List of topics to subscribe to"),
  
  conf: s.record("Conf",  [
    s.field("name", self.connname, "default", doc="Connection name for connection"),
    s.field("subscriptions", self.subscriptions, [],
      doc="List of topics to subscribe to. Empty-string topic subscribes to everything")
   ], doc="NetworkObjectReceiver Configuration"),
  
};

moo.oschema.sort_select(nor)

