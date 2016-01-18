# Tabbouleh - Arduino code
### Project Status
> Under development - The code is not yet final and it is still a work in progress

### Introduction
This code provides a command line interface for the [Arduino] (or ATMEL chip) via serial port that allows configuring, reading and setting values to the PIN of the Arduino.

This code for the Arduino is part of the larger project [Tabbouleh] - a DIY home automation.

Although there are many projects that use Arduino or [Raspberry PI] for home automation, this specific project aims on creating a generic device based on Arduino for use with different switches and sensors.

The idea is to upload the code once to the ATMEL chip without modification done to the code. The configuration of the devices that are connected to which pin can be done remotely via a command line interface.

### The full idea
It started when I wanted to control the lights in the house based on certian logic. The logic will be feed input from sensors scattered around in the house. This logic will be configurable or at worst easy to code with a set of easy API calls.

The challenge here is to integrate with the current light switches in the house without having a centrelized distribution box.

The communication will be based on bluetooth using a bluetooth module for Arduino - The one that was used for this project was the [HC-06]. The HC-05 also will serve the purpose.

For example, when it's dark and there is movement in the living room, turn on the lights. If it's late at night then probably best to dimm the lights and fade-in the light and fade-out when movement stopped for a certian interval.

The above case is very usefull when going to get a glass of water in the middle of the night when everyone is asleep. It would be very nice that the house would light up the lights dimmed but just enough so you can see the path to the kitchen clearly without disturbing others, and then turn them off when not required.

Another example would be a setting of profiles such as "Guest Visiting" profile where the lights will all be lit or "Movie Time" profile where the house will sense if there is a need to close all the windows to make the house dark or simply dimm the lights during the night. "Sleeping Time" profile is also a good example where all the lights in the house will be turned off and when sensing movement they will be lit dimmed so they won't disturb others.

All this is nice, but the main challenge is to make all of this happen without changing sockets or light switches in the house with expensive systems.

## Down to business
So what does this code do? Let's start with a diagram:

![Sample House Diagram](/images/Sample_House_Diagram.jpg?raw=true "Example 1")

In the above diagram, there are movement sensors, light intensity sensor, moister sensor and light switches. All these sensors and switches communicate via bluetooth to the main control system which uses a Raspberry Pi based system. Each element in the diagram (sensors, motion detectores and switches) are all using the same code. The only differance is the configuraiton done for each one of them.

### Device Abstraction
This Arduino code will provide a user interface via a command line interface (CLI) that is achieved via the serial port via the bluetooth - or for testing purposes, via the normal Serial of the [Arduino IDE].

The PINs of the Arduino that will be used are as follows:

|INDEX | IN PIN | OUT PIN|
|:-:|:-:|:-:|
|**0** | 8 | A0|
|**1** | 9 | A1|
|**2** | 10 | A2|
|**3** | 11 | A3|
|**4** | 12 | A4|
|**5** | 13 | A5|

### 

[//]:# (Links mapping)
[Arduino]: https://www.arduino.cc/
[Raspberry PI]: https://www.raspberrypi.org/
[Tabbouleh]: https://still.did.not.create.a.site
[Arduino IDE]: https://www.arduino.cc/en/Main/Software
