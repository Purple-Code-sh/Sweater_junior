#define piso A3;

#define front_R A7;
#define front_L A0;

int startPin = 2;

int motorR_1 = 3;
int motorR_2 = 6;

int motorL_1 = 9;
int motorL_2 = 5;

int v_Front;

int proportionalUsed;

void setup()
{
  Serial.begin(9600);

  pinMode(startPin, INPUT);
  pinMode(front_L, INPUT);
  pinMode(front_R, INPUT);

  pinMode(motorL_1, OUTPUT);
  pinMode(motorL_2, OUTPUT);
  pinMode(motorR_1, OUTPUT);
  pinMode(motorR_2, OUTPUT);

  v_Front = 0;

  proportionalUsed = 0;
}

void loop()
{
start:
  if (digitalRead(startPin) == HIGH) {
    Serial.println("Encendido");

    // ---------- Front ----------

    v_Front = frontSensors();
    if (v_Front == 3)
    {
      if (proportionalUsed == 0) {
        Serial.println("Enfrente - ambos");
        goForward_proportional(80);
        proportionalUsed = 1;
        goto start;
      } else if (proportionalUsed == 1) {
        goForward(255, 255, 30);
        goto start;
      }
    }
    else if (v_Front == 1)
    {
      Serial.println("Enfrente - izquierda");
      goFull_left();
      proportionalUsed = 0;
      goto start;
    }
    else if (v_Front == 2)
    {
      Serial.println("Enfrente - derecha");
      goFull_right();
      proportionalUsed = 0;
      goto start;
    }
    else
    {
      Serial.println("Ninguno");
      goForward_proportional(20);
      delay(150);
      proportionalUsed = 0;
      goto start;
    }
  } else {
    Serial.println("Apagado");
    stopi(false);
    proportionalUsed = 0;
  }

} // <<<---loop end


// ------------------------- Sensors Functions ------------------------- .

int frontSensors()
{
  int valFrontL = analogRead(front_L);
  int front_L;
  if (valFrontL >= 25) {
    front_L = 1;
  } else {
    front_L = 0;
  }

  int valFrontR = analogRead(front_R);
  int front_R;
  if (valFrontR >= 25) {
    front_R = 1;
  } else {
    front_R = 0;
  }

  int addition_Front = front_L + front_R;

  return addition_Front;
}

// ------------------------- Stop Functions ------------------------- .

void stopi(bool reverse)
{
  if (reverse == false)
  {
    digitalWrite(motorL_2, HIGH);
    digitalWrite(motorL_1, HIGH);

    digitalWrite(motorR_2, HIGH);
    digitalWrite(motorR_1, HIGH);
  } else if (reverse == true)
  {
    digitalWrite(motorL_2, LOW);
    digitalWrite(motorL_1, LOW);

    digitalWrite(motorR_2, LOW);
    digitalWrite(motorR_1, LOW);
  }
}

// ------------------------- Motors Functions ------------------------- .

void goForward(int powerL_f, int powerR_f, int workTime_f)
{
  analogWrite(motorL_1, powerL_f);
  digitalWrite(motorL_2, LOW);

  analogWrite(motorR_1, powerR_f);
  digitalWrite(motorR_2, LOW);

  delay(workTime_f);
  stopi(false);
}

void goFull_left() {
  digitalWrite(motorL_1, LOW);
  digitalWrite(motorL_2, HIGH);

  digitalWrite(motorR_1, HIGH);
  digitalWrite(motorR_2, LOW);
}

void goFull_right() {
  digitalWrite(motorL_1, HIGH);
  digitalWrite(motorL_2, LOW);

  digitalWrite(motorR_1, LOW);
  digitalWrite(motorR_2, HIGH);
}

void goBack(int powerL_b, int powerR_b, int workTime_b)
{
  digitalWrite(motorL_1, LOW);
  analogWrite(motorL_2, powerL_b);

  digitalWrite(motorR_1, LOW);
  analogWrite(motorR_2, powerR_b);

  delay(workTime_b);
  stopi(true);
}

// ------------------------- Proportional Functions ------------------------- .

void goForward_proportional(int workTime_fp) {
  for (int i = 5; i < 255; i = i + 50) {
    if (i < 200) {
      goForward(i, i, round(workTime_fp / 4));
    } else {
      goForward(i, i, workTime_fp);
    }
  }
}
