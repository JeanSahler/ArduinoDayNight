#include <Time.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <DS1307RTC.h>

// Morning and evening array as {  hours, minutes }
const int morning[] = {  7, 30 };
const int evening[] = { 20, 15 };

const boolean debug = true;

// Led pin for morning and evening
const int morningLedPin = 13;
const int eveningLedPin = 12;

void setup()
{
  if(debug == true)
    Serial.begin(9600);
  
  pinMode(morningLedPin, OUTPUT);
  pinMode(eveningLedPin, OUTPUT);
  
  // wait until Arduino Serial Monitor opens
  while (!Serial);
  
  // the function to get the time from the RTC
  setSyncProvider(RTC.get);
  
  if(timeStatus()!= timeSet)
  {
     if(debug == true)
       Serial.println("Unable to sync with the RTC");
  }
  else
  {
    int morningSeconds = 3600 * morning[0] + 60 * morning[1];
    int eveningSeconds = 3600 * evening[0] + 60 * evening[1];
    int currentSeconds = 3600 * hour()     + 60 * minute() + second() ;
    
    // Set the light vs current time
    if(currentSeconds > morningSeconds && currentSeconds < eveningSeconds)
    {
      LightEvening();
    }
    else
    {
      LightMorning();
    }
    // Start the alarms
    Alarm.alarmRepeat(morning[0], morning[1], 0, LightMorning);
    Alarm.alarmRepeat(evening[0], evening[1], 0, LightEvening);
  }
}

void loop()
{
  if (timeStatus() == timeSet)
  {
    if(debug == true)
      digitalClockDisplay();
  }
  else
  {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    Alarm.delay(5000);
  }
  Alarm.delay(1000); // wait one second between clock display
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void LightMorning()
{
  digitalWrite(morningLedPin, HIGH);
  digitalWrite(eveningLedPin, LOW);
  if(debug == true)
    Serial.println("We are now on morning");
}

void LightEvening()
{
  digitalWrite(morningLedPin, LOW);
  digitalWrite(eveningLedPin, HIGH);
  if(debug == true)
    Serial.println("We are now on evening");
}
