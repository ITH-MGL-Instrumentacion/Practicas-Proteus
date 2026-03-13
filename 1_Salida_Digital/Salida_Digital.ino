int ledPin1 = 3;   
int ledPin2 = 5;   

int onTime1 = 200;  
int offTime1 = 800;  

int onTime2 = 300;   
int offTime2 = 700;  

void setup() {
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
}

void loop() {
    // LED1 encendido, LED2 apagado
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    delay(onTime1);

    // LED1 apagado, LED2 encendido
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(onTime2);

    // LED2 apagado (para completar su ciclo)
    digitalWrite(ledPin2, LOW);
    delay(offTime2);
}
