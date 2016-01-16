# Tabbouleh - Arduino code

### Introduction
Although there are many projects that use Arduino for home automation, this specific project aims on creating a generic device based on Arduino for use with different switches.

The idea is to upload the code once to the ATMEL chip and the configuration of which device is connected to which pin can be done remotely. In other words, the only action required here is uploading the sketch to the ATMEL ([Arduino]) and that is it.

### Project Status
Under development - The code is not yet final and it is still a work in progress

### The full idea
It started when I wanted to control the lights in the house based on certian logic. The logic will be feed input from sensors scattered around in the house. This logic will be configurable or at worst easy to code with a set of easy API calls.

The challenge here is to integrate with the current light switches in the house without having a centrelized distribution box.

The communication will based on bluetooth using a bluetooth module 

(for example, when it's dark and there is movement in the living room, turn on the lights. If it's late at night then probably best to dimm the lights and fade-in the light and fade-out when movement stopped for a certian interval).

The above case is very usefull when going to the bathroom. It would be very nice that the house would light up the lights slightly so you can see the path to the bathroom clearly without disturbing others and turn them off when not required.

Another example would

[/]:# (Links mapping)
[Arduino]: https://www.arduino.cc/
