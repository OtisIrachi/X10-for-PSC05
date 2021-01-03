//***************************************************************************
//  X10RCI.cpp
//  PSC05 x10 Read and Write Interface Driver for Arduino
//  Allows the user to Turn ON and OFF X10 Devices.
//  Performs Write Functions,
//  Also performs Read Functions from Power Line.
//  No DIM Functions are provided.
//  by RCI
//  Connect PSC05:
//           P16F688          PSC05        Phone Cable	Signal
//              C0              1          Black/Org   	ZC
//              C1              4          Yellow		TRANS	
//              C2              3          Green		RCV			
//              GND             2          Red			GND
//  Connect PSC04:
//           P16F688          PSC04        Phone Cable	Signal
//              C0              1          Black/Org   	ZC
//              C1              4          Yellow		TRANS	
//              GND             3          Green		GND			
//              GND             2          Red			GND
//*************************************************************************** 
#include "Arduino.h"
#include "X10RCI.h"

//***************************************************************************
X10::X10(unsigned char ZcPin, unsigned char TransPin)
   {
   zc = ZcPin;
   trans = TransPin; 
   pinMode(zc, INPUT_PULLUP); 
   pinMode(trans, OUTPUT);
   _zc = zc;
   _trans = trans;
 
   } 
//***************************************************************************
X10::X10(unsigned char ZcPin, unsigned char TransPin, unsigned char RcvPin)
   {
   zc = ZcPin;
   trans = TransPin; 
   rcv = RcvPin; 
   pinMode(zc, INPUT_PULLUP); 
   pinMode(trans, OUTPUT);
   pinMode(rcv, INPUT_PULLUP);
   _zc = zc;
   _trans = trans;
   _rcv = rcv;
 
   } 
//***************************************************************************
unsigned char const House_Code_Table[17] = {'M','N','O','P','C','D','A','B','E',
                                  'F','G','H','K','L','I','J'};
unsigned char const Key_Code_Table[17] = {13, 14, 15, 16, 3, 4, 1, 2, 5, 6, 7,
                                  8, 11, 12, 9, 10};
unsigned char txt4[4], setbit;
unsigned char house_code, unit_code;
//***************************************************************************
unsigned char X10::shiftRight(unsigned char number)
{
   return number >>= 1;
}
//***************************************************************************
unsigned char X10::X10_data_ready()
{
   unsigned char temp;
   temp = !(digitalRead(_rcv));
   return temp;
}
//***************************************************************************
void X10::wait_for_zero_crossing()
{
   if(digitalRead(_zc))
      while(digitalRead(_zc)) ;
   else
      while(!digitalRead(_zc)) ;
}
//***************************************************************************
void X10::X10_write_bits(unsigned char datam, unsigned char numofbits, unsigned char startbit)
{
   unsigned char loopcount;

   for(loopcount = 1; loopcount <= numofbits; ++loopcount)
     {
     wait_for_zero_crossing();
     datam = datam >> 1;
     digitalWrite(_trans, (datam & 0x01));
     delay(1);
     digitalWrite(_trans, 0);
     if(startbit == 0)
        {
        wait_for_zero_crossing();
        digitalWrite(_trans, !(datam & 0x01));
        delay(1);
        digitalWrite(_trans, 0);
        }
     }
     return;
}
//***************************************************************************
void X10::X10_write(unsigned char house_code2, unsigned char unit_code2)
{
   unsigned char i;
   i = 0;
   while (House_Code_Table[i] != house_code2)
   i++;
   house_code2 = i;
   if(unit_code2 <= 16)
      {
      i = 0;
      while (Key_Code_Table[i] != unit_code2)
      i++;
      unit_code2 = i;
      }
   X10_write_bits(7 * 2, 4, 1);
   X10_write_bits(house_code2 * 2, 4, 0);
   X10_write_bits(unit_code2 * 2, 5, 0);
   X10_write_bits(0, 6, 1);
   return;
}
//***************************************************************************
unsigned char X10::X10_read_bits(unsigned char num)
{
unsigned char ic, dataval;

   for(ic = 1; ic <= num; ++ic)
      {
      wait_for_zero_crossing();
      delayMicroseconds(700);            // was 210    700
      if (digitalRead(_rcv) == 1)
         {
         setbit = 128;
         }
      else
         {
         setbit = 0;
         }
      dataval = dataval + setbit;
      dataval = shiftRight(dataval);
      wait_for_zero_crossing();
      delayMicroseconds(700);            // was 210    8334
   }
   for(ic = 0; ic < 7 - num; ++ic)
      {
      dataval = shiftRight(dataval);
      }
   return dataval;
}
//***************************************************************************
unsigned char X10::X10_read_second()   //(char house_code, char key_code)
{
   unit_code   = X10_read_bits(5);
   if(unit_code <= 16)
   unit_code = Key_Code_Table[unit_code];
   return unit_code;
}
//***************************************************************************
unsigned char X10::X10_read_first()   //(char house_code, char key_code)
{
   X10_read_bits(2);
   house_code = X10_read_bits(4);
   house_code = House_Code_Table[house_code];
   return house_code;
}
//****************************************************************************
void X10::X10Trans(unsigned char House, unsigned char Chan, unsigned char OnOff)
{
   X10_write(House, Chan);       // Send House and Channel First
   X10_write(House, Chan);       // Must Transmit Data Twice
   delay(1000);
   X10_write(House, OnOff);      // Send House and Key Code Next
   X10_write(House, OnOff);      // Must Transmit Data Twice
}
//***************************************************************************
