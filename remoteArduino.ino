//ROS
#include "ros.h"
#include <std_msgs/UInt16.h>
#include <ros/time.h>

ros::NodeHandle nh;

std_msgs::UInt16 button_msg;
ros::Publisher pub_button("buttons", &button_msg);

std_msgs::UInt16 vel;
ros::Publisher pub_vel("vel", &vel);

std_msgs::UInt16 curv;
ros::Publisher pub_curv("curv", &curv);


// timers for the sub-main loop
unsigned long currentMillis;
long previousMillis = 0;    // set up timers
float loopTime = 20;

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
  nh.advertise(pub_button);   // advertise topic

}

uint16_t deadzone(uint16_t value) {
  return value;
     if (value > 50) {
        value = value - 50;
     }
     else if (value < -50) {
      value = value +50;
     }
     else {
      value = 0;
     }
     value = value / 500;   // scale so that we get 0.0 ~ 1.0
     return value;  
}

int invButtons(int value) {
  
    if (value == 0) {
      value = 1;
    }
    else if (value == 1) {
      value = 0;
    }
    return value;
}



// put your main code here, to run repeatedly:
void loop() {
  if(analogRead(A6)>350)
    digitalWrite(13,HIGH);
  else
    digitalWrite(13,LOW);

  currentMillis = millis();
  if (currentMillis - previousMillis < loopTime) return;
  
  previousMillis = currentMillis;          // reset the clock to time it 

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

/*stick1 = stick1 - 512;
            stick1 = deadzone(stick1);
            stick2 = stick2 - 512;
            stick2 = deadzone(stick2);
            stick3 = stick3 - 512;
            stick3 = deadzone(stick3);
            stick4 = stick4 - 512;
            stick4 = deadzone(stick4);
            stick5 = stick5 - 512;
            stick5 = deadzone(stick5);
            stick6 = stick6 - 512;
            stick6 = deadzone(stick6);            

            stick5 = stick5 * -1;   // invert value/direction as required based on wiring
            stick3 = stick3 * -1;

            //stick3 = stick3 * 2;    // extra scaling for yaw to mke driving nice
*/

            // *** broadcast cmd_vel twist message **

            vel.data = stick2;
            curv.data = stick1;
            pub_vel.publish(&vel);
            pub_curv.publish(&curv);



            // *** broadcast buttons ***

            button_msg.data = combo;
            pub_button.publish(&button_msg);  

            nh.spinOnce();                  // make sure we do ROS stuff every loop




}
