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

//array indexes of the sensors
#define MICROPHONE_IDX 0
#define PIEZO_IDX 1
#define LEVEL_IDX 2
#define LIGHT_IDX 3
#define TEMPERATURE_IDX 4
#define CONTACT_IDX 5
#define MOTION_IDX 6

#define WASHING_MACHINE 19  //0010011
#define DRYER 19            //0010011
#define GARAGE_DOOR 68      //1000100
#define TELEVISION 115      //1110011
#define SUMP_PUMP 34        //0100010

//number of samples to average for single value sensors
#define SINGLE_VAL_SAMPLES 5

// Pins used for analog sensor input
#define LIGHT_PIN A5
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
  Serial.print("Average analog reading ");
  Serial.println(average);

  // convert the value to resistance
  average = (1023.0 / average) - 1;
  average = 10000.0 / average; //[10000 is series resistor value]
  Serial.print("Thermistor resistance ");
  Serial.println(average);
  float steinhart;
  steinhart = average / 10000.0; // (R/Ro)  [10000 is nominal thermistor value]
  steinhart = log(steinhart); // ln(R/Ro)
  steinhart /= 4038; // 1/B * ln(R/Ro)  [4038 is beta coefficient (usually 3000-4000)]
  steinhart += 1.0 / (25 + 273.15); // + (1/To) [25 is temp. for nominal resistance (almost always 25 C)]
  steinhart = 1.0 / steinhart; // Invert
  steinhart -= 273.15; // convert to C

  Serial.print("---Temp---");
  Serial.print(steinhart);
  Serial.print("---");
  Serial.println((steinhart*1.8)+32);

  return steinhart;
}

void motion_read()
{
  motion = digitalRead(MOTION_PIN);

  if(motion == 1)
  {
    Serial.println("motion detected!");  
  }  
}

void lightlevel_read()
{
  light_level = analogRead(LIGHT_IDX);
  Serial.println(light_level);  
}

void setup()
{
  //configure analog i/o
  pinMode(LIGHT_IDX, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  
  // Configure digital i/o
  pinMode(MOTION_PIN, INPUT);
  pinMode(CONTACT_PIN, INPUT);

  sensors = WASHING_MACHINE;

  Serial.begin(115200);
}

void loop()
{
  if(sensors & 16)
  {
    Serial.println("Temperature");
    temperature_read();  
  }

  if(sensors & 8)  
  {
    Serial.println("Light");
    //lightlevel_read();  
  }

  if(sensors & 64)
  {
    Serial.println("Motion");
    //motion_read();  
  }
  
  if(sensors & 1)
  {
    Serial.println("Microphone");
    // mic_read();  
  }

  if(sensors & 2)
  {
    Serial.println("Peizo");
    // piezo_read();  
  }

  if(sensors & 4)
  {
    Serial.println("Level");
    // level_read();  
  }

  if(sensors & 32)
  {
    Serial.println("Contact");
    // contact_read();  
  }
  delay(5000);
}


