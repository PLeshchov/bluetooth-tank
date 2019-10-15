#include <Arduino.h>
#include "L298N.h"
#include <SoftwareSerial.h>

/*
 ����� � ������� "Bluetooth ������� �� Arduino"
 �������� ������� (�����, ��������): https://alexgyver.ru/bluetooth-car/
 ��������� �� GitHub: https://github.com/AlexGyver/BluetoothCar
 ��������, ��� ������� � ��������������� ���? �������� ������! https://alexgyver.ru/support_alex/
 �����: AlexGyver Technologies, 2018
 http://AlexGyver.ru/
 */

#define MOTOR_MAX 255   // ������������ ������ �� ����� (max 255)
#define JOY_MAX 40      // ������� ��� ��������� (�� ����������)

#define IN1 10
#define IN2 9
#define PWD_A 11
#define IN3 8
#define IN4 7
#define PWD_B 6

/*
 Bluetooth ��� ����� ���� $valueX valueY;
 ��� ������� parsing ��������� ���� ����� � ������ intData
 ������� ��������� ����������, �� ����������� � � ������� �� ������ ������,
 ��� ��� ������������ ������� ��������� �������. ��� �������� �������, �� � ��� ����
 */
#define PARSE_AMOUNT 2         // ����� �������� � �������, ������� ����� ��������

int intData[PARSE_AMOUNT];     // ������ ��������� �������� ����� ��������
boolean recievedFlag;
int dutyR, dutyL;
int signalX, signalY;
int dataX, dataY;

Motor *motorL = new Motor(PWD_B, IN3, IN4);
Motor *motorR = new Motor(PWD_A, IN1, IN2);

DriveSystem *driveSystem = new DriveSystem(motorL, motorR);

boolean getStarted;
byte index;
String string_convert = "";

void parsing() {
    if (Serial.available() > 0) {
        char incomingByte = Serial.read(); // ����������� ������ �������� ������
        if (getStarted) {    // ���� ������� ��������� ������ (������� ��������)
            if (incomingByte != ' ' && incomingByte != ';') { // ���� ��� �� ������ � �� �����
                string_convert += incomingByte;       // ���������� � ������
            } else {                       // ���� ��� ������ ��� ; ����� ������
                intData[index] = string_convert.toInt(); // ����������� ������ � int � ����� � ������
                string_convert = "";                  // ������� ������
                index++;     // ��������� � �������� ���������� �������� �������
            }
        }
        if (incomingByte == '$') {                // ���� ��� $
            getStarted = true;              // ��������� ����, ��� ����� �������
            index = 0;                              // ���������� ������
            string_convert = "";                    // ������� ������
        }
        if (incomingByte == ';') {       // ���� ���� ������� ; - ����� ��������
            getStarted = false;                     // �����
            recievedFlag = true;                    // ���� �� ��������
        }
    }
}

void setup() {
    Serial.begin(9600);
    //PWMfrequency(IN2, 1);   // 31 ���
}

void loop() {
    parsing();                    // ������� ��������
    if (recievedFlag) {           // ���� �������� ������
        recievedFlag = false;
        dataX = intData[0];
        dataY = intData[1];
        /*for (byte i = 0; i < PARSE_AMOUNT; i++) { // ������� �������� �������
         Serial.print(intData[i]); Serial.print(" ");
         } Serial.println();*/

    }

    if (dataX == 0 && dataY == 0) {   // ���� �� � "������" ����
        motorR->stop();           // �� ���������
        motorL->stop();
        dutyR = 0;
        dutyL = dutyR;
    } else {
        signalY = map((dataY), -JOY_MAX, JOY_MAX, -MOTOR_MAX, MOTOR_MAX); // ������ �� �
        signalX = map((dataX), -JOY_MAX, JOY_MAX, -MOTOR_MAX / 2,
        MOTOR_MAX / 2); // ������ �� �

        dutyR = signalY + signalX;
        dutyL = signalY - signalX;

        dutyR = constrain(dutyR, -MOTOR_MAX, MOTOR_MAX);
        dutyL = constrain(dutyL, -MOTOR_MAX, MOTOR_MAX);

//    Serial.print(dutyR);
//    Serial.print(" ");
//    Serial.println(dutyL);
    }
//  motorR.setSpeed(dutyR);
//  motorL.setSpeed(dutyL);
    driveSystem->setSpeed(dutyL, dutyR);
    delay(5);
}
