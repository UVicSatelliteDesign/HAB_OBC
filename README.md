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
The file that runs the entire program is [main.c](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18-documentation/Core/Src/main.c).
> [Main.c](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18-documentation/Core/Src/main.c) is run based on the function [StartPollingLoop](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/fa95cb4608e997175df61c51ad960b2182373af1/Core/Src/main.c#L936). 
As a method of further understanding the code, tracing backwards starting from the [StartPollingLoop](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/fa95cb4608e997175df61c51ad960b2182373af1/Core/Src/main.c#L936) function assists in simplifying the flow of the code.

https://github.com/UVicSatelliteDesign/HAB_OBC/blob/3626df974995ceb8e7b035839624b04891ad77e6/Core/Src/main.c#L936-L959

#### Explanation
The function includes an infinite loop that runs multiple functions upon satisfaction of different conditions. These include [checkBattery](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L240-L246) , [checkLocation](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L259-L272) and [lowpowerMode](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L190-L201). The only function void of any condition is [logData](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L167-L175).

The [checkBattery](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L240-L246) function utilises the [getVoltage](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L203-L220) function and initiates [lowpowerMode](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L190-L201) if the value is less than a particular predecided value.

[getVoltage](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L203-L220) reads the value of a particular ADC pin and gives the value after convesrion using a macros.

[checkLocation](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L259-L272) utilises the function [getLocation](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/c6c52318ced085b472d8ff25e9015fa5ad5add45/Core/Src/HAWB_APRS.c#L21-L23) to read the latitude and the longitude as well as the [checkAltitude](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/c6c52318ced085b472d8ff25e9015fa5ad5add45/Core/Src/main.c#L248-L257) function. If the functions satisfy certain conditions, then the [cutBalloon](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/c6c52318ced085b472d8ff25e9015fa5ad5add45/Core/Src/main.c#L177-L188) function. Each of the functions have their general meaning.

[lowpowerMode](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L190-L201) runs an infinite loop that uses interupts to stall and system. This reduces the load on the system. Additionally, it continuously logs data as well using the [logData](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L167-L175) function.

[logData](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/18c05590419fd35818cc9dfefb51043be98964c4/Core/Src/main.c#L167-L175) uses the [fetchData](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/c6c52318ced085b472d8ff25e9015fa5ad5add45/Core/Src/main.c#L156-L165)
> Captures all the data using every function. A structure consisting of all the possible values

 and the [write_Data](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/c6c52318ced085b472d8ff25e9015fa5ad5add45/Core/Src/data_storage.c#L34-L50) functions under certian conditins. 

The [write_Data](https://github.com/UVicSatelliteDesign/HAB_OBC/blob/c6c52318ced085b472d8ff25e9015fa5ad5add45/Core/Src/data_storage.c#L34-L50) function writes the data to the flash memory.














