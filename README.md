# networkqueue

Proof-of-concept for DUNE `DAQModule`s that connect `appfwk` queues to the network. Implementation is in the `QueueToNetworkAdapterDAQModule<T>` and `NetworkToQueueAdapterDAQModule<T>` classes. The type `T` that is passed over the network must be (de)serializable using `to_json()` and `from_json` functions as described at https://nlohmann.github.io/json/features/arbitrary_types . Creating classes using `moo` schema makes this (relatively) straightforward.

## Example

A simple example uses the `FakeSerializableDataProducerDAQModule` and `FakeSerializableDataConsumerDAQModule` located in `test/plugins/`. The corresponding command schema files are `schema/fsdp-job-with-serialization.json` for the producer and `schema/fsdc-job-with-serialization.json` for the consumer. To run the example, you'll need at least three terminals: one for the producer `daq_application`, one for the consumer `daq_application` and one to send commands to the applications. Eg, with `fileCommandFacility`:

```bash
# Any terminal
mkfifo /tmp/rodrigues/consumer.fifo
mkfifo /tmp/rodrigues/producer.fifo

# Producer terminal
daq_application -c 'file:///tmp/rodrigues/producer.fifo?fmt=jstream'

# Consumer terminal
daq_application -c 'file:///tmp/rodrigues/consumer.fifo?fmt=jstream'

# Control terminal
#  init the producer
jq '.[] | select(.id =="init")' fsdp-job-with-serialization.json > /tmp/rodrigues/producer.fifo
#  init the consumer
jq '.[] | select(.id =="init")' fsdc-job-with-serialization.json > /tmp/rodrigues/consumer.fifo
# ...and similarly for the "conf", "start" and "stop" commands
```

## Limitations

The queue adapters are `DAQModule`s templated on the class type `T` that's being transferred, so the implementation all exists in headers. Normally for template code there's some cpp file that uses the template with a concrete value for `T`, which causes the compiler to generate the actual machine code for the template with the concrete value of `T`. But there is no cpp file directly using, eg `QueueToNetworkAdapterDAQModule<SomeSerializableClass>`, so for every serializable class we create using `moo` codegen, we also create a cpp file that has the necessary `DEFINE_DUNE_DAQ_MODULE(QueueToNetworkAdapterDAQModule<SomeSerializableClass>)` call. There's a further wrinkle in that `cetlib` doesn't allow our class names to contain `<>`, so the generated code first creates a `typedef` of our class that is amenable to `cetlib`, and calls `DEFINE_DUNE_DAQ_MODULE` on that. Configuration code has to use that typedef.

The templates that generate the cpp files (`schema/networkqueue-ntoq.cpp.j2` and `schema/networkqueue-qton.cpp.j2`) have at least two problems:

1. They loop over all the types in the `moo` schema file, creating the typedef and `DEFINE_DUNE_DAQ_MODULE` for each. But `cetlib` requires that we have one `DAQModule` per file (or rather, per library), so this will fail when there is more than one type defined in the `moo` schema.
2. `cetlib` requires that the library name matches the (typedef'ed) class name, but the class name is generated at `moo` render time, so we don't know how to name the output file. In the example, the output filename is currently hardcoded.

Both of these could probably be worked around with `moo render-many` and another layer of codegen (first generate the input for `render-many`, then use `render-many` to generate the output), but at some point it just gets silly.
