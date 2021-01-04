#include <SoftwareSerial.h>
#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Adafruit_SSD1306 display(-1);

//----------------------------------------------VARIABLES-----------------------------------------

float highVoltage;
String number1 = "";
int maxVal = 0;
uint32_t lastSample = 0;
uint32_t lastSample1 = 0;

//variables for error msg handling
boolean sendMsg = true;
boolean isErrorSentOrange = false;
boolean isErrorSentRed = false;
boolean isErrorSentBig = false;
boolean isErrorSentCut = false;
boolean isErrorSentNormal = true;
boolean isStarPressed = false;

//variables for recieving SMS
SoftwareSerial sim(10, 11);
String incomingData = "";
char incomingChar;
String number = "+37128310105";                //Phone number the error message should be sent to

//keypad variables
const byte ROWS = 4;                           //number of keypad rows
const byte COLS = 4;                          //number of keypad columns
byte rowPins[ROWS] = {9, 8, 7, 6};         //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2};         //connect to the column pinouts of the keypad

char keymap[ROWS][COLS] = {                  //keypad layout as 2D array
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);  //initializes an instance of the Keypad class

//variables for tranmiter
RF24 radio(12, 13);
const byte address[6] = "00001";

//----------------------------------------------MAIN SETUP-----------------------------------------
void setup() {

  Serial.begin(9600);
  sim.begin(9600);
  sim.print("AT+CMGF=1\r");
  delay(100);
  sim.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  Serial.println("Wait...");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  delay(5000);
  Serial.println("System Started...");
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();
}
//---------------------------------------------FUNCTIONS-------------------------------------------
//checks if message has been sent ORANGE
void errorMsgOrange() {
  if (isErrorSentOrange == false && sendMsg == true) {
    Serial.println("Error: low voltage, Orange warning!");       //Serial print of error message
    sendMessageOrange();                              //Sends error message SMS
    isErrorSentOrange = true;                         //sets to true so it wont send more than one message
  }
}

//Sends SMS message to phone ORANGE
void sendMessageOrange() {
  if (sim.available() > 0)
    Serial.write(sim.read());
  {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Error: low voltage. Orange warning!";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
  }
}

//checks if message has been sent RED
void errorMsgRed() {
  if (isErrorSentRed == false && sendMsg == true) {
    Serial.println("Error: low voltage. Red warning!");
    sendMessageRed();
    isErrorSentRed = true;
  }
}

//Sends SMS message to phone RED
void sendMessageRed() {
  if (sim.available() > 0)
    Serial.write(sim.read());
  {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Error: low voltage. Red warning!";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
  }
}

//checks if message has been sent BIG
void errorMsgBig() {
  if (isErrorSentBig == false && sendMsg == true) {
    Serial.println("Error: low voltage. Big problems in wire or grounding!");
    sendMessageBig();
    isErrorSentBig = true;
  }
}

//Sends SMS message to phone BIG
void sendMessageBig() {
  if (sim.available() > 0)
    Serial.write(sim.read());
  {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Error: low voltage. Big problems in wire or grounding!";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
  }
}

//checks if message has been sent CUT
void errorMsgCut() {
  if (isErrorSentCut == false && sendMsg == true) {
    Serial.println("Error: extremly low voltage. Probably cut wire!");
    sendMessageCut();
    isErrorSentCut = true;
  }
}

//Sends SMS message to phone CUT
void sendMessageCut() {
  if (sim.available() > 0)
    Serial.write(sim.read());
  {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Error: extremly low voltage. Probably cut wire!";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
  }
}

//checks if message has been sent NORMAL
void errorMsgNormal() {
  if (isErrorSentNormal == false && sendMsg == true) {
    Serial.println("System is running normaly!");
    sendMessageNormal();
    isErrorSentNormal = true;
  }
}

//Sends SMS message to phone NORMAL
void sendMessageNormal() {
  if (sim.available() > 0)
    Serial.write(sim.read());
  {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "System is back to normal!";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
  }
}

void sendTestMessage() {
  if (sim.available() > 0)
    Serial.write(sim.read());
  {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "System test";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
  }
}

void clearPhoneNumber() {
  number = "+371";
  number1 = "";
}
void recieveSMS() {
  if (sim.available() >= 0) {
    incomingData = sim.readString();
    Serial.print(incomingData);
    delay(10);
  }
}

void sendOn() {
  const char text1[] = "on";
  radio.write(&text1, sizeof(text1));
  delay(100);



}

void sendOff () {
      const char text[] = "off";
    radio.write(&text, sizeof(text));
    delay(100);

  }
//-----------------------------------------------LOOP----------------------------------------------

void loop() {

  recieveSMS();

  if (incomingData.indexOf("STATUS") >= 0) {
    if (sim.available() > 0)
      Serial.write(sim.read());
    { 
      sim.println("AT+CMGF=1");
      delay(1000);
      sim.println("AT+CMGS=\"" + number + "\"\r");
      delay(1000);
      String SMS = "Current voltage: " + (String)highVoltage;
      sim.println(SMS);
      delay(100);
      sim.println((char)26);
      delay(1000);
    }
  }

  if (incomingData.indexOf("ON") >= 0) {
    if (sim.available() > 0)
      Serial.write(sim.read());
    {
      sendOn();
    }
  }

  if (incomingData.indexOf("OFF") >= 0) {
    if (sim.available() > 0)
      Serial.write(sim.read());
    {
      sendOff();
    }
  }

  //prints out voltage every 1600 ms
  if (millis() - lastSample > 1600) {             // Checks every 1600ms:
    lastSample = millis();
    highVoltage = maxVal * (10000.0 / 1023.0);
    Serial.println(highVoltage);
    Serial.print("V ");
    maxVal = 0;
  }

  //sets reading to variable
  int reading = analogRead(1);
  if (reading > maxVal) {
    maxVal = reading;
  }

  //catches low voltage error
  if (millis() - lastSample1 > 8000) {            //Checks every 8 seconds
    lastSample1 = millis();
    float highVoltage = maxVal * (10000.0 / 1023.0);
    //if voltage is low calls errorMsg function
    if (highVoltage > 4500 && highVoltage <= 7000) {
      errorMsgOrange();
      isErrorSentRed = false;
      isErrorSentBig = false;
      isErrorSentCut = false;
      isErrorSentNormal = false;
    }
    else if (highVoltage > 3000 && highVoltage <= 4500) {
      errorMsgRed();
      isErrorSentOrange = false;
      isErrorSentBig = false;
      isErrorSentCut = false;
      isErrorSentNormal = false;
    }
    else if (highVoltage > 500 && highVoltage <= 3000) {
      errorMsgBig();
      isErrorSentOrange = false;
      isErrorSentRed = false;
      isErrorSentCut = false;
      isErrorSentNormal = false;
    }
    else if (highVoltage < 500) {
      errorMsgCut();
      isErrorSentOrange = false;
      isErrorSentRed = false;
      isErrorSentBig = false;
      isErrorSentNormal = false;
    }
    else {
      isErrorSentOrange = false;
      isErrorSentRed = false;
      isErrorSentBig = false;
      isErrorSentCut = false;
      errorMsgNormal();
    }
  }

  //reads pressed keys
  char keypressed = myKeypad.getKey();
  if (keypressed == 'C') {
    clearPhoneNumber();
    Serial.println("Enter the new phone number");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 4);
    display.println("Enter the new   phone number");
    display.display();
    delay(130);
    display.clearDisplay();
  }
  if (keypressed >= '0' && keypressed <= '9') {
    if (number.length() <= 11) {
      number1 += keypressed;
      Serial.println(number1);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 4);
      display.println(number1);
      display.display();
      delay(130);
      display.clearDisplay();
    } else {
      Serial.println("Clear phone number first to enter new one.");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 4);
      display.println("Clear phone number   first to enter new    one.");
      display.display();
      delay(130);
      display.clearDisplay();
    }
  }
  if (keypressed == 'A') {
    number += number1;
    number1 = "";
    Serial.println("Phone number: " + number);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Phone number: " + number);
    display.display();
    delay(100);
    display.clearDisplay();
  }
  if (keypressed == '*') {
    sendTestMessage();
  }
}
