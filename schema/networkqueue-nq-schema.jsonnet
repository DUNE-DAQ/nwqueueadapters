local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local ns = "dunedaq.networkqueue.nq";
local s = moo.oschema.schema(ns);

local networkqueue = {
  data: s.any("Data",
    doc="An opaque object holding lower layer substructure"),

  duration : s.number("Duration", "i4",
    doc="A duration in milliseconds"),
  
  ntoqconf: s.record("NetworkToQueueAdapterConf", [
    s.field("queue_timeout_ms", self.duration, 100,
      doc="Milliseconds to wait on queue before timing out"),
    s.field("connection_info", self.data,
      doc="Connection info for the IPM socket"),
  ], doc="NetworkToQueueAdapterDAQModule Configuration"),

  qtonconf: s.record("QueueToNetworkAdapterConf", [
    s.field("queue_timeout_ms", self.duration, 100,
      doc="Milliseconds to wait on queue before timing out"),
    s.field("connection_info", self.data,
      doc="Connection info for the IPM socket"),
  ], doc="QueueToNetworkAdapterDAQModule Configuration"),
  
};

moo.oschema.sort_select(networkqueue, ns)

