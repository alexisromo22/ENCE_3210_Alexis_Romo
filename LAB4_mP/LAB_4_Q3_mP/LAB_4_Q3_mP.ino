//***using the potentiometer to on pcb to ask change the voltages being produced by "solar panel"

//here we are including our libraries 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//here we are assigning our pins to the variables
#define SOLAR_PANEL_PIN A0  
#define BATTERY_PIN A1      
#define CHARGING_VOLTAGE_THRESHOLD 800  // Threshold for fully charged battery 
#define SOLAR_THRESHOLD 512  // Solar panel voltage threshold (around 2.5V)

//here we are setting up the screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // here we starting serial communication
  Serial.begin(9600);  
  
  //here we are initializing the screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Halt if screen fails to initialize
  }

  //here we are setting up the screen
  //by setting the text size, color, and cursor
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0,0);     
  display.print(F("Solar Charge Controller"));
  display.display();
}

void loop() {
  //here we are reading the voltages from the solar panel and the battery
  int solarPanelVoltage = analogRead(SOLAR_PANEL_PIN);
  int batteryVoltage = analogRead(BATTERY_PIN);

  //here we are printing the raw analog reads to the serial monitor
  Serial.print("Solar Panel Voltage (Raw): ");
  Serial.println(solarPanelVoltage);
  Serial.print("Battery Voltage (Raw): ");
  Serial.println(batteryVoltage);

  //here we are clearning the display
  display.clearDisplay();

  //then displaying the current voltage readings
  display.setCursor(0, 0);
  display.print(F("Solar: "));
  display.print(solarPanelVoltage);
  display.print(F("  Battery: "));
  display.println(batteryVoltage);

  //here we are checking if the solar panel voltage is greater than the threshold
  //depending on it is greater than or less than the threshold voltage will determine which 
  //component is providing power
  if (solarPanelVoltage > SOLAR_THRESHOLD) {
    //here we are setting the cursor for where the text should be written on the screen
    display.setCursor(0, 32); 
    display.print(F("Power: Solar Panel"));
    Serial.println("Solar panel is providing power.");
  } 
  else {
    display.setCursor(0, 32); 
    display.print(F("Power: Battery"));
    Serial.println("Solar panel is not providing power. Using battery.");
  }

  //here we are checking if the battery voltage is lower than the solar panel voltage
  //if that is the case then the battery will be considered to be charging and this will be displayed on the screen
  if (batteryVoltage < solarPanelVoltage) {
    //here we are setting the cursor for where the text should be written on the screen
    display.setCursor(0, 48); 
    display.print(F("Battery Charging"));
    Serial.println("Battery voltage is lower than solar panel. Charging battery.");
  } else {
    display.setCursor(0, 48); 
    display.print(F("Battery Sufficient"));
    Serial.println("Battery voltage is sufficient.");
  }

  //here we are checking ff the battery is sufficiently charged
  //if this is the case then the battery will stop charging and use solar power
  if (batteryVoltage >= CHARGING_VOLTAGE_THRESHOLD) {
    //here we are setting the cursor for where the text should be written on the screen
    display.setCursor(0, 56); 
    display.print(F("Battery Full"));
    Serial.println("Battery is fully charged. Switching to solar power.");
  }

  //here we are updating the display with the new information
  display.display();  

  //here we have a small delay being reading the information again
  delay(1000);  
}
