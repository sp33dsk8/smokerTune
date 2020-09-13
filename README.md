# smokerTune
Ugly Drum Smoker (UDS) controlled via Arduino

This 55 gallon drum smoker temperature controller is an ATC (Automatic Temperature controller) and is built on Drone stabilization control logic (PID) using an Arduino UNO.  I use a LCD to display current temp and adjust setpoint, large 12v fan (Muffin XL MD12B1) to push air through a 3/4" metal pipe and a MAX6675 Themocouple at the cooking level to determine current temperature.


The 55 gallon drum was built using this guide: https://smokingmeatgeeks.com/ugly-drum-smoker-plans-uds-build/

FastPID Library v1.3.2: https://github.com/mike-matera/FastPID/releases/tag/v1.3.2

LCD with buttons DFRobot V1.0: https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_

Standard Arduino LiquidCrystal library v1.0.7: https://github.com/arduino-libraries/LiquidCrystal/releases/tag/1.0.7

Thermocouple: https://www.amazon.com/gp/product/B07QBPGVZZ/

Thermocouple Library from Adafruit v1.1.0: https://github.com/adafruit/MAX6675-library/releases/tag/1.1.0

Fan: https://www.digikey.com/en/product-highlight/c/comair-rotron/muffin-xl-series-fans

The fan is controlled by a TIP120 Transistor at 12v via a 180 ohm resistor from base pin to arduino control pin to limit the current down to 20mA.

----
Pins

| Arduino Pin 	| Device Pin                                  	|
|-------------	|---------------------------------------------	|
| 0           	| LCD Button Analog pin                       	|
| 3           	| D0 - Thermocouple                           	|
| 4-9         	| LCD                                         	|
| 11          	| TIP120 Base pin (Fan Controller transistor) 	|
| 12          	| CSa - Thermocouple                          	|
| 13          	| Clk - Thermocouple                          	|
