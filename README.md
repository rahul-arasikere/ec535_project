# Robot

The final project submission for BU ENG EC535 by Asbel and Rahul.

## Hardware Requirements

- 2 BeagleBone Blacks (BBB)
- TouchScreen and Cape for BBBB
- Ethernet Cables along with Router or Managed Switch
- 2 Channel PWM Controlled Robot Drivetrain

## Instructions for compiling software

Clone git repo, the `arrowpad/` folder contains the Qt user application.
Build it with `qmake && make`. Copy the application generated on to the BBB.

`robot` contains the kernel module for controlling the robot and it's required `Makefile`.
It needs to be built against `linux-4.19.82-ti-rt-r33` kernel, you can compile it with `make`, and copied over to the BBB.

`udp server` is the user application that acts as the command server and can be built using `make`.

## Wiring Instructions

Our robot required a 12V battery to run, this may differ for your robot.
The ground of the PWM cables need to be connected to any of the ground ports on the BBB.
The power of the PWM cables need to be connected to the 5V port, P9_6, on the BBB.
We used EHRPWM0A/B, P9_21 and P9_22. Since these PWMs output 3.3V logic you may need to use a logic shifter for 5V logic.

## Connections

Connect both beaglebones to ethernet and ensure the ip addresses are correct in `arrowpad` before building the applications.

## Running the Software

On the beaglebone connected to the robot run:
`mknod /dev/robot c 61 0` followed by `insmod ./path/to/robot.ko`.
After which start the server with `./server`.

On the beaglebone connected to the touch screen:
`./arrowpad` will start the touchscreen application.
