// This example renders a png file that is stored in a FLASH array
// using the PNGdec library (available via library manager).

// Image files can be converted to arrays using the tool here:
// https://notisrac.github.io/FileToCArray/
// To use this tool:
//   1. Drag and drop file on "Browse..." button
//   2. Tick box "Treat as binary"
//   3. Click "Convert"
//   4. Click "Save as file" and move the header file to sketch folder
//   5. Open the sketch in IDE
//   6. Include the header file containing the array (panda.h in this example)

// Include the PNG decoder library
#include <PNGdec.h>
#include "spring.h" // Image is stored here in an 8 bit array
#include "spring2.h" // Image is stored here in an 8 bit array
#include "summer.h" // Image is stored here in an 8 bit array
#include "summer2.h" // Image is stored here in an 8 bit array
#include "autumn.h" // Image is stored here in an 8 bit array
#include "autumn2.h" // Image is stored here in an 8 bit array
#include "winter.h" // Image is stored here in an 8 bit array
#include "winter2.h" // Image is stored here in an 8 bit array
#include "SPI.h"
#include <TFT_eSPI.h>              // Hardware-specific library
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

PNG png; // PNG decoder inatance

#define MAX_IMAGE_WDITH 240 // Adjust for your images

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        17 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 48 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int16_t xpos = 0;
int16_t ypos = 0;
int tcount = 0;
int16_t rc;

int pins[] = {13, 39, 33, 36};

String mode = "summer";

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n Using the PNGdec library");

  // Initialise the TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);

  Serial.println("\r\nInitialisation done.");

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // END of Trinket-specific code.
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  int spring = analogRead(pins[0]);
  int summer = analogRead(pins[1]);
  int autumn = analogRead(pins[2]);
  int winter = analogRead(pins[3]);

  if (summer > 1000){
    mode = "summer";
  }

  if (autumn > 1000){
    mode = "autumn";
  }

    if (spring > 1000){
    mode = "spring";
  }

  if (winter > 1000){
    mode = "winter";
  }

  if (mode == "summer"){
      rc = png.openFLASH((uint8_t *)summer2, sizeof(summer2), pngDraw);
  }

  if (mode == "autumn"){
      rc = png.openFLASH((uint8_t *)autumn2, sizeof(autumn2), pngDraw);
  }

  if (mode == "spring"){
      rc = png.openFLASH((uint8_t *)spring2, sizeof(spring2), pngDraw);
  }

  if (mode == "winter"){
      rc = png.openFLASH((uint8_t *)winter2, sizeof(winter2), pngDraw);
  }

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }

    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      if (mode == "summer"){
        pixels.setPixelColor(i, pixels.Color(255, 255, 0));
      }

      if (mode == "autumn"){
        pixels.setPixelColor(i, pixels.Color(255, 128, 0));
      }

      if (mode == "spring"){
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      }

      if (mode == "winter"){
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      }

      pixels.show();   // Send the updated pixel colors to the hardware.
    }

  // if (tcount == 600){
  //   tcount = 0;
  //   mode = "summer";
  // } else if (tcount == 300) {
  //   mode = "autumn"; 
  // } else {
  //   tcount++;
  // }
}


//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
