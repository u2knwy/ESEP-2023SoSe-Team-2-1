# showsimstatus.py #

Commanl line tool for displaying the current state of the Fest Transfer System simulation. The tool can be configured to display the data in various modes (see start options).

The programm can only be ended by pressing ctrl-c (maybe multiple times).

## Configuration of the Simulation ##
To enable status reporting in the simulationion the following steps are required:

- Enable report messages by setting the macro `SIM_REPORT` in the compile options in the Make-file.
- Include all source file from the folder simudp in the project build.
- Add a file `simudp.conf` in the root folder of the target (Compare notes on Twin-Operation). Add in line 5 the IP-adress and in line 6 the port number the report is send to (typically your development host). The configuration file has to end with an empty new line.

## Parameter ##
- `-ip <IP>` Local IP address where status information is received on. Typically not required.
- `-p <port>` Receive status information on that given port.
- `--showASCII` Show the current state as shortcut waterfall. New line is only shown if a value has been changed.
- `--showRawMsg` Shows the raw JSON string received from the simulation.
- `--signals` Shows Markdown table of the signal abbreviations.

The options are non-exclusive, they can be mixed, but the output may  make no sense anymore.

Example listening on port 6000:

`python3 showsimstatus.py -p 6000 --showASCII`

## In-Signals ##
### Lightbarrier Begin ###
|value|short|long|
|----:|----:|:---|
|    0|    i|interrupted|
|    1|    c|closed|

### Lightbarrier Hight Measurement ###
|value|short|long|
|----:|----:|:---|
|    0|    i|interrupted|
|    1|    c|closed|
### Hight Correct Sensor ###
|value|short|long|
|----:|----:|:---|
|    0|    g|correct height|
|    1|    w|wrong height|
### Lightbarrier Feed Separator ###
|value|short|long|
|----:|----:|:---|
|    0|    i|interrupted|
|    1|    c|closed|
### Metal Sensor###
|value|short|long|
|----:|----:|:---|
|    0|    n|non-metal|
|    1|    m|metal|
### Feed Separator Sensor ###
|value|short|long|
|----:|----:|:---|
|    0|    c|closed|
|    1|    o|open|
### Lightbarrier Slide ###
|value|short|long|
|----:|----:|:---|
|    0|    i|interrupted|
|    1|    c|closed|
### Lightbarrier End ###
|value|short|long|
|----:|----:|:---|
|    0|    i|interrupted|
|    1|    c|closed|
### Button Start ###
|value|short|long|
|----:|----:|:---|
|    0|    p|pressed|
|    1|    r|released|
### Button Stop ###
|value|short|long|
|----:|----:|:---|
|    1|    p|pressed|
|    0|    r|released|
### Button Reset ###
|value|short|long|
|----:|----:|:---|
|    0|    p|pressed|
|    1|    r|released|
### Button E-Stop ###
|value|short|long|
|----:|----:|:---|
|    1|    p|pressed|
|    0|    r|released|
## Out-Signals ##
### Belt ###
|value|short|long|
|----:|----:|:---|
|    0|   st|stop|
|    1|   r |moves right|
|    2|   l |moves left|
|    3|   il|illegal|
|    5|   sr|moves slow right|
|    6|   sl|moves slow left|
|    7|   il|illegal|
|    8|   sc|stop command|
|    9|   sc|stop command|
|   10|   sc|stop command|
|   11|   sc|stop command|
|   12|   sc|stop command|
|   13|   sc|stop command|
|   14|   sc|stop command|
|   15|   sc|stop command|
### Lamp Red ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
### Lamp Yellow ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
### Lamp Green ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
### Feed Separator Open ###
|value|short|long|
|----:|----:|:---|
|    0|    c|closed|
|    1|    o|open|
### LED Start Button ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
### LED Reset Button ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
### LED Q1 ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
### LED Q2 ###
|value|short|long|
|----:|----:|:---|
|    0|    o|off|
|    1|    e|enlighted|
