// The schema used by NetworkObjectSender

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local s = moo.oschema.schema("dunedaq.nwqueueadapters.networkobjectsender");

// Object structure used by NetworkObjectSender
local nos = {

  stype: s.string("SerializationString", doc="String describing serialization type"),
    
  connname: s.string("ConnectionName", doc="Connection name to send to"),

  topic: s.string("Topic", doc="A topic string for publisher-type senders"),
  
  conf: s.record("Conf",  [
    s.field("stype", self.stype, "msgpack",
      doc="Serialization type"),
    s.field("name", self.connname, "default", doc="Connection name for connection"),
    s.field("topic", self.topic, "",
      doc="topic for publisher-type senders")
  ], doc="NetworkObjectSender Configuration"),
  
};

moo.oschema.sort_select(nos)

