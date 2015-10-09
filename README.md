# office-control
some bits of code to control my desk and work environment

## desk_control_toggle:
Toggles a motorized sit/stand desk between sitting position and standing position.

### Hardware/Circuit Requirements:
 * (2) 5v coil, 20V+/0.5A+ relay (or some electrically activated switch/transistor that matches the values of your desk controls)
 * (1) motor-controlled desk with a simple 2-direction, 3-conductor control interface
 * (1) normally-open momentary switch
 * (2) normally-open reed switches
 * (2) arrays of (14) 6.4mm x 1.8mm disc neodymium magnets taped to max/min
 * wires and cables and stuff
 ** (I used:
 **  - A leviton-type RJ45 jack to connect the circuit to the controller
 **  - 3.5mm stereo jacks to connect the button to the circuit

### This would be more efficient/expensive with:
 * reed switches at the max and min points
 * some way to directly measure height/distance

## shush:
Simple momentary key control from a single momentary switch mounted to my desk.
Used to control Shush: http://mizage.com/shush/

It's a Push-to-talk/toggle/cough button for microphones.
Very handy for anybody who does a lot of group audio/videoconferencing.
