#! python3
"""
Client class for the decoding the report messages from the Festo-Transfer-System
simulation used in the ESEP training.

2020-2023 (c) Thomas Lehmann

"""
import socket
import threading
import logging
import json
import time

class SimulationStatusFacadeUDP():
    """
    Decoder for the incoming report messages from the simulation. The decoded information can be
    retrieved by querry methods or by passing handler objects for various typs of changes.
    """
    def __init__(self, recv_ip, recv_port, logger=logging.getLogger(__name__)):
        self.logger = logger
        self.structlock = threading.Lock()
        self.status_msg = None      # last received message
        self.gpio_observer = []
        self.msg_observer = []
        self.simulation_time = 0
        self.cfg_feed_separator = False
        self.cfg_ejector = False
        self.cfg_system = ''
        self.item_lost_observer = []
        self.item_added_observer = []
        self.items = {}
        self.items_known = {}
        self.items_new_IDs = {}
        self.items_lost_IDs = {}
        self.items_roi_slide = []   # ID-Numbers only
        self.items_roi_slide_entered = []
        self.items_roi_exit = []    # ID-Numbers only
        self.items_roi_exit_entered = []    # ID-Numbers only
        self.sensors = 0
        self.actuators = 0
        self.analog = 0       
        self.sensors_old = 0
        self.actuators_old = 0
        self.receiver = StatusReceiverUDP(recv_ip=recv_ip, recv_port=recv_port, msg_reception_handler=self, logger=logger)
        self.receiver.start()
        #print("started", flush=True)
    def _la(self):
        self.structlock.acquire()
    def _lr(self):
        self.structlock.release()
    def close(self):
        self.receiver.close()
    def handle_status_msg(self, msg_datastruct):
        """
        Parse the given report and update internal mirror. Call call-back-method afterwards.

        param: Dictionary of status fields. Field items ist an array of item-objects as dictionary.
        """
        # this method is as call-back executed in other thread context
        # so we make a copy for local processing.
        self._la()
        self.status_msg = msg_datastruct.copy()
        self._lr()
        
        # get GPIO for faster access
        self.cfg_system = self.status_msg["id"]
        self.sensors = self.status_msg["sensors"]
        self.actuators = self.status_msg["actors"]
        self.analog = self.status_msg["analog"]
        self.simulation_time = self.status_msg["T"]

        # get configuration
        if 'C' in self.status_msg.keys():
            cfg_code = self.status_msg['C']
            self.cfg_feed_separator = (cfg_code[0] == 'F')
            self.cfg_ejector = (cfg_code[0] == 'P')
            
        # check items in system
        self._la()
        self.items_new_IDs = {}
        self.items_lost_IDs = {}
        self.items = self.status_msg["items"]   # ->list of dictionaries
        
        # find new ones
        known = {}                         # -> Map with pair id to item dictionary
        for i in self.items:
            #print(i)
            ID = i["ID"] 
            known[ID] = i
            if ID not in self.items_known.keys():
                self.items_new_IDs[ID] = i
        known_IDs = list(known.keys())     # -> List of Numbers

        # find lost ones
        for k,v in self.items_known.items():
            #print(k,v)
            if k not in known_IDs:
                self.items_lost_IDs[k]=v
        self._lr()

        # remove lost from regions of interresst
        self.items_roi_slide = self.items_roi_slide and known_IDs
        self.items_roi_exit = self.items_roi_exit and known_IDs
        self.items_roi_slide_entered = []
        self.items_roi_exit_entered = []    # ID-Numbers only

        # update list to new status
        self.items_known = known
        
        for k,v in self.items_known.items():
            if v['y'] > 90:
                if k not in self.items_roi_slide:
                    self.items_roi_slide.append(k)
                    self.items_roi_slide_entered.append(k)
                    #print("enterd slide", k, v)
            else:
                if  635 < v['x'] < 675:
                    if k not in self.items_roi_exit:
                        self.items_roi_exit.append(k)
                        self.items_roi_exit_entered.append(k)
                        #print("enterd exit", k, v)
                else:
                    if k in self.items_roi_exit:
                        #print("left exit", k, v)
                        i = self.items_roi_exit.index(k)
                        del self.items_roi_exit[i]

        #print("result:", self.items_known, self.items_new_IDs, self.items_lost_IDs)

        # inform various listeners
        # inform item change listeners
        for ob in self.item_lost_observer:
            ob.handle_lost_items(self.items_lost_IDs, self.items_known)
        
        for ob in self.item_added_observer:
            ob.handle_added_items(self.items_new_IDs, self.items_known)

        # inform message listeners
        #print(self.status_msg)
        for ob in self.msg_observer:
            ob.handle_msg(self.status_msg)

        # inform gpio change listeners
        if self.sensors_old != self.sensors or self.actuators_old != self.actuators:
            #print(sensors, actuators, analog, flush=True)
            for ob in self.gpio_observer:
                ob.notify_gpio_changed_to(self.sensors, self.actuators, self.analog)
        self.sensors_old = self.sensors
        self.actuators_old = self.actuators

    def register_gpio_changed_observer(self, ob):
        if hasattr(ob, "notify_gpio_changed_to"):
            self.gpio_observer.append(ob)

    def register_msg_reception_observer(self, ob):
        if hasattr(ob, "handle_msg"):
            self.msg_observer.append(ob)

    def register_item_appeared_observer(self, ob):
        if hasattr(ob, "handle_appeared_items"):
            self.item_added_observer.append(ob)
            
    def register_item_lost_observer(self, ob):
        if hasattr(ob, "handle_lost_items"):
            self.item_lost_observer.append(ob)
    def get_new_IDs(self):
        self._la()
        IDs = list(self.items_new_IDs)
        self._lr()
        return IDs
    def get_all_lost_IDs(self):
        self._la()
        IDs = list(self.items_lost_IDs)
        self._lr()
        return IDs
    def get_sensors_raw(self):
        self._la()
        result = self.sensors
        self._lr()
        return result
    def get_actuators_raw(self):
        self._la()
        result = self.actuators
        self._lr()
        return result
    def get_items(self):
        self._la()
        result = self.items_known.copy()
        self._lr()
        return result
    # Simulation states
    def get_current_simulation_time(self):
        self._la()
        value = self.simulation_time
        self._lr()
        return value
    def get_current_height(self):
        self._la()
        value = self.analog
        self._lr()
        return value
    def get_config_is_feed_separator(self):
        return self.cfg_feed_separator
    def get_config_is_ejector(self):
        return self.cfg_ejector
    def get_config_is_system_b(self):
        return self.cfg_system == 'B'
    def get_config_is_system_client(self):
        return self.cfg_system == 'C'
    # Lightbarrier State 
    def is_open_lb_beginning(self):
        self._la()
        value = (self.sensors & 0x0001) == 0
        self._lr()
        return value
    def is_open_lb_slide(self):
        self._la()
        value = (self.sensors & 0x0040) == 0
        self._lr()
        return value
    def is_open_lb_end(self):
        self._la()
        value = (self.sensors & 0x0080) == 0
        self._lr()
        return value
    def is_button_start_pressed(self):
        self._la()
        value = (self.sensors & 0x0100) != 0
        self._lr()
        return value
    def is_button_stop_pressed(self):
        self._la()
        value = (self.sensors & 0x0200) == 0    # active low
        self._lr()
        return value
    def is_button_reset_pressed(self):
        self._la()
        value = (self.sensors & 0x0400) != 0
        self._lr()
        return value
    def is_button_estop_pressed(self):
        self._la()
        value = (self.sensors & 0x0800) == 0    # active low
        self._lr()
        return value
    def is_separator_open(self):
        self._la()
        value = (self.sensors & 0x0020) == 0    # active low
        self._lr()
        return value
    # Current control commands
    def is_feedseparator_commanded_open(self):
        self._la()
        value = (self.actuators & 0x0080) != 0
        self._lr()
        return value
    def is_lamp_red_on(self):
        self._la()
        value = (self.actuators & 0x0010) != 0
        self._lr()
        return value
    def is_lamp_yellow_on(self):
        self._la()
        value = (self.actuators & 0x0020) != 0
        self._lr()
        return value
    def is_lamp_green_on(self):
        self._la()
        value = (self.actuators & 0x0040) != 0
        self._lr()
        return value
    def is_lamp_start_on(self):
        self._la()
        value = (self.actuators & 0x0100) != 0
        self._lr()
        return value
    def is_lamp_reset_on(self):
        self._la()
        value = (self.actuators & 0x0200) != 0
        self._lr()
        return value
    def is_lamp_q1_on(self):
        self._la()
        value = (self.actuators & 0x0400) != 0
        self._lr()
        return value
    def is_lamp_q2_on(self):
        self._la()
        value = (self.actuators & 0x0800) != 0
        self._lr()
        return value
    # Regions of Interrest
    def get_items_in_roi_slide(self):
        self._la()
        items = self.items_roi_slide[:]
        self._lr()
        return items
    def get_items_in_roi_exit(self):
        self._la()
        items = self.items_roi_exit[:]
        self._lr()
        return items
    def has_entered_exit_any(self):
        result = False
        if self.items_roi_exit_entered:
            result = True
        return result
    def has_entered_exit_flat(self):
        result = False
        for item_id in self.items_roi_exit_entered:
            if self.items_known[item_id]["T"] == "f":
                result = True
        return result
    def has_entered_exit_mettal_up(self):
        result = False
        for item_id in self.items_roi_exit_entered:
            if self.items_known[item_id]["T"] == "MU":
                result = True
        return result
    def has_entered_exit_mettal_down(self):
        result = False
        for item_id in self.items_roi_exit_entered:
            if self.items_known[item_id]["T"] == "MD":
                result = True
        return result
    def has_entered_exit_hole_up(self):
        result = False
        for item_id in self.items_roi_exit_entered:
            if self.items_known[item_id]["T"] == "HU":
                result = True
        return result
    def has_entered_exit_hole_donw(self):
        result = False
        for item_id in self.items_roi_exit_entered:
            if self.items_known[item_id]["T"] == "HD":
                result = True
        return result
    def has_entered_slide_any(self):
        result = False
        if self.items_roi_slide_entered:
            result = True
        return result
    def has_entered_slidet(self):
        result = False
        for item_id in self.items_roi_slide_entered:
            if self.items_known[item_id]["T"] == "f":
                result = True
        return result
    def has_entered_slide_mettal_up(self):
        result = False
        for item_id in self.items_roi_slide_entered:
            if self.items_known[item_id]["T"] == "MU":
                result = True
        return result
    def has_entered_slide_mettal_down(self):
        result = False
        for item_id in self.items_roi_slide_entered:
            if self.items_known[item_id]["T"] == "MD":
                result = True
        return result
    def has_entered_slide_hole_up(self):
        result = False
        for item_id in self.items_roi_slide_entered:
            if self.items_known[item_id]["T"] == "HU":
                result = True
        return result
    def has_entered_slide_hole_donw(self):
        result = False
        for item_id in self.items_roi_slide_entered:
            if self.items_known[item_id]["T"] == "HD":
                result = True
        return result
    
class StatusReceiverUDP(threading.Thread):
    """
    Thread which waits on a socket for incoming status report messages. The incoming
    messages are just translated from JSON to dictionary and passed to the given
    message-handler object for further processing.
    """
    def __init__(self, recv_ip, recv_port, msg_reception_handler=None, logger=logging.getLogger(__name__)):
        threading.Thread.__init__(self)
        self.listening_IP = recv_ip
        self.listening_port = recv_port
        self.socket = None
        self.running = True
        self.logger = logger
        self.msg_rec_handler = None
        self.set_msg_reception_handler(msg_reception_handler)
        #self.start()   # maybe that is critical...
    def set_msg_reception_handler(self, handler):
        """ 
        Replaces the old handler for message handling with
        the given one, if it has a callback method.
        """
        if hasattr(handler, "handle_status_msg"):
            self.msg_rec_handler = handler
    def run(self):
        """ 
        Main loop of thread to receive UDP messages.
        JSON Messages are decoded and published to registered
        observer.
        """
        self.logger.debug("("+str(threading.get_ident())+") "+"Receiver thread started...")
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((self.listening_IP, self.listening_port))
        #print(f"UDP listening on {self.listening_IP} : {self.listening_port}", flush=True)
        while self.running:
            data, addr = self.socket.recvfrom(1024) # buffer size is 1024 bytes
            #print(addr, data)
            if data != b'':
                datastring = data.decode('utf-8')
                if datastring != u"" and datastring != "\n":
                    if datastring[0:2] == '0x':
                        # client format, fake a message
                        gpio_in = int(datastring[0:6], 16) 
                        fake_msg = '{"id": "C", "T": 0, "actors": 0, "sensors": %i, "analog": 3600,  "items": []}' % gpio_in
                        #print(datastring, fake_msg)
                        datastring = fake_msg
                    datastruct = json.loads(datastring)
                    #print(datastruct)
                    if self.msg_rec_handler != None and hasattr(self.msg_rec_handler, "handle_status_msg"):
                        self.msg_rec_handler.handle_status_msg(datastruct)
        self.logger.debug("(MsgReceiver "+str(threading.get_ident())+")"+"Receiver thread ended")
        print('', flush=True)
    def close(self):
        self.running = False

class SimStatusIOObserverDumpConsole:
    def __init__(self, logger):
        self.logger = logger
    def notify_io_changed_to(self, sensors, actuators, analog):
        notification = "changed to Sensors:"+hex(sensors)+" Actuators:"+hex(actuators)+" Analog:"+hex(analog)
        print(notification)
        if self.logger is not None:
            self.logger.info(notification)

if __name__ == '__main__':
    """ 
    If started as script, all GPIO changes are logged for 5 min.
    """ 
    logging.getLogger(__name__).setLevel(logging.INFO)
    target_ip = '192.168.101.20'
    target_ip = ''
    target_port = 6001
    print("receive on ", target_ip, ":", target_port)
    h = SimulationStatusFacadeUDP(target_ip, target_port)
    h.register_gpio_changed_observer(SimStatusIOObserverDumpConsole(None))

    time.sleep(300)
    h.close()
    print("end of main")

