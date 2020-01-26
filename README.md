# RX5.1
Firmware for the esk8 rx unit HW5.1.x

# Features
## Communication

- COBS-package-based communication
- Request-based data transmission to the remote
- Rawinput mode
- Failsafe

## Motor controller

- Multiple ride modes
  - Hybrid (speed based)
  - Eco
  - Cruise 
  - Sport (No FW limits)

- Automatic mode switch according to the battery state (10%/25%/25%/25%)
- Cruise control

## Ride controller

- Speedometer
- Odometer
- Current trip
- "Continue last trip" feature
- Average speed
- Left disance approximation
- Benchmark mode (Saves realtime data to the array and saves it to file)

## BMS

- Battery volatge
- Battery percents calc.
- Battery capacity left/consumed 

## Light controller

- 3 front lights modes 
  - High   beam
  - Dipped beam
  - Blink  mode
- 3 rear lights modes 
  - Blink + Full brightness
  - Half  + Full brightness
  - Full         brightness)
- Underlights
- Automatic light switch acording to enviroment brightness

## Logging

- Logging to SD card of the
  - Speed
  - Trip
  - Mode
  - Battery voltage
- Benchmark log files
