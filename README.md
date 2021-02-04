# networkqueue

DUNE `DAQModule`s that connect `appfwk` queues to [IPM](https://github.com/DUNE-DAQ/ipm) network connections. Implementation is in the `QueueToNetworkAdapterDAQModule` and `NetworkToQueueAdapterDAQModule` classes ("QToN" and "NToQ" for short). The type that's held by the queue must be (de)serializable by the functions in [the DUNE DAQ serialization package](https://github.com/DUNE-DAQ/serialization). For each type you want to use, you also have to generate a plugin to be used by QToN or NToQ, as follows:

```cpp
#include "networkqueue/NetworkQueueMacros.hpp"

#include "MyClass.hpp"
#include "MyClassSerializationFunctions.hpp" // if the serialization functions don't live in MyClass.hpp

DEFINE_DUNE_NETWORKQUEUE(mynamespace::MyClass)
```

Build the plugin with `daq_add_plugin(MyClass duneNetworkQueue LINK_LIBRARIES networkqueue)`. In the job configuration, pass the following parameters to NToQ/QToN at conf time:

```json
"msg_type": "mynamespace::MyClass",
"msg_module_name": "MyClass",
```

The "msg_type" should match the argument to `DEFINE_DUNE_NETWORKQUEUE()` and "msg_module_name" should match the plugin name in `daq_add_plugin()`. For an actual example, see [`FakeData.cpp`](./test/plugins/FakeData.cpp). The queue which is the input/output to QToN/NToQ should be passed at init time in the usual way, with name "input"/"output" as appropriate. A full job configuration example can be found in the python directory.

## Design and implementation

A natural way to implement QToN and NToQ would be as classes templated over the type they're sending/receiving, ie `QueueToNetworkAdapterDAQModule<T>`, but `DAQModule`s are loaded as plugins using [cetlib](https://cdcvs.fnal.gov/redmine/projects/cetlib/wiki), which doesn't support plugin names with template arguments, so we need a slightly more complicated setup. QToN cannot itself depend on a template parameter, but it must call code that _does_ depend on the sent/received type (eg, to do `T t = serialization::deserialize(bytes)`). So we "hide" the code that depends on the sent/received type in `QueueToNetworkImpl<T>`, which inherits from `QueueToNetworkBase`. `QueueToNetworkAdapterDAQModule` holds a `QueueToNetworkBase*`, via which it interacts with the underlying `QueueToNetworkImpl<T>`. For a given `T`, the  `QueueToNetworkImpl<T>` is instantiated in the cetlib plugin defined by the `DEFINE_DUNE_NETWORKQUEUE` macro. (The same technique is used for `NetworkToQueueAdapterDAQModule`, making the appropriate substitutions).

The "inner" plugin used by QToN/NToQ uses essentially the same technique as used for `DAQModule`s, with a slight variation. Whereas the `DEFINE_DUNE_DAQ_MODULE` macro defines a `make` function that is essentially:

```cpp
DAQModule* make() {
  return new ConcreteDAQModule;
}
```

`DEFINE_DUNE_NETWORKQUEUE` defines macros with arguments; roughly:

```cpp
QueueToNetworkBase* makeQToN(string plugin_name) {
  if(plugin_name=="mynamespace::MyClass") return new QueueToNetworkImpl<mynamespace::MyClass>();
  else return nullptr;
}
```

This requires a slightly more complicated configuration (we have to pass both "msg_type" and "msg_module_type"), but it decouples the name of the plugin's shared library file from the name of the sent/received class. I think this has some potential advantages:

1. The fully-qualified name of the class can be specified in the config file (ie, in "msg_type")
2. "msg_type"s that take template parameters can be specified (eg, if you wanted to wrap a `MyClass<T>`)
3. One shared library file can service multiple classes (have the `makeQToN` function switch on `plugin_name`). This is potentially useful for `moo`-generated classes, where many classes may be generated from the same file

