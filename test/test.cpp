/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void show_display(int state, bool mode);
void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();  // Clear the LCD screen
  
show_display(1,1);
}

void loop()
{
  // Your loop code goes here
}

void show_display(int state, bool mode){
  lcd.setCursor(0,0);
  lcd.print("State:");
  lcd.setCursor(7,0);
  lcd.print("1");
  lcd.setCursor(0,1);
  lcd.print("Mod:");
  lcd.setCursor(6,1);
  lcd.print(mode ? "Otomatik" : "Manual");

}