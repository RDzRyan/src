/*
 * rosserial Servo Control Example
Program ROSSERIAL ARDUINO ATAS _ PUB PB DAN SUB SERVO
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

ros::NodeHandle  nh;

Servo servo;

void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo.write(cmd_msg.data); //set servo angle, should be from 0-180  
  digitalWrite(7, HIGH-digitalRead(7));  //toggle led  
}

void pompa_cb( const std_msgs::Bool& cmd_msg){
  if(cmd_msg.data == true){
     digitalWrite(12, HIGH);
  }
  else{digitalWrite(12, LOW);}
    
}


ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb);
ros::Subscriber<std_msgs::Bool> dub("pompa", pompa_cb);
std_msgs::Bool pushed_msg;
ros::Publisher pub_button("pushed", &pushed_msg);

const int button_pin = 4;
const int led_pin = 8;

bool last_reading;
long last_debounce_time=0;
long debounce_delay=50;
bool published = true;

void setup(){
  pinMode(6, OUTPUT); // LED POWER
  //initialize an LED output pin 
  //and a input pin for our push button
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  digitalWrite(6, HIGH);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(dub);
  nh.advertise(pub_button);
  
  //Enable the pullup resistor on the button
  digitalWrite(button_pin, HIGH);
  
  //The button is a normally button
  last_reading = ! digitalRead(button_pin);
  
  servo.attach(3); //attach it to pin 3
}

void loop(){
  bool reading = ! digitalRead(button_pin);
  
  if (last_reading!= reading){
      last_debounce_time = millis();
      published = false;
  }
  
  //if the button value has not changed for the debounce delay, we know its stable
  if ( !published && (millis() - last_debounce_time)  > debounce_delay) {
    digitalWrite(led_pin, reading);
    pushed_msg.data = reading;
    pub_button.publish(&pushed_msg);
    published = true;
  }

  last_reading = reading;
  
  nh.spinOnce();
  delay(30);
}
