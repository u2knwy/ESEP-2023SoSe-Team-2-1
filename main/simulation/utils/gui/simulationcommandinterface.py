#! python3
"""
API interface for the control of actions with the Festo-Transfer-simulation
use in ESEP. The Interface operates in the direction of the Simulation software
only an does not interpret report messages from the system.

The Command Interface can be used in own programms or from the python shell by
incarnating the SimulationCommandInterface-class.

2020-2023 (c) Thomas Lehmann

Contributers:
2022 Winter term: Niklas Kück
    Injection and Decoration of Binary Workpieces and Lego
    Clear specific Work Pieces
    Injection of Lego Bricks
"""
import socket
import time

class UDPSender:
    def __init__(self, target='127.0.0.1', port=6000):
        self.target = target
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    def send_msg(self, msg):
        if type(msg)==type(""):
            msg = msg.encode('UTF-8')
        #print(type(msg), msg)
        bytes_send = self.socket.sendto(msg, (self.target, self.port))
        #print(bytes_send)
    def close(self):
        self.socket.close()

class SimulationCommandInterface:
    def __init__(self, target='127.0.0.1', port=6000):
        self.sender = UDPSender(target, port)
    def __encode_item_action_msg(self, kind, flip=False, sticky=False, at=0, x=0, y=60): 
        msg = ""
        if kind in ['flat', 'metalup','metaldown', 'holeup', 'holedown', 'code0',  'code1', 'code2', 'code3', 'code4', 'code5', 'code6',  'code7', 'lego1', 'lego2', 'lego3']:
            flipmsg = "true" if flip else "false"
            stickymsg = "true" if sticky else "false"
            msg = '{"type":"itemaction", "atTime": '+str(at)+', "action": "add", "kind": "' + kind + '", "x": '+str(int(x))+', "y": '+str(int(y))+', "f": '+flipmsg+', "sticky": '+stickymsg+' }\n'
        return msg
    def close(self):
        self.sender.close()
    # Simulation Control Commands
    def start_simulation(self):
        """ Start the simulation. Simulation has to be compiled for manual start."""
        msg = '{"type":"simctrl", "action": "start"}\n'
        self.sender.send_msg(msg)
    # Item Commands
    def add_item_flat(self, flip=False, sticky=False, at=0, x=0, y=60):
        """ Add a flat item now at default position. """
        msg = self.__encode_item_action_msg('flat', flip, sticky, at, x, y)
        self.sender.send_msg(msg)
    def add_item_metalup(self, flip=False, sticky=False, at=0, x=0, y=60):
        """ 
        Add an item with metal bore with the metal ring on the top now at default position. 
        The item can be configured to flip on passing to second system 
        or to be sticky at the separator.
        """
        msg = self.__encode_item_action_msg('metalup', flip, sticky, at, x ,y)
        self.sender.send_msg(msg)
    def add_item_metaldown(self, flip=False, sticky=False, at=0, x=0, y=60):
        """ 
        Add an item with metal bore  with the metal ring on the bottom side now at default position. 
        The item can be configured to flip on passing to second system 
        or to be sticky at the separator.
        """
        msg = self.__encode_item_action_msg('metaldown', flip, sticky, at, x ,y)
        self.sender.send_msg(msg)
    def add_item_holeup(self, flip=False, sticky=False, at=0, x=0, y=60):
        """ 
        Add an item with non-metal bore with the bore on the top side now at default position. 
        The item can be configured to flip on passing to second system 
        or to be sticky at the separator.
        """
        msg = self.__encode_item_action_msg('holeup', flip, sticky, at, x ,y)
        self.sender.send_msg(msg)
    def add_item_holedown(self, flip=False, sticky=False, at=0, x=0, y=60):
        """ 
        Add an item with non-metal bore  with the bore on the bottom side now at default position. 
        The item can be configured to flip on passing to second system 
        or to be sticky at the separator.
        """
        msg = self.__encode_item_action_msg('holedown', flip, sticky, at, x, y)
        self.sender.send_msg(msg)
    def add_item_coded(self, code, flip=False, sticky=False, at=0, x=0, y=60):
        """ 
        Add a binary coded item now at default position with given encoding. 
        The item can be configured to flip on passing to second system 
        or to be sticky at the separator.
        """
        if 0 <= code <= 7:
            msg = self.__encode_item_action_msg('code'+str(code), flip, sticky, at, x ,y)
            self.sender.send_msg(msg)
    def add_item(self, kind='flat', flip=False, sticky=False, at=0, x=0, y=60):
        """ 
        Add a item according to the specification.
        The item can be configured to flip on passing to second system 
        or to be sticky at the separator.
        """
        msg = self.__encode_item_action_msg(kind, flip, sticky, at, x, y)
        self.sender.send_msg(msg)
    def remove_all(self, at=0):
        """ 
        Remove all items from the simulation independed of thier position 
        within the system.
        """
        msg = '{"type":"itemaction", "atTime": '+str(at)+', "action": "removeall", "kind": "flat", "x": 0, "y": 60, "f": false, "sticky": false}\n'
        self.sender.send_msg(msg)
    def remove_all_slide(self, at=0):
        """ Remove all items with a position on the slide."""
        msg = '{"type":"itemaction", "atTime": '+str(at)+', "action": "removeallslide", "kind": "flat", "x": 0, "y": 60, "f": false, "sticky": false}\n'
        self.sender.send_msg(msg)
    def remove_all_end(self, at=0):
        """ Remove all items with a position at the end."""
        msg = '{"type":"itemaction", "atTime": '+str(at)+', "action": "removeatend", "kind": "flat", "x": 0, "y": 60, "f": false, "sticky": false}\n'
        self.sender.send_msg(msg)
    def remove_id(self, at=0, item_id=0):
        """ Remove item with the given id."""
        msg = '{"type":"itemaction", "atTime": '+str(at)+', "action": "removeid", "id": '+str(item_id)+'}\n'
        self.sender.send_msg(msg)
    # Button commands
    def press_start_only(self, at=0):
        """ Press and hold the start button and release all other including E-Stop."""
        msg = '{"type" : "hciaction", "atTime": '+str(at)+', "pattern": 1}\n'
        self.sender.send_msg(msg)
    def press_stop_only(self, at=0):
        """ Press and hold the stop button and release all other including E-Stop."""
        msg = '{"type" : "hciaction", "atTime": '+str(at)+', "pattern": 2}\n'
        self.sender.send_msg(msg)
    def press_reset_only(self, at=0):
        """ Press and hold the reset button and release all other including E-Stop."""
        msg = '{"type" : "hciaction", "atTime": '+str(at)+', "pattern": 4}\n'
        self.sender.send_msg(msg)
    def press_emergency_stop_only(self, at=0):
        """ Press and hold the E-Stop and release all other buttons."""
        msg = '{"type" : "hciaction", "atTime": '+str(at)+', "pattern": 8}\n'
        self.sender.send_msg(msg)
    def press_code(self, code, at=0):
        """
        Don't use for manual operations!
        Send activation code to simulation.
        """
        msg = '{"type" : "hciaction", "atTime": '+str(at)+', "pattern": '+str(code)+'}\n'
        self.sender.send_msg(msg)
    def release_all(self, at=0):
        """ Release all buttons """
        msg = '{"type" : "hciaction", "atTime": '+str(at)+', "pattern": 0}\n'
        self.sender.send_msg(msg)
    def configure(self, parameter: str, value: bool, at=0) -> None:
        """ set configuration value """
        #print(parameter, value)
        msg = '{"type" : "simconfaction", "atTime": '+str(at)+', "parameter": "'+ parameter + '", "value": ' + ('true' if value else 'false') + '}\n'
        self.sender.send_msg(msg)
        
if __name__ == '__main__':
    target_ip = '192.168.101.101'
    target_port = 41019
    ci = SimulationCommandInterface(target_ip, port=target_port)
    
    ci.press_start_only()
    time.sleep(1)
    ci.release_all()
    time.sleep(2)
    ci.add_item_flat()
