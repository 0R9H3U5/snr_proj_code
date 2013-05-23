#define LIN_OUT 1
#define FHT_N 256
#include <FHT.h>
#include <ADC.h>

uint16_t TotalFHT[FHT_N/2];
volatile uint16_t TopFreq1 = 0;
volatile uint16_t TopFreq2 = 0;
volatile uint16_t TopFreq3 = 0;

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  
  MicrophoneSampling();
  //delay(1000);
  VibrationSampling();
  //delay(1000);
}

void MicrophoneSampling()
{
  int count = 0;
  ADC_InitMic();
  
  while(count < 100)
  {
    for(int i=0; i<FHT_N; i++)
    {
      ADC_Start();
      while(!(ADCSRA & 0x10));
     
      byte m = ADCL;
      byte j = ADCH;
      int k = (j<<8)|m;
      k -= 0x200;
      k<<=6;
      fht_input[i] = k;
    }
    
    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_lin();
    
    for(int i=0; i<FHT_N/2; i++)
    {
       TotalFHT[i] += fht_lin_out[i]; 
    }
    count++;
  }
  
  FindTopFrequencies(TotalFHT, FHT_N/2);
  Serial.println("Data");
  Serial.print("SoundFreq1:");
  Serial.println(TopFreq1, DEC);
  Serial.print("SoundFreq2:");
  Serial.println(TopFreq2, DEC);
  Serial.print("SoundFreq3:");
  Serial.println(TopFreq3, DEC);
  Reset();
}

void VibrationSampling()
{
  int count = 0;
  ADC_InitVib();
  
  while(count < 100)
  {
    for(int i=0; i<FHT_N; i++)
    {
      ADC_Start();
      while(!(ADCSRA & 0x10));
     
      byte m = ADCL;
      byte j = ADCH;
      int k = (j<<8)|m;
      k -= 0x200;
      k<<=6;
      fht_input[i] = k;
    }
    
    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_lin();
    
    for(int i=0; i<FHT_N/2; i++)
    {
       TotalFHT[i] += fht_lin_out[i]; 
    }
    count++;
  }
  
  FindTopFrequencies(TotalFHT, FHT_N/2);
  Serial.print("VibrationFreq1:");
  Serial.println(TopFreq1, DEC);
  Serial.print("VibrationFreq2:");
  Serial.println(TopFreq2, DEC);
  Serial.print("VibrationFreq3:");
  Serial.println(TopFreq3, DEC);
  Reset();
}

void Reset()
{
   for(int i=0; i<FHT_N/2; i++)
   {
      TotalFHT[i] = 0; 
   }
   
   ADC_Reset();
}

void FindTopFrequencies(uint16_t arr[], int n)
{
  uint16_t m1, m2, m3;
  arr[0] = 0;
  arr[1] = 0;
  arr[2] = 0;
  
  m1 = FindMax(arr, n);
  arr[m1] = 0;
  m2 = FindMax(arr, n);
  arr[m2] = 0;
  m3 = FindMax(arr, n);
  arr[m3] = 0;
  
  TopFreq1 = GetFrequency(m1);
  TopFreq2 = GetFrequency(m2);
  TopFreq3 = GetFrequency(m3);    
}

uint16_t GetFrequency(uint16_t binNum)
{
   return binNum * (9259.259 / FHT_N);
}

uint16_t FindMax(uint16_t array[], int N)
{
  uint16_t m = 0;
  uint16_t val = 0;
//  array[0] = 0;
//  array[1] = 0;
//  array[2] = 0;
  for(int i = 0; i<N; i++)
  {
   if (array[i] > val)
   {
     m = i;
     val = array[i]; 
   }
  }
  return m;
}
//
//ISR(ADC_vect)
//{
//  if (position >= FHT_N)
//    return;
//  byte m = ADCL;
//  byte j = ADCH;
//  int k = (j<<8)|m;
//  k -= 0x200;
//  k<<=6;
//  fht_input[position] = k;
//
//  position++;
//}


