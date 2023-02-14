/******************************************************************************************************* 
TiTi Source-Code belongs to the time-line development variations of the open-source project BoB
#bobthebiped, which started in 2013 by Kevin Biagini.
Check: https://www.personalrobots.biz/bob-the-biped-robot/ & https://www.thingiverse.com/k120189/designs
Next variations and improvements of the former job were performed by different authors and teams, the 
most disruptive in terms of mechanical efficiency and innovation was Zowi, created originally by Javier 
Isabel Hernandez, Check: https://github.com/JavierIH/zowi 
Which included the Oscillator library from https://github.com/Obijuan/ArduSnake/tree/master/ArduSnake
developed by Juan Gonzalez-Gomez (Obijuan) in 2011. This library is meant to Generate sinusoidal 
oscillations in the servos which achieved a better locomotion of the robot due to a better harmonic 
synchronization.
Finally, the most profitable and well-known version of these sequence of biped robots is the
Otto-DIY, which by its own definition is the most refined version of the Bob. Check: https://www.ottodiy.com/

All of these versions contributed with a different scope of wide-used project started a long time ago with 
the same purpose, share knowledge about technology to anyone. 

For us, we decided to use all of this legacy to drive an initiative focused on children who live close
to the oilfield operations in Ecuador, all of the variations, adaptations and contributions are released 
under the same license, and they're available to any contributor and developer who may want to use it 
for the same purpose. 
********************************************************************************************************/


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  //#include "WProgram.h"
  //#include <pins_arduino.h>
#endif

#include "Titi.h"
#include <Oscillator.h>


void Titi::init(int YL, int YR, int RL, int RR, bool load_calibration, int Buzzer) {

  servo_pins[0] = YL;
  servo_pins[1] = YR;
  servo_pins[2] = RL;
  servo_pins[3] = RR;

  attachServos();
  isTitiResting=false;

  if (load_calibration) {
    for (int i = 0; i < 4; i++) {
      int servo_trim = EEPROM.read(i);
      if (servo_trim > 128) servo_trim -= 256;
      servo[i].SetTrim(servo_trim);
    }
  }

  //Buzzer pin:
  pinBuzzer = Buzzer;
  pinMode(Buzzer,OUTPUT);
 
}
///////////////////////////////////////////////////////
void Titi::initMATRIX(int DIN, int CS, int CLK, int rotate){
ledmatrix.init( DIN, CS, CLK, 1, rotate);   // set up Matrix display
}
void Titi::matrixIntensity(int intensity){
ledmatrix.setIntensity(intensity);
}

///////////////////////////////////////////////////////////////////
//-- ATTACH & DETACH FUNCTIONS ----------------------------------//
///////////////////////////////////////////////////////////////////
void Titi::attachServos(){
    servo[0].attach(servo_pins[0]);
    servo[1].attach(servo_pins[1]);
    servo[2].attach(servo_pins[2]);
    servo[3].attach(servo_pins[3]);
}

void Titi::detachServos(){
    servo[0].detach();
    servo[1].detach();
    servo[2].detach();
    servo[3].detach();
}

///////////////////////////////////////////////////////////////////
//-- OSCILLATORS TRIMS ------------------------------------------//
///////////////////////////////////////////////////////////////////
void Titi::setTrims(int YL, int YR, int RL, int RR) {
  servo[0].SetTrim(YL);
  servo[1].SetTrim(YR);
  servo[2].SetTrim(RL);
  servo[3].SetTrim(RR);
}

void Titi::saveTrimsOnEEPROM() {

  for (int i = 0; i < 4; i++){
      EEPROM.write(i, servo[i].getTrim());
  }
}

///////////////////////////////////////////////////////////////////
//-- BASIC MOTION FUNCTIONS -------------------------------------//
///////////////////////////////////////////////////////////////////
void Titi::_moveServos(int time, int  servo_target[]) {

  attachServos();
  if(getRestState()==true){
        setRestState(false);
  }

  final_time =  millis() + time;
  if(time>10){
    for (int i = 0; i < 4; i++) increment[i] = (servo_target[i] - servo[i].getPosition()) / (time / 10.0);

    for (int iteration = 1; millis() < final_time; iteration++) {
      partial_time = millis() + 10;
      for (int i = 0; i < 4; i++) servo[i].SetPosition(servo[i].getPosition() + increment[i]);
      while (millis() < partial_time); //pause
    }
  }
  else{
    for (int i = 0; i < 4; i++) servo[i].SetPosition(servo_target[i]);
    while (millis() < final_time); //pause
  }

  // final adjustment to the target. if servo speed limiter is turned on, reaching to the goal may take longer than 
  // requested time.
  bool f = true;
  while(f) {
    f = false;
    for (int i = 0; i < 4; i++) {
      if (servo_target[i] != servo[i].getPosition()) {
        f = true;
        break;
      }
    }
    if (f) {
      for (int i = 0; i < 4; i++) {
        servo[i].SetPosition(servo_target[i]);
      }
      partial_time = millis() + 10;
      while (millis() < partial_time); //pause
    }
  };
}

void Titi::_moveSingle(int position, int servo_number) {
if (position > 180) position = 90;
if (position < 0) position = 90;
  attachServos();
  if(getRestState()==true){
        setRestState(false);
  }
int servoNumber = servo_number;
if (servoNumber == 0){
  servo[0].SetPosition(position);
}
if (servoNumber == 1){
  servo[1].SetPosition(position);
}
if (servoNumber == 2){
  servo[2].SetPosition(position);
}
if (servoNumber == 3){
  servo[3].SetPosition(position);
}
}

void Titi::oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle=1){

  for (int i=0; i<4; i++) {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()){
     for (int i=0; i<4; i++){
        servo[i].refresh();
     }
  }
}

void Titi::_execute(int A[4], int O[4], int T, double phase_diff[4], float steps = 1.0){

  attachServos();
  if(getRestState()==true){
        setRestState(false);
  }


  int cycles=(int)steps;

  //-- Execute complete cycles
  if (cycles >= 1)
    for(int i = 0; i < cycles; i++)
      oscillateServos(A,O, T, phase_diff);

  //-- Execute the final not complete cycle
  oscillateServos(A,O, T, phase_diff,(float)steps-cycles);
}

///////////////////////////////////////////////////////////////////
//-- HOME = Titi at rest position -------------------------------//
///////////////////////////////////////////////////////////////////
void Titi::home(){

  if(isTitiResting==false){ //Go to rest position only if necessary

    int homes[4]={90, 90, 90, 90}; //All the servos at rest position
    _moveServos(500,homes);   //Move the servos in half a second

    detachServos();
    isTitiResting=true;
  }
}

bool Titi::getRestState(){
    return isTitiResting;
}

void Titi::setRestState(bool state){

    isTitiResting = state;
}

///////////////////////////////////////////////////////////////////
//-- PREDETERMINED MOTION SEQUENCES -----------------------------//
///////////////////////////////////////////////////////////////////
//-- Titi movement: Jump
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//---------------------------------------------------------
void Titi::jump(float steps, int T){

  int up[]={90,90,150,30};
  _moveServos(T,up);
  int down[]={90,90,90,90};
  _moveServos(T,down);
}

//---------------------------------------------------------
//-- Titi gait: Walking  (forward or backward)
//--  Parameters:
//--    * steps:  Number of steps
//--    * T : Period
//--    * Dir: Direction: FORWARD / BACKWARD
//---------------------------------------------------------
void Titi::walk(float steps, int T, int dir){
  //-- Oscillator parameters for walking
  //-- Hip sevos are in phase
  //-- Feet servos are in phase
  //-- Hip and feet are 90 degrees out of phase
  //--      -90 : Walk forward
  //--       90 : Walk backward
  //-- Feet servos also have the same offset (for tiptoe a little bit)
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi gait: Turning (left or right)
//--  Parameters:
//--   * Steps: Number of steps
//--   * T: Period
//--   * Dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void Titi::turn(float steps, int T, int dir){

  //-- Same coordination than for walking (see Titi::walk)
  //-- The Amplitudes of the hip's oscillators are not igual
  //-- When the right hip servo amplitude is higher, the steps taken by
  //--   the right leg are bigger than the left. So, the robot describes an
  //--   left arc
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)};

  if (dir == LEFT) {
    A[0] = 30; //-- Left hip servo
    A[1] = 10; //-- Right hip servo
  }
  else {
    A[0] = 10;
    A[1] = 30;
  }

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi gait: Lateral bend
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    dir: RIGHT=Right bend LEFT=Left bend
//---------------------------------------------------------
void Titi::bend (int steps, int T, int dir){

  //Parameters of all the movements. Default: Left bend
  int bend1[4]={90, 90, 62, 35};
  int bend2[4]={90, 90, 62, 105};
  int homes[4]={90, 90, 90, 90};

  //Time of one bend, constrained in order to avoid movements too fast.
  //T=max(T, 600);
  //Changes in the parameters if right direction is chosen
  if(dir==-1)
  {
    bend1[2]=180-35;
    bend1[3]=180-60;  //Not 65. Titi is unbalanced
    bend2[2]=180-105;
    bend2[3]=180-60;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2=800;

  //Bend movement
  for (int i=0;i<steps;i++)
  {
    _moveServos(T2/2,bend1);
    _moveServos(T2/2,bend2);
    delay(T*0.8);
    _moveServos(500,homes);
  }

}

//---------------------------------------------------------
//-- Titi gait: Shake a leg
//--  Parameters:
//--    steps: Number of shakes
//--    T: Period of one shake
//--    dir: RIGHT=Right leg LEFT=Left leg
//---------------------------------------------------------
void Titi::shakeLeg (int steps,int T,int dir){

  //This variable change the amount of shakes
  int numberLegMoves=2;

  //Parameters of all the movements. Default: Right leg
  int shake_leg1[4]={90, 90, 58, 35};
  int shake_leg2[4]={90, 90, 58, 120};
  int shake_leg3[4]={90, 90, 58, 60};
  int homes[4]={90, 90, 90, 90};

  //Changes in the parameters if left leg is chosen
  if(dir==-1)
  {
    shake_leg1[2]=180-35;
    shake_leg1[3]=180-58;
    shake_leg2[2]=180-120;
    shake_leg2[3]=180-58;
    shake_leg3[2]=180-60;
    shake_leg3[3]=180-58;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2=1000;
  //Time of one shake, constrained in order to avoid movements too fast.
  T=T-T2;
  T=max(T,200*numberLegMoves);

  for (int j=0; j<steps;j++)
  {
  //Bend movement
  _moveServos(T2/2,shake_leg1);
  _moveServos(T2/2,shake_leg2);

    //Shake movement
    for (int i=0;i<numberLegMoves;i++)
    {
    _moveServos(T/(2*numberLegMoves),shake_leg3);
    _moveServos(T/(2*numberLegMoves),shake_leg2);
    }
    _moveServos(500,homes); //Return to home position
  }

  delay(T);
}

//---------------------------------------------------------
//-- Titi movement: up & down
//--  Parameters:
//--    * steps: Number of jumps
//--    * T: Period
//--    * h: Jump height: SMALL / MEDIUM / BIG
//--              (or a number in degrees 0 - 90)
//---------------------------------------------------------
void Titi::updown(float steps, int T, int h){

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi movement: swinging side to side
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void Titi::swing(float steps, int T, int h){

  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2, -h/2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi movement: swinging side to side without touching the floor with the heel
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void Titi::tiptoeSwing(float steps, int T, int h){

  //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, 0, 0};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi gait: Jitter
//--  Parameters:
//--    steps: Number of jitters
//--    T: Period of one jitter
//--    h: height (Values between 5 - 25)
//---------------------------------------------------------
void Titi::jitter(float steps, int T, int h){

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h=min(25,h);
  int A[4]= {h, h, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi gait: Ascending & turn (Jitter while up&down)
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    h: height (Values between 5 - 15)
//---------------------------------------------------------
void Titi::ascendingTurn(float steps, int T, int h){

  //-- Both feet and legs are 180 degrees out of phase
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h=min(13,h);
  int A[4]= {h, h, h, h};
  int O[4] = {0, 0, h+4, -h+4};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi gait: Moonwalker. Titi moves like Michael Jackson
//--  Parameters:
//--    Steps: Number of steps
//--    T: Period
//--    h: Height. Typical valures between 15 and 40
//--    dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void Titi::moonwalker(float steps, int T, int h, int dir){

  //-- This motion is similar to that of the caterpillar robots: A travelling
  //-- wave moving from one side to another
  //-- The two Titi's feet are equivalent to a minimal configuration. It is known
  //-- that 2 servos can move like a worm if they are 120 degrees out of phase
  //-- In the example of Titi, the two feet are mirrored so that we have:
  //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //--  is 60 degrees.
  //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //-   offset so that the robot tiptoe lightly

  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2+2, -h/2 -2};
  int phi = -dir * 90;
  double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//----------------------------------------------------------
//-- Titi gait: Crusaito. A mixture between moonwalker and walk
//--   Parameters:
//--     steps: Number of steps
//--     T: Period
//--     h: height (Values between 20 - 50)
//--     dir:  Direction: LEFT / RIGHT
//-----------------------------------------------------------
void Titi::crusaito(float steps, int T, int h, int dir){

  int A[4]= {25, 25, h, h};
  int O[4] = {0, 0, h/2+ 4, -h/2 - 4};
  double phase_diff[4] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Titi gait: Flapping
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//--    h: height (Values between 10 - 30)
//--    dir: direction: FOREWARD, BACKWARD
//---------------------------------------------------------
void Titi::flapping(float steps, int T, int h, int dir){

  int A[4]= {12, 12, h, h};
  int O[4] = {0, 0, h - 10, -h + 10};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

///////////////////////////////////////////////////////////////////
//-- MOUTHS & ANIMATIONS ----------------------------------------//
///////////////////////////////////////////////////////////////////
void Titi::setLed(byte X, byte Y, byte value){
  ledmatrix.setDot( X,  Y, value);
}


// EXAMPLE putAnimationMouth(dreamMouth,0);
void Titi::putAnimationMouth(unsigned long int aniMouth, int index){

ledmatrix.writeFull(PROGMEM_getAnything (&Gesturetable[aniMouth][index]));

}

//EXAMPLE putMouth(smile);
void Titi::putMouth(unsigned long int mouth, bool predefined){
  if (predefined){
// Here a direct entry into the Progmem Mouthttable is used!!

ledmatrix.writeFull(PROGMEM_getAnything(&Mouthtable[mouth]));
  }
  else{
    ledmatrix.writeFull(mouth);
  }
}


void Titi::clearMouth(){

  ledmatrix.clearMatrix();
}

void Titi::writeText(const char * s, byte scrollspeed){
 int a ;
 int b ;
  for(a = 0; s[a] != '\0'; a++){
    b = a +1 ;
    if (b > 9 ) b = 9; // only maximum of nine characters allowed
  }
  for(int charNUMBER = 0; charNUMBER <b; charNUMBER++){
      if ((* s < 48) || (* s > 91)) {
        if (* s == 32){
          ledmatrix.sendChar (44, charNUMBER, b, scrollspeed);
        }
        else
        {
          ledmatrix.sendChar (43, charNUMBER, b, scrollspeed);
        }
     }
      else
      {
      ledmatrix.sendChar ((* s - 48), charNUMBER, b, scrollspeed);
     }
  * s++;
  }

}

///////////////////////////////////////////////////////////////////
//-- SOUNDS -----------------------------------------------------//
///////////////////////////////////////////////////////////////////

void Titi::_tone (float noteFrequency, long noteDuration, int silentDuration){

    // tone(10,261,500);
    // delay(500);

      if(silentDuration==0){silentDuration=1;}

      tone(Titi::pinBuzzer, noteFrequency, noteDuration);
      delay(noteDuration);       //milliseconds to microseconds
      //noTone(PIN_Buzzer);
      delay(silentDuration);
}

void Titi::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration){

  //Examples:
  //  bendTones (880, 2093, 1.02, 18, 1);
  //  bendTones (note_A5, note_C7, 1.02, 18, 0);

  if(silentDuration==0){silentDuration=1;}

  if(initFrequency < finalFrequency)
  {
      for (int i=initFrequency; i<finalFrequency; i=i*prop) {
          _tone(i, noteDuration, silentDuration);
      }

  } else{

      for (int i=initFrequency; i>finalFrequency; i=i/prop) {
          _tone(i, noteDuration, silentDuration);
      }
  }
}

void Titi::sing(int songName){
  switch(songName){

    case S_connection:
      _tone(note_E5,50,30);
      _tone(note_E6,55,25);
      _tone(note_A6,60,10);
    break;

    case S_disconnection:
      _tone(note_E5,50,30);
      _tone(note_A6,55,25);
      _tone(note_E6,50,10);
    break;

    case S_buttonPushed:
      bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      bendTones (note_E6, note_D7, 1.04, 10, 2);
    break;

    case S_mode1:
      bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
    break;

    case S_mode2:
      bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
    break;

    case S_mode3:
      _tone(note_E6,50,100); //D6
      _tone(note_G6,50,80);  //E6
      _tone(note_D7,300,0);  //G6
    break;

    case S_surprise:
      bendTones(800, 2150, 1.02, 10, 1);
      bendTones(2149, 800, 1.03, 7, 1);
    break;

    case S_OhOoh:
      bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i=880; i<2000; i=i*1.04) {
           _tone(note_B5,5,10);
      }
    break;

    case S_OhOoh2:
      bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i=1880; i<3000; i=i*1.03) {
          _tone(note_C6,10,10);
      }
    break;

    case S_cuddly:
      bendTones(700, 900, 1.03, 16, 4);
      bendTones(899, 650, 1.01, 18, 7);
    break;

    case S_sleeping:
      bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      bendTones(400, 100, 1.04, 10, 1);
    break;

    case S_happy:
      bendTones(1500, 2500, 1.05, 20, 8);
      bendTones(2499, 1500, 1.05, 25, 8);
    break;

    case S_superHappy:
      bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      bendTones(5999, 2000, 1.05, 13, 2);
    break;

    case S_happy_short:
      bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      bendTones(1900, 2500, 1.05, 10, 8);
    break;

    case S_sad:
      bendTones(880, 669, 1.02, 20, 200);
    break;

    case S_confused:
      bendTones(1000, 1700, 1.03, 8, 2);
      bendTones(1699, 500, 1.04, 8, 3);
      bendTones(1000, 1700, 1.05, 9, 10);
    break;

    case S_fart1:
      bendTones(1600, 3000, 1.02, 2, 15);
    break;

    case S_fart2:
      bendTones(2000, 6000, 1.02, 2, 20);
    break;

    case S_fart3:
      bendTones(1600, 4000, 1.02, 2, 20);
      bendTones(4000, 3000, 1.02, 2, 20);
    break;

  }
}

///////////////////////////////////////////////////////////////////
//-- GESTURES ---------------------------------------------------//
///////////////////////////////////////////////////////////////////

void Titi::playGesture(int gesture){
 int gesturePOSITION[4];
  
  switch(gesture){

    case TitiHappy: 
        _tone(note_E5,50,30);
        putMouth(smile);
        sing(S_happy_short);
        swing(1,800,20); 
        sing(S_happy_short);

        home();
        putMouth(happyOpen);
    break;


    case TitiSuperHappy:
        putMouth(happyOpen);
        sing(S_happy);
        putMouth(happyClosed);
        tiptoeSwing(1,500,20);
        putMouth(happyOpen);
        sing(S_superHappy);
        putMouth(happyClosed);
        tiptoeSwing(1,500,20); 

        home();  
        putMouth(happyOpen);
    break;


    case TitiSad: 
        putMouth(sad);
        gesturePOSITION[0] = 110;//int sadPos[6]=      {110, 70, 20, 160};
        gesturePOSITION[1] = 70;
         gesturePOSITION[2] = 20;
          gesturePOSITION[3] = 160;
        _moveServos(700, gesturePOSITION);     
        bendTones(880, 830, 1.02, 20, 200);
        putMouth(sadClosed);
        bendTones(830, 790, 1.02, 20, 200);  
        putMouth(sadOpen);
        bendTones(790, 740, 1.02, 20, 200);
        putMouth(sadClosed);
        bendTones(740, 700, 1.02, 20, 200);
        putMouth(sadOpen);
        bendTones(700, 669, 1.02, 20, 200);
        putMouth(sad);
        delay(500);

        home();
        delay(300);
        putMouth(happyOpen);
    break;


    case TitiSleeping:
    gesturePOSITION[0] = 100;//int bedPos[6]=      {100, 80, 60, 120};
        gesturePOSITION[1] = 80;
         gesturePOSITION[2] = 60;
          gesturePOSITION[3] = 120;
        _moveServos(700, gesturePOSITION);     
        for(int i=0; i<4;i++){
          putAnimationMouth(dreamMouth,0);
          bendTones (100, 200, 1.04, 10, 10);
          putAnimationMouth(dreamMouth,1);
          bendTones (200, 300, 1.04, 10, 10);  
          putAnimationMouth(dreamMouth,2);
          bendTones (300, 500, 1.04, 10, 10);   
          delay(500);
          putAnimationMouth(dreamMouth,1);
          bendTones (400, 250, 1.04, 10, 1); 
          putAnimationMouth(dreamMouth,0);
          bendTones (250, 100, 1.04, 10, 1); 
          delay(500);
        } 

        putMouth(lineMouth);
        sing(S_cuddly);

        home();  
        putMouth(happyOpen);
    break;


    case TitiFart:
    gesturePOSITION[0] = 90;// int fartPos_1[6]=   {90, 90, 145, 122};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 145;
          gesturePOSITION[3] = 122;
        _moveServos(500,gesturePOSITION);
        delay(300);     
        putMouth(lineMouth);
        sing(S_fart1);  
        putMouth(tongueOut);
        delay(250);
        gesturePOSITION[0] = 90;// int fartPos_2[6]=   {90, 90, 80, 122};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 80;
          gesturePOSITION[3] = 122;
        _moveServos(500,gesturePOSITION);
        delay(300);
        putMouth(lineMouth);
        sing(S_fart2); 
        putMouth(tongueOut);
        delay(250);
        gesturePOSITION[0] = 90;// int fartPos_3[6]=   {90, 90, 145, 80};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 145;
          gesturePOSITION[3] = 80;
        _moveServos(500,gesturePOSITION);
        delay(300);
        putMouth(lineMouth);
        sing(S_fart3);
        putMouth(tongueOut);    
        delay(300);

        home(); 
        delay(500); 
        putMouth(happyOpen);
    break;


    case TitiConfused:
    gesturePOSITION[0] = 110;//int confusedPos[6]= {110, 70, 90, 90};
        gesturePOSITION[1] = 70;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 90;
        _moveServos(300, gesturePOSITION); 
        putMouth(confused);
        sing(S_confused);
        delay(500);

        home();  
        putMouth(happyOpen);
    break;


    case TitiLove:
        putMouth(heart);
        sing(S_cuddly);
        crusaito(2,1500,15,1);

        home(); 
        sing(S_happy_short);  
        putMouth(happyOpen);
    break;


    case TitiAngry: 
    gesturePOSITION[0] = 90;//int angryPos[6]=    {90, 90, 70, 110};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 70;
          gesturePOSITION[3] = 110;
        _moveServos(300, gesturePOSITION); 
        putMouth(angry);

        _tone(note_A5,100,30);
        bendTones(note_A5, note_D6, 1.02, 7, 4);
        bendTones(note_D6, note_G6, 1.02, 10, 1);
        bendTones(note_G6, note_A5, 1.02, 10, 1);
        delay(15);
        bendTones(note_A5, note_E5, 1.02, 20, 4);
        delay(400);
        gesturePOSITION[0] = 110;//int headLeft[6]=    {110, 110, 90, 90};
        gesturePOSITION[1] = 110;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 90;
        _moveServos(200, gesturePOSITION); 
        bendTones(note_A5, note_D6, 1.02, 20, 4);
        gesturePOSITION[0] = 70;//int headRight[6]=   {70, 70, 90, 90};
        gesturePOSITION[1] = 70;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 90;
        _moveServos(200, gesturePOSITION); 
        bendTones(note_A5, note_E5, 1.02, 20, 4);

        home();  
        putMouth(happyOpen);
    break;


    case TitiFretful: 
        putMouth(angry);
        bendTones(note_A5, note_D6, 1.02, 20, 4);
        bendTones(note_A5, note_E5, 1.02, 20, 4);
        delay(300);
        putMouth(lineMouth);

        for(int i=0; i<4; i++){
          gesturePOSITION[0] = 90;//int fretfulPos[6]=  {90, 90, 90, 110};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 110;
          _moveServos(100, gesturePOSITION);   
          home();
        }

        putMouth(angry);
        delay(500);

        home();  
        putMouth(happyOpen);
    break;


    case TitiMagic:

        //Initial note frecuency = 400
        //Final note frecuency = 1000
        
        // Reproduce the animation four times
        for(int i = 0; i<4; i++){ 

          int noteM = 400; 

            for(int index = 0; index<6; index++){
              putAnimationMouth(adivinawi,index);
              bendTones(noteM, noteM+100, 1.04, 10, 10);    //400 -> 1000 
              noteM+=100;
            }

            clearMouth();
            bendTones(noteM-100, noteM+100, 1.04, 10, 10);  //900 -> 1100

            for(int index = 0; index<6; index++){
              putAnimationMouth(adivinawi,index);
              bendTones(noteM, noteM+100, 1.04, 10, 10);    //1000 -> 400 
              noteM-=100;
            }
        } 
 
        delay(300);
        putMouth(happyOpen);
    break;


    case TitiWave:
        
        // Reproduce the animation four times
        for(int i = 0; i<2; i++){ 

            int noteW = 500; 

            for(int index = 0; index<10; index++){
              putAnimationMouth(wave,index);
              bendTones(noteW, noteW+100, 1.02, 10, 10); 
              noteW+=101;
            }
            for(int index = 0; index<10; index++){
              putAnimationMouth(wave,index);
              bendTones(noteW, noteW+100, 1.02, 10, 10); 
              noteW+=101;
            }
            for(int index = 0; index<10; index++){
              putAnimationMouth(wave,index);
              bendTones(noteW, noteW-100, 1.02, 10, 10); 
              noteW-=101;
            }
            for(int index = 0; index<10; index++){
              putAnimationMouth(wave,index);
              bendTones(noteW, noteW-100, 1.02, 10, 10); 
              noteW-=101;
            }
        }    

        clearMouth();
        delay(100);
        putMouth(happyOpen);
    break;

    case TitiVictory:
        
        putMouth(smallSurprise);
        //final pos   = {90,90,150,30}
        for (int i = 0; i < 60; ++i){
          int pos[]={90,90,90+i,90-i};  
          _moveServos(10,pos);
          _tone(1600+i*20,15,1);
        }

        putMouth(bigSurprise);
        //final pos   = {90,90,90,90}
        for (int i = 0; i < 60; ++i){
          int pos[]={90,90,150-i,30+i};  
          _moveServos(10,pos);
          _tone(2800+i*20,15,1);
        }

        putMouth(happyOpen);
        //SUPER HAPPY
        //-----
        tiptoeSwing(1,500,20);
        sing(S_superHappy);
        putMouth(happyClosed);
        tiptoeSwing(1,500,20); 
        //-----

        home();
        clearMouth();
        putMouth(happyOpen);

    break;

    case TitiFail:
        putMouth(sadOpen);
         gesturePOSITION[0] = 90;//int bendPos_1[6]=   {90, 90, 70, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 70;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        _tone(900,200,1);
        putMouth(sadClosed);
        gesturePOSITION[0] = 90;//int bendPos_2[6]=   {90, 90, 55, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 55;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        _tone(600,200,1);
        putMouth(confused);
        gesturePOSITION[0] = 90;//int bendPos_3[6]=   {90, 90, 42, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 42;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        _tone(300,200,1);
        gesturePOSITION[0] = 90;//int bendPos_4[6]=   {90, 90, 34, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 34;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        putMouth(xMouth);

        detachServos();
        _tone(150,2200,1);
        
        delay(600);
        clearMouth();
        putMouth(happyOpen);
        home();

    break;

  }
} 

void Titi::enableServoLimit(int diff_limit) {
  for (int i = 0; i < 4; i++) {
    servo[i].SetLimiter(diff_limit);
  }
}

void Titi::disableServoLimit() {
  for (int i = 0; i < 4; i++) {
    servo[i].DisableLimiter();
  }
}


