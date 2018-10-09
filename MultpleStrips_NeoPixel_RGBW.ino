//#############################################################################
//                    University Carlos III of Madrid
//             Department of Systems Engineering and Automation
// Summary: arduino code for controlling the LedStrip in the chest of robot TEO
// Several functions to show different lighting color effects were implemented
// (e.g. boomerang, energy, etc). Additionally, a bluetooth control by smartphone
// was included. The code was tested in Arduino Nano, Arduino Mega ADK and Arduino
// Uno boards.
// Author: edwinDOS (09/10/2018)
//#############################################################################

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <SoftwareSerial.h>

#define PINA 4 //LedStrip 1 -- Rayo izquierda
#define PINB 5 //LedStrip 2 -- Rayo derecha
#define PINC 6 //LedStrip 3 -- Rayo arriba
#define PIND 8 //Circle small
#define PINE 7//Circle large

#define NUM_LEDSA 32 //LedStrip 1
#define NUM_LEDSB 32 //LedStrip 2
#define NUM_LEDSC 11 //LedStrip 3
#define NUM_LEDSD 24 //Circle small
#define NUM_LEDSE 45 //Circle large

#define BRIGHTNESS 100

SoftwareSerial mySerial(2, 3); // RX, TX

Adafruit_NeoPixel strip_r1 = Adafruit_NeoPixel(NUM_LEDSA, PINA, NEO_GRBW + NEO_KHZ800); //LedStrip rayo izquierda
Adafruit_NeoPixel strip_r2 = Adafruit_NeoPixel(NUM_LEDSB, PINB, NEO_GRBW + NEO_KHZ800); //LedStrip rayo derecha
Adafruit_NeoPixel strip_r3 = Adafruit_NeoPixel(NUM_LEDSC, PINC, NEO_GRBW + NEO_KHZ800); //LedStrip rayo arriba
Adafruit_NeoPixel strip_cp = Adafruit_NeoPixel(NUM_LEDSD, PIND, NEO_GRBW + NEO_KHZ800); //LedStrip circulo pequeño
Adafruit_NeoPixel strip_cg = Adafruit_NeoPixel(NUM_LEDSE, PINE, NEO_GRB + NEO_KHZ800); //LedStrip circulo grande

int flag = 0;
char Ledmode = 0;
int r = 0;
int g = 0;
int b = 0;
int w = 10;

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  //***LedStrip 1
  strip_r1.setBrightness(BRIGHTNESS);
  strip_r1.begin();
  strip_r1.show(); // Initialize all pixels to 'off'
  //***LedStrip 2
  strip_r2.setBrightness(BRIGHTNESS);
  strip_r2.begin();
  strip_r2.show(); // Initialize all pixels to 'off'
  //***LedStrip 3
  strip_r3.setBrightness(BRIGHTNESS);
  strip_r3.begin();
  strip_r3.show(); // Initialize all pixels to 'off'
  //***Circle
  strip_cp.setBrightness(BRIGHTNESS);
  strip_cp.begin();
  strip_cp.show(); // Initialize all pixels to 'off'
  //***Circle
  strip_cg.setBrightness(BRIGHTNESS);
  strip_cg.begin();
  strip_cg.show(); // Initialize all pixels to 'off'
  //****************************
  // initialize serial:
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello!! \n Hello \n case x \n");
}

void loop() {
  if (mySerial.available()) {
    Ledmode = mySerial.read();
    mySerial.println(Ledmode);
  }

  switch (Ledmode){
    case '0':
      flag = 0;
      //Efecto Inicialización Color Aleatorio
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      InitialSequence(60); //random mode
      break;
    case '1':
      flag = 0;
      //Efecto Inicialización Secuencial ----OKK
      r = 255; g = 0; b = 0;
      InitialSequence(60); //InitialSequence(wait)
      r = 0; g = 255; b = 0;
      InitialSequence(60); //InitialSequence(wait)
      r = 0; g = 0; b = 255;
      InitialSequence(60); //InitialSequence(wait)
      break;
    case '2':
      flag = 0;
      //Efecto Energy 1 color random -----OKK
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      w = random(10,255);
      energyfocused_1(1,40,2);
      break;
    case '3':
      flag = 0;
      //Efecto Energy 2 color random---OKK
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      w = random(10,255);
      energyfocused_2(1,1,3);
      break;
    case '4':
      flag = 0;
      //Efecto Energy 2 color selected---OKK
      r = 0; g=0; b= 255; w=255; //blue
      energyfocused_2(1,1,3);
      r = 148; g = 0; b = 211; w=0; //iros
      energyfocused_2(1,1,3);
      r = 0; g=255; b= 0; w=0; //green
      energyfocused_2(1,1,3);
      delay(10);
      break;
    case '5':
      flag = 0;
      //Efecto Energy 2 color selected---OKK
      r = 145; g=15; b= 255; w=102; //iros 1
      energyfocused_2(1,1,3);
      r = 125; g = 31; b = 122; w=0; //iros 2
      energyfocused_2(1,1,3);
      r = 191; g=23; b= 115; w=0; //iros 3
      energyfocused_2(1,1,3);
      delay(10);
      break;
    case '6':
      flag = 0;
      //Efecto Cuentagotas in random color --- OKK
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      for(int k=1; k <4; k++){
      cuentagotas(10,60/k,3);
      showoff_leds();
      }
      break;
    case '7':
      flag = 0;
      //Efecto All LedStrips turn on in Color
      r = 0; g = 0; b = 255;
      pulseColor(20); //(color(r,g,b), wait)  
      showoff_leds();
      break;
    case '8':
      flag = 0;
      //Efecto All LedStrips turn on random Color
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      pulseColor(20); //(wait)  
      showoff_leds();
      break;
    case '9':
      flag = 0;
      //Efecto Movimiento en circulo pequeño
      show_background_leds();
      colorOverWhite(10,10,3);
      break;
    case 'a':
      flag = 0;
      // Efecto Full White --- OKK
      fullWhite();
      delay(2000);
      break;
    case 'c':
      flag = 0;
      // Efecto RainbowFade2White ---
      show_background_leds();
      rainbowFade2White_cp(3,3,1);
      break;
    case 'b':
      flag = 0;
      //Efecto Boomerang each ledStrip
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      showoff_leds();
      boomerang_r1(10);
      boomerang_r2(10);
      boomerang_r3(10);
      boomerang_cg(10);
      boomerang_cp(10);
      break;
    case 'e': // Tira de leds en light white
      flag = 0;      
      show_background_leds();
      break;
    case 'f': // Apagar la tira de leds
      flag = 0;      
      showoff_leds();
      break;
    case 'g':
      flag = 0;
      rainbowFade_all(3, 3);
      break;
    case 'd': //Efecto Demo
      flag = 0;      
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      InitialSequence(60); //random mode
      energyfocused_1(1,40,2);
      for(int k=1; k <4; k++){
        cuentagotas(10,60/k,3);
        showoff_leds();
      }
      energyfocused_2(1,1,3);
      pulseColor(20); // pulso de color
      for(int k=1; k <3; k++){
        colorOverWhite(10,40,3);
      }
      rainbowFade2White_cp(3,3,1);
      fullWhite();
      delay(2000);
      rainbowFade_all(3, 3);
      break;
    default:
      if (flag < 1){
        r = 252; g = 10; b = 28; w=0; //iros
        for(int p = 0; p <3; p++){
          fullColor();
          delay(200);
          showoff_leds();
          delay(200);
        }
        mySerial.println("Use numbers between 0 to 9, or characters between a to f");
        flag = 3;
      }
      r = 148; g = 0; b = 211; w=0; //iros
      fullColor();
      delay(2000);
  }
      //Efecto Boomerang All ledStrips
      //to develop...
}   
//************End loop***************

//***Fill the dots one after the other*********************
void InitialSequence(uint8_t wait){

  //Show off the LedStrips
  showoff_leds();
  
    //Rayo 1
    for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
    strip_r1.setPixelColor(i, strip_r1.Color(r, g, b, 10));
    strip_r1.show();
    delay(wait);
    }
    // Rayo 2
    for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r1.Color(r, g, b, 10));
    strip_r2.show();
    delay(wait);
    }
    //Rayo 3
    for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r1.Color(r, g, b, 10));
    strip_r3.show();
    delay(wait);
    }
    //Circulo grande
    for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_r1.Color(r, g, b));
    strip_cg.show();
    delay(wait);
    }
    //Circulo pequeño
    for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_r1.Color(r, g, b, 10));
    strip_cp.show();
    delay(wait);
    }
    pulseColor(5);
}//****************************************************

//****Energy focused 1*********************************
void energyfocused_1(uint8_t wait, uint8_t colorSpeed, uint8_t colorLength){
  //Call to Show off the LedStrips
  showoff_leds();
  
  //******Begin effect
  for(int k=1;k<6;k++){
    //Upper Led Strip
    if(colorLength >= strip_r1.numPixels()) colorLength = strip_r1.numPixels() - 1;
    int head = colorLength - 1;
    int tail = 0;
    int loops = 1;
    int loopNum = 0;
    static unsigned long lastTime = 0;
    int LedOffset = strip_r1.numPixels()-strip_r3.numPixels();
    while(true){
        for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
          if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
            strip_r1.setPixelColor(i, strip_r1.Color(r, g, b, w) ); //in chosen color
            strip_r2.setPixelColor(i, strip_r1.Color(r, g, b, w) ); //in chosen color
            //********To synchonize the end of the effect according to strips' lenght
            if(i >= LedOffset){
               strip_r3.setPixelColor(i - LedOffset, strip_r1.Color(r, g, b, 10) ); //in chosen color
            }
          }
          else{
            strip_r1.setPixelColor(i, strip_r1.Color(0,0,0, 10)); // in Light White
            strip_r2.setPixelColor(i, strip_r2.Color(0,0,0, 10 )); // in Light White
            strip_r3.setPixelColor(i - LedOffset, strip_r3.Color(0,0,0, 10 )); // in Light White
          }
        }
  
        if(millis() - lastTime > colorSpeed/k) {
          head++;
          tail++;
          if(head == strip_r1.numPixels()){
            loopNum++;
          }
          lastTime = millis();
        }
  
        if(loopNum == loops) break;
        
        strip_r1.show();
        strip_r2.show();
        strip_r3.show();
        delay(wait);
    }
  
    //++++Pulse of ring large
    for(int j = 60; j < 256 ; j++){
        for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
            strip_cg.setPixelColor(i, strip_cg.Color(r,g,b) );
          }
          strip_cg.show();
        }
  }
      
  //+++++Pulse of ring small+++++++
  for(int j = 60; j < 256 ; j++){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j] ) );
        }
        delay(wait/2);
        strip_cp.show();
      }

  for(int j = 255; j >= 60 ; j--){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j] ) );
        }
        delay(wait/2);
        strip_cp.show();
      }
}//*************************************************

//****Energy focused 2 *******************************
void energyfocused_2(uint8_t wait, uint8_t colorSpeed, uint8_t colorLength){
  //Call to Show off the LedStrips
  showoff_leds();
  
  //******Begin effect
  for(int k=1;k<6;k++){
    //Upper Led Strip
    if(colorLength >= strip_r1.numPixels()) colorLength = strip_r1.numPixels() - 1;
    int head = colorLength - 1;
    int tail = 0;
    int loops = 1;
    int loopNum = 0;
    static unsigned long lastTime = 0;
    int LedOffset = strip_r1.numPixels()-strip_r3.numPixels();
    while(true){
        for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
          if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
            strip_r1.setPixelColor(i, strip_r1.Color(r, g, b, w) ); //in chosen color
            strip_r2.setPixelColor(i, strip_r1.Color(r, g, b, w) ); //in chosen color
            //********To synchonize the end of the effect according to strips' lenght
            if(i >= LedOffset){
               strip_r3.setPixelColor(i - LedOffset, strip_r1.Color(r, g, b, 10) ); //in chosen color
            }
          }
          else{
            strip_r1.setPixelColor(i, strip_r1.Color(0,0,0, 10)); // in Light White
            strip_r2.setPixelColor(i, strip_r2.Color(0,0,0, 10 )); // in Light White
            strip_r3.setPixelColor(i - LedOffset, strip_r3.Color(0,0,0, 10 )); // in Light White
          }
        }
  
        if(millis() - lastTime > colorSpeed) {
          head++;
          tail++;
          if(head == strip_r1.numPixels()){
            loopNum++;
          }
          lastTime = millis();
        }
  
        if(loopNum == loops) break;
        
        strip_r1.show();
        strip_r2.show();
        strip_r3.show();
        delay(wait);
    }
  
    //++++Pulse of ring large
    for(int j = 60; j < 256 ; j++){
        for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
            strip_cg.setPixelColor(i, strip_cg.Color(r,g,b) );
          }
          strip_cg.show();
        }
  }
      
  //+++++Pulse of ring small+++++++
  for(int j = 60; j < 256 ; j++){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j] ) );
        }
        delay(wait/2);
        strip_cp.show();
      }

  for(int j = 255; j >= 60 ; j--){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j] ) );
        }
        delay(wait/2);
        strip_cp.show();
      }
}//*************************************************


//*****Pulse ON/OFF in color with all ledStrips**********
void pulseColor(uint8_t wait){
  //show off ledStrips
  showoff_leds();
  
  //*******************************************
  for(int j = 60; j < 256 ; j++){
      for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
          strip_r1.setPixelColor(i, strip_r1.Color(r,g,b, neopix_gamma[j] ) );
          strip_r2.setPixelColor(i, strip_r2.Color(r,g,b, neopix_gamma[j] ) );
          strip_r3.setPixelColor(i, strip_r3.Color(r,g,b, neopix_gamma[j] ) );
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j] ) );
          strip_cg.setPixelColor(i, strip_cg.Color(r,g,b));
        }
        delay(wait);
        strip_r1.show();
        strip_r2.show();
        strip_r3.show();
        strip_cp.show();
        strip_cg.show();
      }

  for(int j = 255; j >= 60 ; j--){
      for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
          strip_r1.setPixelColor(i, strip_r1.Color(r,g,b, neopix_gamma[j] ) );
          strip_r2.setPixelColor(i, strip_r2.Color(r,g,b, neopix_gamma[j] ) );
          strip_r3.setPixelColor(i, strip_r3.Color(r,g,b, neopix_gamma[j] ) );
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j] ) );
          strip_cg.setPixelColor(i, strip_cg.Color(r,g,b) );
        }
        delay(wait);
        strip_r1.show();
        strip_r2.show();
        strip_r3.show();
        strip_cp.show();
        strip_cg.show();
      }
}//**********************************************************

//**********************************************************
void cuentagotas( uint8_t wait, uint8_t colorSpeed, uint8_t colorLength){
  //show off ledStrips
  show_background_leds();

  //******Begin effect
  //Upper Led Strip
  if(colorLength >= strip_r3.numPixels()) colorLength = strip_r3.numPixels() - 1;
  int head = colorLength - 1;
  int tail = 0;
  int loops = 1;
  int loopNum = 0;
  static unsigned long lastTime = 0;
  while(true){
      for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip_r3.setPixelColor(i, strip_r3.Color(r,g,b,w)); //in chosen color
        }
        else{
          strip_r3.setPixelColor(i, strip_r3.Color(0,0,0, 10 )); // in Light White
        }
        
      }

      if(millis() - lastTime > colorSpeed) {
        head++;
        tail++;
        if(head == strip_r3.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) break;
      
      strip_r3.show();
      delay(wait);
  }
  //****Color pulse in large circle
  for(int j = 60; j < 100 ; j++){
      for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
          strip_cg.setPixelColor(i, strip_cg.Color(r,g,b) );
        }
        delay(wait/2);
        strip_cg.show();
      }

  for(int j = 100; j >= 60 ; j--){
      for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
          strip_cg.setPixelColor(i, strip_cg.Color(r,g,b));
        }
        delay(wait/2);
        strip_cg.show();
      }
  //****Color pulse in small circle
  for(int j = 60; j < 256 ; j++){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j]) );
        }
        delay(wait/2);
        strip_cp.show();
      }

  for(int j = 255; j >= 60 ; j--){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(r,g,b, neopix_gamma[j]) );
        }
        delay(wait/2);
        strip_cp.show();
      }
  //----------------------------------------------
  //*****LedStrip 2 and 3
  if(colorLength >= strip_r1.numPixels()) colorLength = strip_r1.numPixels() - 1;
  head = strip_r1.numPixels();//colorLength - 1;
  tail = strip_r1.numPixels()+ colorLength - 1; //0 whiteLength - 1
  loops = 1;
  loopNum = 0;
  lastTime = 0;
  while(true){
      for(uint16_t i=strip_r1.numPixels(); i>0; i--) {
        //if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
        if((i <= tail && i >= head) || (tail < head && i <= tail) || (tail < head && i >= head) ){
          strip_r1.setPixelColor(i, strip_r3.Color(r,g,b, w) ); //in chosen color
          strip_r2.setPixelColor(i, strip_r3.Color(r,g,b, w) ); //in chosen color
        }
        else{
          strip_r1.setPixelColor(i, strip_r1.Color(0,0,0, 10 )); // in Light White
          strip_r2.setPixelColor(i, strip_r2.Color(0,0,0, 10 )); // in Light White
        }
        
      }

      if(millis() - lastTime > colorSpeed) {
        head--;
        tail--;
        if(head == 0){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) break;
      
      strip_r1.show();
      strip_r2.show();
      delay(wait);
  }
}//***********************************************

//******Fill the dots one after the other with a color and back
void boomerang_all(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
    strip_r1.setPixelColor(i, c);
    strip_r1.show();
    delay(wait);
    }

    for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
    strip_r1.setPixelColor(i, strip_r1.Color(0,0,0));
    strip_r1.show();
    delay(1);
    }
    
    for(uint16_t i=strip_r1.numPixels(); i > 0; i--) {
    strip_r1.setPixelColor(i, c);
    strip_r1.show();
    delay(wait);
    }

    for(uint16_t i=strip_r1.numPixels(); i > 0; i--) {
    strip_r1.setPixelColor(i, strip_r1.Color(0,0,0));
    strip_r1.show();
    delay(1);
    }
}//*****************************************

//******Fill the dots one after the other with a color and back
void boomerang_r1(uint8_t wait) {
    for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
    strip_r1.setPixelColor(i, strip_r1.Color(r,g,b));
    strip_r1.show();
    delay(wait);
    }

    for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
    strip_r1.setPixelColor(i, strip_r1.Color(0,0,0));
    strip_r1.show();
    delay(1);
    }
    
    for(uint16_t i=strip_r1.numPixels(); i > 0; i--) {
    strip_r1.setPixelColor(i, strip_r1.Color(r,g,b));
    strip_r1.show();
    delay(wait);
    }

    for(uint16_t i=strip_r1.numPixels(); i > 0; i--) {
    strip_r1.setPixelColor(i, strip_r1.Color(0,0,0));
    strip_r1.show();
    delay(1);
    }
}//*****************************************
void boomerang_r2(uint8_t wait) {
    for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r2.Color(r,g,b));
    strip_r2.show();
    delay(wait);
    }

    for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r2.Color(0,0,0));
    strip_r2.show();
    delay(1);
    }
    
    for(uint16_t i=strip_r2.numPixels(); i > 0; i--) {
    strip_r2.setPixelColor(i, strip_r2.Color(r,g,b));
    strip_r2.show();
    delay(wait);
    }

    for(uint16_t i=strip_r2.numPixels(); i > 0; i--) {
    strip_r2.setPixelColor(i, strip_r2.Color(0,0,0));
    strip_r2.show();
    delay(1);
    }
}//*****************************************
//******************************************
void boomerang_r3(uint8_t wait) {
    for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r3.Color(r,g,b));
    strip_r3.show();
    delay(wait);
    }

    for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r3.Color(0,0,0));
    strip_r3.show();
    delay(1);
    }
    
    for(uint16_t i=strip_r3.numPixels(); i > 0; i--) {
    strip_r3.setPixelColor(i, strip_r3.Color(r,g,b));
    strip_r3.show();
    delay(wait);
    }

    for(uint16_t i=strip_r3.numPixels(); i > 0; i--) {
    strip_r3.setPixelColor(i, strip_r3.Color(0,0,0));
    strip_r3.show();
    delay(1);
    }
}//*****************************************
//******************************************
void boomerang_cp(uint8_t wait) {
    for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_cp.Color(r,g,b));
    strip_cp.show();
    delay(wait);
    }

    for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_cp.Color(0,0,0));
    strip_cp.show();
    delay(1);
    }
    
    for(uint16_t i=strip_cp.numPixels(); i > 0; i--) {
    strip_cp.setPixelColor(i, strip_cp.Color(r,g,b));
    strip_cp.show();
    delay(wait);
    }

    for(uint16_t i=strip_cp.numPixels(); i > 0; i--) {
    strip_cp.setPixelColor(i, strip_cp.Color(0,0,0));
    strip_cp.show();
    delay(1);
    }
}//*****************************************
//******************************************
void boomerang_cg(uint8_t wait) {
    for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_cg.Color(r,g,b));
    strip_cg.show();
    delay(wait);
    }

    for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_cg.Color(0,0,0));
    strip_cg.show();
    delay(1);
    }
    
    for(uint16_t i=strip_cg.numPixels(); i > 0; i--) {
    strip_cg.setPixelColor(i, strip_cg.Color(r,g,b));
    strip_cg.show();
    delay(wait);
    }

    for(uint16_t i=strip_cg.numPixels(); i > 0; i--) {
    strip_cg.setPixelColor(i, strip_cg.Color(0,0,0));
    strip_cg.show();
    delay(1);
    }
}//*****************************************


//******************************************
void pulseWhite(uint8_t wait) {
  //show off ledStrip 2
  for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, 10 ) );
        }
        strip_cp.show();
  
  for(int j = 60; j < 256 ; j++){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        strip_cp.show();
      }

  for(int j = 255; j >= 60 ; j--){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        strip_cp.show();
      }
}//*******************************************

//********************************************
void pulseIros(uint8_t wait) {
  //show off ledStrip 2
  for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
          strip_r1.setPixelColor(i, strip_cp.Color(0,0,0, 10 ) );
        }
        strip_r1.show();
  
  for(int j = 10; j < 60 ; j++){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(148,0,211, neopix_gamma[j] ) );
        }
        delay(wait);
        strip_cp.show();
      }

  for(int j = 60; j >= 10 ; j--){
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(148,0,211, neopix_gamma[j] ) );
        }
        delay(wait);
        strip_cp.show();
      }
      
  //show off ledStrip Circle
  for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
          strip_cp.setPixelColor(i, strip_cp.Color(0,0,0,10) );
        }
        strip_cp.show();
}//*******************************************************

//**************************************************************************
void colorOverWhite(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {
  
  if(whiteLength >= strip_cp.numPixels()) whiteLength = strip_cp.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;

  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip_cp.setPixelColor(i, strip_cp.Color(0,0,255, 255 ) );
        }
        else{
          strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, 10 ));
        }
        
      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == strip_cp.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip_cp.numPixels();
      tail%=strip_cp.numPixels();
        strip_cp.show();
        delay(wait);
    }
  }
}//********************************************************

//****Full White ******************************************
void fullWhite(){
  for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
     strip_r1.setPixelColor(i, strip_r1.Color(0,0,0, 255) );
  }
  strip_r1.show();
  for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r2.Color(0,0,0, 255) );
  }
  strip_r2.show();
  for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r3.Color(0,0,0, 255) );
  }
  strip_r3.show();
  for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, 255) );
  }
  strip_cp.show();
  for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_cg.Color(255,255,255) );
  }
  strip_cg.show();
}//******************************************************

//****Full White ******************************************
void fullColor(){
  for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
     strip_r1.setPixelColor(i, strip_r1.Color(r,g,b) );
  }
  strip_r1.show();
  for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r2.Color(r,g,b) );
  }
  strip_r2.show();
  for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r3.Color(r,g,b) );
  }
  strip_r3.show();
  for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_cp.Color(r,g,b) );
  }
  strip_cp.show();
  for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_cg.Color(r,g,b) );
  }
  strip_cg.show();
}//******************************************************

//*****RainbowFade2White CP *****************************
void rainbowFade2White_cp(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip_cp.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip_cp.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip_cp.setPixelColor( i, strip_cp.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        strip_cp.show();
        delay(wait);
    }
  
  }
  delay(500);

  for(int k = 0 ; k < whiteLoops ; k ++){

    for(int j = 0; j < 256 ; j++){

        for(uint16_t i=0; i < strip_cp.numPixels(); i++) {
            strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, neopix_gamma[j] ) );
          }
          strip_cp.show();
        }

        delay(2000);
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < strip_cp.numPixels(); i++) {
            strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, neopix_gamma[j] ) );
          }
          strip_cp.show();
        }
  }
  delay(500);
}//****************************************

//*****RainbowFade2White CP *****************************
void rainbowFade_all(uint8_t wait, int rainbowLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  int rainbowLoops_1 = rainbowLoops;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip_cg.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip_cg.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip_cp.setPixelColor( i, strip_cp.Color( redVal, greenVal, blueVal ) );
        strip_cg.setPixelColor( i, strip_cg.Color( redVal, greenVal, blueVal ) );
        strip_r1.setPixelColor( i, strip_r1.Color( redVal, greenVal, blueVal ) );
        strip_r2.setPixelColor( i, strip_r2.Color( redVal, greenVal, blueVal ) );
        strip_r3.setPixelColor( i, strip_r3.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        strip_cp.show();
        strip_cg.show();
        strip_r1.show();
        strip_r2.show();
        strip_r3.show();
        delay(wait);
    }
  }
  delay(500);

//  fadeVal = 0;
//  for(int k = 0 ; k < rainbowLoops_1 ; k ++){
//    
//    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
//
//      for(int i=0; i< strip_r1.numPixels(); i++) {
//
//        wheelVal = Wheel(((i * 256 / strip_r1.numPixels()) + j) & 255);
//
//        redVal = red(wheelVal) * float(fadeVal/fadeMax);
//        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
//        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);
//
//        strip_r1.setPixelColor( i, strip_r1.Color( redVal, greenVal, blueVal ) );
//        strip_r2.setPixelColor( i, strip_r2.Color( redVal, greenVal, blueVal ) );
//        strip_r3.setPixelColor( i, strip_r3.Color( redVal, greenVal, blueVal ) );
//
//      }
//
//      //First loop, fade in!
//      if(k == 0 && fadeVal < fadeMax-1) {
//          fadeVal++;
//      }
//
//      //Last loop, fade out!
//      else if(k == rainbowLoops_1 - 1 && j > 255 - fadeMax ){
//          fadeVal--;
//      }
//
//        strip_r1.show();
//        strip_r2.show();
//        strip_r3.show();
//        delay(wait);
//    }
//  
//  }
//  delay(500);

//  for(int k = 0 ; k < whiteLoops ; k ++){
//
//    for(int j = 0; j < 256 ; j++){
//
//        for(uint16_t i=0; i < strip_cp.numPixels(); i++) {
//            strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, neopix_gamma[j] ) );
//          }
//          strip_cp.show();
//        }
//
//        delay(2000);
//    for(int j = 255; j >= 0 ; j--){
//
//        for(uint16_t i=0; i < strip_cp.numPixels(); i++) {
//            strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, neopix_gamma[j] ) );
//          }
//          strip_cp.show();
//        }
//  }
//  delay(500);
}//****************************************



//*********************************************************
void showoff_leds(void){
  //show off ledStrips
  for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
     strip_r1.setPixelColor(i, strip_r1.Color(0,0,0, 0 ) );
  }
  strip_r1.show();
  for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r2.Color(0,0,0, 0 ) );
  }
  strip_r2.show();
  for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r3.Color(0,0,0,0) );
  }
  strip_r3.show();
  for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_cp.Color(0,0,0,0) );
  }
  strip_cp.show();
  for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_cg.Color(0,0,0) );
  }
  strip_cg.show();//*******end show off
}//******************************************************

//*******************************************************
void show_background_leds(void){
  //Turn leds in light white
  for(uint16_t i=0; i<strip_r1.numPixels(); i++) {
     strip_r1.setPixelColor(i, strip_r1.Color(0,0,0, 10) );
  }
  strip_r1.show();
  for(uint16_t i=0; i<strip_r2.numPixels(); i++) {
    strip_r2.setPixelColor(i, strip_r2.Color(0,0,0, 10) );
  }
  strip_r2.show();
  for(uint16_t i=0; i<strip_r3.numPixels(); i++) {
    strip_r3.setPixelColor(i, strip_r3.Color(0,0,0, 10) );
  }
  strip_r3.show();
  for(uint16_t i=0; i<strip_cp.numPixels(); i++) {
    strip_cp.setPixelColor(i, strip_cp.Color(0,0,0, 10) );
  }
  strip_cp.show();
  for(uint16_t i=0; i<strip_cg.numPixels(); i++) {
    strip_cg.setPixelColor(i, strip_cg.Color(10,10,10) );
  }
  strip_cg.show();//*******end show off
}//***************************************************************

//*****************************************************************
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip_cp.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip_cp.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip_cp.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
