#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>

 volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
unsigned long newtime;
unsigned long oldtime = 0;
float vel;
float newposition;
float oldposition = 0;
float pos_radians;

float start_signal=0; 
int i;
uint8_t k=false;


void start_function(const std_msgs::Bool& start_msg)
{ 
  k=start_msg.data;
  if(k)
  start_signal=1;
  else 
  start_signal=0;
  
}

ros::NodeHandle nh;

std_msgs::Float32 encoder_count;
std_msgs::Float32 encoder_velocity;


ros::Publisher encoder_pub1("encoder_pos_radians", &encoder_count);
ros::Publisher encoder_pub2("encoder_vel_radians", &encoder_velocity);
ros::Subscriber<std_msgs::Bool> sub("start_signal_topic", &start_function);




void setup() {
  cli();//stop interrupts

//set timer1 interrupt at 100Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments  
  OCR1A = 155 ;// = (16*10^6) / (1*1024) - 1 (must be <65536)  %77
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  
  Serial.begin(57600);
  
  nh.initNode();
  nh.advertise(encoder_pub1);
   nh.advertise(encoder_pub2);
   nh.subscribe(sub);
  
  
  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2  (white)...
  // orange- open
  
  pinMode(3, INPUT_PULLUP); // internalเป็น pullup input pin 3 (black)
//Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  //attachInterrupt(0, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  //attachInterrupt(1, ai1, RISING);
  }
   
  void loop() {
  
 nh.spinOnce();
   if(start_signal==0)
   {
   pos_radians=0;
   oldposition=0;
   counter=0; 
   TIMSK1=0;
   
   }
   else if(start_signal==1)
   {
    
    attachInterrupt(0, ai0, RISING);
    attachInterrupt(1, ai1, RISING);
    TIMSK1 |= (1 << OCIE1A);
   }

  }
   
  void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
  counter--;
  }else{
  counter++;
  }
}


ISR(TIMER1_COMPA_vect){//timer1 interrupt 100 Hz Service Routine
//
  //if( counter != temp )
  {
  pos_radians=(float(counter)/2048)*6.283;   // Position in radians
  encoder_count.data=pos_radians;
  encoder_pub1.publish(&encoder_count);
  
  temp = counter;

  newposition= pos_radians;
  

   vel=(newposition-oldposition)/0.01; //************************************************************** Velocity in radians per second
   encoder_velocity.data=vel;
   encoder_pub2.publish(&encoder_velocity);
  

  oldposition=newposition;
   

  
  }
  i=i+1;
}





