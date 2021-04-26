import controlP5.*;

// by Megan Hendrie and Joshua Boynton 
//imported librarires 
import controlP5.*; 
import processing.serial.*; 
//defining variables 
ControlP5 cp5; 
Textarea myTextarea; 
Serial port; 
Slider frequencySlider; 
Slider xStrainSlider; 
Slider yStrainSlider; 
float timePerCycleTempTemp; 
float powertoSpeedX; 
float powertoSpeedY; 
float j; 
Textlabel cycleTextLabel; 
int myColor = color(51,153,255); 
float valueMessage; 
String cycleNumber = "0"; 
int sliderTicks1 = 100; 
Slider abc; 
//sets the size of the window 
void setup() {   
  size(2500,2500);   
  cp5 = new ControlP5(this); 
  // creates a text box with the following parameters   
  myTextarea = cp5.addTextarea("txt") 
     .setPosition(800,880) 
     .setSize(800,800) 
     .setFont(createFont("arial",50)) 
     .setLineHeight(56) 
     .setColor(color(204)) 
     .setColorBackground(color(255,100)) 
     .setColorForeground(color(255,100)); 
     ; 
  myTextarea.setText("This is how the well will deform."); 
// this creates a vertical and horizontal slider each with a pre-set strain of 10% 
     xStrainSlider=cp5.addSlider("Strain_X") 
     .setRange(0,20) 
     .setValue(10) 
     .setPosition(700,1800) 
     .setSize(800,120) 
     .setNumberOfTickMarks(21) 
     .setFont(createFont("arial",50)) 
     ; 
     yStrainSlider=cp5.addSlider("Strain_Y") 
     .setPosition(1870,600) 
     .setSize(120,800) 
     .setRange(0,20) 
     .setValue(10) 
     .setNumberOfTickMarks(21) 
     .setFont(createFont("arial",50)) 
     ; 
     noStroke(); 
     cp5 = new ControlP5(this); 
 
   //String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port 
//IMPORTANT, this code sets up the connection from the GUI to the Arduino  
   //port = new Serial(this, portName, 9600); 
  // create the frequency slider with tick marks, starts at 1 Hz. 
     frequencySlider=cp5.addSlider("Frequency Hz") 
     .setPosition(200,600) 
     .setSize(120,800) 
     .setRange(0.1,1) 
     .setValue(1) 
     .setNumberOfTickMarks(10) 
     .setFont(createFont("arial",50)) 
     ; 
     //sets the home position of the linear actuators to be at "zero" 
     cp5.addButton("Home")    .setValue(0) 
     .setPosition(700,200) 
     .setSize(800,80) 
     .setFont(createFont("arial",50)) 
     ; 
   //tells program to initiate non-alternating strain pattern    
     cp5.addButton("Strain")    
     .setValue(0) 
     .setPosition(700,300) 
     .setSize(380,80) 
     .setFont(createFont("arial",50)) 
     ;   
   //tells program to initiate alternating strain pattern 
     cp5.addButton("Alt_Strain") 
     .setValue(0) 
     .setPosition(1090,300) 
     .setSize(410,80) 
     .setFont(createFont("arial",50)) 
     ;   
     cycleTextLabel = cp5.addTextlabel("cyclelabel") //creates label for number of cycles 
     .setPosition(2000,500) 
     .setFont(createFont("arial",50));    
     //tells program to cease running    
     cp5.addButton("STOP")    
     .setValue(0) 
     .setPosition(700,400) 
     .setSize(800,80) 
     .setFont(createFont("arial",50)) 
     ; 
} 
// maintaines text when adjusting box area 
void keyPressed() {   
  if(key=='r') { 
     myTextarea.setText("This is how the well will deform.");    
  } else if(key=='c') { 
     myTextarea.setColor(0xffffffff);   
  } 
} 
//changes the size of the text as box area changes and updates the number of cycles 
void draw() {   
  background(0);   
  if(keyPressed && key==' ') { 
     myTextarea.scroll((float)mouseX/(float)width); 
  } 
  if(keyPressed && key=='l') { 
     myTextarea.setLineHeight(mouseY); 
  } 
  cycleTextLabel.setText(cycleNumber + " cycles"); 
} 
//changes the size of the box area when the sliders change 
void Strain_X(int theValue) {   
  myTextarea.setWidth(theValue*40); 
} 
void Strain_Y(int theValue) {   
  myTextarea.setHeight(theValue*40);  
} 
void slider(float theColor) {   
  myColor = color(theColor);   
  println("a slider event. setting background to "+theColor); 
} 
 
public void Strain(){ //program that runs when pressing strain button, runs non-alternating strain pattern 
println("I'm running"); 
float frequencyValue = frequencySlider.getValue(); // value taken from frequency slider 
int timePerCycleTemp = round((100/frequencyValue)); // converts frequency to time value, multiplies it by 100 for easier conversion to arduino. Divides it by 100 later in arduino program. 
println(timePerCycleTemp); 
float displacementXinput = xStrainSlider.getValue(); // value taken from x strain slider 
float displacementYinput = yStrainSlider.getValue(); // value taken from y strain slider 
//int powertoSpeedX =round(((((((displacementXinput/2)*0.1)+0.5*displacementXinput)/25.4)*2)/0.0 023137)*frequencyValue); // IMPORTANT: converts x_strain value to power input for x direction. 0=minimum, 255= maximum. This equation needs to be changed to change the length the well is streteched in this direction. 
//int powertoSpeedY =round(((((((displacementYinput/2)*0.1)+0.5*displacementYinput)/25.4)*2)/0.0 023137)*frequencyValue); // IMPORTANT: converts y_strain value to power input for y direction. 0=minimum, 255= maximum. This equation needs to be changed to change the length the well is streteched in this direction. 
println(powertoSpeedX); 
if(powertoSpeedX>255 || powertoSpeedY>255){ 
println("ERROR parameters too high for motors, lower frequency and/or strain");} //error for when power input values are too high for motor to handle 
else{ //values given to arduino to run code 
port.write(str(timePerCycleTemp)); // time value 
port.write(","); 
port.write(str(powertoSpeedX)); // x power input value 
port.write(","); 
port.write(str(powertoSpeedY)); // y power input value 
port.write(","); port.write(str(1)); // selector value for running different programs 
port.write("q"); // random variable to end string reading in arduino 
} 
} 
public void STOP(){ //program that runs when pressing stop button, stops actuators 
println("I'm stopping"); 
//values given to arduino to run code 
port.write(str(1)); // time value 
port.write(","); 
port.write(str(0)); // x power input value 
port.write(","); 
port.write(str(0)); // y power input value 
port.write(","); 
port.write(str(2));// selector value for running different programs 
port.write("q");   // random variable to end string reading in arduino 
} 
public void Home(){ //program that runs when pressing home button, sets actuator to home postition  
println("Going home"); 
//values given to arduino to run code 
port.write(str(1)); // time value 
port.write(","); port.write(str(255)); // x power input value 
port.write(","); port.write(str(255)); // y power input value 
port.write(","); port.write(str(2)); // selector value for running different programs 
port.write("q"); // random variable to end string reading in arduino 
} 
public void Alt_Strain(){ //program that runs when pressing alt strain button, runs alternating strain pattern 
println("I'm running"); 
float frequencyValue = frequencySlider.getValue(); // value taken from frequency slider 
int timePerCycleTemp =round((100/frequencyValue)); // converts frequency to time value, multipites it by 100 for easier conversion to arduino. Divides it by 100 later in arduino program. 
println(timePerCycleTemp); 
float displacementXinput = xStrainSlider.getValue(); // value taken from x strain slider 
float displacementYinput = yStrainSlider.getValue(); // value taken from y strain slider 
//int powertoSpeedX =round(((((((displacementXinput/2)*0.1)+0.5*displacementXinput)/25.4)*2)/0.0 023137)*frequencyValue); // IMPORTANT: converts x_strain value to power input for x direction. 0=minimum, 255= maximum. This equation needs to be changed to change the length the well is streteched in this direction. 
//int powertoSpeedY =round(((((((displacementYinput/2)*0.1)+0.5*displacementYinput)/25.4)*2)/0.0 023137)*frequencyValue); // IMPORTANT: converts y_strain value to power input for y direction. 0=minimum, 255= maximum. This equation needs to be changed to change the length the well is streteched in this direction. 
println(powertoSpeedX); 
if(powertoSpeedX>255 || powertoSpeedY>255){ 
println("ERROR parameters too high for motors, lower frequency and/or strain");} //error for when power input values are too high for motor to handle 
else{ //values given to arduino to run code 
port.write(str(timePerCycleTemp)); // time value 
port.write(","); 
port.write(str(powertoSpeedX)); // x power input value 
port.write(","); 
port.write(str(powertoSpeedY)); // y power input value 
port.write(","); 
port.write(str(3)); // selector value for running different programs 
port.write("q"); // random variable to end string reading in arduino 
} 
} 
void serialEvent(Serial p) { //returns the current amount of cycles from arduino to GUI 
  // get message till line break (ASCII > 13)   
  String message = port.readStringUntil(13);   
  if (message != null) { 
    valueMessage = float(message); 
    cycleNumber = str(valueMessage); 
//  println("This many cycles: "+ value); 
  } 
} 
 
 
 
