#include <TFT_eSPI.h> 
#include <RotaryEncoder.h>
#include <FastLED.h>
#include "fonts.h"
TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);

#define NUM_LEDS 3
#define DATA_PIN 42
#define CLOCK_PIN 45
CRGB leds[NUM_LEDS];

#define PIN_IN1 2
#define PIN_IN2 1

#define color1 TFT_WHITE
#define color2  0x8410
#define color3 TFT_ORANGE
#define color4 TFT_BLACK  //background
#define color5 0xC638

unsigned short colorsF[3]={TFT_WHITE,TFT_BLACK,TFT_WHITE};
unsigned short colorsB[3]={TFT_BLACK,color5,TFT_BLACK};
unsigned short colorsL[3]={TFT_RED,TFT_BLACK,TFT_RED};
unsigned short colorsD[3]={TFT_WHITE,TFT_BLACK,TFT_RED};
volatile int counter = 0;
float VALUE[3];
float lastValue=0;

double rad=0.01745;
float x[3][360];
float y[3][360];

float px[3][360];
float py[3][360];

float lx[3][360];
float ly[3][360];

int r2=156;
int sx=160;
int sy=170;

String cc[10]={"0","10","20","30","40","50","60","70","80","90"};
int start[3][10];
int startP[3][60];

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

const int ledPin2 = 27;
const int freq2 = 5000;
const int ledChannel2 = 2;
const int resolution2 = 8;
int dutyCycle2 = 128;

bool onOff=0;
bool debounce=0;

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
int chosen=0;

void setup() {
     pinMode(0,INPUT_PULLUP);
     pinMode(46, OUTPUT);
     digitalWrite(46, HIGH);

     pinMode(15, OUTPUT);
     digitalWrite(15, HIGH);

     FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
   
  tft.begin();
  tft.writecommand(0x11);
  tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(4);
        
    img.setSwapBytes(true);
    img.createSprite(320, 170);
    

int b[3]={0,0};
int b2[3]={0,0};

    for(int j=0;j<3;j++){
      int r=r2-j*38;
      for(int i=0;i<360;i++)
    {
       x[j][i]=((r-(j*4))*cos(rad*i))+sx;
       y[j][i]=((r-(j*4))*sin(rad*i))+sy;
       px[j][i]=((r-16)*cos(rad*i))+sx;
       py[j][i]=((r-16)*sin(rad*i))+sy;

       lx[j][i]=((r-21)*cos(rad*i))+sx;
       ly[j][i]=((r-21)*sin(rad*i))+sy;
       
       if(i%36==0){
       start[j][b[j]]=i;
       b[j]++;
       }

       if(i%6==0){
       startP[j][b2[j]]=i;
       b2[j]++;
       }
      }}

        leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  FastLED.show();
  

}
int angle[3]={270};
int lastAngle[3]={0};
int deb=0;

void loop() {

 if(digitalRead(0)==0){
  if(deb==0){
      deb=1;
      chosen++;
    if(chosen>2) chosen=0; delay(100);}
  }else {deb=0;}
  
 static int lastCounter = 0;
 read_encoder();

  if(angle[chosen] != lastCounter){
  lastCounter = angle[chosen]; }

  if(angle[chosen]!=lastAngle[chosen])
  {
     lastAngle[chosen]=angle[chosen];      

     VALUE[chosen]=((angle[chosen]-270)/3.60)*-1;
     if(VALUE[chosen]<0)
     VALUE[chosen]=VALUE[chosen]+100;    
     
 img.fillSprite(color4);
 img.setTextDatum(4);
 img.fillSmoothCircle(sx,sy,170,TFT_SILVER,color4);
 img.fillSmoothCircle(sx,sy,169,TFT_BLACK,TFT_SILVER);
 img.fillSmoothCircle(sx,sy,130,color5,TFT_BLACK);
 img.fillSmoothCircle(sx,sy,89,TFT_BLACK,color5);
 img.setTextColor(TFT_WHITE,TFT_BLACK);
 img.setFreeFont(&FreeSans9pt7b);



   for(int j=0;j<3;j++)
  for(int i=0;i<60;i++)
  if(startP[j][i]+angle[j]<360)
 img.fillCircle(px[j][startP[j][i]+angle[j]],py[j][startP[j][i]+angle[j]],1,colorsD[j]);
 else
 img.fillCircle(px[j][(startP[j][i]+angle[j])-360],py[j][(startP[j][i]+angle[j])-360],1,colorsD[j]);

 for(int j=0;j<3;j++){
  if(j==0)img.setFreeFont(&FreeSans9pt7b);
  if(j==1)img.setTextFont(2);
  if(j==2)img.setTextFont(0);
  img.setTextColor(colorsF[j],colorsB[j]);
 for(int i=0;i<10;i++)
 if(start[j][i]+angle[j]<360){
 img.drawString(cc[i],x[j][start[j][i]+angle[j]],y[j][start[j][i]+angle[j]]);
 img.drawLine(px[j][start[j][i]+angle[j]],py[j][start[j][i]+angle[j]],lx[j][start[j][i]+angle[j]],ly[j][start[j][i]+angle[j]],colorsL[j]);
 }
 else
 {
 img.drawString(cc[i],x[j][(start[j][i]+angle[j])-360],y[j][(start[j][i]+angle[j])-360]);
 img.drawLine(px[j][(start[j][i]+angle[j])-360],py[j][(start[j][i]+angle[j])-360],lx[j][(start[j][i]+angle[j])-360],ly[j][(start[j][i]+angle[j])-360],colorsL[j]);
 }}

img.fillTriangle(154,sy-40,166,sy-40,160,sy-56,TFT_ORANGE);
img.drawLine(160,30,160,sy-56,TFT_PURPLE);
 img.setFreeFont(&DSEG7_Classic_Bold_18);
 img.drawString(String((int)VALUE[0])+" . "+String((int)VALUE[1])+" . "+String((int)VALUE[2]),sx,sy-10);
 img.setTextFont(0);
 img.drawString("POWER",sx,sy-32);
 img.setTextDatum(0);
 img.setTextColor(TFT_WHITE,color4);

 img.drawString(String(VALUE[0]),12,6);
 img.drawString(String(VALUE[1]),12,16);
 img.drawString(String(VALUE[2]),12,26);

 img.fillCircle(6,(chosen+1)*10,3,TFT_ORANGE);

    
    img.drawCircle(280,18,7,TFT_WHITE);
    img.drawCircle(300,18,7,TFT_WHITE);
    img.fillCircle(300,18,6,TFT_BLACK);
    img.drawLine(280,11,300,11,TFT_WHITE);
    img.drawLine(280,25,300,25,TFT_WHITE);
    img.fillRect(281,12,18,13,TFT_BLACK);
    
    img.fillCircle(280,18,4,TFT_ORANGE); 
 

 
  img.pushSprite(0, 0);

 if(VALUE==0 || onOff==1)
 {
}else{
 dutyCycle2=map((int)VALUE,0,100,0,255);
 ledcWrite(ledChannel2,dutyCycle2);}
  }
}

void read_encoder() {

  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    
    if(newPos>pos)
    angle[chosen]=angle[chosen]+2;
      if(newPos<pos)
    angle[chosen]=angle[chosen]-2;
    
    pos = newPos;
  } 
  if(angle[chosen]>=360)
  angle[chosen]=0;

  if(angle[chosen]<0)
  angle[chosen]=354;
}
