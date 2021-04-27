// include the SPI library:

#include <SPI.h>    //defines variable names
// L9958 slave select pins for SPI

#define SS_M3 13
#define SS_M2 12
#define SS_M1 11
#define SS_M4 14
// L9958 DIRection pins
#define DIR_M1 2
#define DIR_M2 3
#define DIR_M3 4
#define DIR_M4 7
// L9958 PWM pins
#define PWM_M1 9
#define PWM_M2 10    // Timer1
#define PWM_M3 5
#define PWM_M4 6   

// L9958 Enable for all 4 motors //define variables to be used in code
#define ENABLE_MOTORS 8
int pwm1, pwm2, pwm3, pwm4;
boolean dir1, dir2, dir3, dir4;

const int numberOfFields = 4;    // How many values are being sent from GUI to ardunino 
int fieldIndex = 0;              // The current field index being received
int values[numberOfFields];      // An array which holds values of all the fields
int i = 1;                       // A variable for running the motor loop
float cycleCount = 0.0;          // A variable to store the number of cycles

void setup() {
  // setup code here, to run once:
  
  Serial.begin(9600);            // Open serial connection, 9600 baud to interface with GUI
  unsigned int 
  configWord;

   // setting up definitions for power and direction
  pinMode(SS_M1, OUTPUT); digitalWrite(SS_M1, LOW);  // HIGH = not selected
  pinMode(SS_M2, OUTPUT); digitalWrite(SS_M2, LOW);
  pinMode(SS_M3, OUTPUT); digitalWrite(SS_M3, LOW);
  pinMode(SS_M4, OUTPUT); digitalWrite(SS_M4, LOW);

  // L9958 DIRection pins
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);  
  pinMode(DIR_M3, OUTPUT);
  pinMode(DIR_M4, OUTPUT);

  // L9958 PWM pins
  pinMode(PWM_M1, OUTPUT);  digitalWrite(PWM_M1, LOW);
  pinMode(PWM_M2, OUTPUT);  digitalWrite(PWM_M2, LOW);    // Timer1
  pinMode(PWM_M3, OUTPUT);  digitalWrite(PWM_M3, LOW);
  pinMode(PWM_M4, OUTPUT);  digitalWrite(PWM_M4, LOW);

  // L9958 Enable for all 4 motors
  pinMode(ENABLE_MOTORS, OUTPUT); 
 digitalWrite(ENABLE_MOTORS, HIGH);  // HIGH = disabled

  // set to max current limit and disable ISR slew limiting
  configWord = 0b0000010000001100;

  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE1);  // clock pol = low, phase = high

  // defines 4 motors and what variables go to them
  // Motor 1
  digitalWrite(SS_M1, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M1, HIGH);
  // Motor 2
  digitalWrite(SS_M2, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M2, HIGH);
  //Motor 3
  digitalWrite(SS_M3, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M3, HIGH);
    //Motor 4
  digitalWrite(SS_M3, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M4, HIGH);
  
  //Set initial actuator settings to pull at 0 speed for safety
  dir1 = 0; dir2 = 0; dir3 = 0; dir4=0; // Set direction
  pwm1 = 0; pwm2 = 0; pwm3 = 0; pwm4=0;// Set speed (0-255)

  digitalWrite(ENABLE_MOTORS, LOW);// LOW = enabled

// IMPORTANT: this is the code that autmatically places the motor in the home postition when powered on/booting up the GUI
  dir1=0;
  pwm1=255;
  digitalWrite(DIR_M1, dir1);
  analogWrite(PWM_M1, pwm1); // write to pins

  dir2 = 0;
  pwm2 = 255;
  digitalWrite(DIR_M2, dir2);
  analogWrite(PWM_M2, pwm2);

  dir3 = 0;
  pwm3 = 255;
  digitalWrite(DIR_M3, dir3);
  analogWrite(PWM_M3, pwm3);

  dir4 = 0;
  pwm4 = 255;
  digitalWrite(DIR_M4, dir4);
  analogWrite(PWM_M4, pwm4);
  
  delay(5000);
} // End setup

void loop()
  {
  if( Serial.available()>0) {                           // following code is for reading what infomration was sent from GUI to arduino
    char ch = Serial.read();      // Store the serial input
    if(ch >= '0' && ch <= '9')    // Check if this an ascii digit between 0 and 9
    {
      // If it is, accumulate the value in the array
      values[fieldIndex] = (values[fieldIndex] * 10) + (ch - '0'); 
    }
    else if (ch == ',')  // If it's a comma (the chosen seperator), move on to the next field
    {
      if(fieldIndex < numberOfFields - 1)
        fieldIndex++;    // Move onto the next field index
    }
    else
    {
      // Each of the values collected in the array are stored as variables
      int timePerCycleTemp = (int)values[0]; //variable that will be used to generate timePerCycle value
      int xStrainValue = (int)values[1]; //variable for the velocity of the actuators to strain in the x direction
      int yStrainValue = (int)values[2]; //variable for the velocity of the actuators to strain in the y direction
      int selectorvalue = (int)values[3]; //variable that will be used to select case to perform a sepcific action, all these value were taken from GUI
      
      float timePerCycle=(timePerCycleTemp/100); //variable for setting the time per cycle,

      for(int j=0; j <= fieldIndex; j++)
      {
        values[j] = 0; // Clear the array values now that they are stored elsewhere
      }
      fieldIndex = 0;  // Reset the field index - ready to start over

switch (selectorvalue){ //selects which case to run based on selector value

case 1:   //IMPORTANT: code for running strain and stop actions (stop program has power input values for x and y strain as 0, so actuators don't move during either section of this code)

while(Serial.available() ==0){      
while(i==1){ 
    //all actuators extend in this step
    dir1 = 1; //sets direction of motor. 0=retract, 1=extend                                       
    pwm1 = xStrainValue; //sets speed of motor. 0 is minimum value, 255 is the maximum value.  
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1); // write to pins, tells motors 

    dir2 = 1;
    pwm2 = yStrainValue;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);

    dir3 = 1;
    pwm3 = yStrainValue;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    dir4 = 1;
    pwm4 = xStrainValue;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);

    delay(timePerCycle*500); // length of time motors run this step for, 1000=1 second

    i=i+1;
}

while(i==2){
    //all actuators retract in this step
    dir1 = 0;
    pwm1 = xStrainValue;
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1);

    dir2 = 0;
    pwm2 = yStrainValue;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);

    dir3 = 0;
    pwm3 = yStrainValue;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    dir4 = 0;
    pwm4 = xStrainValue;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);

    delay(timePerCycle*500);

    i=i-1;
    cycleCount = cycleCount+1;
    Serial.println(cycleCount); //send number of cycles to GUI to be displayed
   }

} 
break; 

case 2:  //IMPORTANT: code for running home action
    // all actuators retract at max speed until they are fully retracted 
    dir1 = 0;
    pwm1 = xStrainValue; //set direction and speed 
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1); // write to pins

    dir2 = 0;
    pwm2 = yStrainValue;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);

    dir3 = 0;
    pwm3 = yStrainValue;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    dir4 = 0;
    pwm4 = xStrainValue;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);

    delay(timePerCycle*500);  // length of time motors run this step for
break;

case 3:  //IMPORTANT: code for running alt strain action

while(Serial.available() ==0){      
while(i==1){
  // actuators in x direction extend
    dir1 = 1;
    pwm1 = xStrainValue; //set direction and speed 
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1); // write to pins

    dir4 = 1;
    pwm4 = xStrainValue;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);

    dir2 = 1;
    pwm2 = 0;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);

    dir3 = 1;
    pwm3 = 0;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    delay(timePerCycle*500);  // length of time until motors run this step for
    i=i+1;
}

while (i==2){
  // actuators in x direction retract
    dir1 = 0;
    pwm1 = xStrainValue; //set direction and speed 
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1); // write to pins

    dir4 = 0;
    pwm4 = xStrainValue;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);
    
    dir2 = 1;
    pwm2 = 0;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);


    dir3 = 1;
    pwm3 = 0;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    delay(timePerCycle*500);  // length of time until motors run this step for
    i=i+1;
}


while(i==3){
// actuators in y direction extend
    dir1 = 0;
    pwm1 = 0;
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1);

    dir4 = 0;
    pwm4 = 0;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);

    dir2 = 1;
    pwm2 = yStrainValue;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);

    dir3 = 1;
    pwm3 = yStrainValue;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    delay(timePerCycle*500);  // length of time until motors run this step for
     i=i+1;
}


while(i==4){
// actuators in y direction retract 
    dir1 = 0;
    pwm1 = 0;
    digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M1, pwm1);

    dir4 = 0;
    pwm4 = 0;
    digitalWrite(DIR_M4, dir4);
    analogWrite(PWM_M4, pwm4);
    
    dir2 = 0;
    pwm2 = yStrainValue;
    digitalWrite(DIR_M2, dir2);
    analogWrite(PWM_M2, pwm2);

    dir3 = 0;
    pwm3 = yStrainValue;
    digitalWrite(DIR_M3, dir3);
    analogWrite(PWM_M3, pwm3);

    delay(timePerCycle*500);

    i=i-3;
    cycleCount = cycleCount+1;
    Serial.println(cycleCount); //send number of cycles to GUI to be displayed
   }

} 
break; 
}
  }
  }
  }
  
