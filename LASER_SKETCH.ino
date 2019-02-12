// Libraries
#include <Encoder.h>

// Lasers
#define RED_LASER_OUT 5
#define GREEN_LASER_OUT 6
#define BLUE_LASER_OUT 7

// Rotary Encoder
#define ENC_ROTOR_IN 3
#define ENC_BUTTON_IN 2

#define MOTOR_STOP_BUTTON 13

// Motor
#define PWM_MOTOR_OUT 10

#define DIR_MOTOR_OUT_IN1 11
#define DIR_MOTOR_OUT_IN2 12



int presetMode = 6;

int currentSpeed = 180;

Encoder rotaryEncoder(8, 9);

long oldPosition  = -999;

void setup() {

  pinMode(RED_LASER_OUT, OUTPUT);
  pinMode(GREEN_LASER_OUT, OUTPUT);
  pinMode(BLUE_LASER_OUT, OUTPUT);

  pinMode(ENC_ROTOR_IN, INPUT);
  pinMode(ENC_BUTTON_IN, INPUT_PULLUP);

  pinMode(PWM_MOTOR_OUT, OUTPUT);
  pinMode(DIR_MOTOR_OUT_IN1, OUTPUT);
  pinMode(DIR_MOTOR_OUT_IN2, OUTPUT);

  pinMode(MOTOR_STOP_BUTTON, INPUT_PULLUP);

  digitalWrite(DIR_MOTOR_OUT_IN1, HIGH); // INIT direction of motor
  digitalWrite(DIR_MOTOR_OUT_IN2, LOW);

}

void loop() {

  checkRotaryInput();
  checkLaserButton();

}

void turnLaserOn(int laserId) {
  if (!digitalRead(laserId)) {
    digitalWrite(laserId, HIGH);
    return true;
  } else {
    return false;
  }
}


boolean turnLaserOff(int laserId) {
  if (digitalRead(laserId)) {
    digitalWrite(laserId, LOW);
    return true;
  } else {
    return false;
  }
}


void turnMotorOff() {
  analogWrite(PWM_MOTOR_OUT, 0);
}


void setMotorSpeed(int pwmAmount) {
  analogWrite(PWM_MOTOR_OUT, 210);
  delay(100);
  analogWrite(PWM_MOTOR_OUT, pwmAmount);
}

void changeMotorDirection(int pwmAmount) {

  if (pwmAmount < 0 && !digitalRead(DIR_MOTOR_OUT_IN1)) {
    digitalWrite(DIR_MOTOR_OUT_IN1, HIGH);
    digitalWrite(DIR_MOTOR_OUT_IN2, LOW);
  }

  else if (pwmAmount > 0 && !digitalRead(DIR_MOTOR_OUT_IN2)) {
    digitalWrite(DIR_MOTOR_OUT_IN1, LOW);
    digitalWrite(DIR_MOTOR_OUT_IN2, HIGH);
  }
}


boolean changeMotorSpeed(int pwmAmount) {
  
  if (pwmAmount > 254 || pwmAmount < -254) { // If out of bonds for PWM, reset to speed
    pwmAmount = 180;
  }

  if (pwmAmount > -180 && pwmAmount < 0) {
    pwmAmount = 180;
  }

  if (pwmAmount < 180 && pwmAmount > 0) {
    pwmAmount = -180;
  }

  changeMotorDirection(abs(pwmAmount));
  
  setMotorSpeed(abs(pwmAmount));
  
  currentSpeed = pwmAmount;
  
  return true;
}

void checkRotaryInput() {

  long currentPosition = rotaryEncoder.read();

  if (currentPosition != oldPosition) {
    if (currentPosition < oldPosition) {
      changeMotorSpeed(currentSpeed - 7);
    } else {
      changeMotorSpeed(currentSpeed + 7);
    }
  }
}


void checkLaserButton() {

  if (digitalRead(ENC_BUTTON_IN)) {
    if (presetMode == 6) {
      presetMode = 0;
    }
    else {
      presetMode++;
    }

    switch (presetMode) {

      case 0 :
        turnLaserOn(RED_LASER_OUT); //RGB
        turnLaserOn(GREEN_LASER_OUT);
        turnLaserOn(BLUE_LASER_OUT);

      case 1 :
        turnLaserOff(GREEN_LASER_OUT); // RB

      case 2 :
        turnLaserOn(GREEN_LASER_OUT); //GB
        turnLaserOff(RED_LASER_OUT);

      case 3 :
        turnLaserOn(RED_LASER_OUT);
        turnLaserOff(BLUE_LASER_OUT); //RG

      case 4 :
        turnLaserOn(RED_LASER_OUT); //R
        turnLaserOff(GREEN_LASER_OUT);
        turnLaserOff(BLUE_LASER_OUT);

      case 5 :
        turnLaserOn(GREEN_LASER_OUT);//G
        turnLaserOff(RED_LASER_OUT);
        turnLaserOff(BLUE_LASER_OUT);

      case 6 :
        turnLaserOn(BLUE_LASER_OUT); //B
        turnLaserOff(GREEN_LASER_OUT);
        turnLaserOff(RED_LASER_OUT);
    }
  }
}

void checkMotorStopButton() {
  if (digitalRead(MOTOR_STOP_BUTTON)) {
    turnMotorOff();
  }
}
