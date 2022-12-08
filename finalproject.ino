//Bridget LaBonney
//CPE 301
//Final Project

//TO DO
//temp/humid monitoring
//display
//pin changes and proper values for LEDs
//buttons
//water monitoring
//motor
//clock
//ISR???



//VARIABLES

//buttons
volatile unsigned char* startStop = 0;
volatile unsigned char* reset = 0;

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
 

//RED LED
volatile unsigned char* port_r = (unsigned char*) 0x25; 
volatile unsigned char* ddr_r  = (unsigned char*) 0x24; 
 

//BLUE LED
volatile unsigned char* port_b = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 


//GREEN LED
volatile unsigned char* port_g = (unsigned char*) 0x25; 
volatile unsigned char* ddr_g  = (unsigned char*) 0x24; 


//water/humidity monitor & fan variables
bool WaterMonitorEnable = true;
bool tempMonitorEnable = true;
bool FanMotorBool = false;
unsigned int waterThresh = 10;
unsigned int tempThresh = 60;

//for operating the loop 
unsigned int state = NULL; //0 idle, 1 disabled, 2 running, 3 error

// Timer Pointers
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int *myTCNT1 = (unsigned int *) 0x84;
volatile unsigned char *myTIFR1 = (unsigned char *) 0x36;

void ClockReport() {
//This gets the current real time and reports it to the Serial monitor
int currentTime = 0;
printChar(currentTime);
}

//state functions
void IDLE() {
  //turn all LED's off except for green
  stateReport(0);
  //turn on greenLED
  *port_g &= 0x87; //needs a proper pin
  FanMotor(false);
  
  
}

void ERROR() {
  //timestamp
  stateReport(3);
  //turn motor off
  FanMotor(false);
  //reset button -> idle
  //if(button pressed) { IDLE(); }
  //stop button -> disabled
  //red led on, turn all others off
  *port_r &= 0x87;
  *port_g &= 0x11;
  *port_y |= 0x40; //nice
  *port_b |= 0x11;
}
void RUNNING() {
  //timestamp
  stateReport(2);
 //turn motor on
 FanMotor(true);
//blue LED on, turn all others off
*port_r &= 0x87;
  *port_g &= 0x11;
  *port_y |= 0x40; //nice
  *port_b |= 0x11;
}

void DISABLED() {
  stateReport(1);
  //turn yellow LED on
  *port_y &= 0xBF; 
  WaterMonitorEnable = false;
  tempMonitorEnable = false;
}

void FanMotor(bool on) {
  //check if the fan motor is on or off already
  if(FanMotorBool) { 
    if(!on) {//we want it off and its on, turn it off
    }
    //otherwise do nothing
  }
  else {
    if(on) {
      //turn it on!
    }
    //otherwise, we want it off and it's off, do nothing 
  }
}

void stateReport(int x){
  //get a timestamp and print the state, as well as change the state tracker
  state = x;
  char s;
  switch(x) {
    case 0:
      s = "IDLE";
      break;
    case 1:
      s = "DISABLED";
      break;
    case 2:
      s = "RUNNING";
      break;
    case 3:
      s = "ERROR";
      break;
    default: //big miss steak
      s = "NULL";
  }
  printChar(s);
  ClockReport();

  //every state with this function needs humidity/temp monitoring

}

//temp/humidity monitoring
int getTemp() {
 return 0;
}

int getWaterLevel() {
  return 0;
}

void THMR() {
//the temperature and humidity monitoring routine

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

  //setup water monitor

  //setup humidity/temp monitor

  //setup display

}

void loop() {
  // put your main code here, to run repeatedly:
  if(state == NULL) IDLE(); //should be placed in IDLE when powered on
  //temp too high? time to cool it down
  if(getTemp() > tempThresh) {
    RUNNING();
  }
  else IDLE(); //temp is at our desired temp, turn it to idle
  if(true) { //start/stop button pressed
  if(state == 2) DISABLED(); 
  else RUNNING();
  }
  if(true) IDLE(); //reset button pressed
  if(getWaterLevel() >= waterThresh) IDLE();
  if(state != 1) THMR();
  else ERROR();

}
