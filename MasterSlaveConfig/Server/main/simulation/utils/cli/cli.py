import argparse
import json
import os
import sys
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../visualisation')))
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../gui')))

import simulationcommandinterface as sci

from dataclasses import dataclass

@dataclass
class Arguments:
    target_IP: str = '127.0.0.1'
    target_port:int = 40000
    configfilename: str = 'config.json'

if __name__ == '__main__':
    # Argparse
    # - IP
    # - port
    # - kind, ...
    parser = argparse.ArgumentParser(description='Send commands to the Fest Transfer-System Simulaiton.')
    parser.add_argument('command', action='store',
                        help='Select the action: add, remove-all, remove-all-slide, remove-id, configuration')
    parser.add_argument('-tip', '--targetip', 
                        dest='target_IP', type=str,
                        help='Overwrite IP-adress of target system (simulation) from configuration file.')
    parser.add_argument('-tp', '--targetport', 
                        dest='target_port', type=int,
                        help='Overwrite UDP-port to send comannds on target to from configuration file.')
    parser.add_argument('-kind', action='store',
                        dest='kind', 
                        help='Select the type of Item: f, mu, md, hu, hd, cn with n for binary code.  Default: flat',
                        default='f')
    parser.add_argument('-flip', action='store_true',
                        dest='flip', 
                        help='Simulate the flipping of the item while passing to next system. Default: no flip',
                        default=False)
    parser.add_argument('-sticky', action='store_true',
                        dest='sticky', 
                        help='Simulate a slow movement in feed separator. Default: not sticky',
                        default=False)
    parser.add_argument('-at', action='store',
                        dest='at', 
                        help='Schedule command for given simulation time. Default: now ',
                        default=0)
    parser.add_argument('-cf', '--configfile', 
                        dest='configfilename', type=str,
                        help='File name with communication configuration information.')
    parser.add_argument('-x', '--x', 
                        dest='x', type=float,
                        help='x-coordinate to place the item at.',
                        default = 0)
    parser.add_argument('-y', '--y', 
                        dest='y', type=float,
                        help='y-coordinate to place the item at.',
                        default=60)
    parser.add_argument('-p', '--p', 
                        dest='p', type=str,
                        help='parameter to configure.')
    parser.add_argument('-v', '--v', 
                        dest='v', type=str,
                        help='new value of configuration parameter, "on" is true / "off" is false',
                        default='off')
    parser.add_argument('-id', '--id', 
                        dest='id', type=int,
                        help='ID of the item to remove, only in combination with remove-id',
                        default=0)
    
    args = parser.parse_args()

    arguments = Arguments()
    if args.configfilename is not None:
        arguments.configfilename = args.configfilename
    if os.path.isfile(arguments.configfilename):
        with open(arguments.configfilename) as configfile:
            configuration = configfile.read()
            try:
                configuration_dir = json.loads(configuration)
            except json.decoder.JSONDecodeError as err:
                print(f"Error in config file {arguments.configfilename}")
                print(err)
            else:
                if 'targetip' in configuration_dir:
                    arguments.target_IP = configuration_dir['targetip']
                else:
                    print("Configfile error, targetip-entry not found!")
                if 'targetport' in configuration_dir:
                    arguments.target_port = configuration_dir['targetport']
                else:
                    print("Configfile error, targeport-entry not found!")
    arguments.target_IP = args.target_IP if args.target_IP is not None else arguments.target_IP
    arguments.target_port = args.target_port if args.target_port is not None else arguments.target_port
    
    ci = sci.SimulationCommandInterface(target=arguments.target_IP, port=arguments.target_port)
    
    if args.command == 'add':
        if args.kind.lower() == 'f':
            ci.add_item_flat(flip=args.flip, sticky=args.sticky, at=args.at, x=args.x, y=args.y)
        elif args.kind.lower() == 'mu':
            ci.add_item_metalup(flip=args.flip, sticky=args.sticky, at=args.at, x=args.x, y=args.y)
        elif args.kind.lower() == 'md':
            ci. add_item_metaldown(flip=args.flip, sticky=args.sticky, at=args.at, x=args.x, y=args.y)
        elif args.kind.lower() == 'hu':
            ci.add_item_holeup(flip=args.flip, sticky=args.sticky, at=args.at, x=args.x, y=args.y)
        elif args.kind.lower() == 'hd':
            ci.add_item_holedown(flip=args.flip, sticky=args.sticky, at=args.at, x=args.x, y=args.y)
        elif args.kind.lower()[0] == 'c':
            code = int(args.kind[1])
            ci.add_item_coded(code, flip=args.flip, sticky=args.sticky, at=args.at, x=args.x, y=args.y)
        else:
            print("Unknown kind to add:", args.kind)
    elif args.command == 'remove-all':
        ci.remove_all(at=args.at)
    elif args.command == 'remove-all-slide':
        ci.remove_all_slide(at=args.at)
    elif args.command == 'remove-all-end':
        ci.remove_all_end(at=args.at)
    elif args.command == 'remove-id':
        ci.remove_id(at=args.at, item_id=args.id)
    elif args.command == 'configure' or args.command == 'configuration':
        parameter = args.p
        value = args.v
        if len(parameter) != 0 and (value in ['on', 'off']):
            value_bool = True if value == 'on' else False
            ci.configure(parameter, value_bool)
    else:
        print("Unknown command:", args.command)
    ci.close()
