//ROS
#include "ros.h"
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>

ros::NodeHandle nh;

std_msgs::UInt16 button_msg;
ros::Publisher pub_button("buttons", &button_msg);

std_msgs::UInt16 vel;
ros::Publisher pub_vel("vel", &vel);

std_msgs::UInt16 curv;
ros::Publisher pub_curv("curv", &curv);

std_msgs::UInt16 pitch;
ros::Publisher pub_pitch("pitch", &pitch);

std_msgs::UInt16 eex;
ros::Publisher pub_eex("eex", &eex);

std_msgs::UInt16 eey;
ros::Publisher pub_eey("eey", &eey);

std_msgs::UInt16 eez;
ros::Publisher pub_eez("eez", &eez);


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
  nh.advertise(pub_vel);      // advertise topic
  nh.advertise(pub_curv);      // advertise topic
  nh.advertise(pub_pitch);      // advertise topic
  nh.advertise(pub_button);   // advertise topic
  nh.advertise(pub_eex);   // advertise topic
  nh.advertise(pub_eey);   // advertise topic
  nh.advertise(pub_eez);   // advertise topic
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


  // *** broadcast movement stick ***
  vel.data = stick2;
  curv.data = stick1;
  pitch.data = stick3;
  pub_vel.publish(&vel);
  pub_curv.publish(&curv);
  pub_pitch.publish(&pitch);

  // *** broadcast EE stick ***
  eex.data = stick4;
  eey.data = stick5;
  eez.data = stick6;
  pub_eex.publish(&eex);
  pub_eey.publish(&eey);
  pub_eez.publish(&eez);

  // *** broadcast buttons ***
  button_msg.data = combo;
  pub_button.publish(&button_msg);  

  nh.spinOnce();                  // make sure we do ROS stuff every loop
}
