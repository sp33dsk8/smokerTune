# smokerTune
Ugly Drum Smoker (UDS) controlled via Arduino

This 55 gallon drum smoker temperature controller is an ATC (Automatic Temperature controller) and is built on Drone stabilization control logic (PID) using an Arduino UNO.  I use a LCD to display current temp and adjust setpoint, large 12v fan (Muffin XL MD12B1) to push air through a 3/4" metal pipe and a MAX6675 Themocouple at the cooking level to determine current temperature.


The 55 gallon drum was built using this guide: https://smokingmeatgeeks.com/ugly-drum-smoker-plans-uds-build/

PID Library: https://github.com/mike-matera/FastPID

LCD with buttons DFRobot V1.0: https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_

Thermocouple: https://www.amazon.com/gp/product/B07QBPGVZZ/

Fan: https://www.digikey.com/en/product-highlight/c/comair-rotron/muffin-xl-series-fans

The fan is controlled by a TIP120 Transistor at 12v via a ~100 ohm resistor to limit current on the control pin.
