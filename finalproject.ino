//Bridget LaBonney
//CPE 301
//Final Project

//VARIABLES

//serial port stuff
 #define RDA 0x80
 #define TBE 0x20  
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
 volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;

//currently on pin 12 (YELLOW LED)
volatile unsigned char* port_y = (unsigned char*) 0x25; 
volatile unsigned char* ddr_y  = (unsigned char*) 0x24; 
volatile unsigned char* pin_y  = (unsigned char*) 0x23; 

//RED LED
volatile unsigned char* port_r = (unsigned char*) 0x25; 
volatile unsigned char* ddr_r  = (unsigned char*) 0x24; 
volatile unsigned char* pin_r  = (unsigned char*) 0x23; 

//BLUE LED
volatile unsigned char* port_b = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
volatile unsigned char* pin_b  = (unsigned char*) 0x23; 

//GREEN LED
volatile unsigned char* port_g = (unsigned char*) 0x25; 
volatile unsigned char* ddr_g  = (unsigned char*) 0x24; 
volatile unsigned char* pin_g  = (unsigned char*) 0x23; 

bool WaterMonitorEnable = true;
bool FanMotor = false;
void ClockReport() {
//This gets the current real time and reports it to the Serial monitor

}

//state functions
void IDLE() {
  //timestamp
  stateReport("IDLE");
  //turn on greenLED
  *port_g &= 0x87;
  
}

void ERROR() {
  //timestamp
  //turn motor off
  //reset button -> idle
  //red led on, turn all others off
  *port_r &= 0x87;
  *port_g &= 0x11;
  *port_y &= 0x69; //nice
  *port_b |= 0x11;
}

void FanMotor() {
  
  if(on) {
    //turn on fan motor
  }
  else { //turn it off 
  }
}

void stateReport(char x) {
  //get a timestamp and print the state
  printChar(x);
  ClockReport();
  //humidity/temp monitoring
if(WaterMonitorEnable) {

}  

}

void RUNNING() {
  //timestamp
 //turn motor on
 FanMotor(true);
 //humidity/temp monitoring
 // -> idle if below water threshold
 // -> ERROR() if water is too low
//blue LED on, turn all others off
*port_r &= 0x87;
  *port_g &= 0x11;
  *port_y &= 0x69; //nice
  *port_b |= 0x11;
}

void DISABLED() {
  stateReport("DISABLED");
  //turn yellow LED on
  *port_y &= 0x87;
  WaterMonitorEnable = false;
}

//serial monitor functions

void U0init(unsigned long baud)
{
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / baud - 1);
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}

unsigned char getChar()
{
 return *myUDR0; 
}

unsigned char available()
{
  return (RDA & *myUCSR0A);
}

unsigned char printChar(unsigned char data) {
while((*myUCSR0A & TBE) == 0) {};
  *myUDR0 = data;
}

void setup() {
  // put your setup code here, to run once:

  //setup LEDs
  *ddr_b |= 0x80;
  *ddr_r |= 0x80;
  *ddr_y |= 0x80;
  *ddr_g |= 0x80;

  //set the baud rate for serial monitor
  U0init(9600); 

}

void loop() {
  // put your main code here, to run repeatedly:
  if(true) IDLE(); //should be placed in IDLE when powered on;

}
