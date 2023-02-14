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

#ifndef Titi_gestures_h
#define Titi_gestures_h

//***********************************************************************************
//*********************************GESTURE DEFINES***********************************
//***********************************************************************************           

#define TitiHappy 		0
#define TitiSuperHappy 	1
#define TitiSad 		2
#define TitiSleeping 	3
#define TitiFart 		4
#define TitiConfused 	5
#define TitiLove 		6
#define TitiAngry 		7
#define TitiFretful 	8
#define TitiMagic 		9
#define TitiWave 		10
#define TitiVictory 	11
#define TitiFail 		12

//*** MOUTH ANIMATIONS***
#define littleUuh		0
#define dreamMouth		1 	
#define adivinawi		2
#define wave 			3


 //*** MOUTH ANIMATIONS***
#define littleUuh   0
#define dreamMouth    1   
#define adivinawi   2
#define wave      3
#define titi      4 // Wordt niet gebruikt.

typedef struct 
{
  uint8_t Character[1];
  uint8_t data[6];
}
LED_Matrix_Font_6x8_TypeDef;

//Terminal
const LED_Matrix_Font_6x8_TypeDef Character_font_6x8[] PROGMEM =
{

    '0',  0x00,0x7C,0x82,0x82,0x7C,0x00,
    '1',  0x00,0x42,0xFE,0x02,0x00,0x00,
    '2',  0x00,0x46,0x8A,0x92,0x62,0x00,
    '3',  0x00,0x44,0x92,0x92,0x6C,0x00,
    '4',  0x00,0x1C,0x64,0xFE,0x04,0x00,
    '5',  0x00,0xF2,0x92,0x92,0x8C,0x00,
    '6',  0x00,0x7C,0x92,0x92,0x4C,0x00,
    '7',  0x00,0xC0,0x8E,0x90,0xE0,0x00,
    '8',  0x00,0x6C,0x92,0x92,0x6C,0x00,
    '9',  0x00,0x64,0x92,0x92,0x7C,0x00,
    ':',  0x00,0x00,0x14,0x00,0x00,0x00,
    ';',  0x00,0x02,0x24,0x00,0x00,0x00,
    '<',  0x00,0x10,0x28,0x44,0x82,0x00, 
    '=',  0x00,0x28,0x28,0x28,0x28,0x00,
    '>',  0x00,0x82,0x44,0x28,0x10,0x00,
    '?',  0x00,0x20,0x4a,0x30,0x00,0x00, //
    '@',  0x00,0x00,0x00,0x00,0x00,0x00,
    'A',  0x00,0x7E,0x88,0x88,0x7E,0x00,
    'B',  0x00,0xFE,0x92,0x92,0x6C,0x00,
    'C',  0x00,0x7C,0x82,0x82,0x44,0x00,
    'D',  0x00,0xFE,0x82,0x82,0x7C,0x00,
    'E',  0x00,0xFE,0x92,0x92,0x82,0x00,
    'F',  0x00,0xFE,0x90,0x90,0x80,0x00,
    'G',  0x00,0x7C,0x82,0x92,0x5C,0x00,
    'H',  0x00,0xFE,0x10,0x10,0xFE,0x00,
    'I',  0x00,0x82,0xFE,0x82,0x00,0x00,
    'J',  0x00,0x0C,0x02,0x02,0xFC,0x00,
    'K',  0x00,0xFE,0x10,0x28,0xC6,0x00,
    'L',  0x00,0xFE,0x02,0x02,0x02,0x00,
    'M',  0x00,0xFE,0x40,0x30,0x40,0xFE,
    'N',  0x00,0xFE,0x40,0x30,0x08,0xFE,
    'O',  0x00,0x7C,0x82,0x82,0x82,0x7C,
    'P',  0x00,0xFE,0x90,0x90,0x60,0x00,
    'Q',  0x00,0x7C,0x82,0x8A,0x84,0x7A,
    'R',  0x00,0xFE,0x98,0x94,0x62,0x00,
    'S',  0x00,0x64,0x92,0x92,0x4C,0x00,
    'T',  0x00,0x80,0xFE,0x80,0x80,0x00,
    'U',  0x00,0xFC,0x02,0x02,0xFC,0x00,
    'V',  0x00,0xF0,0x0C,0x02,0x0C,0xF0,
    'W',  0x00,0xFE,0x04,0x38,0x04,0xFE,
    'X',  0x00,0xC6,0x38,0x38,0xC6,0x00,
    'Y',  0xC0,0x20,0x1E,0x20,0xC0,0x00,
    'Z',  0x00,0x86,0x9A,0xB2,0xC2,0x00,
    '!',  0x00,0x00,0x7a,0x00,0x00,0x00,
    ' ',  0x00,0x00,0x00,0x00,0x00,0x00,
    
};
 

const unsigned long int Gesturetable[4][10] PROGMEM = {
  {
    0b00000000000000001100001100000000, // littleUuh_code1
    0b00000000000000000110000110000000, // littleUuh_code2
    0b00000000000000000011000011000000, // littleUuh_code3
    0b00000000000000000110000110000000, // littleUuh_code4
    0b00000000000000001100001100000000, // littleUuh_code5
    0b00000000000000011000011000000000, // littleUuh_code6
    0b00000000000000110000110000000000, // littleUuh_code7
    0b00000000000000011000011000000000
  } // littleUuh_code8
  ,
  {
    0b00000000000000000000110000110000, // dreamMouth_code1
    0b00000000000000010000101000010000, // dreamMouth_code2
    0b00000000011000100100100100011000, // dreamMouth_code3
    0b00000000000000010000101000010000  // dreamMouth_code4
  } 
  ,
  {
    0b00100001000000000000000000100001, // adivinawi_code1
    0b00010010100001000000100001010010, // adivinawi_code2
    0b00001100010010100001010010001100, // adivinawi_code3
    0b00000000001100010010001100000000, // adivinawi_code4
    0b00000000000000001100000000000000, // adivinawi_code5
    0b00000000000000000000000000000000  // adivinawi_code6
  } 
  ,
  {
    0b00001100010010100001000000000000, // wave_code1
    0b00000110001001010000100000000000, // wave_code2
    0b00000011000100001000010000100000, // wave_code3
    0b00000001000010000100001000110000, // wave_code4
    0b00000000000001000010100100011000, // wave_code5
    0b00000000000000100001010010001100, // wave_code6
    0b00000000100000010000001001000110, // wave_code7
    0b00100000010000001000000100000011, // wave_code8
    0b00110000001000000100000010000001, // wave_code9
    0b00011000100100000010000001000000  // wave_code10
  } 
};

//"PROGMEM_readAnything.h"   FROM http://www.gammon.com.au/progmem
// modified for Titi use by Paul Van De Veen along with all PROGMEM mouths and gestures
#include <Arduino.h>  // for type definitions

template <typename T> void PROGMEM_readAnything (const T * sce, T& dest)
  {
  memcpy_P (&dest, sce, sizeof (T));
  }

template <typename T> T PROGMEM_getAnything (const T * sce)
  {
  static T temp;
  memcpy_P (&temp, sce, sizeof (T));
  return temp;
  }

#endif
