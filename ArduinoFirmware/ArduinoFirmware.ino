// ERM

const int erm1_f = 2;
const int erm1_r = 3; 
const int erm2_f = 5; 
const int erm2_r = 6; 
const int erm3_f = 7; 
const int erm3_r = 8; 
const int erm4_f = 9; 
const int erm4_r = 10; 

bool ermOn[4] = {false, false, false, false};
bool ermBurst[4] = {false, false, false, false};
unsigned long lraBurstStartTime[4] = {0, 0, 0, 0};

int ermBumpRunMS[4] = {100, 100, 100, 100}; // bump 진동 시간(ms)
int ermBumpStopMS[4] = {0, 0, 0, 0};    // bump 쉬는 시간(ms)
unsigned long ermRunStartTime[4] = {0, 0, 0, 0};  // erm 틀기 시작한 시간

int PWMpercent = 80;  // (%)

//for motor test
int millistowait;
unsigned long recordedtime=99999999999999;
unsigned long currenttime=0;
int currentMotorNum;

void setup() {

  TCCR4B = TCCR4B & B11111000 | B00000001;   // for PWM frequency of 31372.55 Hz (D2, D3, D5)
  TCCR4B = TCCR4B & B11111000 | B00000001;   // for PWM frequency of 31372.55 Hz (D6, D7, D8)
  TCCR2B = TCCR2B & B11111000 | B00000001;  // for PWM frequency of 31372.55 Hz (D9, D10)
  
  pinMode (erm1_f, OUTPUT);
  pinMode (erm1_r, OUTPUT);
  pinMode (erm2_f, OUTPUT);
  pinMode (erm2_r, OUTPUT);
  pinMode (erm3_f, OUTPUT);
  pinMode (erm3_r, OUTPUT);
  pinMode (erm4_f, OUTPUT);
  pinMode (erm4_r, OUTPUT);
  
  digitalWrite(erm1_f, LOW);
  digitalWrite(erm1_r, LOW);
  digitalWrite(erm2_f, LOW);
  digitalWrite(erm2_r, LOW);
  digitalWrite(erm3_f, LOW);
  digitalWrite(erm3_r, LOW);
  digitalWrite(erm4_f, LOW);
  digitalWrite(erm4_r, LOW);
  
  Serial.begin(115200);
  while (! Serial);
  
  Serial.println("delimiter-MMTD project");
}
    
void loop() {
  loopSerial();
  loopMotorOnOff();
}

void loopSerial()
{

  if(Serial.available()>0)
  {
    String inString = Serial.readStringUntil('\n');
    char c1 = inString.charAt(0);
    char c2 = inString.charAt(1);
    char c3 = inString.charAt(2);
    char c4 = inString.charAt(3);
    char c5 = inString.charAt(4);
    char c6 = inString.charAt(5);

    if(c2 == 'v')
    {
      int motorNum = (int)c1 - 49;
      if(0 <= motorNum && motorNum < 4)
      {
        ermOn[motorNum] = true;
        ermRunStartTime[motorNum] = millis();
      }  
    }
    else if (c2 == 's')
    {
      int motorNum = (int)c1 - 49;
      if(0 <= motorNum && motorNum < 4)
      {
        ermOn[motorNum] = false;
        ermTurnOff(motorNum);
      }  
    }
    else if (c2 == 't')
    {
      int motorNum = (int)c1 - 49;
      if(0 <= motorNum && motorNum < 4)
      {
        ermOn[motorNum] = true;
        ermBurst[motorNum] = true;
        ermRunStartTime[motorNum] = millis();
      }
    }
    else if (c2 == 'b' && c3 == 'v')
    {
      int motorNum = (int)c1 - 49;
      int runMS = ((int)c4-48)*100+((int)c5-48)*10+((int)c6-48);
      if(0 <= motorNum && motorNum < 4)
      {
        ermBumpRunMS[motorNum] = runMS;
        /*
        Serial.print("ERM motor ");
        Serial.print(motorNum+1);
        Serial.print(" BumpRunMS: ");
        Serial.println(runMS);
        Serial.flush();
        */
      }
    }
    else if (c2 == 'b' && c3 == 's')
    {
      int motorNum = (int)c1 - 49;
      int stopMS = ((int)c4-48)*100+((int)c5-48)*10+((int)c6-48);
      if(0 <= motorNum && motorNum < 4)
      {
        ermBumpStopMS[motorNum] = stopMS;
        /*
        Serial.print("ERM motor ");
        Serial.print(motorNum+1);
        Serial.print(" BumpStopMS: ");
        Serial.println(stopMS);
        Serial.flush();
        */
      }
    }
  }
}

// Function: loopMotorOnOff
// Turn on LRA if true (166Hz full-powered)
void loopMotorOnOff ()
{
  int i;
  for(i=0;i<4;i++)
  {
    if(ermBurst[i])
    {
      unsigned long passedTime = millis() - ermRunStartTime[i];

      /*      
      Serial.print("millis():  ");
      Serial.print(millis());
      Serial.print(", ermRunStartTime[i]: ");
      Serial.print(ermRunStartTime[i]);
      Serial.print(", passedTime: ");
      Serial.println(passedTime);
      */
      if(passedTime > 500)
      {       
        ermTurnOff(i);
        ermOn[i] = false;
        ermBurst[i] = false;
      }  
    }
    if(ermOn[i])
    {
      unsigned long overDrivingTime = 40;
      unsigned long brakingTime = 10;
      unsigned long passedTime = ((millis() - ermRunStartTime[i]) %(ermBumpRunMS[i]+ ermBumpStopMS[i]));
      /*
      Serial.print("passedTime: ");
      Serial.print(passedTime);
      */
      // Normal(steady) vibration
      if(ermBumpStopMS[i] == 0)
      {
        ermTurnOn(i, 1);
        //Serial.println(", On(Normal)");
      }
      // Bumpy (Rough) vibration
      else
      {
        if(0 <= passedTime && passedTime < overDrivingTime)
        {
          ermTurnOn(i, 2);  // 1 : normal (3.3V), 2: Overdrive (5V), 3: Braking (5V)
          //Serial.println(", On(OverDrive) ");
        }
        else if (overDrivingTime <= passedTime && passedTime < ermBumpRunMS[i])
        {
          ermTurnOn(i, 1);  // 1 : normal (3.3V), 2: Overdrive (5V), 3: Braking (5V)
          //Serial.println(", On(Normal)");
        }
        
        else if (ermBumpRunMS[i]<= passedTime && passedTime < ermBumpRunMS[i] + brakingTime)
        {
          ermTurnOn(i, 3);  // 1 : normal (3.3V), 2: Overdrive (5V), 3: Braking (5V)       
          //Serial.println(", On(Braking)");   
        }
        else
        {
          ermTurnOff(i);  
          //Serial.println(", Off");
        }  
      }
    }  
  }
}

void turnOffAll()
{
  int i;
  for(i=0;i<4;i++)
    ermOn[i] = false;
}

// drivingMode: 1 - 3.3V, 2 - 5V
void ermTurnOn(int motorNum, int drivingMode)
{
  int motors_f[4] = {erm1_f, erm2_f, erm3_f, erm4_f};
  int motors_r[4] = {erm1_r, erm2_r, erm3_r, erm4_r};

  PWMpercent = 80;
  if(drivingMode == 1)  // 1 : normal (3.3V)  
  {
    analogWrite(motors_f[motorNum], 255);
    analogWrite(motors_r[motorNum], (255 * (100 - PWMpercent))/100);
  }
  else if (drivingMode == 2)  // 2: Overdrive (5V)    
  {
    analogWrite(motors_f[motorNum], 255);
    analogWrite(motors_r[motorNum], 0);
  }
  else if (drivingMode == 3)  // 3: Braking (5V)       
  {
    analogWrite(motors_f[motorNum], 0);
    analogWrite(motors_r[motorNum], 255);
  }
}

void ermTurnOff(int motorNum)
{
  int motors_f[4] = {erm1_f, erm2_f, erm3_f, erm4_f};
  int motors_r[4] = {erm1_r, erm2_r, erm3_r, erm4_r};

  analogWrite(motors_f[motorNum], 0);
  analogWrite(motors_r[motorNum], 0);
}
