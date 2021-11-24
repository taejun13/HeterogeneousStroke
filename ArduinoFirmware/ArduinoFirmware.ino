/*  
 * Writer: Taejun Kim, HCI Lab KAIST - https://taejun13.github.io/
 * Last Update: 2020. 5. 6
 * Heterogeneous Stroke: Using Unique Vibration Cues to Improve the Wrist-Worn Spatiotemporal Tactile Display
 * ACM CHI 21': Conference on Human Factors in Computing Systems.
 * DOI: https://dl.acm.org/doi/abs/10.1145/3411764.3445448
 * Command (Serial Input) Convention: 
 *  1) Start vibratiing for i-th actuator: iv  (ex. 1v, 2v, 3v, 4v)
 *  2) Stop vibrationg for i-th actuator: is (ex. 1s, 2s, 3s, 4s)
 *  3) Set vibrating frequency F-Hz for i-th actuator: ifF (ex. 1f250, 2f250, 3f170, 4f170)
 *  4) Set power level A(%) for i-th actuator: iaA (ex. 1a100, 2a050, 3a100, 4a050)
 *  5) Set on/off modulation on waveform (for "Bumpy" vibration ) - Vibrating On time T for i-th actuator: ibrT (ex. 1br100, 2br040, 3br100 4br040) 
 *  6) Set on/off modulation on waveform (for "Bumpy" vibration ) - Vibrating Off time T for i-th actuator: ibsT (ex. 1bs000, 2bs040, 3bs000 4bs040) 
 *  (1br100, 1bs000 means that 1st actuator keeps vibrating without on/off modulation)
 *  (2br040, 2bs040 means that 1st actuator repeats 40ms of vibration and 40 ms of rest (= 12.5Hz on/off modulation waveform) 
 *  
 *  Hardware:
 *  - We used BD6221 motor driver to implement micro-level PWM control for LRA motors (BD6221 Datasheet: http://www.datasheetq.com/datasheet-download/308094/1/ROHM/BD6221)
 *  - We used LRA motor of 10 mm diameter and 3.6mm thickness, with 170 Hz resonance frequency. (Our model was provided from Samsung Electro-mechanics but it's not buyable for public)
 */

/* LRA motor driver pin */
const int LRA1_F = 2;   // FIn (Control input - Forward; BD6221 Motor Driver)
const int LRA1_R = 4;   // RIn (Control input - Reverse; BD6221 Motor Driver)
const int LRA2_F = 7;
const int LRA2_R = 8;
const int LRA3_F = 10;
const int LRA3_R = 11;
const int LRA4_F = 12;
const int LRA4_R = 13;

/* Variables for actuator control */
bool LRAOn[4] = {false, false, false, false};
int LRAFrequency[4] = {170, 170, 170, 170};   // Frequency of LRA (Hz)
int LRAPeriod[4] = {(int)(1000000.0/170), (int)(1000000.0/170), (int)(1000000.0/166), (int)(1000000.0/166) };   // Period of LRA (microseconds)
int LRAAmplitude[4] = {100, 100, 100, 100};   // Power (%) of LRA
int LRARunMS_ForBumpiness[4] = {100, 100, 100, 100};  // bump 느낌의 진동에서 진동 시간(ms)
int LRAStopMS_ForBumpiness[4] = {0, 0, 0, 0};    // bump 느낌의 진동에서 쉬는 시간(ms)
unsigned long LRARunStartTime[4] = {0, 0, 0, 0};   // LRA 틀기 시작한 시간

void setup() {
  pinMode (LRA1_F, OUTPUT);
  pinMode (LRA1_R, OUTPUT);
  pinMode (LRA2_F, OUTPUT);
  pinMode (LRA2_R, OUTPUT);
  pinMode (LRA3_F, OUTPUT);
  pinMode (LRA3_R, OUTPUT);
  pinMode (LRA4_F, OUTPUT);
  pinMode (LRA4_R, OUTPUT);

  Serial.begin(115200);
  while (! Serial);
  Serial.println("Heterogeneous Stroke Firmware Start");
}

void loop() {
  loopSerial();
  loopMotorOnOff();
}

void loopSerial()
{
  if(Serial.available() > 0)
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
        LRAOn[motorNum] = true;
        LRARunStartTime[motorNum] = millis();
      }
    }
    else if (c2 == 's')
    {
      int motorNum = (int)c1 - 49;
      if(0 <= motorNum && motorNum < 4)
        LRAOn[motorNum] = false;
    }
    else if (c2 == 'f')
    {
      int motorNum = (int)c1 - 49;
      int freq = ((int)c3-48)*100+((int)c4-48)*10+((int)c5-48);
      int period = (int)((float)1000000 / (float)freq); // Unit : us, microseconds
      if(0 <= motorNum && motorNum < 4)
      {
        LRAFrequency[motorNum] = freq;
        LRAPeriod[motorNum] = period;
      }
    }
    else if (c2 == 'a')
    {
      int motorNum = (int)c1 - 49;
      int amplitude = ((int)c3-48)*100+((int)c4-48)*10+((int)c5-48);
      if(0 <= motorNum && motorNum < 4)
        LRAAmplitude[motorNum] = amplitude;
    }
    else if (c2 == 'b')
    {
      if (c3 == 'r')
      {
        int motorNum = (int)c1 - 49;
        int runMS = ((int)c4-48)*100+((int)c5-48)*10+((int)c6-48);
        if(0 <= motorNum && motorNum < 4)
          LRARunMS_ForBumpiness[motorNum] = runMS;
      }
      else if (c3 == 's')
      {
        int motorNum = (int)c1 - 49;
        int stopMS = ((int)c4-48)*100+((int)c5-48)*10+((int)c6-48);
        if(0 <= motorNum && motorNum < 4)
          LRAStopMS_ForBumpiness[motorNum] = stopMS;
      }
    }
  }
}

void loopMotorOnOff ()
{
  for(int i=0;i<4;i++)
  {
    if(LRAOn[i])
    {
      unsigned long passedTime = ((millis() - LRARunStartTime[i]) %(LRARunMS_ForBumpiness[i] + LRAStopMS_ForBumpiness[i]));
      if(0 <= passedTime && passedTime < LRARunMS_ForBumpiness[i])
        LRAOnOff(i);  
    }  
  }
}

/* PWM Control
 * https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM  
 */
void LRAOnOff(int motorNum)
{
  int LRAs_F[4] = {LRA1_F, LRA2_F, LRA3_F, LRA4_F};
  int LRAs_R[4] = {LRA1_R, LRA2_R, LRA3_R, LRA4_R};
  
  digitalWrite(LRAs_F[motorNum], HIGH);
  digitalWrite(LRAs_R[motorNum], LOW);
  delayMicroseconds((int)(((float)LRAPeriod[motorNum] / (float)2)*((float)LRAAmplitude[motorNum]/(float)100)));
  
  digitalWrite(LRAs_F[motorNum], LOW);
  digitalWrite(LRAs_R[motorNum], LOW);
  delayMicroseconds((int)(((float)LRAPeriod[motorNum] / (float)2)*(1 - ((float)LRAAmplitude[motorNum]/(float)100))));
  
  digitalWrite(LRAs_F[motorNum], LOW);
  digitalWrite(LRAs_R[motorNum], HIGH);
  delayMicroseconds((int)(((float)LRAPeriod[motorNum] / (float)2)*((float)LRAAmplitude[motorNum]/(float)100)));
  
  digitalWrite(LRAs_F[motorNum], LOW);
  digitalWrite(LRAs_R[motorNum], LOW);
  delayMicroseconds((int)(((float)LRAPeriod[motorNum] / (float)2)*(1 - ((float)LRAAmplitude[motorNum]/(float)100))));
}
