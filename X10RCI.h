//***************************************************************************
//  X10RCI.h
//  Arduino Header File for X10.cpp
//  X10 Read and Write Data Driver
//  by RCI
//***************************************************************************
#ifndef X10_h
#define X10_h
//***************************************************************************
#include "Arduino.h"
//***************************************************************************
class X10
{
public:

X10(unsigned char ZcPin, unsigned char TransPin);
X10(unsigned char ZcPin, unsigned char TransPin, unsigned char RcvPin);
unsigned char X10_data_ready();
unsigned char X10_read_second();
unsigned char X10_read_first();
void X10Trans(unsigned char House, unsigned char Chan, unsigned char OnOff);

private:
   unsigned char zc, trans, rcv;
   unsigned char _zc, _trans, _rcv;
   unsigned char X10_read_bits(unsigned char num);
   void wait_for_zero_crossing();
   unsigned char shiftRight(unsigned char number);
   void X10_write(unsigned char house_code2, unsigned char unit_code2);
   void X10_write_bits(unsigned char datam, unsigned char numofbits, unsigned char           startbit);
};

#endif
//***************************************************************************