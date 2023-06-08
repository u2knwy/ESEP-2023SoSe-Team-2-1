# Macro List #
The simulation feature variants are controlled by the following macros. The macros have to be added to the Make-File to have an effect, for instance for the Twin-Feature:

```
CC = qcc -Vgcc_nto$(PLATFORM)
CXX = qcc -lang-c++ -Vgcc_nto$(PLATFORM) -DSIM_TWIN 
LD = $(CXX)
```

Remember to make a clean and rebuild after changes in the make-file, as the make system does not check the make-file for changes.

## Display Features ##
The simulation is silent except for a startup message. With the following macros, additional outputs can be enabled.

- SIM\_SHOW\_SENSORS
- SIM\_SHOW\_ACTUATORS
- SIM\_SHOW\_ACTIONS
- SIM\_SHOW\_POSITIONS
- SIM\_SHOW\_TIMESTAMP
- SIM\_SHOW\_RIO
- SIM\_SHOW\_CYCLE\_DURATION
- SIM\_SHOW\_REPORT

## System Adjustment ##
If the macro `SIM_PUSHER` is set, the pusher is integrated as feed separator.

## Twin-Operation Features ##
For the usage of the Twin-Macros, see extra document.

- SIM\_TWIN
- SIM\_TWIN\_B

## Special Features ##
For manual start of the simulation processing the macro `SIM_MANUAL_START` enables the corresponding API-function. Cannot be combined with `SIM_TWIN_B`.

The simulation can be configured to start timer on first write access with the macro `SIM_AUTOSTART_ON_WRITE`. Simulaiton behaviour is the same as with `SIM_MANUAL_START`, but the start function call is not required as the initial write access automatically starts the simulation timer. Cannot be combined with `SIM_TWIN_B`.

## External Reporting ##

Line 5 and 6 of the file `simudp.conf` have to contain the host-IP and the port the simulation status report should be send to. Additional listener can be added by additional pairs of IP-adress and port in the following lines.

