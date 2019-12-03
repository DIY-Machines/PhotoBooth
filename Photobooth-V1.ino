/*
 * Arduino controlled Photo Booth - DIY Machines
This is an easy to build Arduino powered photo booth. You can customise the housing to suit your event/wedding and as it's controlled by an Arduino Nano you don't need anyone to 'man it' throughout the night.

==========
More info: https://diymachines.co.uk/
3D printed parts can be downloaded from here: https://www.thingiverse.com/DIY_Machines/designs
Project video: https://youtu.be/Fu5Gbpv4EYs 

==========
 * SAY THANKS:
Buy me a coffee to say thanks: https://ko-fi.com/diymachines
Support us on Patreon: https://www.patreon.com/diymachines
SUBSCRIBE:
■ https://www.youtube.com/channel/UC3jc4X-kEq-dEDYhQ8QoYnQ?sub_confirmation=1
INSTAGRAM: https://www.instagram.com/diy_machines/?hl=en
FACEBOOK: https://www.facebook.com/diymachines/
*/


int shutterPin = 12;
int buttonPin = 8;
int buttonVal = 0;  //somewhere to store the button state
int BUTTONLED = 9;

#define NUM_MAX 4
#define ROTATE 270

#define DIN_PIN 11  // 
#define CS_PIN  10  // 
#define CLK_PIN 13  //  

#include "max7219.h"
#include "fonts.h"

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); // open the serial port at 9600 bps:

pinMode(shutterPin, OUTPUT);
digitalWrite(shutterPin, HIGH);

pinMode(BUTTONLED, OUTPUT);
digitalWrite(BUTTONLED, HIGH);

pinMode(buttonPin, INPUT);
digitalWrite(buttonPin, HIGH);   //enable internal pull up resistor

initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,1);
  sendCmdAll(CMD_INTENSITY,15);

}

void loop() {
  // put your main code here, to run repeatedly:

buttonVal = digitalRead(buttonPin);
Serial.println(buttonVal);
delay(200);
  
if (buttonVal == LOW) {
  digitalWrite(BUTTONLED, LOW);
  takePhoto();
  printStringWithShift("  2 LEFT",0);
  delay(2000);
  takePhoto();
  printStringWithShift("  LAST 1",0);
  delay(2000);
  takePhoto();
  printStringWithShift("  DONE  ",0);
  delay(3000);
  printStringWithShift("           ",0);
  digitalWrite(BUTTONLED, HIGH);
}
                   
  
}


int showChar(char ch, const uint8_t *data)
{
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

// =======================================================================

void printCharWithShift(unsigned char c, int shiftDelay) {
  if (c < ' ' || c > MAX_CHAR) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}

// =======================================================================

void printStringWithShift(const char* s, int shiftDelay){
  while (*s) {
    printCharWithShift(*s++, shiftDelay);
  }
}

// =======================================================================

void takePhoto() {
    printStringWithShift("          10          9           8          7         6         5        4         3        2         1         ",20);
  printStringWithShift("SMILE !",0);
  delay(1000);
  digitalWrite(shutterPin, LOW);    // turn the LED off by making the voltage LOW
  printStringWithShift("            ",0);
  delay(1000);
  digitalWrite(shutterPin, HIGH);
}
