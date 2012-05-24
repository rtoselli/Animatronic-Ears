#include <Servo.h> 
const int buttonPin = 2;     // the number of the pushbutton pin
 
Servo servoEsquerdaPan;   
Servo servoDireitaPan; 
Servo servoEsquerdaTilt; 
Servo servoDireitaTilt; 

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonPresses = 0;

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

unsigned long totalMillis = 0;
unsigned long totalSecs = 0;

int pos;

// Posições de cada animação para os servos
// pan esquerda, pan direita, tilt esquerda, tilt direita
int movSet_anger[]          = {0,0,0,0};
int movSet_sad[]            = {0,180,30,160};
int movSet_surprise[]       = {160,20,110,70};
int movSet_happy[]          = {110,70,40,150};
int movSet_tickle[]         = {170,80,90,150};
int movSet_move_right[]     = {0,0,0,0};
int movSet_move_left[]      = {0,0,0,0};
int movSet_roam[]           = {140,60,110,75};
int movSet_default[]        = {170,10,90,90};
int movSet_power_on[]       = {150,20,60,110};


// Controle da animação 
const int EAR_IDLE      = 0;
const int EAR_ANGRY     = 2;
const int EAR_SAD       = 3;
const int EAR_HAPPY     = 4;
const int EAR_TICKLE    = 5;
const int EAR_ROAM      = 6;
const int EAR_MOVELEFT  = 7;
const int EAR_MOVERIGHT = 8;
const int EAR_SURPRISE  = 9;

int EAR_STATE = EAR_IDLE;

const int STATE_IDLE = 0;
const int EXECUTE = 1;

int ACTION_STATE = STATE_IDLE;

unsigned long lastIdle;
unsigned long idleTime;

void setup() 
{
  attachInterrupt(0,readButtonState,CHANGE);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  servoEsquerdaPan.attach(9);  
  servoDireitaPan.attach(10); 
  servoEsquerdaTilt.attach(11);  
  servoDireitaTilt.attach(12);  
  powerOnAnimation();
  lastIdle = millis(); 


} 
 
 
void loop() 
{ 
   
   if ( EAR_STATE == EAR_IDLE && ACTION_STATE == EXECUTE ){
       
     
       if ( buttonPresses == 1)
       {
          animate(EAR_SURPRISE);

       }
       else if ( buttonPresses == 2 )
       {
           animate(EAR_SAD);         
       } 
       else if ( buttonPresses == 3)
       {
           animate(EAR_HAPPY);         
       }
       else if ( buttonPresses == 4)
       {
          animate(EAR_ANGRY);
       }
       else if ( buttonPresses == 5)
       {
          animate(EAR_MOVERIGHT);
       }
       else if ( buttonPresses == 6)
       {
          animate(EAR_MOVELEFT);
       }
      
       buttonPresses = 0;
       lastIdle = millis();
    
   } else if ( EAR_STATE == EAR_IDLE ) {
     
       idleTime = millis() - lastIdle ;  
      
       
       if (idleTime > 6000)
       {
            
             // Executa acao randomica
             switch (random(1,3))
             {
               case 1:
                       Serial.println("Tikcle");
                       animate(EAR_TICKLE);
                       break;
                       
               case 2:
                       Serial.println("roam");
                       animate(EAR_ROAM);
                       break;
               default:
                        animate(EAR_HAPPY);
                       break;
               
             }
              lastIdle = millis();
              idleTime = 0;
       
       } 
   }
   


} 


void animate(int state)
{
      EAR_STATE = state;
      Serial.println("Executando Acao");
      int j = 0; 
      int l = 0;
      int m = 0;
      switch(state)
      {
          
           case EAR_ANGRY:
                           
                           break;
                           
           case EAR_SAD:
                              servoEsquerdaPan.write(movSet_sad[0]);
                              servoDireitaPan.write(movSet_sad[1]);
                              servoEsquerdaTilt.write(movSet_sad[2]);
                              servoDireitaTilt.write(movSet_sad[3]);
                              
                              delay(1500);
                           break;
                           
           case EAR_HAPPY:
                               // {120,60,60,130};
                               
                               // D:{170,10,90,90};
                              j = 10;
                              l = 90;
                              m = 90;
                            for ( int i = movSet_default[0]; i > movSet_happy[0]; i--){
                              
                              servoEsquerdaPan.write(i);
                              servoDireitaPan.write(j++);
                           
                              if (m > movSet_happy[2]) 
                                 servoEsquerdaTilt.write(m--);
                                 
                              if (l < movSet_happy[3])
                                servoDireitaTilt.write(l++); 
                             
                              delay(45);
                              
                            }
                            delay(2000);
                            
                           break;
                           
           case EAR_TICKLE:
                            servoEsquerdaPan.write(movSet_tickle[0]);
                            servoDireitaPan.write(movSet_tickle[1]);
                            
                            for (int i = 0; i < 2 ; i++)
                           {
                             
                             servoDireitaTilt.write(movSet_tickle[3]); 
                             delay(120);
                             servoDireitaTilt.write(movSet_default[3]);
                             delay(150);
                             
                           }
                           
                             servoDireitaPan.write(movSet_default[1]);
                             delay(150);
                             servoDireitaTilt.write(movSet_tickle[3] - 30); 
                             delay(150);
                             servoDireitaTilt.write(movSet_default[3]);
                             delay(150);
                         
                            
                           break;
                           
           case EAR_ROAM:
                            servoEsquerdaPan.write(movSet_roam[0]);
                           
                             
                           for (int i = movSet_default[2]; i < movSet_roam[2] ; i++)
                           {
                             servoEsquerdaTilt.write(i);
                             delay(40);
                           }
                            
                            servoEsquerdaPan.write(movSet_default[0]);
                            servoDireitaPan.write(movSet_roam[1]);  
                            
                           for (int i = movSet_default[3]; i > movSet_roam[3] ; i--)
                           {
                             servoDireitaTilt.write(i);    
                             delay(40);
                           }
                           
                          
                          
                                                  
                           break;
                           
           case EAR_MOVELEFT:
                           break;
                           
           case EAR_MOVERIGHT:
           
                           break;
                           
           case EAR_SURPRISE:
                                   servoEsquerdaPan.write(movSet_surprise[0]);
                                   servoDireitaPan.write(movSet_surprise[1]);
                                   delay(30);
                                   servoEsquerdaTilt.write(movSet_surprise[2]);
                                   servoDireitaTilt.write(movSet_surprise[3]);
                                   
                                   delay(1000);
                                   
             
                           break;
      }
      
      EAR_STATE = EAR_IDLE;
      ACTION_STATE = STATE_IDLE;
      setDefaultServoPositions();
      
}

void powerOnAnimation(){
 
   setDefaultServoPositions();
   
   servoEsquerdaPan.write(movSet_power_on[0]);
   
    for (int i = 0; i < 2; i++ )
    {
      servoEsquerdaTilt.write(movSet_power_on[2]);
      delay(200);
      servoEsquerdaTilt.write(movSet_default[2]);
      delay(200);
    }
   servoDireitaPan.write(movSet_power_on[1]);
   
   for (int i = 0; i < 2; i++)
    {
     servoDireitaTilt.write(movSet_power_on[3]);
      delay(200);
     servoDireitaTilt.write(movSet_default[3]);
      delay(200);
    }
   setDefaultServoPositions();
   
   
}

void setDefaultServoPositions(){
   
   servoEsquerdaPan.write(movSet_default[0]);
   servoDireitaPan.write(movSet_default[1]);
   servoEsquerdaTilt.write(movSet_default[2]);
   servoDireitaTilt.write(movSet_default[3]);
   delay(200);
   
   
}

void readButtonState()
{
  
   if ( EAR_STATE != EAR_IDLE )   
     return ;
     
    lastIdle = millis();
    idleTime = 0;
    
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  
  if ( (millis() - lastDebounceTime) >= 1000)
  {
    
     Serial.println("Execute action");
     ACTION_STATE = EXECUTE;
     
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay && (millis() - lastDebounceTime) < 1000 ) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
    buttonPresses++;
   
     Serial.println("press");
  }

 

  
}
