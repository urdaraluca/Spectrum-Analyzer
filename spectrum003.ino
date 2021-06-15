#include "fix_fft.h"                                  // library to perfom the fixed-point in-place Fast Fourier Transform
#include <Wire.h>                                     // I2C library for OLED
#include <Adafruit_GFX.h>                             // graphics library for OLED
#include <Adafruit_SSD1306.h>                         // OLED driver

#define OLED_RESET 4                                  
Adafruit_SSD1306 display(OLED_RESET);                 

const int FFT_N =256;                                 
char im[FFT_N], data[FFT_N];                          // variables for the FFT
char x = 0, ylim = 60;                                
int i = 0, val,fall;                                      
int dropMax = 0;
int curMax;

void setup()
{
                             
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);          
  display.setTextSize(1);                             
  display.setTextColor(WHITE);                        
  display.clearDisplay();                            
  analogReference(DEFAULT);                          
};

void loop()
{
  int mini=1024, maxi=0;                               
  for (i = 0; i < FFT_N; i++) {                      
    val = analogRead(A0);                             
    data[i] = val / 4 - 128;                          
                                                     
    im[i] = 0;                                        
    if(val>maxi) maxi=val;                              
    if(val<mini) mini=val; 
    //if(fall<mini) mini=fall;                              
  };

  fix_fft(data, im, 8, 0);                            
 

  display.clearDisplay();                                          
     
  for (i = 1; i < FFT_N/2; i++) {                          
    int dat = sqrt(data[i] * data[i] + im[i] * im[i]);              //filter out noise and hum
    dat = (2-dat/60)*dat;                                         
    display.drawLine(0,63,128,63,WHITE);                              
    display.fillRect(i + x, ylim-dat+5, 2, 63-ylim+dat, WHITE);         
  };                                                
  display.setCursor(0,0);                             
  curMax=(dropMax+2*min((maxi-mini)/4-12,128))/3;    
  display.fillRect(0,0,curMax,3,WHITE);
  if (dropMax>curMax+4) {
    dropMax=dropMax-3;
    display.fillRect(dropMax-4,0,3,3,WHITE);
  } else {
    dropMax=curMax;
  }
  display.display();         
};  
