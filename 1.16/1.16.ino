  //============GUI INTERFACE
// DATE: 26/4/2018
// Write by: Viet Anh
// Goal: Fix some bug from stop button, and --> and <-- button



#include <UTFT.h> 
#include <URTouch.h>
#define stp 8
#define dir 9





//==== Creating Objects
UTFT myGLCD(ILI9341_16, 38, 39, 40, 41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch( 6, 5, 4, 3, 2);

//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
char currentPage, selectedUnit;
int x, y,hienthi,hienthi1,hienthi2,hienthi3,l=1,k=2800;
int dem,dem1,dem3,dem4,dem5,dem6,dem7;
int buttonStatus;
int button = 15;
float tang,tang2,tang1,lap,m,n;
int flag=1;
int x1;
int biendem;
    
//===Bien toc do, do dai=======
int distance;
int speeds;
//==========================
void blinkled (void);
void blinkledF (void);
int stop_button (void);
 void press_button (void);
 void drawHomeScreen();
 void press_button (void);
void motor_auto (int a,int b);
  void motor_init (void);
  void StepForwardDefault(int nhap);

void setup() {
  //==LED INDICATOR====
//=======Initial setup
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
 //=====Defining Pin Modes

  drawHomeScreen();  // Draws the Home Screen
  currentPage = '0'; // Indicates that we are at Home Screen
  selectedUnit = '0'; // Indicates the selected unit for the first example, cms or inches
  //=====Serial
  //=====Motor========
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  
  pinMode(11,INPUT);
  pinMode(10,INPUT);
  pinMode(button, INPUT);  //Cài đặt chân D2 ở trạng thái đọc dữ liệu


  //=======================
 motor_init();
//StepForwardDefault();
}
void loop() { 

         if (myTouch.dataAvailable()){
                  myTouch.read();
                  x = myTouch.getX();// X coordinate where the screen has been pressed
                  y=myTouch.getY(); // Y coordinates where the screen has been pressed
                  
             press_button ();  
             // If we press the Scan Button------------ 
             if ((x>=15) && (x<=65) && (y>=100) && (y<=130)) {
              flag = 1;

              myGLCD.setColor(255,255,255); // Sets green color
              myGLCD.drawRoundRect (15, 65, 100, 130);
              delay(50);
              myGLCD.setColor(0,0,0); // Sets green color
              myGLCD.drawRoundRect (15, 65, 100, 130);
              delay(50);
              myGLCD.setColor(255,255,255); // Sets green color
              myGLCD.drawRoundRect (15, 65, 100, 130);
              delay(50);
             // StepForwardDefault();
             
   
                   for ( dem7 = 0; dem7 < 50; dem7++){

                        for ( lap = 0; lap < 50 && flag ==1; lap++){
                        

                       motor_auto(distance,speeds); } 
                        
                      for (dem5 = 0; dem5  < 1000 &&flag == 1; dem5++){
                              biendem = biendem + 1;
                              if (digitalRead(11) == LOW) break; 
                                  digitalWrite(13, HIGH);
                                  delayMicroseconds(3500); // Code test with CWW Motor rotate
                                  digitalWrite(13, LOW);
                                   delayMicroseconds(3500); 
                               
                      }
                                   StepForwardDefault(10000);
                                   
                                    
                       for (dem6 = 0; dem6  < biendem &&flag == 1; dem6++){
                                  digitalWrite(12, HIGH);
                                  delayMicroseconds(3500); // Code test with CWW Motor rotate
                                  digitalWrite(12, LOW);
                                   delayMicroseconds(3500); 
                               
                       }
                       biendem = 0;
   
                   }
             }
                                    
                                         
                                                     
                       
             
      }
      
}
      
  

             
        
//=============================================================================
//==============================================================================
// drawHomeScreen - Custom Function
void drawHomeScreen() {
  // Title
  int n,m,lap;
  myGLCD.setBrightness(16);
  myGLCD.fillScr(192,192,192);
  
  myGLCD.setColor(0, 0, 0); // Sets color to gray
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.setBackColor(192, 192, 192); //Set background color to black
  myGLCD.print("Nozzle Settings", CENTER, 10); // Prints the string on the screen


  //===BME LOGO========
  myGLCD.setColor(8, 46, 84); // Sets color to BLUE 
  myGLCD.drawCircle(25,20, 13);
  myGLCD.drawCircle(25,20, 12);
  myGLCD.drawLine(20,20, 25,15);
    myGLCD.drawLine(21,21, 26,16);
  myGLCD.drawLine(25,15, 27,25);
    myGLCD.drawLine(26,16, 28,26);
  myGLCD.drawLine(27,25, 30,15);
  myGLCD.setColor(255, 0,0); // Sets color to BLUE 
  myGLCD.fillCircle(25,12,3);
  
  

  //Virtual Slider

  myGLCD.setColor(0, 0, 0); // Sets color to gray
  myGLCD.setFont(SmallFont); // Sets font to big
  myGLCD.setBackColor(192, 192,192 ); //Set background color to black
  myGLCD.print("0", CENTER, 35);
  myGLCD.print("-50", RIGHT, 35);  
  myGLCD.print(" 50", LEFT, 35);
  myGLCD.print("25", 80, 35);
  myGLCD.print("-25", 220, 35);

//=======Making dot 
//----------------------------------------------------------
  myGLCD.setColor(0,0, 0); //Set background color to black
  myGLCD.fillCircle(35,40, 2);
  myGLCD.fillCircle(50,40, 2);
  myGLCD.fillCircle(70,40, 2);

  myGLCD.fillCircle(110,40, 2);
  myGLCD.fillCircle(130,40, 2);
  myGLCD.fillCircle(150,40, 2);

  myGLCD.fillCircle(170,40, 2);
  myGLCD.fillCircle(190,40, 2);
  myGLCD.fillCircle(210,40, 2);

  
  //myGLCD.fillCircle(240,40, 2);
  myGLCD.fillCircle(260,40, 2);
  myGLCD.fillCircle(280,40, 2);
//---------------------------------------------------------
//-----Scan button
//--------------------------------------------------------
  myGLCD.setColor(220, 220,220); // Sets green color
  myGLCD.fillRoundRect (15, 65, 100, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (15, 65, 100, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(220, 220, 220); // Sets the background color of the area where the text will be printed to green, same as the button
   myGLCD.setColor(0, 0, 0); // Sets color to white
  myGLCD.print("SCAN",25, 90); // Prints the string


  
//------STOP button--------------------------------------
//--------------------------------------------------------
  myGLCD.setColor(255, 0, 0); // Sets green color
  myGLCD.fillRoundRect (15, 150, 100, 215); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (15, 150, 100, 215); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 0,0); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("STOP", 25, 175); // Prints the string

//-----------------------------------------------------------
//-----------------------------------------------------------

//-------Position button
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (110, 80, 160, 120); // Draws filled rounded rectangle
  //------------------------------------------------
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (165, 90, 235, 120);
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("<-",170 ,95); // Prints the string

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (240, 90, 315, 120);
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("->",245 ,95); // Prints the string

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Position [mm]", 165, 75); // Prints the string

//-------Distance button
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (110, 135, 160, 175); // Draws filled rounded rectangle
  //-------------------------------------------------------------------------
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (165, 145, 235, 175);
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.print(" + ",170 ,155); // Prints the string

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (240, 145, 315, 175);
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print(" -  ",245 ,155); // Prints the string

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Distance [mm]", 165, 130); // Prints the string

//-------Speed button--------------------------------------
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (110, 185, 160, 225); // Draws filled rounded rectangle

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (165, 195, 235, 225);
  myGLCD.setColor(0,0,0); // Sets green color
   myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print(" + ",170,200); // Prints the string

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.drawRoundRect (240, 195, 315, 225);
  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print(" -  ",245 ,200); // Prints the string

  myGLCD.setColor(0,0,0); // Sets green color
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(192,192,192 ); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Speed [mm/s]", 165, 180); // Prints the string

  //----------------------------------------------

}
  



 void press_button (void){
   //=======If we press the STOP Control Button 
              if ((x>=15) && (x<=150) && (y>=0) && (y<=65)) {
              myGLCD.setColor(255, 255, 255); // Sets color to white
              myGLCD.drawRoundRect (15, 150, 100, 215);
              delay(50);
              myGLCD.setColor(0,0,0); // Sets green color
              myGLCD.drawRoundRect (15, 150, 100, 215);
              delay(50);
              myGLCD.setColor(255,255,255); // Sets green color
              myGLCD.drawRoundRect (15, 150, 100, 215);
              delay(50);
              }

              //========// If we press "<--" Button
              if ((x>=160) && (x<=235) &&(y>=90) && (y<=125)) {
                
                  flag = 1;

                 tang++;
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 90, 235, 120);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 90, 235, 120);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 90, 235, 120);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(tang,0, 120, 90); // Prints the string
                 
                 for (dem3 = 0; dem3 < 50 &&flag==1 ; dem3++){

                      digitalWrite(13, HIGH);
                      delayMicroseconds(1000); // Code test with CWW Motor rotate
                      digitalWrite(13, LOW);
                      delayMicroseconds(1000);
                      flag = stop_button();}
                      }
                   //========// If we press "-->" Button
              if ((x>=240) && (x<=315) &&(y>=110) && (y<=125)) {
                
                      flag = 1;
                
                 tang--;
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 90, 315, 120);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 90, 315, 120);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 90, 315, 120);
                 delay(50);

                  
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(tang,0, 120, 90); // Prints the string

                    
                 for (dem3 = 0; dem3 < 50 &&flag==1 ; dem3++){

                
                      digitalWrite(12, HIGH);
                      delayMicroseconds(1000); // Code test with CWW Motor rotate
                      digitalWrite(12, LOW);
                      delayMicroseconds(1000);
                      flag = stop_button();}

                  }

                 //==========================================
                  //========// If we press "+ Distance" Button
              if ((x>=165) && (x<=235) &&(y>=50) && (y<=80)) {

                 //tang1++;
                    distance = distance + 175;
                    
                 hienthi = (distance/7);
                  distance*7;
                 myGLCD.setColor(0,0,0); // Sets green color
                myGLCD.drawRoundRect (165, 145, 235, 175);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                myGLCD.drawRoundRect (165, 145, 235, 175);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
               myGLCD.drawRoundRect (165, 145, 235, 175);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(hienthi,0, 120, 145); // Prints the string

              

               
                 }
                   //========// If we press "- Distance" Button
              if ((x>=240) && (x<=315) &&(y>=50) && (y<=80)) {
                
              
                distance = distance - 175;
                if (distance < 0) distance = 0;
                hienthi1 = distance/7;
                distance*7;
                
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 145, 315, 175);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 145, 315, 175);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 145, 315, 175);
                 delay(50);

                  
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(hienthi1,0, 120, 145); // Prints the string
                  
                
              
            

                  }


                   //==========================================
                  //========// If we press "+ Speed" Button
              if ((x>=165) && (x<=235) &&(y>=0) && (y<=50)) {
                
    
                if (hienthi >= 41) hienthi = 0;
                    hienthi = hienthi + 5;

                switch (hienthi){
                  
                         
                case 5:
             
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(5,0, 120, 195); // Prints the string

                 speeds = 14000;
                 break;

                 case 10:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(10,0, 120, 195); // Prints the string

                 speeds = 7000;
                 break;
                 case 15:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(15,0, 120, 195); // Prints the string

                 speeds = 4672;
                 break;
                 case 20:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(20,0, 120, 195); // Prints the string

                 speeds = 3500;
                 break;

                 case 25:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(25,0, 120, 195); // Prints the string

                 speeds = 2805;
                 break;

                 case 30:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(30,0, 120, 195); // Prints the string

                 speeds = 2345;
                 break;

                 case 35:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(35,0, 120, 195); // Prints the string

                 speeds = 2005;
                 break;

                 case 40:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(40,0, 120, 195); // Prints the string

                 speeds = 1760;
                 break;
                 default: 
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (165, 195, 235, 225);
                 delay(50);
                 
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.print("Fast", 120, 195); // Prints the string
                 speeds = 700;

                 break;

               
                 }
              }
                   //========// If we press "- Speed" Button
              if ((x>=240) && (x<=315) &&(y>=0) && (y<=50)) {
                
                hienthi = hienthi -5;
                if (hienthi < 0) hienthi = 0;
              

                switch (hienthi){
                 
                case 5:
             
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(5,0, 120, 195); // Prints the string
                 speeds = 14000;
                 break;

                 case 10:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(10,0, 120, 195); // Prints the string
                 speeds = 7000;
                 break;
                 
                 case 15:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(15,0, 120, 195); // Prints the string
                 speeds = 4672;
                 break;
                 
                 case 20:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(20,0, 120, 195); // Prints the string
                 speeds = 3500;
                 break;

                 case 25:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(25,0, 120, 195); // Prints the string
                 speeds = 2805;
                 break;

                 case 30:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(30,0, 120, 195); // Prints the string
                 speeds = 2345;
                 break;

                 case 35:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(35,0, 120, 195); // Prints the string
                 speeds = 2005;
                 break;

                 case 40:
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.printNumF(40,0, 120, 195); // Prints the string
                 speeds = 1760;
                 break;






                 
                 default:  myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(255,255,255); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.drawRoundRect (240, 195, 315, 225);
                 delay(50);
                 
                 myGLCD.setColor(0,0,0); // Sets green color
                 myGLCD.setFont(SmallFont); // Sets the font to big
                 myGLCD.setBackColor(192,192,192); // Sets the background color of the area where the text will be printed to green, same as the button
                 myGLCD.print("Slow", 120, 195); // Prints the string
                 speeds = 14000;

                 break;


                  }
     
                }
              
 }

int stop_button (void){
  
  
    if (myTouch.dataAvailable()){
            myTouch.read();
            x = myTouch.getX();// X coordinate where the screen has been pressed
            y=myTouch.getY(); // Y coordinates where the screen has been pressed
          
                    if ((x>=15) && (x<=150) && (y>=0) && (y<=65)) {
                            myGLCD.setColor(255, 255, 255); // Sets color to white
                            myGLCD.drawRoundRect (15, 150, 100, 215);
                            delay(50);
                            myGLCD.setColor(0,0,0); // Sets green color
                            myGLCD.drawRoundRect (15, 150, 100, 215);
                            delay(50);
                            myGLCD.setColor(255,255,255); // Sets green color
                            myGLCD.drawRoundRect (15, 150, 100, 215);
                            return 0;
              
                        }
                                     
                     }
             return 1;
 }

void motor_auto (int a,int b)
{
        

                      for (dem = 0; dem < a &&flag==1 ; dem++){
                      digitalWrite(13, HIGH);
                      delayMicroseconds(b); // Code test with CWW Motor rotate
                      digitalWrite(13, LOW);
                      delayMicroseconds(b);

                      flag = stop_button();}
//                      
                      for (dem = a; dem > 0 && flag==1  ; dem--){
                      digitalWrite(12, HIGH);
                      delayMicroseconds(b); // Code test with CWW Motor rotate
                      digitalWrite(12, LOW);
                      delayMicroseconds(b);

                      flag = stop_button();
                      }

}

void motor_init (void)
{
 
  for (dem4 = 0; dem4  < 7000 &&flag ==1; dem4++){
              
                  
                      if (digitalRead(10) == LOW) break; 
                      digitalWrite(12, HIGH);
                      delayMicroseconds(4500); // Code test with CWW Motor rotate
                      digitalWrite(12, LOW);
                      delayMicroseconds(4500);
                       flag = stop_button();}
                        
   for (dem5 = 0; dem5  < 650 &&flag == 1; dem5++){
                      digitalWrite(13, HIGH);
                      delayMicroseconds(3500); // Code test with CWW Motor rotate
                      digitalWrite(13, LOW);
                      delayMicroseconds(3500); 
                      flag = stop_button();}                  
}
void StepForwardDefault(int nhap)
{
  for(x1= 0; x1<nhap; x1++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }

}
void cleanning ()
{
     if (digitalRead(20) == LOW){
      
      StepForwardDefault(10);
     }
     }






