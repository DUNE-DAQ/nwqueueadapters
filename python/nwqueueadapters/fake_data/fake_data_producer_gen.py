# Set moo schema search path
from dunedaq.env import get_moo_model_path
import moo.io
moo.io.default_load_path = get_moo_model_path()

from pprint import pprint
pprint(moo.io.default_load_path)
# Load configuration types
import moo.otypes
moo.otypes.load_types('rcif/cmd.jsonnet')
moo.otypes.load_types('appfwk/cmd.jsonnet')
moo.otypes.load_types('appfwk/app.jsonnet')

moo.otypes.load_types('nwqueueadapters/queuetonetwork.jsonnet')
moo.otypes.load_types('nwqueueadapters/fakedataproducer.jsonnet')
moo.otypes.load_types('nwqueueadapters/networkobjectsender.jsonnet')


# Import new types
import dunedaq.cmdlib.cmd as basecmd # AddressedCmd, 
import dunedaq.rcif.cmd as rccmd # AddressedCmd, 
import dunedaq.appfwk.cmd as cmd # AddressedCmd, 
import dunedaq.appfwk.app as app # AddressedCmd,

import dunedaq.nwqueueadapters.queuetonetwork as qton
import dunedaq.nwqueueadapters.fakedataproducer as fdp
import dunedaq.nwqueueadapters.networkobjectsender as nos

from appfwk.utils import mcmd, mrccmd, mspec

import json
import math
from pprint import pprint


#===============================================================================
def acmd(mods: list) -> cmd.CmdObj:
    """ 
    Helper function to create appfwk's Commands addressed to modules.
        
    :param      cmdid:  The coommand id
    :type       cmdid:  str
    :param      mods:   List of module name/data structures 
    :type       mods:   list
    
    :returns:   A constructed Command object
    :rtype:     dunedaq.appfwk.cmd.Command
    """
    return cmd.CmdObj(
        modules=cmd.AddressedCmds(
            cmd.AddressedCmd(match=m, data=o)
            for m,o in mods
        )
    )

#===============================================================================
def generate(
        NETWORK_ENDPOINTS: dict
):
    cmd_data = {}

    # Define modules and queues
    queue_specs = [
        app.QueueSpec(inst="hose", kind='FollySPSCQueue', capacity=10000)
    ]

    mod_specs = [
        mspec("fdp", "FakeDataProducer",
              [app.QueueInfo(name="output", inst="hose", dir="output")]
              ),

        mspec("qton", "QueueToNetwork",
              [app.QueueInfo(name="input", inst="hose", dir="input")]
              )
    ]

    cmd_data['init'] = app.Init(queues=queue_specs, modules=mod_specs)

    cmd_data['conf'] = acmd([
        ("fdp", fdp.Conf(
            nIntsPerVector=10,
            starting_int=-4,
            ending_int=140,
            queue_timeout_ms=100,
            wait_between_sends_ms=10
        )),
        ("qton", qton.Conf(msg_type="dunedaq::nwqueueadapters::fsd::FakeData",
                           msg_module_name="FakeData",
                           sender_config=nos.Conf(ipm_plugin_type="ZmqPublisher",
                                                  address=NETWORK_ENDPOINTS["fake_data"],
                                                  topic="foo")
                           )
         )
    ])

    startpars = rccmd.StartParams(run=1, disable_data_storage=False)
    cmd_data['start'] = acmd([
        ("qton", startpars),
        ("fdp", startpars),
    ])

    cmd_data['pause'] = acmd([])

    cmd_data['resume'] = acmd([])
    
    cmd_data['stop'] = acmd([
        ("fdp", None),
        ("qton", None),
    ])

    cmd_data['scrap'] = acmd([
    #     ("tpm", None),
    ])

    return cmd_data
