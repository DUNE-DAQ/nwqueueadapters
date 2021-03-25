// The schema used by classes in the appfwk code tests.
//
// It is an example of the lowest layer schema below that of the "cmd"
// and "app" and which defines the final command object structure as
// consumed by instances of specific DAQModule implementations (ie,
// the test/Fake* modules).

local moo = import "moo.jsonnet";

local s_nor = import "networkobjectreceiver.jsonnet";
local nor = moo.oschema.hier(s_nor).dunedaq.nwqueueadapters.networkobjectreceiver;
// A schema builder in the given path (namespace)
local ns = "dunedaq.nwqueueadapters.networktoqueue";
local s = moo.oschema.schema(ns);

// Object structure used by the test/fake producer module
local ntoq = {
  name: s.string("Name", ".*",
    doc="Name of a plugin etc"),

  conf: s.record("Conf", [
    s.field("msg_type", self.name,
      doc="The fully-qualified name of the type in the queue"),
    s.field("msg_module_name", self.name,
      doc="The name of the plugin containing the code for the message type"),
    s.field("receiver_config", nor.Conf,
      doc="Configuration for the NetworkObjectReceiver"),
  ], doc="NetworkToQueueAdapterDAQModule Configuration"),
};

s_nor + moo.oschema.sort_select(ntoq)
