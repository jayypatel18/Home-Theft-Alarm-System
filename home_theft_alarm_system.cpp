#include <Adafruit_LiquidCrystal.h>

#include <Servo.h>
#include <Keypad.h>
Adafruit_LiquidCrystal lcd(0); 

Servo servo;
int trigger_pin = 10;
int echo_pin = 11;
int PIR = 8;
int buzzer = 9;
int buzzer_pin = A0;
int PIRstatus;
int time;
int distance; 
const byte ROWS = 4;		
const byte COLS = 4;		
char keys[ROWS][COLS] =
{ 
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'}, 
  {'*','0','#', 'D'} 
};

byte rowPins[ROWS] = { 7, 6, 5, 4 };	
byte colPins[COLS] = { 3, 2, 1, 0 };	

Keypad kp = Keypad (makeKeymap (keys), rowPins, colPins, ROWS, COLS);


char correctPass[] = "1802";
int number = sizeof (correctPass) - 1;
char pass[sizeof (correctPass) - 1];
int i = 0, counter = 0;

bool door = 1;

void setup ()
{
  lcd.begin(16,2);
  lcd.print("Enter the Key");
  lcd.setCursor (6, 1);
  lcd.print("SAFE");
  pinMode(12,OUTPUT);
  servo.attach (12);
  servo.write (0);
  lcd.setCursor (0, 1);
  pinMode (trigger_pin, OUTPUT); 
  pinMode (echo_pin, INPUT);
  pinMode(PIR, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
}

void loop ()
{
  
  
  digitalWrite (trigger_pin, HIGH);
    delayMicroseconds (10);
    digitalWrite (trigger_pin, LOW);
    time = pulseIn (echo_pin, HIGH);
    distance = (time * 0.034) / 2;
  
  if (distance <= 20) 
        {       
        digitalWrite (buzzer_pin, HIGH);
        tone(buzzer_pin, 300, 3000);
        lcd.setCursor(0,0);
        lcd.print("Intruder in Home");
        lcd.setCursor (0, 1);
        lcd.print ("     ALERT!!!   ");
        delay(1500);
        lcd.clear();
        lcd.setCursor (0, 0);
        lcd.print("Enter the Key.");
        lcd.setCursor (6, 1);
        lcd.print("SAFE");
        delay (0);
        lcd.setCursor (0, 1);
        }
  else {      
        digitalWrite (buzzer_pin, LOW);   
       } 
  
  PIRstatus = digitalRead(PIR);
  if(PIRstatus == HIGH){
    lcd.clear();
    digitalWrite(buzzer, HIGH); 
    tone(buzzer, 300, 5000);
    lcd.setCursor(0,0);
    lcd.print("Intruder in Home");
    lcd.setCursor (0, 1);
    lcd.print ("     ALERT!!!   ");
    delay(7000);
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("Enter the Key.");
    lcd.setCursor (6, 1);
    lcd.print("SAFE");
    lcd.setCursor (0, 1);
  }
  else{
    digitalWrite (buzzer, LOW);
  }
  if (door == 0)
    {
      char key = kp.getKey ();
      if (key == 'C')
	{
	  servo.write (0);
	  i = counter = 0;
	  lcd.clear ();
      lcd.print("Locked.");
      delay(2000);
      lcd.clear();
	  lcd.print ("Enter the Key.");
      lcd.setCursor (6, 1);
      lcd.print("SAFE");
	  lcd.setCursor (0, 1);
	  door = 1;
	}
    }
  else
    {
    char key = kp.getKey ();
      if (key >= '0' && key <= '9')
	{
	  lcd.print ('*');
	  pass[i] = key;
	  i++;
	}
      else if (key == 'D')
	{
	  if (i == number)
	    {
	      for (int j = 0; j < number; j++)
		{
		  if (pass[j] == correctPass[j])
		    counter++;
		}
	      if (counter == number)
		{
		  correct ();
		}
	      else
		{
		  wrong ();
		}
	    }
	  else
	    {
	      wrong ();
	    }
	}
    }
}

void wrong ()
{
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("Wrong Pass      ");
  lcd.setCursor (0, 1);
  lcd.print ("     Try Again.     ");
  delay (2000);
  i = 0;
  counter = 0;
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("Enter the Pass.");
  lcd.setCursor (6, 1);
  lcd.print("SAFE");
  lcd.setCursor (0, 1);
}

void correct ()
{
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("Unlocked.     ");
  delay (2000);
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("The door is open");
  lcd.setCursor (0, 1);
  lcd.print ("'C' to close.");
  servo.write (90);
  delay (1000);
  i = 0;
  counter = 0;
  door = 0;
}