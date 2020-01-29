# RX5.1
Firmware for the esk8 rx unit HW5.1.x

# Features
## Communication

- COBS-package-based communication
- Request-based data transmission to the remote
- Rawinput mode
- Failsafe

## Motor controller

- Multiple ride modes (1 Down btn)
  - Hybrid (speed based)
  - Eco
  - Cruise 
  - Sport (No FW limits)

- Automatic mode switch according to the battery state (10%/25%/25%/25%)
- Cruise control (1)

## Ride controller

- Speedometer (1)
- Odometer (1)
- Current trip (1)
- "Continue last trip" feature (0)
- Trip controller (2)
  - Average speed (3)
- Left disance approximation (1)
- Benchmark mode (Saves realtime data to the array and saves it to file) (2)

## BMS

- Battery volatge (2)
  -Frequent updates with fast exp. filter smoothing
- Battery percents calc (1)
  -Gets voltage for calculations only without throttle
- Battery capacity left/consumed (3) 
  -Based on battery percents

## Light controller

-General modes (1 up btn):
  -Off
  -Rear only
  -Rear+front
  -Auto
    - Automatic light switch acording  to enviroment brightness

- 2 front lights modes (2)
  - High   beam (3)
  - Dipped beam (3)
- 3 rear lights modes (2)
  - Blink + Full brightness (3)
  - Half  + Full brightness (3)
  - None  + Full brightness (3)
- Underlights (2)

## Logging

- Logging to SD card of the
  - Speed
  - Trip
  - Mode
  - Battery voltage
- Benchmark log files
