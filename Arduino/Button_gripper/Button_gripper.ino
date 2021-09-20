
/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Bool.h>

ros::NodeHandle_<ArduinoHardware, 2, 3, 80, 105> nh;

Servo servo;

void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo.write(cmd_msg.data); //set servo angle, should be from 0-180  
  digitalWrite(7, HIGH-digitalRead(7));  //toggle led  
}


ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb);
std_msgs::Bool pushed_msg;

const int button_pin = 4;

bool last_reading;
long last_debounce_time=0;
long debounce_delay=50;
bool published = true;

void setup(){
  //initialize an LED output pin 
  //and a input pin for our push button
  pinMode(button_pin, INPUT);
  pinMode(6, OUTPUT); // LED POWER
  //Enable the pullup resistor on the button
  digitalWrite(button_pin, HIGH);
  digitalWrite(6, HIGH);
  //The button is a normally button
  last_reading = ! digitalRead(button_pin);
  servo.attach(3); //attach it to pin 3
  
  nh.initNode();
  nh.subscribe(sub);
 
}

void loop(){
  bool reading = ! digitalRead(button_pin);
  
  if (last_reading!= reading){
      last_debounce_time = millis();
      published = false;
  }
  
  //if the button value has not changed for the debounce delay, we know its stable
  if ( !published && (millis() - last_debounce_time)  > debounce_delay) {
    pushed_msg.data = reading;
    pub_button.publish(&pushed_msg);
    published = true;
  }

  last_reading = reading;
  nh.spinOnce();
  delay(1);
}
