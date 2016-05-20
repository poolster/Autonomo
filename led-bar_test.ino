/*
* LED bar test program
*/
#define DATA_Pin 10  //DATA IN
#define CLK_Pin  11  //CLK IN
#define LED_pin 2
#define CmdMode 0x0000  //Work on 8-bit mode
#define ON 0x008f            //8-bit 1 data, brightness 0x01..0xff
#define SHUT 0x0000         //8-bit 0 data
 
void send16bitData(unsigned int data)
{
  unsigned char i;
  boolean clk_pin_state;
 
  clk_pin_state = digitalRead(CLK_Pin);
  for(i=0;i<16;i++)
  {
      if(data & 0x8000) digitalWrite(DATA_Pin, HIGH);
      else digitalWrite(DATA_Pin, LOW);
      clk_pin_state = !clk_pin_state;
      digitalWrite(CLK_Pin, clk_pin_state); 
      data <<= 1;
      delayMicroseconds(1);
   }
}
 
//latch routine for MY9221 data exchange
void latchData(void)
{
   unsigned char i;
   boolean data_pin_state;
 
   digitalWrite(DATA_Pin, LOW);
   data_pin_state = LOW;
   delayMicroseconds(300);
   for(i=0;i<8;i++)
   {
     data_pin_state = !data_pin_state;
     digitalWrite(DATA_Pin, data_pin_state);
     delayMicroseconds(1);
   }
   delayMicroseconds(300);
}
 
void sendLED(unsigned int LEDstate)
{
  unsigned char i;
 
  for(i=0;i<12;i++)
  {
    if(LEDstate&0x0001)send16bitData(ON);
    else  send16bitData(SHUT);
    LEDstate >>= 1;
  }
}
 
void setup()
{
    pinMode(DATA_Pin,OUTPUT);   //Data pin
    pinMode(CLK_Pin,OUTPUT);  //CLK pin
    pinMode(LED_pin,OUTPUT);  //LED_pin
    digitalWrite(DATA_Pin, LOW);
    digitalWrite(CLK_Pin, LOW);
}
 
void loop()
{
  unsigned int tmpInt;
 
  for(tmpInt=1;tmpInt < 0xFFF;tmpInt |= (tmpInt<<1))
  {
    send16bitData(CmdMode); //set LED Bar mode
    sendLED(tmpInt);        //send LED Bar data
    latchData();  //make it come into effect
    delay(100);
  }
  digitalWrite(LED_pin, LOW);
  for(;tmpInt != 0;tmpInt >>=1)
  {
    send16bitData(CmdMode); //set LED Bar mode
    sendLED(tmpInt);        //send LED Bar data
    latchData();  //make it come into effect
    delay(100);
  }
  send16bitData(CmdMode); //set LED Bar mode
  sendLED(0);        //send LED Bar data
  latchData();  //make it come into effect
  delay(100);
  digitalWrite(LED_pin, HIGH);
 
}

