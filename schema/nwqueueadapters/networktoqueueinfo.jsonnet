// This is the application info schema used by NetworkToQueue
// It describes the information object structure passed by the application 
// for operational monitoring

local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.nwqueueadapters.networktoqueueinfo");

local info = {
    uint8  : s.number("uint8", "u8",
                     doc="An unsigned of 8 bytes"),

   info: s.record("Info", [
       s.field("received_count",    self.uint8, 0, doc="Number of messages received on network"), 
       s.field("pushed_count",      self.uint8, 0, doc="Number of items successfully pushed to queue"), 
       s.field("push_failed_count", self.uint8, 0, doc="Number of timeouts pushing to queue"), 
   ], doc="NetworkToQueue information")
};

moo.oschema.sort_select(info) 
