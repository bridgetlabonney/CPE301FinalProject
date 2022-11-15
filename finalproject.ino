//Bridget LaBonney
//CPE 301
//Final Project

//VARIABLES

//currently at port PB7 (on-board LED) (YELLOW LED)
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

void ClockReport() {
//This gets the current real time and reports it to the Serial monitor
}

void WaterMonitorEnable(bool x) {
  if(x) {}; //enable water monitor
  else //disable water monitor
}

void IDLE() {
  //timestamp
  //turn on greenLED
  *port_g &= 0x87;
  //if stop button is pressed, DISABLED()
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

void stateReport() {
  //stop button -> turn fan motor off and go to DISABLED()
  //humidity and temp control report on
  //respond to vent position.
}

void RUNNING() {
  //timestamp
 //turn motor on
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
  //timestamp
  //turn yellow LED on
  *port_y &= 0x87;
  WaterMonitorEnable(false);
}

void setup() {
  // put your setup code here, to run once:
  *ddr_b |= 0x80;
  *ddr_r |= 0x80;
  *ddr_y |= 0x80;
  *ddr_g |= 0x80;

}

void loop() {
  // put your main code here, to run repeatedly:

}
