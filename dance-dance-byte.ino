//wave shield pins
#define CLK  8
#define DAT  9

//ultrasonic pins
#define trigPin 23
#define echoPin 22

//LCD pins
/*The circuit:
  LCD RS pin to digital pin 12
  LCD Enable pin to digital pin 11
  LCD D4 pin to digital pin 5
  LCD D5 pin to digital pin 4
  LCD D6 pin to digital pin 3
  LCD D7 pin to digital pin 2
  LCD R / W pin to ground
  LCD VSS pin to ground
  LCD VCC pin to 5V
  10K resistor:
  ends to + 5V and ground
  wiper to LCD VO pin (pin 3)
*/
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int game_started = false;
volatile int game_duration = 60; //secs
bool player_detected = false;

void setup() {
  //wave shield settings
  pinMode(CLK, OUTPUT);
  pinMode(DAT, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  send(0xfff0);//set voice volumn to 0
  delay(300);

  //ultrasonic settings
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //LCD settings
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Dancing Floor!");

  Serial.begin(9600);
}

void loop() {

  if (!player_detected) {
    long duration, distance;
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;
    if (distance > 30 && distance < 40) {
      player_detected = true;
    }
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  } else if (!game_started) {
    send(0000); //file name
    delay(300);
    send(0xfff6);//set voice volumn to 4
    delay(300);
    game_started = true;
    lcd.clear();
    lcd.print("!!Have Fun!!");
  }

}

void send(int data)
{
  digitalWrite(CLK, LOW);
  delay(2);
  for (int i = 15; i >= 0; i--)
  {
    delayMicroseconds(50);
    if ((data >> i) & 0x0001 > 0)
    {
      digitalWrite(DAT, HIGH);
      //Serial.print(1);
    }
    else
    {
      digitalWrite(DAT, LOW);
      // Serial.print(0);
    }
    delayMicroseconds(50);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(50);

    if (i > 0)
      digitalWrite(DAT, LOW);
    else
      digitalWrite(DAT, HIGH);
    delayMicroseconds(50);

    if (i > 0)
      digitalWrite(CLK, LOW);
    else
      digitalWrite(CLK, HIGH);
  }

  delay(20);
}