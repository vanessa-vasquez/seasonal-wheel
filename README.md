# Seasonal Wheel - An Electric Paint Project
This project utilizes the TTGO T-Display, electric paint, and an LED ring to make an interactive wall art display that toggles between different seasons. Toggling between different seasons by interacting with the electrically painted carvings — the interactive surfaces — changes the image on the ESP32 display and the surrounding LED lights to those corresponding to that season. I use the PNGdec and Adafruit Neopixel libraries in order to program images onto the ESP32 and alter the lighting of the LED ring.
<p align="center"><img src="./seasonal-wheel.jpeg" width="400" height="550" /></p>

## Set Up

### Uploading code onto your ESP32
For this project, you will need an ESP32 TTGO T-Display. I will be using the Arduino IDE in order to upload this project onto the ESP32. 

1. Clone this project and begin to set your Arduino IDE environment.
2. To set up the Arduino IDE environment, follow the instructions in the demo [here](https://www.youtube.com/watch?v=adLUgmCJKnM).
- Some issues that came up for me in setting up this environment included lack of dependencies (e.g. python) and the upload speed. For example, I found that I had to lower my upload speed to 115200. Keep these in mind and make changes accordingly if you come across any issues when following the demo -- the output should let you know.
3. Open up code.ino in the Arduino IDE.
3. This project uses 4 data pins for the electric paint connections and 1 data pin for the LED ring. Make sure that your electric connections match up with pin numbers you see in the code. The pins array refers to the electric paint pins. Remove or add more pins from this array as you see fit. Pin 17 is allocated to the LED ring.
3. Connect to your ESP32. Make sure that you select your board on the top left. 
4. Upload your code and voilà! The code should now be running on your ESP32.

## Technical Decisions
The magic of the seasonal wheel comes in two parts: 

1. The seasonal imagery that will be displayed on the ESP32
2. The LED lights that will change according to the season selected

I implemented the seasonal imagery using the PNGDec library and implemented the LED lights using the AdaFruit Neopixel library. Specifically, I used the “Flash_PNG” example from the TFT_eSPI library (which implements the PNGDec library) and combined this with the “simple” example from the Adafruit Neopixel library. 

You’ll find that the original Adafruit Neopixel “simple” code lights each LED one by one due to the for-loop and its delays. For this project, since I just wanted static lights, I removed the delays such that my code looked as follows:
```
for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.
}
```
### Digital Read v. Analog Read - Important to Note
When using electric paint, an aspect that is important to note is whether or not to use digitalRead() or analogRead() in order to read the electric signals from the electrically painted seasonal carvings.

Initially, I thought that digitalRead() was the best approach to reading these electrical signals as I did not have a range of values to analyze. I simply wanted to analyze a binary decision — whether the user pressed the electrically painted seasonal carving or not. What I found, however, was that only two pins on the ESP32 — pins 17 and 27 — were able to toggle from 0 to 1 after touching the electrical carving. At first, I thought this to be the fault of the electric paint itself as I had read reviews saying that it did not work for many people. However, after testing this on another ESP32, I found pins 17 and 27 to be the only seemingly ‘functional’ pins as well. My hypothesis for this is that perhaps the electric signals being sent to the other pins are not ‘high’ enough in voltage to pass the threshold for the digitalRead() to flip from 0 to 1. I found that, by switching to analogRead(), the electrical signals did work. I decided then to set arbitrary thresholds as, when unpressed, the analog signals from the carvings sat around the ~200-600 range, however, when pressed, the analog signals rose above the 1000 range. For this reason, I added the following code:
```
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
```
## Notes
For more details on this project, visit the blog post [here](https://pitch-koala-3bc.notion.site/Seasonal-Wheel-An-Electric-Paint-Project-5f72cab7795a497ca9200ef645dc174c).
