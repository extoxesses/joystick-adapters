# Joystick Adapters

[![cpp-standard-style](https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat)](https://standardjs.com/)

**Joystick Adapters** is a collection of arduino-based scketches developted to allow simple connections between old joystick and rentro-console contollers, and modern PC (via mini USB/USB C connector), and to be used with emulators (i.e. [RetroArch](https://www.retroarch.com/) or [BatoceraLinux](https://batocera.org/)).


[Here](https://www.shortlist.com/lists/history-of-the-video-game-controller) there is an interesting collection of consoles controllers.

## Prerequisite
### Hardware 
For sake of simplicity, I choose to use an Arduino-based board, named Pro Micro by Spurkfun, that has an integrated HID interface.

[Here](./docs/ProMicro16MHzv1.pdf) there is a pdf file containing the pin-out map.

### Software Dependencies
* [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-1.0)
  It allows to mask the Arduino interface, showing him to host system as a joystic controller, simplifying driver management.

## Libraries

* Commodore
  * [Turbo Contoller](./turbo/turbo.md)
