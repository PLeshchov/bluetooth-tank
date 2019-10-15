#include "L298N.h"

//------Motor------
Motor::Motor(int newPwmPin, int newPin1, int newPin2) {
    pwmPin = newPwmPin;
    pin1 = newPin1;
    pin2 = newPin2;
    // РЎС“РЎвЂљРЎРѓР В°Р Р…Р С•Р Р†Р С”Р В° РЎР‚Р ВµР В¶Р С‘Р С�Р В° Р С—Р С•РЎР‚РЎвЂљР С•Р Р†
    pinMode(pwmPin, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
}

void Motor::setForward() { // РЎС“РЎРѓРЎвЂљР В°Р Р…Р С•Р Р†Р С”Р В° Р С—РЎР‚РЎРЏР С�Р С•Р в„– Р С—Р С•Р В»РЎРЏРЎР‚Р Р…Р С•РЎРѓРЎвЂљР С‘
    direction = 1;
    digitalWrite(pin1, LOW); // Р С›Р вЂ�Р Р‡Р вЂ”Р С’Р СћР вЂўР вЂєР В¬Р СњР С› Р РЋР СњР С’Р В§Р С’Р вЂєР С’ Р вЂ™Р В«Р С™Р вЂєР В®Р В§Р вЂўР СњР пїЅР вЂў Р Т‘Р В»РЎРЏ Р С—РЎР‚Р ВµР Т‘Р С•РЎвЂљР Р†РЎР‚Р В°РЎвЂ°Р ВµР Р…Р С‘Р Вµ Р С—Р С•РЎР‚РЎвЂЎР С‘ Р Т‘РЎР‚Р В°Р в„–Р Р†Р ВµРЎР‚Р В°
    digitalWrite(pin2, HIGH);
}

void Motor::setReverse() { // РЎС“РЎРѓРЎвЂљР В°Р Р…Р С•Р Р†Р С”Р В° Р С•Р В±РЎР‚Р В°РЎвЂљР Р…Р С•Р в„– Р С—Р С•Р В»РЎРЏРЎР‚Р Р…Р С•РЎРѓРЎвЂљР С‘
    direction = -1;
    digitalWrite(pin2, LOW);
    digitalWrite(pin1, HIGH);
}

void Motor::stop() { // Р Р†РЎвЂ№Р С”Р В»РЎР‹РЎвЂЎР ВµР Р…Р С‘Р Вµ Р РЃР пїЅР Сљ Р С‘ Р С—Р С•Р Т‘Р В°РЎвЂЎР С‘ РЎС“Р С—РЎР‚Р В°Р Р†Р В»РЎРЏРЎР‹РЎвЂ°Р ВµР С–Р С• Р Р…Р В°Р С—РЎР‚РЎРЏР В¶Р ВµР Р…Р С‘РЎРЏ
    direction = 0;
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    setSpeed(0);
}

void Motor::setSpeed(int speed) { // РЎС“РЎвЂљРЎРѓР В°Р Р…Р С•Р Р†Р С”Р В° Р Т‘Р В»Р С‘Р Р…РЎвЂ№ Р С‘Р С�Р С—РЎС“Р В»РЎРЉРЎРѓР В° Р РЃР пїЅР Сљ
    analogWrite(pwmPin, speed);
}

//-----DriveSystem------
DriveSystem::DriveSystem(Motor *newLeftMotor, Motor *newRightMotor) {
    leftMotor = newLeftMotor;
    rightMotor = newRightMotor;
}

int DriveSystem::getLeftSpeed() {
    return leftSpeed;
}

int DriveSystem::getRightSpeed() {
    return rightSpeed;
}

void DriveSystem::setSpeed(int leftSpeed, int rightSpeed) { // РЎС“РЎвЂљРЎРѓР В°Р Р…Р С•Р Р†Р С”Р В° РЎРѓР С”Р С•РЎР‚Р С•РЎРѓРЎвЂљР С‘
    if (leftSpeed == 0) { // Р ВµРЎРѓР В»Р С‘ Р С—Р ВµРЎР‚Р ВµР Т‘Р В°Р Р… 0 Р Р…Р В° Р В»Р ВµР Р†РЎвЂ№Р в„– Р Т‘Р Р†Р С‘Р С–Р В°РЎвЂљР ВµР В»РЎРЉ
        leftMotor->stop(); // Р С•РЎРѓРЎвЂљР В°Р Р…Р С•Р Р†Р С”Р В°
    } else if (leftSpeed < 0) { // Р ВµРЎРѓР В»Р С‘ Р С—Р ВµРЎР‚Р ВµР Т‘Р В°Р Р…Р С• Р В·Р Р…Р В°РЎвЂЎР ВµР Р…Р С‘Р Вµ Р С�Р ВµР Р…РЎРЉРЎв‚¬Р Вµ Р Р…РЎС“Р В»РЎРЏ
        leftMotor->setReverse(); // Р С—Р ВµРЎР‚Р ВµР С—Р С•Р В»РЎР‹РЎРѓР С•Р Р†Р С”Р В° Р Т‘Р Р†Р С‘Р С–Р В°РЎвЂљР ВµР В»РЎРЏ Р Р…Р В° РЎР‚Р ВµР Р†Р ВµРЎР‚РЎРѓ
        leftMotor->setSpeed(-leftSpeed); // РЎС“РЎРѓРЎвЂљР В°Р Р…Р С•Р Р†Р С”Р В° Р РЃР пїЅР Сљ Р Р…Р В° Р В·Р В°Р Т‘Р В°Р Р…Р Р…РЎС“РЎР‹ Р Т‘Р В»Р С‘Р Р…РЎС“ Р С‘Р С�Р С—РЎС“Р В»РЎРЉРЎРѓР В°
    } else {
        leftMotor->setForward(); // Р С—РЎР‚РЎРЏР С�Р В°РЎРЏ Р С—Р С•Р В»РЎР‹РЎРѓР С•Р Р†Р С”Р В° Р Т‘Р Р†Р С‘Р С–Р В°РЎвЂљР ВµР В»РЎРЏ
        leftMotor->setSpeed(leftSpeed); // РЎС“РЎРѓРЎвЂљР В°Р Р…Р С•Р Р†Р С”Р В° Р РЃР пїЅР Сљ Р Р…Р В° Р В·Р В°Р Т‘Р В°Р Р…Р Р…РЎС“РЎР‹ Р Т‘Р В»Р С‘Р Р…РЎС“ Р С‘Р С�Р С—РЎС“Р В»РЎРЉРЎРѓР В°
    }
    if (rightSpeed == 0) { // Р В°Р Р…Р В°Р В»Р С•Р С–Р С‘РЎвЂЎР Р…Р С• Р Т‘Р В»РЎРЏ Р С—РЎР‚Р В°Р Р†Р С•Р С–Р С• Р С�Р С•РЎвЂљР С•РЎР‚Р В°
        rightMotor->stop();
    } else if (rightSpeed < 0) {
        rightMotor->setReverse();
        rightMotor->setSpeed(-rightSpeed);
    } else {
        rightMotor->setForward();
        rightMotor->setSpeed(rightSpeed);
    }
    // РЎРѓР С•РЎвЂ¦РЎР‚Р В°Р Р…Р ВµР Р…Р С‘Р Вµ Р С—Р ВµРЎР‚Р ВµР Т‘Р В°Р Р…Р Р…РЎвЂ№РЎвЂ¦ РЎРѓР С”Р С•РЎР‚Р С•РЎРѓРЎвЂљР ВµР в„–
    this->leftSpeed = leftSpeed;
    this->rightSpeed = rightSpeed;
}

