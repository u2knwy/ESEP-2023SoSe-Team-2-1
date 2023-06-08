# Festo Transfer System Simulation Visualisation #
Version: 13

## Operation ##
### Installation ###
The Visualisation bases on Game-Framework in Python. Hence, Python and the framework have to be installed on the system.

1. Install current Python (>3.7) for your OS (www.python.org).
2. Install the framework `pygame` via the Python package management system (may require root-rights):
```python -m pip install pygame```

### Runnig ###
The simulation has to be compiled with the macro `SIM_REPORT` and `SIM_EXT_CTRL` set and the configuration file adjusted accordingly:

Line 5 and 6 of the file `simudp.conf` have to contain the host-IP this visualiation is running on and the local port as given configuration file or parameter.

The program reads a default configuration file `config.json` for the setup. A different configuration file can be provided with the parameter `-cf`.

The setting of the configuration file can be overwritten by providing parameters in the commandline.
```python3 festovisualisation.py -tip 192.168.178.101 -tp 40629 -lp 6001```
overrides all config-file parameters.

All parameter:

```
-tip   <IP Target, same as line 3 of simudp.conf>
-tp    <Port Target, same as line 4 of simudp.conf>
-lp    <local receive port, same as line 6 of simudp.conf>
``` 
The configuration of system name A or B and pusher or feed separator is received automatically.

### Quit Visualisation ###
For ending the Visualisation, type Ctrl-C in the consol; maybe twice as some threads may not terminate automatically.

## HCI ##

### Simulation Time ###
The displayed simulation time is a good indicator for receiving reports from the simulation. If this number doesnot increment, the simulation or the connection is down.

### Simulation Start Button ###
If the simulation is configured to manual start (`SIM_MANUAL_START`) the button above the simulation time will start the simulation. The simulation time should stay at zero until the button has been pressed and should increase after.
 
### Buttons ###
Clicks on the HCI-buttons 'start', 'stop', 'reset' and 'E-Stop' of the festo transfer systems are transmitted to the simulation. Instead of the real-world interaction, they operate as switch, so they do not have a push-button behaviour regarding the mouse-presses. To press and release a button, two clicks are needed. 

Interaction becomes only visible if the simulation is running and returns the current state of the buttons seen by the simulaiton.

Buttons below the conveyor belt on System A inject new work pieces. There is no collision detection! Previous to injection, the properties `Sticky` and `Flip` can be selected. They are automatically cleared after injection. 

Clear buttons issue a command to clear the section. Below belt for whole system, top for slide only. If system B is configured, work pieces can be removed at end as well.

### Item Visualisation ###
Color coding, does not correspond to the real colors of the items.

- ring coded: grey, without coding displayed
- flat: blue
- metal: white, black hole
- metal flipped: white
- hole: red, black hole
- hole flipped: red

The color coding only appears after they have passed the corresponding sensor.

### Belt Speed and Direction
An arrow on the right side of the belt indicates the speed and the direction of the conveyor belt.