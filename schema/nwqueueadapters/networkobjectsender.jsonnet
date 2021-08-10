// The schema used by NetworkObjectSender

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local s = moo.oschema.schema("dunedaq.nwqueueadapters.networkobjectsender");

// Object structure used by NetworkObjectSender
local nos = {

  stype: s.string("SerializationString", doc="String describing serialization type"),
  
  ipmtype: s.string("IPMPluginType", doc="IPM plugin type"),
  
  address: s.string("Address", doc="Address to send to"),

  topic: s.string("Topic", doc="A topic string for publisher-type senders"),

  timeout : s.number("Count", "u8",
                     doc="A number of milliseconds to wait for something"),
  
  conf: s.record("Conf",  [
    s.field("stype", self.stype, "msgpack",
      doc="Serialization type"),
    s.field("ipm_plugin_type", self.ipmtype, "ZmqSender",
      doc="IPM plugin type"),
    s.field("address", self.address, "inproc://default",
      doc="Address to send to"),
    s.field("topic", self.topic, "",
      doc="topic for publisher-type senders"),
    s.field("control_timeout", self.timeout, 1000, doc="Timeout for sending the initial control message")
  ], doc="NetworkObjectSender Configuration"),
  
};

moo.oschema.sort_select(nos)

