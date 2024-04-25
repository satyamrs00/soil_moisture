#include "Arduino.h"
#include "LiquidCrystal.h"
#include "SoilMoisture.h"


// Pin Definitions
#define LCD_PIN_RS	12
#define LCD_PIN_E	14
#define LCD_PIN_DB4	5
#define LCD_PIN_DB5	4
#define LCD_PIN_DB6	0
#define LCD_PIN_DB7	2
#define SOILMOISTURE_3V3_PIN_SIG	A0


LiquidCrystal lcd(LCD_PIN_RS,LCD_PIN_E,LCD_PIN_DB4,LCD_PIN_DB5,LCD_PIN_DB6,LCD_PIN_DB7);
SoilMoisture soilMoisture_3v3(SOILMOISTURE_3V3_PIN_SIG);


// define vars for testing menu
const int timeout = 100000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

void setup() 
{
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    lcd.begin(16, 2);
    menuOption = menu();    
}

void loop() 
{    
    if(menuOption == '1') {
        lcd.setCursor(0, 0);
        lcd.print("Display Works !");
        
        lcd.noDisplay();
        delay(500);
        
        lcd.display();
        delay(500);
    }
    else if(menuOption == '2') {
        
        int soilMoisture_3v3Val = soilMoisture_3v3.read();
        double soilMoisture_3v3Val_percentage = 100 - (soilMoisture_3v3Val * 100)/1024;
        Serial.print(F("Moisture percentage: ")); Serial.print(soilMoisture_3v3Val_percentage); Serial.println("%");

        lcd.setCursor(0, 0);
        lcd.print("Percentage: "); lcd.print(soilMoisture_3v3Val_percentage); lcd.print("%");

        delay(100);
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
}


// Menu function for selecting the components to be tested
char menu()
{
    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) LCD 16x2"));
    Serial.println(F("(2) Soil Moisture Sensor"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing LCD 16x2"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing Soil Moisture Sensor"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
    return '0';
}