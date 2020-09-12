//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <FastPID.h>
#include <max6675.h>
#include <Wire.h>


/*******************************************************

Smokey Boi

********************************************************/

//pid config params
int output_bits = 8;
bool output_signed = false;

// default
//float Kp=0.1, Ki=0.5, Kd=0, Hz=10;

// This was slow
// float Kp=0.25, Ki=0.3, Kd=0.9, Hz=10;
// float resetLimit = 0.85;

// 9-12 Very good, Still under/over shooting
float Kp=50.0, Ki=0.6, Kd=5, Hz=10;
float resetLimit = 0.95;

// Maybe increase D value, next resetLimit decrease
//float Kp=50.0, Ki=0.6, Kd=20, Hz=10;
//float resetLimit = 0.95;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);


// Menu variables
bool enable = false;

// Loop vars
unsigned long currentTime;
unsigned long lastTime=0, lastThermo=0;
unsigned long delayTime = Hz;
unsigned long currentThermo;
const long delayThermo = 5000;
int pidStep;

// Temp Pins
int thermoDO = 3;
int thermoCSb = 11;
int thermoCSa = 12;
int thermoCLK = 13;
MAX6675 thermocouple(thermoCLK, thermoCSa, thermoDO);
uint8_t degree[8]  = {140,146,146,140,128,128,128,128};
int temp;



// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int fanpwm = 11;
int lcd_key     = 0;
int adc_key_in  = 0;
int set_temp = 80;
//int set_temp = 175;
float tempb = 200.51;

//Fan action
//unsigned long previousCheckFanMillis = 0;
//const long fanInterval = 2000;

//Display Refresh
unsigned long previousCheckDispMillis = 0;
const long dispInterval = 1000;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;



 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 Serial.begin(9600);
 lcd.createChar(0, degree);
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 pinMode(fanpwm, OUTPUT);
 //myPID.setOutputRange(100,255);
 
 // wait for MAX chip to stabilize
  delay(500);
  currentThermo = millis();
  temp = round(thermocouple.readFahrenheit());
  lastThermo = currentThermo;
}

void loop()
{
  currentThermo = millis();
  if (currentThermo - lastThermo >= delayThermo){
    temp = round(thermocouple.readFahrenheit());
    lastThermo = currentThermo;  // Updates lastTime
    Serial.println(temp);
  }
  
  currentTime = millis();
  if(currentTime - lastTime >= delayTime){
  
  //This needs to be blink without delay
  pidStep = myPID.step(set_temp, temp);
  lastTime = currentTime;  // Updates lastTime
  }
 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     set_temp=set_temp+1;
     lcd.setCursor(6,0);            // move cursor to second line "1" and 9 spaces over
     lcd.print("   ");      // display seconds elapsed since power-up
     delay(80);
     break;
     }
   case btnLEFT:
     {
     set_temp=set_temp-1;
     lcd.setCursor(6,0);            // move cursor to second line "1" and 9 spaces over
     lcd.print("   ");      // display seconds elapsed since power-up
     delay(80);
     break;
     }
   case btnUP:
     {
     set_temp=set_temp+10;
     lcd.setCursor(6,0);            // move cursor to second line "1" and 9 spaces over
     lcd.print("   ");      // display seconds elapsed since power-up
     delay(80);
     break;
     }
   case btnDOWN:
     {
     set_temp=set_temp-10;
     lcd.setCursor(6,0);            // move cursor to second line "1" and 9 spaces over
     lcd.print("   ");      // display seconds elapsed since power-up
     delay(80);
     break;
     }
   case btnSELECT:
     {
        if(enable==true){
          myPID.clear();
        }
        enable =! enable;
        
        lcd.clear();
        delay(1000);
     break;
     }
     case btnNONE:
     {
     break;
     }
 }

    
    
    // Display Refresh Interval

      if (enable){
      lcd.setCursor(0,0);
      lcd.print("Tgt: ");
      lcd.print(set_temp);
          //lcd.write((byte)0);
      } else {
        lcd.setCursor(0,0);
        lcd.print("OFF: ");
        lcd.print(set_temp);
          //lcd.write((byte)0);
      }
      lcd.setCursor(9,0);
      lcd.print("    ");
      lcd.setCursor(9,0);
      lcd.print(round(pidStep/255.00*100));
      lcd.print("%");
      lcd.setCursor(0,1);
      lcd.print("A:");
          //int shorta = tempa+0.5 Round up
          //lcd.print(round(tempa));
      lcd.print(temp);
          //lcd.write((byte)0);
      lcd.setCursor(9,1);
      lcd.print("B:");
      lcd.print(round(tempb));

if (enable) {
  // Not within resetLimit% of target, clear pids set max speed
  if (temp < (set_temp * resetLimit)){
    lcd.setCursor(9,0);
    lcd.print("100%");
    analogWrite(fanpwm, 255);
    myPID.clear();
  } else {
    // Don't turn fan on when requested value is too low
    if (pidStep < 130){
      analogWrite(fanpwm, 0);
    } else {
      analogWrite(fanpwm, pidStep);
    }
  }
} else {
  analogWrite(fanpwm, 0);
}
delay(250);
 
}
