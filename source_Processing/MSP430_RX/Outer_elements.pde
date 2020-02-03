class Outer {
  float x;
  float y;
  float r;
  float Radius;
  float Value;
  float red=255;
 float green=0;
 float blue=0;
 int red_scaler=20;
 int blue_scaler=20;
 int green_scaler=20;
 int SCALE_FACTOR=0;
 
 
 //these are used to scale the outside cirles, the rest is basically color 
 //modify the shift in the main code along with these terms for best results 
 //you actually need to modify these values down below in the functions, 
 //these are good values to use though
 float Stretch = 5.5; //makes the ellipses skinnier
 float Expand =2;//makes the ellipses fater
  
  void Start(float x_, float y_,float Radius_){
 Radius=Radius_;
  x=x_; //these will stay constant 
  y=y_; //this will stay constant as it is the location 
  r=10; //this value will change with pressure 

  //start of the if statements for the color 
  }
  
  void showodd(float Value_, float MAX_Pressure) {
    
  Stretch =1+ 4.5*Radius/190; //makes the ellipses skinnier
  Expand =1+Radius/190;//makes the ellipses fater

  // stroke(255);
   
   //CAN CHANGE THE PERCENTAGE TO ACTUAL VALUES THAT YOU ARE LOOKING FOR  
if (Value_>MAX_Pressure){
SCALE_FACTOR=19;
 }else if(Value_>MAX_Pressure*.95){
SCALE_FACTOR=18;
  } else if(Value_>MAX_Pressure*.90){
SCALE_FACTOR=17; 
  } else if(Value_>MAX_Pressure*.85){
SCALE_FACTOR=16;
  }else if(Value_>MAX_Pressure*.8){
SCALE_FACTOR=15;
  }else if(Value_>MAX_Pressure*.75){
SCALE_FACTOR=14;
  } else if(Value_>MAX_Pressure*.70){
SCALE_FACTOR=13;
  } else if(Value_>MAX_Pressure*.65){
SCALE_FACTOR=12; 
  }else if(Value_>MAX_Pressure*.6){
SCALE_FACTOR=11;
  }else if(Value_>MAX_Pressure*.5){
SCALE_FACTOR=10;
  }else if(Value_>MAX_Pressure*.45){
SCALE_FACTOR=9;
  }else if(Value_>MAX_Pressure*.40){
SCALE_FACTOR=8;     
  }else if(Value_>MAX_Pressure*.35){
SCALE_FACTOR=7;
  }else if(Value_>MAX_Pressure*.30){
SCALE_FACTOR=6;
  } else if(Value_>MAX_Pressure*.25){
SCALE_FACTOR=5;
  }else if(Value_>MAX_Pressure*.20){
SCALE_FACTOR=4;
  }  else if(Value_>MAX_Pressure*.15){
SCALE_FACTOR=3;
  }else if(Value_>MAX_Pressure*.10){
SCALE_FACTOR=2;
  }else if(Value_>MAX_Pressure*.05){
SCALE_FACTOR=1;
  }else{
SCALE_FACTOR=0;
  }

// }//end while statement 
   
   
  if( Value_>0){  
    
  //SETS THE INITIAL VALUES, SCALE FACTOR DECREASE WITH VALUE   
  
  r    = 0 + SCALE_FACTOR*3;
 
  red  = 0 + SCALE_FACTOR*12;  
  blue = 255 - SCALE_FACTOR*10;
  green= 255 - SCALE_FACTOR*12;  

  if (SCALE_FACTOR <5){ 
    
  for(int n=int(r); n> 0 ; n -=3){
  //LEAST AMOUNT OF RED, INCREASE GREEN, INCREASE BLUE 
  red= red-5;
  green= green+10;
  blue=blue+5;    
  r=n;
 // noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
  fill(red, green, blue);
   ellipse(x,y,r*Expand,r*Stretch) ; //NOTICE THAT THE R IS SWITCHED 
    }//ends for 
    
  } else if (SCALE_FACTOR<10){
    
  for(int n=int(r); n> 0 ; n -=3){
  //MAKE LESS RED MORE GREEN MORE BLUE  
  red= red+1;
  green= green+5;
  blue=blue-9;    
  r=n;
 // noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
  fill(red, green, blue);
   ellipse(x,y,r*Expand,r*Stretch) ;
    }//ends for 
    
  }
   else if (SCALE_FACTOR<15){
    
  for(int n=int(r); n> 0 ; n -=3){
  //INCREASE RED AND GREEN FOR A YELLOWISH COLOR
  red= red+10;
  green= green+9;
  blue=blue+1;    
  r=n;
  //noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
  fill(red, green, blue);
   ellipse(x,y,r*Expand,r*Stretch);
    }//ends for 
    
  }
 else{
    
  for(int n=int(r); n> 0 ; n -=3){
  
  //INCREASE RED   
  red= red+10;
  green= green-15;
  blue=blue-15;    
  r=n;
  //noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
  fill(red, green, blue);
   ellipse(x,y,r*Expand,r*Stretch) ;
    }//ends for 
    
  }
    
    
  }//ends if greater than zero 
  
}
  void showeven(float Value_, float MAX_Pressure) {
  Stretch =1+ 4.5*Radius/190; //makes the ellipses skinnier
  Expand =1+Radius/190;//makes the ellipses fater

    //  stroke(255);
    //these are used to scale the outside cirles, the rest is basically color 
 //modify the shift in the main code along with these terms for best results 

   //CAN CHANGE THE PERCENTAGE TO ACTUAL VALUES THAT YOU ARE LOOKING FOR  
if (Value_>MAX_Pressure){
SCALE_FACTOR=19;
 }else if(Value_>MAX_Pressure*.95){
SCALE_FACTOR=18;
  } else if(Value_>MAX_Pressure*.90){
SCALE_FACTOR=17; 
  } else if(Value_>MAX_Pressure*.85){
SCALE_FACTOR=16;
  }else if(Value_>MAX_Pressure*.8){
SCALE_FACTOR=15;
  }else if(Value_>MAX_Pressure*.75){
SCALE_FACTOR=14;
  } else if(Value_>MAX_Pressure*.70){
SCALE_FACTOR=13;
  } else if(Value_>MAX_Pressure*.65){
SCALE_FACTOR=12; 
  }else if(Value_>MAX_Pressure*.6){
SCALE_FACTOR=11;
  }else if(Value_>MAX_Pressure*.5){
SCALE_FACTOR=10;
  }else if(Value_>MAX_Pressure*.45){
SCALE_FACTOR=9;
  }else if(Value_>MAX_Pressure*.40){
SCALE_FACTOR=8;     
  }else if(Value_>MAX_Pressure*.35){
SCALE_FACTOR=7;
  }else if(Value_>MAX_Pressure*.30){
SCALE_FACTOR=6;
  } else if(Value_>MAX_Pressure*.25){
SCALE_FACTOR=5;
  }else if(Value_>MAX_Pressure*.20){
SCALE_FACTOR=4;
  }  else if(Value_>MAX_Pressure*.15){
SCALE_FACTOR=3;
  }else if(Value_>MAX_Pressure*.10){
SCALE_FACTOR=2;
  }else if(Value_>MAX_Pressure*.05){
SCALE_FACTOR=1;
  }else{
SCALE_FACTOR=0;
  }

// }//end while statement 
   
   
  if( Value_>0){  
    
  //SETS THE INITIAL VALUES, SCALE FACTOR DECREASE WITH VALUE   
  
  r    = 0 + SCALE_FACTOR*3;

  red  = 0 + SCALE_FACTOR*12;  
  blue = 255 - SCALE_FACTOR*10;
  green= 255 - SCALE_FACTOR*12;  

  if (SCALE_FACTOR <5){ 
    
  for(int n=int(r); n> 0 ; n -=3){
  //LEAST AMOUNT OF RED, INCREASE GREEN, INCREASE BLUE 
  red= red-5;
  green= green+10;
  blue=blue+5;    
  r=n;
 // noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
   fill(red, green, blue);
   ellipse(x,y,r*Stretch,r*Expand) ;
    }//ends for 
    
  } else if (SCALE_FACTOR<10){
    
  for(int n=int(r); n> 0 ; n -=3){
  //MAKE LESS RED MORE GREEN MORE BLUE  
  red= red+1;
  green= green+5;
  blue=blue-9;    
  r=n;
  //noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
   fill(red, green, blue);
   ellipse(x,y,r*Stretch,r*Expand) ;
    }//ends for 
    
  }
   else if (SCALE_FACTOR<15){
    
  for(int n=int(r); n> 0 ; n -=3){
  //INCREASE RED AND GREEN FOR A YELLOWISH COLOR
  red= red+10;
  green= green+9;
  blue=blue+1;    
  r=n;
 // noFill();
  stroke(red,green,blue);
  strokeWeight(3.2);
  fill(red, green, blue);
   ellipse(x,y,r*Stretch,r*Expand) ;
    }//ends for 
    
  }
 else{
    
  for(int n=int(r); n> 0 ; n -=3){
  
  //INCREASE RED   
  red= red+10;
  green= green-15;
  blue=blue-15;    
  r=n;
  //noFill();
  //noStroke();
  stroke(red,green,blue);
  strokeWeight(3.2);
  fill(red, green, blue);
   ellipse(x,y,r*Stretch,r*Expand) ;
    }//ends for 
    
  }
    
    
  }//ends if greater than zero 
  
}
  }