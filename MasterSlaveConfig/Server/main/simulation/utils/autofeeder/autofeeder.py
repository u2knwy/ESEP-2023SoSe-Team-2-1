#! python
"""
Autofeeding of items to the simulation

2021 (c) Thomas Lehmann
"""
import argparse
import json
import time
from dataclasses import dataclass
from typing import Dict
import os
import sys
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../visualisation')))
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../gui')))
import simulationreportreceiver as sri
import simulationcommandinterface as sci

class Autofeeder:
    """ Autofeeder waits for state informations. Than the current distance
    to the first item on the belt is calculated and an item added if needed.
    """
    def __init__(self, status, commandinterface, manifest):
        self.status = status
        self.commandinterface = commandinterface
        self.manifest = manifest
        self.manifest_load = manifest["load"]
        self.__check_load()
        self.index = 0
        self.list_ended = False
        self.last_feed_at = time.time()
        self.next_start_at_x = 0
        if self.manifest_load:
            if 'x' in self.manifest_load[self.index]:
                self.next_start_at_x = self.manifest_load[self.index]['x']

    def __check_load(self):
        for item in self.manifest_load:
            if item['distance'] < 40:
                item['distance'] = 40
            if item['distance'] > 750:   # max is 700
                item['distance'] = 710
            if 'x' in item:
                item['x'] = max(item['x'], 0)

    def handle_msg(self, status_msg) -> None:
        #print(status_msg)
        # on any received update
        feed = False
        if (time.time() - self.last_feed_at) > 0.1:  ## add delay for processing 
            item_list = self.status.get_items()
            if item_list:
                # items on belt
                min_distance = 800
                for key, item in item_list.items():
                    if item['y'] <= 80:
                        min_distance = min(item['x'], min_distance)
                #print(min_distance)
                if self.index < len(self.manifest_load):
                    if (min_distance-self.next_start_at_x) >= self.manifest_load[self.index]['distance']:
                        feed = True
            else:
                feed = True
            if feed and not self.list_ended: 
                self.__add_item(self.manifest_load[self.index])
                self.index = self.index+1
                if self.index >= len(self.manifest_load):
                    self.list_ended = True
                else:
                    if 'x' in self.manifest_load[self.index]:
                        self.next_start_at_x = self.manifest_load[self.index]['x']
                    else:
                        self.next_start_at_x = 0
                self.last_feed_at = time.time()

    def __add_item(self, item):
        print("add:", item)
        if 'x' in item and 'y' in item:
            self.commandinterface.add_item(kind=item['kind'], flip=item['flip'], sticky=item['sticky'], at=0, x=item['x'], y=item['y'])
        else:
            self.commandinterface.add_item(kind=item['kind'], flip=item['flip'], sticky=item['sticky'], at=0)

def main(target_ip='127.0.0.1', target_port=40000, receive_port=6001, manifest={}):
    receiver_ip = ''
    rr = sri.SimulationStatusFacadeUDP(recv_ip=receiver_ip, recv_port=receive_port)
    ci = sci.SimulationCommandInterface(target=target_ip, port=target_port)
    
    # creat event observer
    af = Autofeeder(rr, ci, manifest)
    rr.register_msg_reception_observer(af)
    # sleeep
    while not af.list_ended:
        time.sleep(1)
    ci.close()
    rr.close()
    print("list processed")

@dataclass
class Arguments:
    target_IP: str = '127.0.0.1'
    target_port: int = 40000
    receive_port: int = 6001
    configfilename: str = os.path.abspath(os.path.dirname(__file__))+'/config.json'
    manifestfilename: str = 'manifest.json'

if __name__ == '__main__': 
    # Argparse
    # - IP
    # - port
    # - port
    # - filename
    # - filename
    parser = argparse.ArgumentParser(description='Automatically feed items to the simulation.')
    parser.add_argument('-tip', '--targetip', 
                        dest='target_IP', type=str,
                        help='IP-adress of target system (simulation).')
    parser.add_argument('-tp', '--targetport', 
                        dest='target_port', type=int,
                        help='UDP-port to send comannds on target to.')
    parser.add_argument('-lp', '--localport', 
                        dest='receive_port', type=int,
                        help='UDP-port to receive report status information from simulation.')
    parser.add_argument('-cf', '--configfile', 
                        dest='configfilename', type=str,
                        help='File name with configuration information.')
    parser.add_argument('-f', '--file', 
                        dest='filename', type=str,
                        help='Name of the file with the manifest information.')
    
    args = parser.parse_args()
    arguments = Arguments()
    arguments.manifest_dict = {}  ## workaround

        # priority on arguments, add missing defaults
    if args.configfilename is not None:
        arguments.configfilename = args.configfilename
        # load config file 
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
                if 'receiveport' in configuration_dir:
                    arguments.receive_port = configuration_dir['receiveport']
                else:
                    print("Configfile error, receiveport-entry not found!")

    # priority on given arguments
    arguments.target_IP = args.target_IP if args.target_IP is not None else arguments.target_IP
    arguments.target_port = args.target_port if args.target_port is not None else arguments.target_port
    arguments.receive_port = args.receive_port if args.receive_port is not None else arguments.receive_port

    arguments.manifestfilename = args.filename if args.filename is not None else arguments.manifestfilename
    if os.path.isfile(arguments.manifestfilename):
            with open(arguments.manifestfilename) as manifestfile:
                manifestjson = manifestfile.read()
                try:
                    arguments.manifest_dict = json.loads(manifestjson)
                except json.decoder.JSONDecodeError as err:
                    print(f"Error in manifest file {arguments.manifestfilename}")
                    print(err)
    else:
        print("Error, cannot read manifest", os.path.abspath(arguments.manifestfilename))
    print(f"Talk to {arguments.target_IP} on port {arguments.target_port}, listening on {arguments.receive_port}")
    
    main(target_ip=arguments.target_IP, target_port=arguments.target_port, receive_port=arguments.receive_port, manifest=arguments.manifest_dict)
