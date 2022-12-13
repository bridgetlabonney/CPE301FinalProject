//Bridget LaBonney
//CPE 301
//Final Project

//TO DO
//display
//pin changes and proper values for LEDs & Fan
//buttons
//water monitoring
//clock


#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
#include <SevSeg.h>
//VARIABLES

//display
SevSeg sevseg;

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking tempMonitor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
//serial port stuff
 #define RDA 0x80
 #define TBE 0x20  
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
 volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;
 volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;




//water/humidity monitor & fan variables
bool WaterMonitorEnable = true;
bool tempMonitorEnable = true;
bool FanMotorBool = false;
unsigned int waterThresh = 10;
unsigned int tempThresh = 60;
float temperature = 60;
float humidity = 0;

//led
volatile unsigned char* port_b = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
//button
volatile unsigned char* port_h = (unsigned char*) 0x25; 
volatile unsigned char* ddr_h = (unsigned char*) 0x0;
//button pins
volatile unsigned char* pin_start = 0;
volatile unsigned char* pin_reset = 0;
//fan
volatile unsigned char* port_e = (unsigned char*) 0x25; 
volatile unsigned char* ddr_e = (unsigned char*) 0x0;


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

//ADC
void adc_init()
{
  // setup the A register
  *my_ADCSRA |=  0x80; // set bit   7 to 1 to enable the ADC
  *my_ADCSRA &=  0xC0;// clear bit 5 to 0 to disable the ADC trigger mode
  *my_ADCSRA &=  0xC0;// clear bit 5 to 0 to disable the ADC interrupt
  *my_ADCSRA &=  0xC0;// clear bit 5 to 0 to set prescaler selection to slow reading
  // setup the B register
  *my_ADCSRB &=  0x70;// clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &=  0x70;// clear bit 2-0 to 0 to set free running mode
  // setup the MUX Register
  *my_ADMUX  &=  0x7C;// clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX  |=  0x7C;// set bit   6 to 1 for AVCC analog reference
  *my_ADMUX  &=  0x40;// clear bit 5 to 0 for right adjust result
  *my_ADMUX  &=  0x40;// clear bit 5 to 0 for right adjust result
  *my_ADMUX  &=  0x40;// clear bit 4-0 to 0 to reset the channel and gain bits
}
unsigned int adc_read(unsigned char adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX  &= 0x40; 
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0x70;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0x8;
  }
  // set the channel selection bits
  *my_ADMUX  += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}

//state functions
void IDLE() {
  //turn all LED's off except for green
  stateReport(0);
  //turn on greenLED
  *port_b &= 0b0010000;
  *port_b |= 0b0010000;
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
  *port_b |= 0x40;
}

void RUNNING() {
  //timestamp
  stateReport(2);
 //turn motor on
 FanMotor(true);
//blue LED on, turn all others off
*port_b &= 0x40;
}

void DISABLED() {
  stateReport(1);
  //turn yellow LED on
  *port_b &= 0b01000000; 
  WaterMonitorEnable = false;
  tempMonitorEnable = false;
}

void FanMotor(bool on) {
  //check if the fan motor is on or off already
  if(FanMotorBool) { 
    if(!on) {//we want it off and its on, turn it off

    ClockReport();
    }
    //otherwise do nothing
  }
  else {
    if(on) {
      //turn it on!
      ClockReport();
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
if(read_thmonitor(&temperature, &humidity) == true) {
//display temp/humidity
}
else {
  //display error
}

}

static bool read_thmonitor(float *temperature, float *humidity)
{
  static unsigned long timestamp = millis( );

  if(millis( ) - timestamp > 1000ul)
  {
    if(tempMonitor.measure(temperature, humidity) == true)
    {
      timestamp = millis( );
      return true;
    }
  }

  return false;
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


 //fan setup
  //set all fan pins to output
  //1 pin needs to go high, the other needs to go low,
  //and fan should be off
  //setup LED, button, and fan pins
  *ddr_b |= 0b11111111; //led
  *ddr_e |= 0b00011010; //fan pins
  //buttons
  *pin_start &= 0x4;
  *pin_reset &= 0x4;

  //set the baud rate for serial monitor
  U0init(9600); 

  //adc
  adc_init();

  
  //setup display

  byte NumberOfDisplays = 4;
  byte NumberOfSharedCommon[] = {44, 46, 48, 50}; // for if you want to config the common pins for multiple displays
  
  // A B C D E F G DP IN ORDER
  byte segmentPins[] = {22,24,26,28,30,32,34};
  bool resistorsOnSegments = true; // is the resistor on the 
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  //sevseg.setSegments();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(state == NULL) IDLE(); //should be placed in IDLE when powered on
  //temp too high? time to cool it down
  if(temperature > tempThresh) {
    RUNNING();
  }
  else IDLE(); //temp is at our desired temp, turn it to idle
  if(true) { //start/stop button pressed
  if(state == 2) DISABLED(); 
  else RUNNING();
  }
  if(true) IDLE(); //reset button pressed
  if(getWaterLevel() >= waterThresh) IDLE();
  if(state != 1) {
    THMR();
  } 
  else ERROR();

}
