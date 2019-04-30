//fast fourier transform light-control

#include <HexWear.h>
#include "arduinoFFT.h"
 
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC
#define MIC_PIN A4       // This is the microphone pin on the HexWear
#define SWITCH1 A7
#define SWITCH2 A9
#define SWITCH3 A10

#define THRESHOLD 1000

arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;  //This is part of the FFT stuff
unsigned long microseconds;       //This is part of the FFT stuff
 
double vReal[SAMPLES];            //This is part of the FFT stuff
double vImag[SAMPLES];            //This is part of the FFT stuff

void setup() {
    Serial.begin(115200);         //This starts serial transmission at a very high rate 
                                  //because you're visualizing frequencies
    
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Gets sampling period in microseconds 
    
    // Initialize the microphone pin as input
    pinMode(MIC_PIN, INPUT);

    pinMode(SWITCH1, OUTPUT);
    pinMode(SWITCH2, OUTPUT);
    pinMode(SWITCH3, OUTPUT);
}
 
void loop() {

    digitalWrite(SWITCH1, 0);
    digitalWrite(SWITCH2, 0);
    digitalWrite(SWITCH3, 0);
    
    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        // Read data from microphone pin
        vReal[i] = analogRead(MIC_PIN);
        vImag[i] = 0;
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/ //Either have the "Serial.println(vReal[i],1) line by itself in the Serial Plotter, or have all these lines uncommented
    Serial.print("\nPeak frequency: ");
    Serial.println(peak);             //Print out what frequency is the most dominant.
    Serial.println("Samples: ");      //Print out what frequency is the most dominant.
    for(int i=10; i<(SAMPLES/2); i++)    //Print out the bins and samples
    {                                   //The bins don't correspond to the correct frequency here
    
        /*View all these lines in serial terminal to see which frequencies has which amplitudes*/
        Serial.print("Sample bin: "); 
        Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        Serial.print("\tAmplitude: ");
        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins

        

        //light control
        if(i < (SAMPLES/6)){
          if(vReal[i] > THRESHOLD){
            digitalWrite(SWITCH1, 100);
            digitalWrite(SWITCH2, 0);
            digitalWrite(SWITCH3, 0);
            delay(10);
          }
        }else if(i < (SAMPLES/3)){{
          if(vReal[i] > THRESHOLD){
            digitalWrite(SWITCH2, 100);
            digitalWrite(SWITCH1, 0);
            digitalWrite(SWITCH3, 0);
            delay(10);
          }
        }
        }else if(vReal[i] > THRESHOLD){
          digitalWrite(SWITCH3, 100);
          digitalWrite(SWITCH2, 0);
          digitalWrite(SWITCH1, 0);
          delay(10); 
        }
        
    }
    
}
