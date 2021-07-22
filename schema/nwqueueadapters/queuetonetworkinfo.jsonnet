// This is the application info schema used by QueueToNetwork
// It describes the information object structure passed by the application 
// for operational monitoring

local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.nwqueueadapters.queuetonetworkinfo");

local info = {
    uint8  : s.number("uint8", "u8",
                     doc="An unsigned of 8 bytes"),

   info: s.record("Info", [
       s.field("popped_count",      self.uint8, 0, doc="Number of items popped from input queue"), 
       s.field("sent_count",        self.uint8, 0, doc="Number of items successfully sent to network"), 
       s.field("send_failed_count", self.uint8, 0, doc="Number of timeouts sending to network"), 
   ], doc="NetworkToQueue information")
};

moo.oschema.sort_select(info) 
