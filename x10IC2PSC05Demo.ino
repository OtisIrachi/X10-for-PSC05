//****************************************************************************
//  x10IC2PSC05Demo.ino
//  X10 PSC05 PowerLine Interface Demo for Arduino Uno.
//  Allows the user to Turn ON and OFF X10 Devices.
//  Monitors AC Line and displays X10 Data
//  No Dimming Functions are provided with this sofware.
//  All X10 Channels are supported.
//  by RCI
//
//  Requires X10RCI.cpp and X10RCI.h Library Files

//**************************************************************************
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C LCD(0x27,16,2);    // Blue LCD

#include <X10RCI.h>   // X10 Library,  See X10.cpp and X10.h
//****************************************************************************

#define ON 20
#define OFF 28

#define ZC 8                 // Black
#define TRANS 9              // Yellow      
#define RCV 10               // Green

#define PB1 11
#define PB2 12

                             // Red = GND
X10 X10(ZC, TRANS, RCV);     // ZC = Yel, Trans = Blk, Rcv = Red
//*** Variable Declarations***************************************************
//                           1234567890123456
const char Message1[17]   = "x10 PSC05 Demo  ";
const char Message2[17]   = "for UNO  By RCI ";
const char Message99[17]  = "                ";

String x10String = "";
String stringOn = "ON ";
String stringOff = "OFF";
char txt[4];
const char array_num = 40;
char house[array_num];
char unit[array_num];
char ONorOff[array_num];
char house_codes, unit_codes; 
char tmp; 
char row, col;
char count, slot;

//**************************************************************************
void Soft_I2C_LCD_Outn(char row, char col, const char s[])
{
  unsigned char i;
  col = col - 1;
   for (i = 0; s[i] != 0; i++)   // Display String Message
    {
    LCD.setCursor(col++, row - 1);
    LCD.print(s[i]);
    }
}
//**************************************************************************
void Soft_I2C_LCD_Out(char row, char col, char *text)
{
  unsigned char i;
  col = col - 1;
   for (i = 0; text[i] != 0; i++)   // Display String Message
      {
      LCD.setCursor(col++, row - 1);
      LCD.print(text[i]);
      }
}
//**************************************************************************
void Soft_I2C_LCD_Chr(char row, char column, char out_char)
{
   col = col - 1;
      LCD.setCursor(col++, row - 1);
      LCD.print(out_char);

}   
//*******************************************************************
void BytetoStr(unsigned char row, unsigned char col, unsigned char num)
{
    LCD.setCursor(col - 1, row - 1);      
    LCD.print(num); 
    LCD.print(" ");
    
} // End BytetoStr
//*********************************************************************************
void X10Display(char row, String hcode, int chan, char OnOff)
{
  String OnOffState;
  int str_len = hcode.length() + 1;
  char hc[str_len];
  x10String = "";
  
  if(OnOff == OFF)
     {
     OnOffState = stringOff;
     }
  if(OnOff == ON)
     {
     OnOffState = stringOn; 
     } 
  hcode.toCharArray(hc, str_len);      

  x10String += hcode;
  x10String += "  ";
  x10String += String (chan);
  x10String += "  ";
  x10String += OnOffState;
  
  LCD.setCursor(0, row - 1);
  LCD.print(x10String); 
     
  return;
}
//****************************************************************************
void setup()
{
  Serial.begin(115200);

  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);
  
  //*****Init LCD Display*****************
  LCD.begin();
  LCD.backlight();
 
  LCD.setCursor(0,0);
  LCD.print(Message1);
  LCD.setCursor(0,1);
  LCD.print(Message2);
  delay(2000);
  LCD.setCursor(0,1);
  LCD.print(Message99);

}   
//************************************************************************
void loop() 
{

  if(digitalRead(PB1) == 0)
    {
    while(digitalRead(PB1) == 0){}    
    X10.X10Trans('A', 10, ON);         
    X10Display(2, "A", 10, ON);
    }
    
  if(digitalRead(PB2) == 0)
    {
    while(digitalRead(PB1) == 0){}    
    X10.X10Trans('A', 10, OFF);          
    X10Display(2, "A", 10, OFF);
    }

   
// *** Reads Data from PSC05 PowerLine Module and Display on sLCD

     if(X10.X10_data_ready())
        {
        house_codes = X10.X10_read_first();            // Read Pulses from PSC05
        unit_codes = X10.X10_read_second();
        
        if (unit_codes <= 19)                      // If valid Chan Number
           {
           tmp = unit_codes;
           }
           
        if (unit_codes == 20)
           {                      //1234567890123456
           Soft_I2C_LCD_Outn(1, 1, "x10 Monitor     ");
           Soft_I2C_LCD_Chr(2, 1, house_codes);    // Display House Code
           BytetoStr(2, 4, tmp); 
           Soft_I2C_LCD_Outn(2, 7, "ON ");
           }

        if (unit_codes == 28)
           {
           Soft_I2C_LCD_Outn(1, 1, "x10 Monitor     ");
           Soft_I2C_LCD_Chr(2, 1, house_codes);    // Display House Code
           BytetoStr(2, 4, tmp);               
           Soft_I2C_LCD_Outn(2, 7, "OFF");
           }

        }// End While

 
} // End Loop
//****************************************************************************
//****************************************************************************
