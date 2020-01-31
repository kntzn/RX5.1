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
  - Lock
    - Full brake no matter what throttle input is
  - Hybrid (or Neutral)
    - Toggles 0-10% throttle if speed is more than 5 km/h
    - Starts to charge batteries if speed is more than 15 km/h
    - Thr = map (speed, 5 km/h, 15 km/h, 10%, 0%)
    - Thr = map (speed, 15km/h, 20km/h, 0%, -30%)
  - Eco
    - 15 km/h limit
    - 2s ramp-up
  - Cruise 
    - 25 km/h limit 
    - 0.5s ramp-up
  - Sport 
    - No FW limits
  
- Failsafe
  - 1-3s ramp-down time to -100% throttle
- Automatic mode switch according to the battery state (10%/25%/25%/25%)
- Cruise control

## Ride controller

- Speedometer
- Acceleration
- Odometer
- Current trip
- "Continue last trip" feature
- Trip controller
  - Average speed
  - Left disance approximation
- Benchmark mode (Saves realtime data to the array and saves it to file) (2)
  - Speed
  - Acceleration
  - Battery voltage

## BMS

- Battery volatge
   - Frequent updates with fast exp. filter smoothing
- Battery percents calc
  - Gets voltage for calculations only without throttle
- Battery capacity left/consumed
  - Based on battery percents

## Light controller

- General modes
  - Off
  - Rear only
  - Rear+front
  - Auto 
    - Automatic light switch acording  to enviroment brightness

- 3 front lights modes
  - Off             (day           mode)
  - Half brightness (dawn and dusk mode)
  - Dipped beam     (night         mode)
- 3 rear lights modes
  
  | Mode      | Throttling brightness| Breaking brightness |    
  |:---------:|:--------------------:|:-------------------:|
  | Day       |         Off          |         Full        |
  | Dawn&Dusk |         Half         |         Full        |
  | Night     |         Blink        |         Full        |
  
- Underlights

## Logging

- Logging to SD card of the
  - Throttle
  - Speed
  - Trip
  - Mode
  - Odo
  - Lights' and underlights' modes
  - Battery's
    - Voltage
    - Percents
    - Consumption
- Benchmark log files
