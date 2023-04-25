# Command Line Interface for the Simulation #

The Python script is intended to provide command access to the simulation from command line. Call the script with the parameter `-h` for information on the supporte actions and parameter:

```python3 cli.py -h```

Attention: The communication is just in direction of the simulation without any confirmation (fire-and-forget/asynchronous).

The communication configuration is read from a default configuration file `config.json`. A different configuration file can be provided with the parameter `-cf`. If no configuration file is provided, target IP and port must be specified by parameter or a default value is used.