# HIGH ALTIBUTE BALLOON: ONBOARD COMPUTER
The OBC for the HAB is a firmware system built on a STM32 microcontroller.

## Overview
### Description

### Mission and Goals

## Table of Contents
- [Overview](#overview)
  - [Description](#description)
  - [Mission and Goals](#mission-and-goals)
- [Usage](#usage)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Usage 
This OBC is used for controlling the actions based on circumstances during the balloon's upward trajectory.

### Main file
The file that runs the entire program is [main.c](./Core/Src/main.c).
> [Main.c](./Core/Src/main.c) is run based on the function [StartPollingLoop](./Core/Src/main.c#L936). 
As a method of further understanding the code, tracing backwards starting from the [StartPollingLoop](./Core/Src/main.c#L936) function assists in simplifying the flow of the code.
'''c
./Core/Src/main.c#L936-L959
'''
#### Explanation
The function includes an infinite loop that runs multiple functions upon satisfaction of different conditions. These include [checkBattery](./Core/Src/main.c#L240-L246) , [checkLocation](./Core/Src/main.c#L259-L272) and [lowpowerMode](./Core/Src/main.c#L190-L201). The only function void of any condition is [logData](./Core/Src/main.c#L167-L175).

The [checkBattery](./Core/Src/main.c#L240-L246) function utilises the [getVoltage](./Core/Src/main.c#L203-L220) function and initiates [lowpowerMode](./Core/Src/main.c#L190-L201) if the value is less than a particular predecided value.

[getVoltage](./Core/Src/main.c#L203-L220) reads the value of a particular ADC pin and gives the value after convesrion using a macros.

[checkLocation](./Core/Src/main.c#L259-L272) utilises the function [getLocation](./Core/Src/HAWB_APRS.c#L21-L23) to read the latitude and the longitude as well as the [checkAltitude](./Core/Src/main.c#L248-L257) function. If the functions satisfy certain conditions, then the [cutBalloon](./Core/Src/main.c#L177-L188) function. Each of the functions have their general meaning.

[lowpowerMode](./Core/Src/main.c#L190-L201) runs an infinite loop that uses interupts to stall and system. This reduces the load on the system. Additionally, it continuously logs data as well using the [logData](./Core/Src/main.c#L167-L175) function.

[logData](./Core/Src/main.c#L167-L175) uses the [fetchData](./Core/Src/main.c#L156-L165)
> Captures all the data using every function. A structure consisting of all the possible values

 and the [write_Data](./Core/Src/data_storage.c#L34-L50) functions under certian conditins. 

The [write_Data](./Core/Src/data_storage.c#L34-L50) function writes the data to the flash memory.














