FeeLights Lighting System
=========================

A lighting system that helps dancers feel the mood
--------------------------------------------------

The goal of this system is to make a dance-room feel alive by merging music and lighting into a single, smooth experience.

The controller device uses LED strips setup around the room to light the walls and ceilings with visualisations well suited to the music being played in the room. Powered by a machine learning model, the controller is not only able to adjust to the tempo and overall energy of the song you hear, but also adjust the scene to its' emotional undertones. Using an Adaptive Gain microphone, the system will keep adjusting to the mood from the most pumped-up beginning, to the late-night, mellow end.

## Architecture

The system is designed to be autonomous and easy to setup, requiring only a power supply. Dance venues don't always come with WiFi or good cellular service, but there is usually enough power for sound and lights. Given that, power consumption of the system is not a consideration.

![Software Component Diagram](assets/software-component-diagram.png)

The heart of the system is the Lightning Controller, which takes audio input, uses the DSP and ML libraries to get all the necessary information to produce a lighting scene, which it can then output onto LED strips using the Rendering component.

![Hierarchy of Control Diagram](assets/hierarchy-of-control-diagram.png)

## System State Machine

The system is designed to be as autonomous as possible, so in normal operation it defaults to power-on-and-forget. If no interaction is detected after 5 seconds from boot, the device will enter Normal Operation mode. 

Nonetheless, after first installation or when issues are observed, the system needs to provide a service mode, which will allow the user to a) set the length of installed LED strips, b) inspect if the connected LED strips have all LED diodes operational and the power distribution allows 100% intensity along the full length of the strip(s), and c) make sure the device microphone is able to capture the surrounding sound. The service mode will only be accessible on startup and use the device LCD screen. If service mode is entered, the system will go into Normal Operation mode after 30 seconds of user inactivity.

![System state machine](assets/system-sm.png)
