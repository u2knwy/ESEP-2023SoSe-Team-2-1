# Festo Transfer System QNX-Simulation #

The simulation for the Festo Transfer system mimics the C-Interface to the hardware and parts of the hardware. As the user interacts with the hardware, the simulation can simulate the user interactions by pre-programmed actions or via external control.

## QNX-I/O-API ##

The simulation provides a similar interfaces to simulated hardware of the Beagle Bone Black with QNX and the mechanical system. The behaviour of the GPIO and the interrupt system is nearly identical with the real hardware, the ADC is reduced to a simplified system.

### GPIO-Interface ###
Provides access to GPIO bank 0, 1, and 2 of the BeagleBone black. Depending on the need for controlling the real Festo Transfer System the following registers are simulated:

- OE
- IRQSTATUS\_1
- IRQSTATUS\_SET\_1
- RAISINGDETECT
- FALLINGDETECT
- CLEARDATAOUT
- SETDATAOUT
- DATAIN

Hence, only raising and falling edge interrupts for interrupt line 1 are supported. (Interupt line on real hardware causes some trouble anyhow.)

The GPIO registers are accessed via `in32()` and `out32()`.

### ADC Support ###
The ADC in the simulation diviates from the BBB ADC. The handling protocol is simplified and sampling is interlocked with the simulation cycle. 

#### ADC Interupt Handling ####
The ADC generates an IRQ with number 16 after completion of sampling.

To enable the ADC interrupt on sampling complete, set the bit 1 in the register IRQ\_ENABLE\_SET register. To disable the IRQ, write 1 to bit 1 in the IRQ\_ENABLE\_CLR register. On read of the IRQ\_ENABLE\_SET register, the current enable status is given.

The status of the IRQ can be read from the IRQ\_STATUS register. Bit 1 is set if IRQ has been raised. To clear the IRQ-flag, write 1 to bit 1 in the IRQ\_STATUS register, otherwise no new sampling can be started.

#### Sampling ####

The ADC is preconfigured, except the IRQ handling. To enable the sampling of one vaule, IRQ handling has to be configured, no pending interrupt and the bit 0 in CTRL register has to be set (auto-cleared). The sampled value is then provided in the DATA register. The IRQ is issued on sampling completion. The DATA register is only updated on IRQ, hence IRQ handling must be configured.

#### ADC Registers ####
ADC registers are located form base address 0x44E0D000 on. The register set has a range of 0x00002000.

```
(rw) IRQ_ENABLE_SET 0x2c
(wo) IRQ_ENABLE_CLR 0x30
(rw) IRQ_STATUS 0x28
(rw) CTRL 0x40
(ro) DATA 0x100
```

### Interrupt Handling ###
In the current release, only the sending of pulse messages on interrupt by means of InterruptAttachEvent and the sending of pulse messages as the result of an ISR is supported. 


## QNX Simulation API ##

### Acces to simulated Time ###

The simulation is started via a singleton of type `SimulationStarterQNX` that is accessible via the global variable `simulationStarter`. Hence, only one instance of the simulation exists. Via the singleton, the simulation can be querried for the current simulation time in milliseconds by the method `currentSimTime()`.

Example for displaying current simulation time:

```
std::cout << simulationStarter->currentSimTime() << std::endl;
```

With the macro `SIM_SHOW_TIMESTAMP` is defined, the time stamp of the simulation is printed in each simulation cycle.

### Manual Start ###
The simulation can be configured to wait for an internal or external start command. 

For internal start via C/C++ function call, the signleton `simulationStarter` provides a method `startSimulation()` to start the simulation on demand.

Example:

```
simulationStarter->startSimulation();
```

To enable this feature, the macro `SIM_MANUAL_START` has to be defined globally. This feature cannot be combined with a configuration for system B of a Twin-system.

## Usage ##
### Header Files ###
All simulation sources have to be inlcuded into the QNX-project. 
The simulation core runs independend from other parts of the programm. Access to the simulated hardware is provided by redirection of QNX-functions for GPIO-access, ADC-access and interrupt services. 

To use the redirection, dedicated include files from the simulation package have to be included into the source-files (.cpp) ***as last include***!

The following functions are replaced by the simulation with the header file `simqnxirqapi.h`:

- InterruptAttach
- InterruptAttachEvent
- InterruptMask
- InterruptUnmask 
- InterruptDetach

InterruptMask and InterruptUnmask are in comparison to the QNX documetation not recursive!
 
The following functions are replaced by the simulation with the header file `simqnxgpioapi.h`:

- out32 
- in32
- mmap\_device\_io

### Settings / Makefile ###
For compilation, the include-pathes have to be set to each directory in the Makefile. The pathes are either copys from the repository or logical links (Mac/Linux) to the repository (use the later), so the names used here may need adaptation.

```
INCLUDES += -Isrc/simulationbase
INCLUDES += -Isrc/simulationadapterqnx
INCLUDES += -Isrc/simulationudpqnx
INCLUDES += -Isrc/simulationcore
```

If the sources of the folder `simudp` are added in the project, the library socket has to be added to the list of libraries for linking in the Make-file. Look for the two lines shown and and add the third.

```
#LIBS += -L/path/to/my/lib/$(PLATFORM)/usr/lib -lmylib
#LIBS += -L../mylib/$(OUTPUT_DIR) -lmylib
LIBS += -lsocket
```

For information on additional settings (Macros) to enable/disable additional features in the simulation, see other documentation.

## Simulation Time ##

### Time Resolution ###
The simulation cylce is set to 20 ms.

## Predefined Actions ##
The simulation starts automaticically if not otherwise configured. The following actions are preprogrammed, but commented out:

- pressing and release of start-button two times.
- adding and removing of an item two times.

For more information on preprogramming of actions, see other documentaion.
