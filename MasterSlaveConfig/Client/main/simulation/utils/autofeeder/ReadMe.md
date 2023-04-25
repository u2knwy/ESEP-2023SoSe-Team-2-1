# Autofeeder for the Simulation #

The Python script is intended to automatically add itmes based on distance to the previous item instead of time. Call the script with the parameter `-h` for information on the supporte actions and parameter:

```python3 autofeeder.py -h```

The programm reads the default settings and manifest file. Afterwards the given sequence is executed only once.

## Configuration Parameter
The program reads a default configuration file `config.json` for the communication setup. A different configuration file can be provided with the parameter `-cf`. The autofeeder requires the target-IP, target-port and local receiver UDP-port as parameter.

The setting of the configuration file can be overwritten by providing parameters in the commandline.
```python3 autofeeder.py -tip 192.168.178.101 -tp 40629 -lp 6001```
overrides all config-file parameters.

All parameter:

```
-tip <IP Target, same as line 3 of simudp.conf>
-tp  <Port Target, same as line 4 of simudp.conf>
-lp  <local receive port, same as line 6 of simudp.conf>
-cf  <alternative configuration file>
-f   <alternative manifest file>
``` 

## Manifest
The mainifest file contains a list of items with thier properties added to the system. The file is named `manifest.json` by default. A different file can be use by using the parameter `-f`.

See the example file for information on the JSON-structure.

The distance given is the distance from center to center of the item. Hence, with distance of 40 mm, they are placed close to each other. Every value below 40 mm is automatically set to 40 mm. A distance abouve is limited to 720 mm, whereas the belt is only 700 mm long. 

The specified item is placed on the belt, if

- the belt is empty
- the distance of placement to next item is larger than the specified distance. 

Items in the slide are not taken into account.
