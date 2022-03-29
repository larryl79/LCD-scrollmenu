#include <Arduino.h>

// control menu by buttons
#define pin_up 5
#define pin_down 18
#define pin_button 26

// control menu by rotary switch
#define ROTARY_A 4      // somtimes reffering as CLK on rotary switch
#define ROTARY_B  15    // somtimes reffering as DT on rotary switch
#define ROTARY_SW  27   // rotary button pin

#define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 
#include "ESPRotary.h";
ESPRotary r;
#include "Button2.h"; //  https://github.com/LennartHennigs/Button2
Button2 b;

#include <LiquidCrystal_I2C.h>
#define LCDADDR 0x27                           // address of LCD on I2C bus
#define LCDLINES 4                             // how many lines of the LCD
#define LCDCOLUMNS 20                          // how many columns of the LCD  
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLUMNS, LCDLINES);
// roundup = ceil();

int menu_cursor = 0;
int cursorpos_prev=1;   //dont change it

const int maxmenuitem = 14;      // elements number of menu_labels
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

bool menu_selected [maxmenuitem] = { false };
/*
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
    };
*/

/*  not in use.
 //count elements of an array
int arrayCount (char *arr)
{
  return sizeof(arr)/sizeof(*arr);
}
*/

/*
 * 
 * rotary
 * 
 */

// on change
void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
  if ( r.directionToString(r.getDirection()) == "LEFT" )
    {
      buttonDown();
    }
  if ( r.directionToString(r.getDirection()) == "RIGHT" )
    {
      buttonUp();
    }
   
}
 
// single click
void click(Button2& btn) {
  buttonSelect();
  Serial.println("Click!");
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}



/*
 * 
 * MENU
 * 
 */
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
        delay(10);
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
        delay(10);
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
    }

  if (cursorpos < LCDLINES)
  {
    lcd.setCursor(0,cursorpos);
    if (!menu_selected[menu_cursor])
      {
      lcd.print(">");
      }
      else
      {
      lcd.print("X");
      }
    }
   else
   {
    lcd.setCursor(0,LCDLINES-1);
    if (!menu_selected[menu_cursor])
      {
      lcd.print(">");
      }
      else
      {
      lcd.print("X");
      }
    }
    cursorpos_prev = cursorpos;
  
}

void buttonUp()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ( (interrupt_time - last_interrupt_time) >= 200) 
    {
    if ( menu_cursor < (maxmenuitem - 1) && !menu_selected[menu_cursor] )
      {
      menu_cursor++;
      //menuCursorPos(menu_cursor);
      }
    }
  last_interrupt_time = interrupt_time;
  }

void buttonDown()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ( (interrupt_time - last_interrupt_time) >= 200) 
    {
    if ( menu_cursor > 0 && !menu_selected[menu_cursor] ) 
      {
      menu_cursor--;
      //menuCursorPos(menu_cursor);
      }
    }
  last_interrupt_time = interrupt_time;
  }

void buttonSelect()
  {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ( (interrupt_time - last_interrupt_time) >= 200) 
    {
    menu_selected[menu_cursor] = !menu_selected[menu_cursor];
    if (menu_selected[menu_cursor])
      {
      Serial.print("menu selected: "); 
      }
      else
      {
      Serial.print("menu unselected: "); 
      }
      Serial.println(menu_cursor);
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
  // set up butons
  pinMode(pin_up, INPUT_PULLUP); //RotaryCLK
  pinMode(pin_down, INPUT_PULLUP); //RotaryDT
  pinMode(pin_button, INPUT_PULLUP); //Button
  
  attachInterrupt(digitalPinToInterrupt(pin_up), buttonUp, FALLING);     //PushButton down pin is an interrupt pin
  attachInterrupt(digitalPinToInterrupt(pin_down), buttonDown, FALLING); //PushButton down pin is an interrupt pin
  attachInterrupt(digitalPinToInterrupt(pin_button), buttonSelect, FALLING);   //PushButton pin is an interrupt pin

  // set up rotary
  pinMode(ROTARY_A, INPUT_PULLUP); //RotaryCLK
  pinMode(ROTARY_B, INPUT_PULLUP); //RotaryDT
  pinMode(ROTARY_SW, INPUT_PULLUP); //Button
  
  r.begin(ROTARY_A, ROTARY_B, CLICKS_PER_STEP);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  b.begin(ROTARY_SW);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);

  //attachInterrupt(digitalPinToInterrupt(ROTARY_A), buttonUp, FALLING);     //PushButton down pin is an interrupt pin
  //attachInterrupt(digitalPinToInterrupt(ROTARY_B), buttonDown, FALLING); //PushButton down pin is an interrupt pin
  //attachInterrupt(digitalPinToInterrupt(ROTARY_SW), buttonSelect, FALLING);   //PushButton pin is an interrupt pin
  
  //maxmenuitem = arrayCount(*menu_labels);
  Serial.println(maxmenuitem);
  lcd.begin();
  lcd.clear();
  menuCursorPos(menu_cursor);
  }

void loop()
{
  yield();
  r.loop();
  b.loop();
  menuCursorPos(menu_cursor);
  
  }
