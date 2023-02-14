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

//-- Titi 

#ifndef TitiHumanoid_h
#define TitiHumanoid_h

#ifdef ARDUINO_ARCH_ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif
#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>

#include "Titi_matrix.h"
#include "Titi_mouths.h"
#include "Titi_sounds.h"
#include "Titi_gestures.h"

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30

class TitiHumanoid
{
  public:

    //-- Titi initialization
    void initHUMANOID(int YL, int YR, int RL, int RR,int LA, int RA, bool load_calibration, int NoiseSensor, int Buzzer, int USTrigger, int USEcho);
    void initDC(int NoiseSensor, int Buzzer, int USTrigger, int USEcho);
    //-- Attach & detach functions
    void attachServos();
    void detachServos();

    //-- Oscillator Trims
    void setTrims(int YL, int YR, int RL, int RR,int LA, int RA);
    void saveTrimsOnEEPROM();

    //-- Predetermined Motion Functions
    void _moveServos(int time, int  servo_target[]);
    void oscillateServos(int A[6], int O[6], int T, double phase_diff[6], float cycle);

    //-- HOME = Titi at rest position
    void home();
    bool getRestState();
    void setRestState(bool state);
    
    //-- Predetermined Motion Functions
    void jump(float steps=1, int T = 2000);

    void walk(float steps=4, int T=1000, int dir = FORWARD);
    void turn(float steps=4, int T=2000, int dir = LEFT);
    void bend (int steps=1, int T=1400, int dir=LEFT);
    void shakeLeg (int steps=1, int T = 2000, int dir=RIGHT);

    void updown(float steps=1, int T=1000, int h = 20);
    void swing(float steps=1, int T=1000, int h=20);
    void tiptoeSwing(float steps=1, int T=900, int h=20);
    void jitter(float steps=1, int T=500, int h=20);
    void ascendingTurn(float steps=1, int T=900, int h=20);

    void moonwalker(float steps=1, int T=900, int h=20, int dir=LEFT);
    void crusaito(float steps=1, int T=900, int h=20, int dir=FORWARD);
    void flapping(float steps=1, int T=1000, int h=20, int dir=FORWARD);
    void armsup();
    void armsdown();
    void armwave(int dir =RIGHT);

    //-- Sensors functions
    float getDistance(); //US sensor
    int getNoise();      //Noise Sensor
    
    //-- Mouth & Animations
    void putMouth(unsigned long int mouth, bool predefined = true);
    void putAnimationMouth(unsigned long int anim, int index);
    void clearMouth();

    //-- Sounds
    void _tone (float noteFrequency, long noteDuration, int silentDuration);
    void bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
    void sing(int songName);

    //-- Gestures
    void playGesture(int gesture);
    void initMATRIX(int DIN, int CS, int CLK, int rotate);
    void matrixIntensity(int intensity);
    void setLed(byte X, byte Y, byte value);
    void writeText (const char * s, byte scrollspeed);
  private:
   
    Oscillator servo[6];
    US us;
    Titi_Matrix ledmatrix;
    int servo_pins[6];
    int servo_trim[6];
    int servo_position[6];
	int set_A[6];
	int set_O[6];
	double set_phase_diff[6];

    int pinBuzzer;
    int pinNoiseSensor;
    
    unsigned long final_time;
    unsigned long partial_time;
    float increment[6];

    bool isTitiResting;
    bool isHUMANOID;

    unsigned long int getMouthShape(int number);
    unsigned long int getAnimShape(int anim, int index);
    void _execute(int A[6], int O[6], int T, double phase_diff[6], float steps);

};

#endif