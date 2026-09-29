#include "hardware_serial.h"
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

void setup() {
  Serial.begin(115200);
  nh.initNode();
  nh.advertise(chatter);
}

void loop() {
  str_msg.data = "Hello from E203!";
  chatter.publish(&str_msg);
  nh.spinOnce();
}

