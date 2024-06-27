
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void show_display(int state, bool mode, int counter){ 
  lcd.setCursor(0,0);
  lcd.print("Sayac:");
  lcd.setCursor(7,0);
  lcd.print(counter);
  lcd.setCursor(15,0);
  lcd.print(state);
  lcd.setCursor(0,1);
  lcd.print("Mod  :");
  lcd.setCursor(7,1);
  lcd.print(mode ? "Otomatik" : "Manual  ");

}