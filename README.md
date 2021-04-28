# Robot

The final project submission for BU ENG EC535 by Asbel and Rahul.

## Hardware Requirements

- 2 BeagleBone Blacks
- TouchScreen and Cape for BeagleBone
- Ethernet Cables along with Router or Managed Switch
- 6 Wheeled Robot Drive Train controlled by PWM.

## Instructions for compiling software

Clone git repo, the `arrowpad/` folder contains the Qt user application.
Build it with `qmake && make`. Copy the application generated on to the beaglebone.

`robot` contains the kernel module for controlling the robot. It needs to be built against `linux-4.19.82-ti-rt-r33` kernel and copied over to the beaglebone.

`udp server` is the user application that acts as the command server and can be built using `make`.

## Wiring Instructions

## Connections

Connect both beaglebones to ethernet and ensure the ip addresses are correct in `arrowpad` before building the applications.

## Running the Software

On the beaglebone connected to the robot run:
`mknod /dev/robot c 61 0` followed by `insmod ./path/to/robot.ko`. After which start the server with `server`.

On the beaglebone connected to the touch screen:
`./arrowpad` will start the touchscreen application.
