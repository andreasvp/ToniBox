#include <Arduino.h>

// https://randomnerdtutorials.com/esp8266-pwm-arduino-ide/
// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

const int bluePin = 4;
const int orangePin = 0;

const int touchPin = A0;

int modus = 1;
boolean modusInit = false;
float modusCounterBlue = 0.0f;
float modusCounterOrange = 0.0f;

void dimming(float adder, boolean blue, boolean orange)
{
  if (!modusInit)
  {
    modusInit = true;
    modusCounterBlue = 0.0f;
    modusCounterOrange = 0.0f;
    analogWrite(bluePin, modusCounterBlue);
    analogWrite(orangePin, modusCounterOrange);
  }

  if (blue)
  {
    if (modusCounterBlue <= 255.0f)
    {
      analogWrite(bluePin, modusCounterBlue);
    }
    else
    {
      analogWrite(bluePin, 255 - (modusCounterBlue - 255.0f));
    }
    modusCounterBlue = modusCounterBlue + adder;
    if (modusCounterBlue >= 510.0f)
    {
      modusCounterBlue = 0.0f;
    }
  }
  if (orange)
  {
    if (modusCounterBlue <= 255.0f)
    {
      analogWrite(orangePin, modusCounterOrange);
    }
    else
    {
      analogWrite(orangePin, 255 - (modusCounterOrange - 255.0f));
    }
    modusCounterOrange = modusCounterOrange + adder;
    if (modusCounterOrange >= 510.0f)
    {
      modusCounterOrange = 0.0f;
    }
  }
}

void dimming(float adder)
{
  dimming(adder, true, true);
}

void flackern()
{
  analogWrite(orangePin, random(0, 255));
  analogWrite(bluePin, random(0, 255));
}

void staticOn(float value, boolean blue, boolean orange)
{
  if (!modusInit)
  {
    modusInit = true;
    modusCounterBlue = 0.0f;
    modusCounterOrange = 0.0f;
    if (!blue)
      analogWrite(bluePin, 0);
    if (!orange)
      analogWrite(orangePin, 0);
  }

  if (orange)
    analogWrite(orangePin, (int)value);
  if (blue)
    analogWrite(bluePin, (int)value);
}

void setup()
{
  // Serial
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("-----------------------------------------------------------------------------");
  Serial.println("Init Done");
  Serial.println("-----------------------------------------------------------------------------");
}

void loop()
{

  switch (modus)
  {
  case 1:
    dimming(0.5f);
    break;
  case 2:
    dimming(1.0f);
    break;
  case 3:
    dimming(2.0f);
    break;
  case 4:
    flackern();
    break;
  case 5:
    dimming(0.5f, true, false);
    break;
  case 6:
    dimming(0.5f, false, true);
    break;
  case 7:
    dimming(20.0f, true, false);
    break;
  case 8:
    dimming(254.0f, true, false);
    break;
  case 9:
    dimming(20.0f, false, true);
    break;
  case 10:
    dimming(254.0f, false, true);
    break;
  case 11:
    dimming(10.0f, true, true);
    break;
  case 12:
    dimming(254.0f, true, true);
    break;
  default:
    modus = 1;
  }

  int touchVal = analogRead(touchPin);

  if (touchVal > 20)
  {
    modusInit = false;
    modusCounterOrange = 0.0f;
    modusCounterBlue = 0.0f;
    modus++;
    Serial.print("Modus now:");
    Serial.println(modus);
    if (modus > 12)
      modus = 1;

    delay(300);
  }
}
