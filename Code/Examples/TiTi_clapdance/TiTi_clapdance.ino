#include <Titi.h>
Titi Titi; 
/***   Global variables and function definition  ***/
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer  12 
int randomDance=0;
int randomSteps=0;
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=30;   

/***   Setup  ***/
void setup() {
    Titi.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
    Titi.playGesture(TitiHappy);
    Titi.home();
    randomSeed(analogRead(A6));
    // initialize digital pin 10 & 11 as an output for LEDs.
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
}

/***   Loop  ***/
void loop() {

    if (analogRead(A6) >= 300) {
          delay(100);
            blinkingON();
            Titi.putMouth(bigSurprise);
            Titi.sing(S_OhOoh);
            Titi.putMouth(random(4,14));
            randomDance=random(4,14);
            move(randomDance);
            Titi.home();
            blinkingOFF();
            delay(500); //Wait for possible noise of the servos while get home
            Titi.putMouth(happyOpen); 
    }
}

void move(int moveId){

  bool manualMode = false;

  switch (moveId) {
    case 0:
      Titi.home();
      break;

    case 4: //M 6 1000 30
      blinkingON();
      Titi.updown(3, 1500, 30);
      blinkingOFF();
      break;
      
    case 5: //M 6 1000 30
      blinkingON();
      Titi.moonwalker(3,T,moveSize,1);
      blinkingOFF();
      break;
      
    case 6: //M 7 1000 30
      blinkingON();
      Titi.moonwalker(3,T,moveSize,-1);
      blinkingOFF();
      break;
      
    case 7: //M 8 1000 30
      blinkingON();
      Titi.ascendingTurn(2, T, 50);
      blinkingOFF();
      break;
      
    case 8: //M 9 1000 30
      blinkingON();
      Titi.flapping(1,T,moveSize,-1);
      blinkingOFF();
      break;
      
    case 10: //M 11 1000
      blinkingON();
      Titi.swing(2, 1000, 30);
      blinkingOFF();
      break;
      
    case 11: //M 15 500
      blinkingON();
      Titi.shakeLeg(1,T,-1);
      blinkingOFF();
      break;
      
    case 12: //M 17 500
      blinkingON();
      Titi.shakeLeg(1,T,1);
      blinkingOFF();
      break;
      
    case 13: //M 12 1000 30
      blinkingON();
      Titi.flapping(1,T,moveSize,1);
      blinkingOFF();
      break;
    
    case 14: //M 13 1000 30
      blinkingON();
      Titi.crusaito(3,T,20,1);
      blinkingOFF();
      break;   


    default:
        manualMode = true;
      break;
  }       
}


void blinkingON() {
  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, HIGH);
  //delay(800);
}

void blinkingOFF() {
  digitalWrite(10, LOW);   // turn the LED off (LOW is the voltage level)
  digitalWrite(11, LOW);
  //delay(350);
}
