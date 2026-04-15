 #include <Arduino.h>
int ledPin1 = 3;
int onTime = 200;
int offTime = 800;
int ledPin2 = 5;
void setup() { 		
      pinMode(ledPin1, OUTPUT);
      pinMode(ledPin2, OUTPUT);
      
}      

void loop() {
digitalWrite(ledPin1, HIGH);
digitalWrite(ledPin2, LOW);
delay (300);
digitalWrite(ledPin1, LOW);
digitalWrite(ledPin2, HIGH);
delay (700);
} 