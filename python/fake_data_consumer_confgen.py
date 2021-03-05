# Set moo schema search path
from dunedaq.env import get_moo_model_path
import moo.io
moo.io.default_load_path = get_moo_model_path()

# Load configuration types
import moo.otypes
moo.otypes.load_types('appfwk-cmd-schema.jsonnet')
#moo.otypes.load_types('nwqueueadapters-QueueToNetworkAdapterDAQModule-schema.jsonnet')
moo.otypes.load_types('nwqueueadapters-NetworkToQueueAdapterDAQModule-schema.jsonnet')
moo.otypes.load_types('nwqueueadapters-FakeDataConsumer-schema.jsonnet')
moo.otypes.load_types('serialization-NetworkObjectReceiver-schema.jsonnet')

# Import new types
import dunedaq.appfwk.cmd as cmd # AddressedCmd, 
import dunedaq.nwqueueadapters.networktoqueueadapterdaqmodule as ntoq
import dunedaq.nwqueueadapters.fakedataconsumer as fdc
import dunedaq.serialization.networkobjectreceiver as nor

from appfwk.utils import mcmd, mspec

import json
import math

def generate():
    
    # Define modules and queues
    queue_specs = cmd.QueueSpecs([
            cmd.QueueSpec(inst="hose", kind='FollySPSCQueue', capacity=10000)
        ])
    
    mod_specs = [
        mspec("fdc", "FakeDataConsumer",
              [cmd.QueueInfo(name="input", inst="hose", dir="input")]
              ),

        mspec("ntoq", "NetworkToQueueAdapterDAQModule",
              [cmd.QueueInfo(name="output", inst="hose", dir="output")]
              )
    ]

    init_specs = cmd.Init(queues=queue_specs, modules=mod_specs)

    initcmd = cmd.Command(
        id=cmd.CmdId("init"),
        data=init_specs
    )


    confcmd = mcmd("conf",
                   [
                       ("fdc", fdc.Conf(nIntsPerVector=10,
                                        starting_int=-4,
                                        ending_int=14,
                                        queue_timeout_ms=100)
                        ),
                       
                       ("ntoq", ntoq.Conf(msg_type="dunedaq::nwqueueadapters::fsd::FakeData",
                                          msg_module_name="FakeData",
                                          receiver_config=nor.Conf(ipm_plugin_type="ZmqReceiver",
                                                                   address= "tcp://127.0.0.1:12345")
                                          )
                        )
                   ]
                   )

    startpars = cmd.StartParams(run=1234)
    startcmd = mcmd("start", [
            ("", startpars)
    ])

    emptypars = cmd.EmptyParams()

    stopcmd = mcmd("stop", [
            ("", emptypars)
    ])

    # Create a list of commands
    cmd_seq = [initcmd, confcmd, startcmd, stopcmd]

    # Print them as json (to be improved/moved out)
    jstr = json.dumps([c.pod() for c in cmd_seq], indent=4, sort_keys=True)
    return jstr
        
if __name__ == '__main__':
    # Add -h as default help option
    CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

    import click

    @click.command(context_settings=CONTEXT_SETTINGS)
    @click.argument('json_file', type=click.Path(), default='nwqueueadapters-fake-data-consumer-job.json')
    def cli(json_file):
        """
          JSON_FILE: Input raw data file.
          JSON_FILE: Output json configuration file.
        """

        with open(json_file, 'w') as f:
            f.write(generate())

        print(f"'{json_file}' generation completed.")

    cli()
    
