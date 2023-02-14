/*
 * MaxMatrix
 * Version 1.0 Feb 2013
 * Copyright 2013 Oscar Kin-Chung Au
 * Adapted for Titi version
 */

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

#ifndef _Titi_matrix_H_
#define _Titi_matrix_H_

#include "Arduino.h"
#include "Titi_gestures.h"

#if defined(ESP32)
# include <SPI.h>
#endif

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

class Titi_Matrix
{
  private:
    byte data;
    byte load;
    byte clock;
    byte num;
    byte buffer[8];
    byte CHARbuffer[80];
    int  rotation;
    void reload();
    char rotation2;
    
  public:
    Titi_Matrix();
    
    void init(byte data, byte load, byte clock, byte num, int rotation);
    void clearMatrix();
    void setCommand(byte command, byte value);
    void setIntensity(byte intensity);
    void setColumn(byte col, byte value);
    void setColumnAll(byte col, byte value);
    void setDot(byte col, byte row, byte value);
    void writeFull(unsigned long value);
    void sendChar ( const byte data, byte pos, byte number, byte scrollspeed);
};

#endif
