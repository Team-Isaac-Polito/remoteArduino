//ROS
#include "ros.h"
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>

ros::NodeHandle nh;

std_msgs::UInt16 button_msg;
ros::Publisher pub_button("buttons", &button_msg);

geometry_msgs::Twist twist;
ros::Publisher pub_movement("twist_joystick", &twist);

// timers for the sub-main loop
unsigned long currentMillis;
long previousMillis = 0;    // set up timers
float loopTime = 80;

uint16_t stick1;
uint16_t stick2;
uint16_t stick3;
uint16_t stick4;
uint16_t stick5;
uint16_t stick6;
uint16_t combo = 0;

float deadzone(float value) {
  if (value >= 462 && value <= 562) {
    return 512;
  return value;  
}


void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  
  pinMode(13,OUTPUT);


  //nh.getHardware()->setBaud(115200);      // set baud rate to 115200
  nh.initNode();              // init ROS
  nh.advertise(pub_movement);      // advertise topic
  nh.advertise(pub_button);   // advertise topic
}



// put your main code here, to run repeatedly:
void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis < loopTime) return;
  
  previousMillis = currentMillis;          // reset the clock to time it 

  // buzzer
  if(analogRead(A6)>350)
    digitalWrite(13,HIGH);
  else
    digitalWrite(13,LOW);

  // buttons
  combo = 1024 * digitalRead(2) + 
          512 * digitalRead(3) + 
          256 * digitalRead(4) + 
          128 * digitalRead(5) + 
          64 * digitalRead(6) + 
          32 * digitalRead(8) +
          16 * digitalRead(9) + 
          8 * digitalRead(10) +
          4 * digitalRead(11) + 
          2 * digitalRead(12);
  
  stick1 = analogRead(A0);
  stick2 = analogRead(A1);
  stick3 = analogRead(A2);
  stick4 = analogRead(A3);
  stick5 = analogRead(A4);
  stick6 = analogRead(A5);

  // *** broadcast joysticks ***
  twist.linear.x = deadzone(stick1);
  twist.linear.y = deadzone(stick2);
  twist.linear.z = deadzone(stick3);

  twist.angular.x = deadzone(stick4);
  twist.angular.y = deadzone(stick5);
  twist.angular.z = deadzone(stick6);

  pub_movement.publish(&twist);

  // *** broadcast buttons ***
  button_msg.data = combo;
  pub_button.publish(&button_msg);  

  nh.spinOnce();                  // make sure we do ROS stuff every loop
}
