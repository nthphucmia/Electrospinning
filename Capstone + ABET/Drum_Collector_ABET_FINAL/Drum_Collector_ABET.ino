/*------------------MOTOR_CONTROL----------------------------------------*/
#include <avr/interrupt.h>

#define SETUP             0
#define RUN               1
#define RUN_UP_DOWN       2
#define EEPROM_SETUP      3
#define RUN_EEPROM        4
#define SAVE_EEPROM       5
#define STOP              6
#define CLEAR             7
#define DEFALT            8

// Direction
#define CWISE             0
#define CCWISE            1

int           state=DEFALT;
const int     pwm_pin_CW=12;
const int     pwm_pin_CCW=13;
const int     H_OFF_pin=14;
unsigned char Direction;
int           rpm_default;
int pulse_speed, rpm_control;
volatile int timer1_flag, rpm=10;      //timer1
volatile int pulse_pullup;            // timer 2
volatile int timer3_flag;             // timer 3

/*---------------------KEYPAD---------------------------------------------*/
#include <Keypad.h>//header for keypad commands enabling
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
String  spd = "";
int Speed; // Variable to convert string to integer

char Setup_But= NO_KEY;
// Define the Keymap for Keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {11, 10, 9, 8};
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = {7, 6, 5, 4};
//  Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/*---------------------------LCD DISPLAY---------------------------------*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F,20,4); 
int j = 15;

/*-------------------------------ENCODER----------------------------------*/
const int phase_a=2   ; //timer 0, xanh luc
const int phase_b=3   ; //determine the way, trang
int CCW = false;

/*-------------------------EEPROM-----------------------------------------*/
#include <EEPROM.h>
int eeprom_value;
int myInteger_1, myInteger;

/*----------------------------------PID CONTROL----------------------------*/
#define Sampling_time 25          // ms
#define inv_Sampling_time 40 
volatile long int Kp=8, Kd=10, Ki=1; // for speed control
volatile long int pPart=0, iPart=0, dPart=0; // PID gains
volatile long int Output;
volatile long int Err, pre_Err;
volatile long int counter_pulse=0;
unsigned int pre_Pulse, Pulse=0;
volatile long time0=0;
volatile long timeF=0;
float time_interval;
float pulse_value;
float encoderSpeed = 0;

void(* resetFunc) (void) = 0;//declare reset function at address 0
/**************************-----------------------------******************************/
void setup()
{
/*-------------------------LCD SETUP-------------------------------------*/
   // lcd initialization
   lcd.init();
   lcd.backlight();
   lcd.clear();
   
   //menu1
   lcd.setCursor(2, 0);
   lcd.print("CAPSTONE PROJECT");
   effect_cursor_CHAY_CHU();
   lcd.setCursor(2, 2);
   lcd.print(" DRUM COLLECTOR ");
   lcd.setCursor(2, 3);
   lcd.print("*ELECTROSPINNING*");
   delay(500);
   
   //cwise and ccwise
   lcd.clear();
   lcd.setCursor(1, 0);
   lcd.print("*CAPSTONE PROJECT*");
   lcd.setCursor(3, 1);
   lcd.print("DRUM COLLECTOR");
   lcd.setCursor(0, 2);
   lcd.print("CWISE    PRESS 1");
   lcd.setCursor(0, 3);
   lcd.print("CCWISE   PRESS 2");
   
   //Pinout Configuration
   pinMode(H_OFF_pin,OUTPUT);
   pinMode(pwm_pin_CW,OUTPUT);
   pinMode(pwm_pin_CCW,OUTPUT);

  /* Encoder */
  pinMode(phase_a, INPUT_PULLUP);
  pinMode(phase_b, INPUT_PULLUP);

  /* Serial initialization */
  Serial.begin(9600);
}

void loop()
{
   Setup_But = kpd.getKey();
/*--------------------------------DIRECTION---------------------------------*/  
   if (Setup_But=='1')
   {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DIRECTION: CWISE");
    lcd.setCursor(0,6);
    lcd.print("3: SET_UP");   
    lcd.setCursor(0,2);
    lcd.print("4: DEFAULT");
    Direction=CWISE;
   }
   
   if (Setup_But=='2')
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIRECTION: CCWISE");
    lcd.setCursor(0,6);
    lcd.print("3: SET_UP");
    lcd.setCursor(0,2);
    lcd.print("4: DEFAULT");
    Direction=CCWISE;
   }
/*---------------------------SETUP OR DEFAULT----------------------------------*/ 
      if(Setup_But=='3')           
    {
      state=SETUP;
   }

   if (Setup_But=='4')
    {
      state= EEPROM_SETUP;
    }
/*---------------------------INC OR DEC-----------------------------------------*/        
   if (Setup_But=='D')      //decrease
    {
      Speed=Speed-20;
      Serial.println("decrease");
      lcd.setCursor(14, 0);
      lcd.print("    ");
      lcd.setCursor(14, 0);
      lcd.print(Speed);
      state=RUN_UP_DOWN;
      //test
      //Serial.println(Speed);
    }

   if (Setup_But=='A')      //increase
    {
      Speed=Speed+20;
      Serial.println("increase");
      lcd.setCursor(14, 0);
      lcd.print("    ");
      lcd.setCursor(14, 0);
      lcd.print(Speed);
      state=RUN_UP_DOWN;
      //test
      //Serial.println(Speed);
    }
/*---------------------------STOP MOTOR-------------------------------------*/
  if (Setup_But=='B')
   {
    state=STOP;
   }
/*---------------------------CLEAR LCD--------------------------------------*/     
   if(Setup_But=='C')          
    {
     //state=CLEAR;   
     resetFunc(); //call reset              
    }  
/*=================================================================================
                                  SWITCHING CASES
===================================================================================*/
  
/*-------------------------------SETUP - 0-----------------------------------------*/ 
switch(state)
{
  case SETUP:   
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DESIRED SPEED:");
    lcd.setCursor(0, 1);
    lcd.print("ACTUAL SPEED:");
    lcd.setCursor(0, 2);
    lcd.print("*:SAVE B:STOP #:RUN");
    lcd.setCursor(0, 3);
    lcd.print("A:UP C:CLEAR D:DOWN");
        
    while(1)
    {
      Setup_But=kpd.getKey();
      if(Setup_But!=NO_KEY)
      {
        lcd.setCursor(14, 0);
        lcd.print("   ");
        lcd.setCursor(j, 0);
        spd += (char)Setup_But; 
        j=j++;
        Speed = spd.toInt();
        lcd.print(Speed);
        effect_cursor();
        
         if(Setup_But=='*')
            {
              state=SAVE_EEPROM;
              break;
            }     

          if(Setup_But=='#')
            {
              state=RUN;
              break;
            }     
        }
      }
  break;
/*-------------------------------RUN MOTOR - 1-----------------------------------------*/
  case RUN:
      digitalWrite(H_OFF_pin,LOW); // RUN MOTOR
      Motor_control_setting(Speed); 
      Encoder_control();
      if(Setup_But=='*')
         {
            state=SAVE_EEPROM;
            break;
          }
       //test
       Serial.println("RUN");     
  break;
/*-------------------------------RUN_UP_DOWN - 2-------------------------------------------*/
  case RUN_UP_DOWN:
       digitalWrite(H_OFF_pin,LOW);            // RUN MOTOR
       Motor_control(Speed);
       Encoder_control();  
       EEPROM_Write(Speed);
       lcd.setCursor(18, 0);
       lcd.print("E");  
//       if(Setup_But=='*')
//          {
//             EEPROM_Write(Speed);
//             lcd.setCursor(18, 0);
//             lcd.print("E");  
//             break;
//          }
  break; 
/*-------------------------------EEPROM DEFAULT - 3-----------------------------------------*/
  case EEPROM_SETUP:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DESIRED SPEED:");
        lcd.setCursor(0, 1);
        lcd.print("ACTUAL SPEED:");
        lcd.setCursor(0, 2);
        lcd.print("#:RUN");
        
        while(1)
        {
          Setup_But=kpd.getKey();
          if(Setup_But!=NO_KEY)
          {
              if(Setup_But=='#')
                {
                  state=RUN_EEPROM;
                  break;
              }     
            }
          }
  break;  
/*-------------------------------EEPROM DEFAULT - 4-----------------------------------------*/
  case RUN_EEPROM:
        digitalWrite(H_OFF_pin,LOW); // RUN MOTOR
        // doc tu eeprom
        eeprom_value=EEPROM_Read();
        lcd.setCursor(14,0);
        lcd.print(eeprom_value);
        Motor_control_setting(eeprom_value); 
        Encoder_control(); 
        //test
        Serial.println(eeprom_value); 
  break;  
/*-------------------------------SAVE EEPROM - 5 ----------------------------------------------*/
  case SAVE_EEPROM:
        lcd.setCursor(18, 0);
        lcd.print("*");
        EEPROM_Write(Speed);   
         if(Setup_But=='#')
              {
                state=RUN;
                break;
              }
          //test    
         Serial.println("SAVE_EEPROM");    
  break;
  /*-------------------------------STOP MOTOR - 6 -----------------------------------------*/
  case STOP:
   digitalWrite(H_OFF_pin,HIGH); // STOP MOTOR 
   //test
   Serial.println("STOP");
  break;
  
/*---------------------------------CLEAR LCD - 7-------------------------------------------*/
  case CLEAR:
        
  break;
 /*-------------------------------DEFAULT - 8 ----------------------------------------------*/ 
  default:
  break;
}
}
/*==============================================================================
                            EEPROM
=================================================================================*/ 
void EEPROM_Write (int rpm_eeprom)
{
EEPROM.update(0, highByte(rpm_eeprom));
EEPROM.update(1, lowByte(rpm_eeprom));
}

int EEPROM_Read (void)
{
int bytehigh=EEPROM.read(0);
int bytelow=EEPROM.read(1);
    myInteger=word(bytehigh, bytelow);
return myInteger;
}

/************************************************************************************
                            TIMER1
Note: every 1s, the motor speed increased 10 rpm
*************************************************************************************/ 
void Motor_control_setting(int rpm_setting)
{
      /*SETUP TIMER1 after 1*/
      cli();                              // stop interrupt
      /* RESET T/C1 */     
      TCCR1A=0;
      TCCR1B=0;
      TIMSK1=0;

      /*Setup T/C 1*/
      TCCR1B=0x04;            // khoi dong timer scale=62500 Hz
      TCNT1=3036;             // sau 1s

      /*Timer/Counter 1 Interrupt(s) initialization */
      TIMSK1=(1<<TOIE1);    
      sei()             ;    // cho phep ngat toan cuc
/*-----------------------------------------------------------------------*/
      if(rpm<=rpm_setting)
      {
        timer1_flag=1;
        Motor_control(rpm);
      }
      else if (rpm>rpm_setting)
      {
        timer1_flag=0;
        lcd.setCursor(19, 0);
        lcd.print("*");
      }
      //test
      Serial.println(rpm);
      Serial.println("-------");
}

ISR (TIMER1_OVF_vect) 
{
  TCNT1=3036;               // sau 1s
  if (timer1_flag==1)
     rpm=rpm+10;
  else if (timer1_flag==0)
     TIMSK1 &= ~_BV(TOIE1); // disable timer1 overflow interrupt
}
/*----------------------------------TIMER2 - CONTROL MOTOR----------------------------------
+ Động cơ Step FULL bước là 1,8 độ -> 1 vòng có 360/1.8 = 200 bước
+ Vận tốc cần: rpm <=> (rpm/60) V/s
-> trong 1s bạn cần xuất 200*(rpm/60) = (10/3)*rpm= rpm/0.3
------------------------------------------------------------------------------------------*/
void Motor_control (unsigned int rpm_control)
{   
      cli();                              // stop interrupt
      
      pulse_speed=(rpm_control/0.3);            //Hz 
      pulse_pullup=62500/pulse_speed;

      /* RESET T/C1 */     
      TCCR3A=0;
      TCCR3B=0;
      TIMSK3=0;

      /*Setup T/C 1*/
      TCCR3B=0x04;            // khoi dong timer scale/256=62 500 Hz
      TCNT3=65535-pulse_pullup;

      /*Timer/Counter 1 Interrupt(s) initialization */
      TIMSK3=(1<<TOIE3);    
      sei()             ;    // cho phep ngat toan cuc

      //test
      Serial.println("frequency");
      Serial.println(pulse_speed/2);
      Serial.println(pulse_pullup);
}


ISR (TIMER3_OVF_vect) 
{
    TCNT3=65535-pulse_pullup;
    switch(Direction)
          {
            case CWISE:
            digitalWrite(pwm_pin_CW, !digitalRead(pwm_pin_CW));
            break;
        
            case CCWISE:
            digitalWrite(pwm_pin_CCW, !digitalRead(pwm_pin_CCW));
            break;
          }  
}
/*=================================================================================
                            PID CONTROL
===================================================================================*/ 
void Encoder_control()
{ 
    attachInterrupt(0, doEncoderA, FALLING);
    time0=millis();
    delay(1000);
    timeF = millis(); 
    time_interval=timeF-time0; 
    if ((time_interval)>=1000)
    {
        detachInterrupt(0);   
        pre_Pulse = counter_pulse;    
        if (CCW == true) {
         pulse_value = (float)(Pulse-pre_Pulse);
        }
        else {
              pulse_value = (float)(pre_Pulse-Pulse);
            }
        encoderSpeed=(float)(((pulse_value)/400)/time_interval)*60000;
        delay(100);
        Pulse = pre_Pulse;
        time0=millis();
        counter_pulse=0;
        Pulse=0; 
        // DISPLAY THE ENCODER READING
        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13, 1);
        lcd.print(encoderSpeed);
        delay(100);   
        //test
        //Serial.println(pulse_value);    
        //Serial.println(time_interval);
        //Serial.println(encoderSpeed);
    }
  /*--------------------Motor_Speed_PID-------------------------------------- */ 
//  Err=int(rpm_default-encoderSpeed);           // tinh error    
//  //  PID parts
//  pPart=Kp*Err;
//  dPart=Kd*(Err-pre_Err)*inv_Sampling_time;
//  iPart=Ki*Sampling_time*(Err+pre_Err)/1000 ;// transform ms into s
//  Output+=pPart+dPart+iPart;
//  
//  // Speed control by PID
//  Motor_control_setting(Output);
//  pre_Err=Err;      //luu lai gia tri Err
}

void doEncoderA()
{
  if(digitalRead(phase_a)==HIGH)
  {
    if(digitalRead(phase_b)==LOW)
    {
      CCW=false;}
    else
    {CCW=
    true;}
  }
  else 
  {
    if(digitalRead(phase_b)==LOW)
    {CCW=true;}
    else
    {CCW=false;}
  } 

  if(CCW==true){
    counter_pulse--; 
    //Serial.println("CCW");  
  }
  else{
    counter_pulse++;
    //Serial.println("CW");  
  }
}
/*=================================================================================
                            CURSOR EFFECT
===================================================================================*/ 
void effect_cursor_CHAY_CHU()
{
 for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // Cuộn 1 vị trí sang trái:
    lcd.scrollDisplayLeft();
    delay(50);
  }
 
  // Cuộn 29 vị trí sang phải:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // Cuộn 1 vị trí sang phải:
    lcd.scrollDisplayRight();
    delay(50);
  }
 
  // Cuộn 16 vị trí sang trái
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // Cuộn 1 vị trí sang trái:
    lcd.scrollDisplayLeft();
    delay(50);
  }
  delay(50);
}

void effect_cursor()
{
    lcd.noCursor();
    delay(500);
    lcd.cursor();
    delay(500);
}
