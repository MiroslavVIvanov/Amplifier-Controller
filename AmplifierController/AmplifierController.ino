//button || function || code

//power  || power    || 0x1828, 0x1BD7
//SDB    || att      || 0x95C, 0xAA3        - resistors fault
//VolUp  || Vol up   || 0x1AC8, 0xAEF41B5E  - resistors fault
//VolDn  || Vol dn   || 0x18C8, 0x1B37
//CD     || source   || 0xB58, 0x8A7        - resistors fault
//sleep  || sleep    || 0x1938, 0xA84E3D6C  - resistors fault
//>>     || tune up  || 0x968, 0x133A7E0E
//<<     || tune dn  || 0xB68, 0x57CBF3BE
//>>|    || auto up  || 0x868, 0xB97
//|<<    || auto dn  || 0xA68, 0x997
//preset>|| presetUp || 0x19C8, 0x2B534B4C
//preset<|| presetDn || 0x1BC8, 0x6FE4C0FC


#include <IRremote.h>

#define volUp 2
#define att 3
#define volDown 4
#define source 5
#define tuneUp 6
#define tuneDown 7
#define pinsShort 8
#define mute 9
#define input 10
//#define irReceiver 11
#define power 12 //1828 || E0E0926D

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

int atxState;
//0 - off
//1 - automatic on
//2 - manual on
//3 - pending power off

int inputCount; // use for auto power on/off
int loopCount; // use to skip checks on number of loop iterations
int powerOffCountDown;


void setup()
{
  atxState = 0;
  inputCount = 0;
  loopCount = 0;
  powerOffCountDown = 256; //1 min //2048 todo: change value to pass more time before receiver shuts down

  irrecv.enableIRIn(); // Start the receiver

  pinMode(volUp, INPUT);
  pinMode(att, INPUT);
  pinMode(volDown, INPUT);
  pinMode(source, INPUT);
  pinMode(tuneUp, INPUT);
  pinMode(tuneDown, INPUT);
  pinMode(pinsShort, INPUT);
  pinMode(mute, INPUT);

  pinMode(power, OUTPUT);

  digitalWrite(volUp, LOW);
  digitalWrite(att, LOW);
  digitalWrite(volDown, LOW);
  digitalWrite(source, LOW);
  digitalWrite(tuneUp, LOW);
  digitalWrite(tuneDown, LOW);
  digitalWrite(pinsShort, LOW);
  digitalWrite(mute, LOW);

  digitalWrite(power, LOW);
}


//digitalRead(input);
//powerATXandReceiver(2);

void loop() {
  delay(1);
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch (results.value) {
      //power
      case 0x1828:
      case 0x1BD7:
        if (digitalRead(power)) {
          powerOff();
        } else {
          powerOnATXandReceiver(2);
        }
        break;

      //volume up
      case 0x1AC8:
      case 0xAEF41B5E:
        pinMode(volUp, OUTPUT);
        digitalWrite(volUp, LOW);
        delay(120);
        pinMode(volUp, INPUT);
        break;

      //volume down
      case 0x18C8:
      case 0x1B37:
      case 0x4D9DCFD4:
        pinMode(volDown, OUTPUT);
        digitalWrite(volDown, LOW);
        delay(120);
        pinMode(volDown, INPUT);
        break;

      //att
      case 0x95C:
      case 0xAA3:
        pinMode(att, OUTPUT);
        digitalWrite(att, LOW);
        delay(120);
        pinMode(att, INPUT);
        break;

      //source
      case 0xB58:
      case 0x8A7:
        pinMode(source, OUTPUT);
        digitalWrite(source, LOW);
        delay(120);
        pinMode(source, INPUT);
        break;

      //sleep
      case 0x1938:
      case 0xA84E3D6C:
        pinMode(source, OUTPUT);
        digitalWrite(source, LOW);
        delay(2000);
        pinMode(source, INPUT);
        break;

      // tune up
      case 0x968:
      case 0x133A7E0E:
        pinMode(tuneUp, OUTPUT);
        digitalWrite(tuneUp, LOW);
        delay(120);
        pinMode(tuneUp, INPUT);
        break;

      //tune down
      case 0xB68:
      case 0x57CBF3BE:
        pinMode(tuneDown, OUTPUT);
        digitalWrite(tuneDown, LOW);
        delay(120);
        pinMode(tuneDown, INPUT);
        break;

      // auto tune up
      case 0x868:
      case 0xB97:
        pinMode(tuneUp, OUTPUT);
        digitalWrite(tuneUp, LOW);
        delay(550);
        pinMode(tuneUp, INPUT);
        break;

      //auto tune down
      case 0xA68:
      case 0x997:
        pinMode(tuneDown, OUTPUT);
        digitalWrite(tuneDown, LOW);
        delay(550);
        pinMode(tuneDown, INPUT);
        break;

      // preset up
      case 0x19C8:
      case 0x2B534B4C:
        pinMode(pinsShort, OUTPUT);
        digitalWrite(pinsShort, LOW);
        delay(10);
        pinMode(tuneUp, OUTPUT);
        digitalWrite(tuneUp, LOW);
        delay(120);
        pinMode(tuneUp, INPUT);
        pinMode(pinsShort, INPUT);
        break;

      // preset dn
      case 0x1BC8:
      case 0x1837:
      case 0x6FE4C0FC:
        pinMode(pinsShort, OUTPUT);
        digitalWrite(pinsShort, LOW);
        delay(10);
        pinMode(tuneDown, OUTPUT);
        digitalWrite(tuneDown, LOW);
        delay(120);
        pinMode(tuneDown, INPUT);
        pinMode(pinsShort, INPUT);
        break;
    }
    delay(50); //100
    irrecv.resume(); // Receive the next value
  }

  if (atxState != 2) {
    if (millis() % 1000 == 0) {
      switch (atxState) {
        case 0:
          if (digitalRead(input)) {
            powerOnATXandReceiver(1);
          }
          break;
        case 1:
          if (!digitalRead(input)) {
            powerOff();
          }
          break;
      }
    }
  }
}

void powerOnATXandReceiver(int atxStateNumber) {
  atxState = atxStateNumber;
  digitalWrite(power, HIGH);
  delay(2000);
  //start the receiver
  pinMode(source, OUTPUT);
  digitalWrite(source, LOW);
  delay(120);
  pinMode(source, INPUT);
  delay(500);
  //go to aux source
  pinMode(source, OUTPUT);
  digitalWrite(source, LOW);
  delay(120);
  pinMode(source, INPUT);
  //  //att
  //  delay(500);
  //  pinMode(att, OUTPUT);
  //  digitalWrite(att, LOW);
  //  delay(120);
  //  pinMode(att, INPUT);
  //  //volume up
  //  pinMode(volUp, OUTPUT);
  //  digitalWrite(volUp, LOW);
  //  delay(120);
  //  pinMode(volUp, INPUT);
  //  delay(120);
}

void powerOff() {
  digitalWrite(power, LOW);
  atxState = 0;
  delay(100);
}

