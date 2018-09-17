#define IRSensor A0 //Sharp IP Sensor
int soundSensorPin = 2;
int ForwardPin = 6;
int BackwardPin = 7;
int LeftPin = 4;
int RightPin = 5;
int charge = 0;
int trigPin = 10;
int echoPin = 13;
unsigned long timer;
int backDistance = 20;
int maxDistance = 50;
enum directionStatus {Forward, Backward, Cut};
enum turnStatus {Left, Right, Straight};
directionStatus dir = Forward;
directionStatus preDir = Cut; 
turnStatus turn = Straight;
turnStatus preTurn = Straight;
int testing = 0;
int backUpCounter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(LeftPin, OUTPUT);
  pinMode(RightPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(soundSensorPin, INPUT);
  timer = millis();
  digitalWrite(trigPin, LOW);
}

void loop() {
  
  if(millis() - timer >= 300){
    motorControl();
    preTurn = turn;
    preDir = dir;
    boolean val = digitalRead(soundSensorPin);
    if(val == HIGH){
      charge += 50;
    }
    if(charge > 0 || testing == 0){
      if(dir != Backward){
        float duration, distance;
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = (duration/2) * .0344;
        Serial.print(distance);Serial.print(" || ");
  
        if(distance <= maxDistance){
          if(distance >=2 && distance <= backDistance){
            float volts = analogRead(IRSensor)*0.0048828125;  // value from sensor * (5/1024)
            int distanceir = 13*pow(volts, -1); // worked out from datasheet graph
  
            Serial.print(distanceir);Serial.print(" || ");
            if(distanceir <= 6){
              turn = Right;
              dir = Forward;
              Serial.println("Forward-Right");
            }
            else{
              turn = Left;
              dir = Backward;
              Serial.println("Backward-Left");
              backUpCounter = 8;
            }
          }
          else{
            turn = Right;
            dir = Forward;
            Serial.println("Forward-Right");
          }
        }
        else{
          turn = Straight;
          dir = Forward;
          Serial.println("Forward");
        }
        if(testing == 1){
          charge -= 1;
          dir = Cut;
          turn = Straight;
        }
      }
      else{
        if(backUpCounter == 0){
          turn = Right;
          dir = Forward;
        }
        float volts = analogRead(IRSensor)*0.0048828125;  // value from sensor * (5/1024)
        int distanceir = 13*pow(volts, -1); // worked out from datasheet graph

        Serial.print(distanceir);Serial.print(" || ");
        if(distanceir <= 6){
          turn = Right;
          dir = Forward;
          backUpCounter = 0;
          Serial.println("Forward-Right");
        }
        if(testing == 1){
          charge -= 1;
          dir = Cut;
          turn = Straight;
        }
        backUpCounter -= 1;
      }
    }
    else{
      Serial.println("No Charge");
    }
    timer += 300;
  }
}

void motorControl(){
  switch(dir){
    case Forward:
      if(preDir != dir){
        digitalWrite(ForwardPin , LOW);
        digitalWrite(BackwardPin , LOW);
        delay(200);
        digitalWrite(ForwardPin , HIGH);
        digitalWrite(BackwardPin , LOW);
      }
      break;
    case Backward:
      if(preDir != dir){
        digitalWrite(ForwardPin , LOW);
        digitalWrite(BackwardPin , LOW);
        delay(200);
        digitalWrite(ForwardPin , LOW);
        digitalWrite(BackwardPin , HIGH);
      }
      //delay(2000);
      break;
    case Cut:
        digitalWrite(ForwardPin , LOW);
        digitalWrite(BackwardPin , LOW);
        break;
  }
  switch(turn){
    case Left:
      if(preTurn != turn){
        digitalWrite(LeftPin , LOW);
        digitalWrite(RightPin , LOW);
        delay(200);
        digitalWrite(LeftPin , HIGH);
        digitalWrite(RightPin , LOW);
      }
      break;
    case Right:
      if(preTurn != turn){
        digitalWrite(LeftPin , LOW);
        digitalWrite(RightPin , LOW);
        delay(200);
        digitalWrite(LeftPin , LOW);
        digitalWrite(RightPin , HIGH);
      }
      break;
    case Straight:
      digitalWrite(LeftPin , LOW);
      digitalWrite(RightPin , LOW);
      break;
  }
  return;
}


