#include <math.h>

/*
  Sensor         | Index
 -----------------------
 Microphone      |  1
 Piezo	         |  2
 Level	         |  4
 Light	         |  8
 Temp            | 16
 Contact         | 32
 Motion          | 64
 */

#define SENSOR_COUNT 7
#define ON 1
#define OFF 0

#define WASHING_MACHINE 19  //0010011
#define DRYER 19            //0010011
#define GARAGE_DOOR 68      //1000100
#define TELEVISION 115      //1110011
#define SUMP_PUMP 34        //0100010
#define CUSTOM 27           //0011011

//number of samples to average for single value sensors
#define SINGLE_VAL_SAMPLES 5

// Pins used for analog sensor input
#define LIGHT_PIN A3
#define TEMPERATURE_PIN A4

//pins used for digital sensor input
#define MOTION_PIN 2
#define CONTACT_PIN 3

int light_level = 0;
int motion = 0;
int sensors = 0;

double temperature_read()
{
  //Determine the temperature using the Steinhart-Hart equation
  uint8_t i;
  float average = 0;
  int samples[SINGLE_VAL_SAMPLES];

  // take N samples in a row, with a slight delay
  for (i = 0; i < SINGLE_VAL_SAMPLES; i++) 
  {
    samples[i] = analogRead(TEMPERATURE_PIN);
  }

  // average all the samples out
  for (i = 0; i < SINGLE_VAL_SAMPLES; i++) 
  {
    average += samples[i];
  }
  average /= SINGLE_VAL_SAMPLES;

  // convert the value to resistance
  average = (1023.0 / average) - 1;
  average = 100000.0 / average; //[10000 is series resistor value]
  
  float steinhart;
  steinhart = average / 100000.0; // (R/Ro)  [100000 is nominal thermistor value]
  steinhart = log(steinhart); // ln(R/Ro)
  steinhart /= 4275; // 1/B * ln(R/Ro)  [4038 is beta coefficient (usually 3000-4000)]
  steinhart += 1.0 / (25 + 273.15); // + (1/To) [25 is temp. for nominal resistance (almost always 25 C)]
  steinhart = 1.0 / steinhart; // Invert
  steinhart -= 273.15; // convert to C
  
  return steinhart;
}

int* mic_read()
{
  int mic[] = {8,24,60,128,256};
  return mic;
}

int* piezo_read()
{
  int piezo[] = {4,8,16,32,64};
  return piezo;
}

int contact_read()
{
  return 1;
}

int level_read()
{
  return 1;
}

int motion_read()
{
  return digitalRead( MOTION_PIN );
}

int lightlevel_read()
{
  return analogRead( LIGHT_PIN );
}

void setup()
{
  //configure analog i/o
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  
  // Configure digital i/o
  pinMode(MOTION_PIN, INPUT);
  pinMode(CONTACT_PIN, INPUT);

  sensors = CUSTOM;

  Serial.begin(115200);
}

void loop()
{
  String message = "";
  
  if(sensors & 16)
  {
    //Serial.println("Temperature");
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "temp=";
    message += (int)temperature_read();  
  }

  if(sensors & 8)  
  {
    //Serial.println("Light");
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "light=";
    message += (int)lightlevel_read();  
  }

  if(sensors & 64)
  {
    //Serial.println("Motion");
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "motion=";
    message += motion_read();  
  }
  
  if(sensors & 1)
  {
    //Serial.println("Microphone");
    int *mic = mic_read();
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "sound=";
    message += mic[0];
    message += "&sound=";
    message += mic[1];
    message += "&sound=";
    message += mic[2];
    message += "&sound=";
    message += mic[3];
    message += "&sound=";
    message += mic[4]; 
  }

  if(sensors & 2)
  {
    //Serial.println("Peizo");
    int* piezo = piezo_read();
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "sound=";
    message += piezo[0];
    message += "&sound=";
    message += piezo[1];
    message += "&sound=";
    message += piezo[2];
    message += "&sound=";
    message += piezo[3];
    message += "&sound=";
    message += piezo[4];  
  }

  if(sensors & 4)
  {
    //Serial.println("Level");
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "motion=";
    message += level_read();  
  }

  if(sensors & 32)
  {
    //Serial.println("Contact");
    if(message.length() > 0)
    {
      message += "&";
    }
    
    message += "motion=";
    message += contact_read();  
  }
  
  Serial.println(message);
  
  delay(5000);
}


