#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define LCDADDR 0x27                           // address of LCD on I2C bus
#define LCDLINES 4
#define LCDROWS 20

#define pin_up 5
#define pin_down 18
LiquidCrystal_I2C lcd(LCDADDR, LCDROWS, LCDLINES);
// roundup = ceil();

int cursorpos_prev=1;   //dont change it

const int maxmenuitem = 14;      // have to be same as elements number of menu_labels
const char *menu_labels [maxmenuitem] =   // create menu labels
  {
   "Info page 1",
   "Menu 2",
   "Menu 3",
   "Menu 4",
   "Menu 5",
   "Menu 6",
   "Menu 7",
   "Menu 8",
   "Menu 9",
   "Menu 10",
   "Menu 11",
   "Menu 12",
   "Menu 13",
   "Lastmenu"
    };

int menu_cursor = 0;

int arrayCount (char *arr)
{
  return sizeof(arr)/sizeof(arr[0]);
}

void showMenu(int cursorpos)
{
  Serial.println(cursorpos);
    
  if (cursorpos < LCDLINES)
    {
    if ( maxmenuitem < LCDLINES)
      {
      for (int ROW=0; ROW < maxmenuitem; ROW++) 
        {
        lcd.setCursor(1,ROW);
        lcd.print("               ");
        lcd.setCursor(1,ROW);
        lcd.print(menu_labels[ROW]);
        Serial.print("1st ");
        Serial.print("Menu label: "); Serial.println(menu_labels[ROW]);
        }          
      }
    else
      {
      for (int ROW=0; ROW < LCDLINES; ROW++) 
        {
        lcd.setCursor(1,ROW);
        lcd.print("               ");
        delay(1);
        lcd.setCursor(1,ROW);
        lcd.print(menu_labels[ROW]);
        Serial.print("2nd ");
        Serial.print("Menu label: "); Serial.println(menu_labels[ROW]);
        }
      }
    }
  else
    {
    int labelPos = cursorpos-(LCDLINES-1);
    for (int ROW=0; ROW < 4 ; ROW++) 
      {
      lcd.setCursor(1,ROW);
      lcd.print("               ");
      lcd.setCursor(1,ROW);
      int labelno= labelPos + ROW;
      lcd.print(menu_labels[labelno]);
      Serial.print("3rd ");
      Serial.print("Menu label: "); Serial.println(menu_labels[labelno]);
      } 
    }
}

void menuCursorPos(int cursorpos)
{
  if (cursorpos != cursorpos_prev)
    {
    showMenu(cursorpos);
    lcd.setCursor(0,cursorpos_prev);
    lcd.print(" ");

  if (cursorpos < LCDLINES)
  {
    lcd.setCursor(0,cursorpos);
    lcd.print(">");
    }
   else
   {
    lcd.setCursor(0,LCDLINES-1);
    lcd.print(">");
    }
    cursorpos_prev = cursorpos;
  }
}

void buttonUp()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
    {
    if (menu_cursor<maxmenuitem)
    menu_cursor++;
    }
  last_interrupt_time = interrupt_time;
  }

void buttonDown()
{
    static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
    {
    if (menu_cursor>0)
    menu_cursor--;
    }
  last_interrupt_time = interrupt_time;
  }


void setup()
{
   Serial.begin(115200); //Serial Monitor Console Baud Setting
   while (!Serial)
    {
    ; // wait for serial port to connect. Needed for native USB port only
    }
  Serial.println("");

  attachInterrupt(digitalPinToInterrupt(pin_up), buttonUp, FALLING); //CLK pin is an interrupt pin
  attachInterrupt(digitalPinToInterrupt(pin_down), buttonDown, FALLING); //PushButton pin is an interrupt pin
  
  //maxmenuitem = arrayCount(*menu_labels);
  Serial.println(maxmenuitem);
  lcd.begin();
  lcd.clear();
  menuCursorPos(menu_cursor);
  }

void loop()
{
  menuCursorPos(menu_cursor);
  }
