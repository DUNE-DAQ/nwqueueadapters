// The schema used by classes in the appfwk code tests.
//
// It is an example of the lowest layer schema below that of the "cmd"
// and "app" and which defines the final command object structure as
// consumed by instances of specific DAQModule implementations (ie,
// the test/Fake* modules).

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local ns = "dunedaq.networkqueue.queuetonetworkadapterdaqmodule";
local s = moo.oschema.schema(ns);

// Object structure used by the test/fake producer module
local qton = {
  name: s.string("Name", ".*",
    doc="Name of a plugin etc"),

  any: s.any("Data", doc="Any"),

  conf: s.record("Conf", [
    s.field("msg_type", self.name,
      doc="The fully-qualified name of the type in the queue"),
    s.field("msg_module_name", self.name,
      doc="The name of the plugin containing the code for the message type"),
    s.field("sender_config", self.any,
      doc="Configuration for the NetworkObjectSender"),
  ], doc="NetworkToQueueAdapterDAQModule Configuration"),
};

moo.oschema.sort_select(qton)
