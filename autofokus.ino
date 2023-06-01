#include <Stepper.h> // Inkluderar biblioteket Stepper

#define STEPS 32 // Definierar konstanten STEPS med värdet 32

Stepper stepper(STEPS, 8, 10, 9, 11); // Skapar en instans av Stepper-klassen med parametrarna STEPS, och pinnarna 8, 10, 9 och 11 för att styra stegmotorn

// Variabler definieras
int val = 0;
int buttonState = 0;
int trigPin = 12;    // Trigger
int echoPin = 13;    // Echo
long duration, cm, inches;
int prevState = 0;
int cmDiff = 0;
int cmPrev = 0;
int conversion = 0;

void setup() { // Den seriella monitorn startas, motorns hastighet sätts, sensorn och knappen blir tilldelade pins
  Serial.begin(9600); 
  stepper.setSpeed(400);
  pinMode(7, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  buttonState = digitalRead(7); // Tittar på knappens tillstånd
  prevState = buttonState; // Variabeln prevState ges värdet buttonState

  cmPrev = cm; // cmPrev ges värdet cm

  while (buttonState == HIGH){ // Medan knappen är nedtryckt mäter sensorn tiden det tar för ljudet att komma tillbaka till sensorn
    digitalWrite(trigPin, LOW); // Sensorn skickar ut ljud i pulser
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    pinMode(echoPin, INPUT); // Sensorn tar emot ljudet
    duration = pulseIn(echoPin, HIGH); // Tiden det tog ges till variabeln duration
    
    conversion = (duration/2) / 29.1; // duration omvandlas från en tid till en sträcka

    if (conversion >= 20){ // Gör att cm = 0 när kameran är vid sitt minimum fokus avstånd
      cm = conversion - 20;
    }

    Serial.print(cm); //Skriver ut avståndet i monitorn
    Serial.println("cm");

    delay(250);

    buttonState = digitalRead(7); //Knappens tillstånd avläses igen

    cmDiff = cm - cmPrev; // Räknar ut skillnaden mellan nuvarande och förra avståndet

    Serial.print(cmDiff); // Skriver ut skillnaden i avstånd i monitorn
    Serial.println("diff");

  }

  
  if (prevState != buttonState && buttonState == LOW){ // Körs ifall knappen inte är nedtryckt och knappens förra tillstånd var nedtryckt
    stepper.step(cmDiff * 190 * -1); // Motorn får antalet steg den ska gå beroende på skillnaden i avstånd
  }
}
