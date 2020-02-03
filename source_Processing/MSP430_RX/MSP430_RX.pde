import processing.serial.*;
import java.awt.datatransfer.*;
import java.awt.Toolkit;
import processing.opengl.*;
import objloader.*;
import g4p_controls.*;

Circle[] c;
Outer[] O;

boolean      printSerial = false;
boolean      printPressure8 = false;
boolean      printFLEXMax = false;

int screen_height = 800;
int screen_width = 1200;
int CenterX=screen_width/2;
int CenterY=screen_height/2;
int num_flex=4;
float incrementX;
float incrementY;
float RADIUS_CENTER=0;

// Serial port state.
Serial       port;
final String serialConfigFile = "serialconfig.txt";

PImage img;  // Declare variable "a" of type PImage

int mouseposition;
float mouseposition1;
float[] FLEX = {0,0,0,0}; //this is controlling the ellipses on the outside 
float[] PRESS1 = {0,0,0,0};//this is controlling the center circle 
float[] FLEX1 = {0,0,0,0}; 
float[] PRESS2 = {0,0,0,0};

float[] FLEX_avg = {0,0,0,0};
float[] PRESS_avg = {0,0,0,0};
int i;   // Counter for averaging measurements

//*****************************************************************NEED TO ENTER MAX VALUES FOR THE PRESSURE AND FLEX SENSORS HERE **********************


//YOU DONT REALLY NEED THE MIN
int[] Min_Flex ={3000, 2750, 2800, 2650};
int Min_Press = 3500;

float[] Max_Flex ={3450 - Min_Flex[0], 3200 - Min_Flex[1], 3070 - Min_Flex[2], 3000 - Min_Flex[3]};
float Max_Press = 3540 - Min_Press; //what is the maximum pressure?

float Average_Max_Flex1=0;



// UI controls.
GPanel    configPanel;
GDropList serialList; 
GLabel    serialLabel;
GLabel    calLabel;
GCheckbox printSerialCheckbox;
GCheckbox ShowPressure8Checkbox;
GCheckbox ShowPressureMaxCheckbox;
GDropList ComparePressurelist;


void setup(){

size(1200, 800, OPENGL);
img = loadImage("background1.png");  // Load the image into the program, this is the background
                                    //you can change the background above but dont pick an image
                                     //too large or it wont load



//creates an array for the inner/center elements, note there is more than one 
c  = new Circle[8];
  for (int i = 0; i < c.length; i++) {
    c[i] = new Circle(); // Create each object
    c[i].Start(CenterX,CenterY); //assigns the start position to the center element 
  }
  
  //creates an array of outer elements
O  = new Outer[num_flex]; //change the value in here to how manyflex sensros you have
  for (int i = 0; i < O.length; i++) {
    O[i] = new Outer(); // Create each object
    O[i].Start(CenterX,CenterY,RADIUS_CENTER);//assigns the start position to the center element 
  }

  // Serial port setup.
  // Grab list of serial ports and choose one that was persisted earlier or default to the first port.
  int selectedPort = 0;
  String[] availablePorts = Serial.list();
  if (availablePorts == null) {
    println("ERROR: No serial ports available!");
    exit();
  }
  String[] serialConfig = loadStrings(serialConfigFile);
  if (serialConfig != null && serialConfig.length > 0) {
    String savedPort = serialConfig[0];
    // Check if saved port is in available ports.
    for (int i = 0; i < availablePorts.length; ++i) {
      if (availablePorts[i].equals(savedPort)) {
        selectedPort = i;
      } 
    }
  }
  /******Build serial config UI (the text block at the top with buttons)******/

  // Build serial config UI.
  configPanel = new GPanel(this, 5, 10, width-20, 90, "Configuration (click to hide/show)");
  serialLabel = new GLabel(this,  0, 20, 80, 25, "Serial port:");
  configPanel.addControl(serialLabel);
  serialList = new GDropList(this, 90, 20, 200, 200, 6);
  serialList.setItems(availablePorts, selectedPort);
  configPanel.addControl(serialList);
  //calLabel = new GLabel(this, 300, 20, 350, 25, "Calibration: Sys=? Gyro=? Accel=? Mag=?");
  //configPanel.addControl(calLabel); 
  
  printSerialCheckbox = new GCheckbox(this, 5, 40, 200, 20, "Print serial data");
  ShowPressure8Checkbox = new GCheckbox(this, 5, 55, 200, 20, "Print Pressure");
  ShowPressureMaxCheckbox = new GCheckbox(this, 5, 70, 200, 20, "Print MAX FLEX");
  
  ShowPressure8Checkbox.setSelected(printSerial);
  ShowPressureMaxCheckbox.setSelected(printSerial);
  printSerialCheckbox.setSelected(printSerial);
  configPanel.addControl(printSerialCheckbox);
  configPanel.addControl(ShowPressure8Checkbox);
  configPanel.addControl(ShowPressureMaxCheckbox);
 
  
  // Set serial port.
  setSerialPort(serialList.getSelectedText());
}

void draw(){
  
  // Create an average from flex measurements
  
  
  
  //ignore this TEST VALUES HERE 
  /*mouseposition=2*(mouseX+mouseY);
  mouseposition1=(mouseX+mouseY);
  FLEX[0]=mouseposition;
  FLEX[1]=mouseposition;
  FLEX[2]=mouseposition;
  FLEX[3]=mouseposition;
  PRESS1[0]=mouseposition;
//  PRESS1[1]=mouseposition;
 // PRESS1[2]=mouseposition;
//  PRESS1[3]=mouseposition;
  //ignore this, testing only 
   */
  
  background(240,255);
  //this places the background image
  image(img, 0, 0);//, img.height ,img.width);

  Average_Max_Flex1 = (FLEX_avg[0]+ FLEX_avg[1]+ FLEX_avg[2]+ FLEX_avg[3])/4;
  //println("Average_Max_Flex1 = " + Average_Max_Flex1);

  //this is just testing stuff 
 if (printPressure8){
   textSize(22);
  text("FLEX: ", 25,25);
  text("F0: "+ FLEX_avg[0] +" F1: "+ FLEX_avg[1]+" F2: "+ FLEX_avg[2]+" F3: "+ FLEX_avg[3], 25, 45); //Here's the text call.
  text("PRESSURE: " + String.format("%.2f", PRESS_avg[1]) + " psi", 25, 105);
 // text("Max Pressure_1: " +Average_Max_Flex1, 25, 165); //Here's the text call.
  textSize(22);
 }
   if(printFLEXMax){
  textSize(22);
     text("Max FLEX: " + Average_Max_Flex1, 25, 155); //Here's the text call.
     //text("Max FLEX: " + Average_Max_Flex1, 25, 155); //Here's the text call.
  textSize(22);
  }
 
   //mouse poisition will be read from the pressure sensors 
   //since amplification is done individually with hardware they should
   //all work with the same numbers but can be modified later in software
   //could us a ratio of the pressure sensors to determine if one is 
   //more heavily used versus the other...
   //maybe sum them all together and then use a percentage score for the coloring 
  RADIUS_CENTER = c[0].show(PRESS_avg[0], Max_Press); //use to find the center radius
  
int i=0;
float Shift=.12;//this hides the outer elements slightly behind the larger circle 
float angle=TWO_PI/(float)num_flex;

     O[0].Start(RADIUS_CENTER*sin(angle*0)+CenterX,RADIUS_CENTER*cos(angle*0)+CenterY-RADIUS_CENTER*Shift,RADIUS_CENTER);//assigns the start position to the center element 
     O[0].showeven(FLEX_avg[0], Max_Flex[0]);
   
     O[1].Start(RADIUS_CENTER*sin(angle*1)+CenterX-RADIUS_CENTER*Shift,RADIUS_CENTER*cos(angle*1)+CenterY,RADIUS_CENTER);//assigns the start position to the center element 
     O[1].showodd(FLEX_avg[1], Max_Flex[1]);
       
     O[2].Start(RADIUS_CENTER*sin(angle*2)+CenterX,RADIUS_CENTER*cos(angle*2)+CenterY+RADIUS_CENTER*Shift,RADIUS_CENTER);//assigns the start position to the center element 
     O[2].showeven(FLEX_avg[2], Max_Flex[2]);
       
     O[3].Start(RADIUS_CENTER*sin(angle*3)+CenterX+RADIUS_CENTER*Shift,RADIUS_CENTER*cos(angle*3)+CenterY,RADIUS_CENTER);//assigns the start position to the center element 
     O[3].showodd(FLEX_avg[3], Max_Flex[3]);
       
    
  RADIUS_CENTER = c[0].show(PRESS_avg[0], Max_Press);
  
  //convert the pressure to something that actually means something for this next line
  if(RADIUS_CENTER>140){
    fill(255, 0, 0);
  textSize(45);
  text("WARNING: PRESSURE IS " + String.format("%.2f", PRESS_avg[1]) + " PSI!", screen_width/6, screen_height-50); 
  textSize(45);
  }
  int offset = 150;

  if(FLEX_avg[0]> Average_Max_Flex1 + offset | FLEX_avg[1]>Average_Max_Flex1 + offset | FLEX_avg[2]>Average_Max_Flex1 + offset| FLEX[3]>Average_Max_Flex1+offset){
  
  fill(255, 0, 0);
  textSize(45);
  text("WARNING: VARIATION IN STRUCTURE!", 200, 675); 
  textSize(45);
  
  }
  
}

//***********************THIS IS WHERE YOU TIE THE VARIABLES TO THE INPUTS FROM THE SERIAL*************************************************
//MAKE SURE THE NAMES YOU ARE SEARCHING FOR MAKE THE THINGS YOU ARE SENDING SEPERATED BY A SPACE 

void serialEvent(Serial p) 
{
  String incoming = p.readString();
  if (printSerial) {
    println(incoming);
  }
  
  if ((incoming.length() > 8))
  {
    String[] list = split(incoming, " ");
    
        if ( (list.length > 0) && (list[0].equals("Flex:")) )
    {
  
       FLEX[0] = int(list[1]) - Min_Flex[0]; //make sure these are the other foot values
       FLEX[1] = int(list[2]) - Min_Flex[1];
       
       FLEX[2] = int(list[3]) - Min_Flex[2];
       FLEX[3] = int(list[4]) - Min_Flex[3];
       //println("FLEX[0] = " + FLEX[0] + " FLEX[1] = " + FLEX[1]);
    }
     
     if ( (list.length > 0) && (list[0].equals("Pressure:")) )
    {
       PRESS1[3] = int(list[1]);
       PRESS1[0] = int(list[1]) - Min_Press; // Raw pressure data
       //PRESS1[1] = getPressure(PRESS1[0], 1, 0);            // Pressure in PSI
       PRESS1[1] = float(list[3]); 
       //println("Press1[1] = " + String.format("%.2f", PRESS1[1]));
       //PRESS2[2] = int(list[3]);
       //PRESS3[3] = int(list[4]);  
    }
  }
  if (i < 7)
  {
    i++;
    PRESS2[0] += PRESS1[0];
    PRESS2[1] += PRESS1[1];
    PRESS2[2] += PRESS1[2];
    //println("PRESS2[0] = " + PRESS2[0] + " PRESS2[1] = " + PRESS2[1]);
    FLEX1[0] += FLEX[0];
    FLEX1[1] += FLEX[1];
    FLEX1[2] += FLEX[2];
    FLEX1[3] += FLEX[3];
  }
  else if (i == 7)
  {
    i = 0;
    
    PRESS_avg[0] = PRESS2[0]/7;
    PRESS_avg[1] = PRESS2[1]/7;
    PRESS_avg[2] = PRESS2[2]/7;
    FLEX_avg[0] = FLEX1[0]/7;
    //println("FLEX_avg[0] = " + FLEX_avg[0] + " FLEX_avg[1] = " + FLEX_avg[1]);
    FLEX_avg[1] = FLEX1[1]/7;
    FLEX_avg[2] = FLEX1[2]/7;
    FLEX_avg[3] = FLEX1[3]/7;
    //println("PRESS_avg[1] = " + PRESS_avg[1]);
    
    PRESS2[0] = 0;
    PRESS2[1] = 0;
    PRESS2[2] = 0;
    PRESS2[3] = 0;
    FLEX1[0] = 0;
    FLEX1[1] = 0;
    FLEX1[2] = 0;
    FLEX1[3] = 0;
  }
}





//****THIS IS THE SERIAL PORT STUFF, PROBABLY DONT MESS WITH THIS TOO MUCH, MAKE SURE TO CHANGE THE TXT FILE IF USING A DIFF PORT***********
// Set serial port to desired value.
void setSerialPort(String portName) {
  // Close the port if it's currently open.
  if (port != null) {
    port.stop();
  }
  try {
    // Open port.
    port = new Serial(this, portName, 115200);
    port.bufferUntil('\n');
    // Persist port in configuration.
    saveStrings(serialConfigFile, new String[] { portName });
  }
  catch (RuntimeException ex) {
    // Swallow error if port can't be opened, keep port closed.
    port = null; 
  }
}


// UI event handlers

void handlePanelEvents(GPanel panel, GEvent event) {
  // Panel events, do nothing.
}

void handleDropListEvents(GDropList list, GEvent event) { 
  // Drop list events, check if new serial port is selected.
  if (list == serialList) {
    setSerialPort(serialList.getSelectedText()); 
  }
}


//************* IF YOU ADD MORE CHECK BOXES GO DOWN HERE FOR IF THEY ARE SELECTED WHAT DO YOU DO ******
void handleToggleControlEvents(GToggleControl checkbox, GEvent event) { 
  // Checkbox toggle events, check if print events is toggled.
  if (checkbox == printSerialCheckbox) {
    printSerial = printSerialCheckbox.isSelected(); //SETS printSerial to true or false 
  }
    if (checkbox == ShowPressure8Checkbox) {
    printPressure8 = ShowPressure8Checkbox.isSelected(); 
  }
      if (checkbox == ShowPressureMaxCheckbox) {
    printFLEXMax = ShowPressureMaxCheckbox.isSelected(); 
  }
}