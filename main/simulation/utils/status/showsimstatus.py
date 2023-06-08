#! python3
"""
Tool for displayint the status of a simulation based
on the received status messages.

Depending on the given parameters, objects for either
showning the state as ASCII-waterfall or for showning
the raw messages are passed to the message receiver/
parser.
"""
import time
import argparse

import statusrecvudp
import dataconverter

class ShowRawMessage:
    def handle_msg(self, msg):
        print(str(time.time()).ljust(18), msg, flush=True)

class ShowGPIOASCI:
    def __init__(self, indent=""):
        dataconverter.show_ascii_headline(indent)
    def notify_gpio_changed_to(self, sensors, actuators, analog):
        v_in = sensors
        v_out = actuators
        # show values on terminal
        print(str(time.time()).ljust(18), " ", end="")
        dataconverter.show_ascii(dataconverter.parse(v_in, 'in'))
        dataconverter.show_ascii(dataconverter.parse(v_out, 'out'))
        print(format(v_in,'#06x'), format(v_out,'#06x'), format(analog,'#06x'), "", flush = True)

if __name__ == "__main__":
    receiver_ip = '127.0.0.1'
    receiver_port = 6000
    receiver_ip = ''
    showChangesASCII = False
    showRawMessages = False

    # parse commandline parameter here
    parser = argparse.ArgumentParser()
    parser.add_argument("-ip", "--IP", help="Local IP for receiving simulation status information. Default: 127.0.0.1")
    parser.add_argument("-p", "--port", help="Port for receiving simulation status information. Default: 60000", type=int)
    parser.add_argument("--showASCII", help="show GPIO changes on console", action="store_true")
    parser.add_argument("--showRawMsg", help="shows the raw status messages send by the simulation", action="store_true")
    parser.add_argument("--signals", help="shows table of signals", action="store_true")
    args = parser.parse_args()

    if args.signals:
        dataconverter.show_signals()
    if args.IP:
        receiver_ip = args.IP
    if args.port:
        receiver_port = args.port
    if args.showASCII:
        showChangesASCII = True
    if args.showRawMsg:
        showRawMessages = True

    receiver = statusrecvudp.SimStatusFacadeUDP(receiver_ip, receiver_port)
    if showChangesASCII:
        print("Configured ASCII")
        receiver.register_gpio_changed_observer(ShowGPIOASCI("".rjust(20)))
    if showRawMessages:
        print("Configured Raw Messages")
        receiver.register_msg_reception_observer(ShowRawMessage())
    try:
        while True:
            time.sleep(30)
    except:
        receiver.close()
    
