#include <SPI.h>
#include <mcp2515.h>
MCP2515 mcp2515(9);
const byte PulsesPerRevolution = 20;
const unsigned long ZeroTimeout = 100000;
const byte numReadings = 2;
volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;
unsigned long FrequencyRaw;
unsigned long FrequencyReal;
unsigned long RPM;
unsigned int PulseCounter = 1;
unsigned long PeriodSum;
unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 1;
unsigned int ZeroDebouncingExtra;
unsigned long readings[numReadings];
unsigned long readIndex;
unsigned long total;
unsigned long average;
const double wheelCircumference = 20.42;  // wheel circumference in cm
double speed = 0.0;  // Calculated speed (cm/s)
void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(3), Pulse_Event, RISING);
  delay(1000);
  // Initialize SPI
  SPI.begin();
   // Initialize MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}
void loop() {
  LastTimeCycleMeasure = LastTimeWeMeasured;
  CurrentMicros = micros();
  if (CurrentMicros < LastTimeCycleMeasure) {
    LastTimeCycleMeasure = CurrentMicros;
  }
  FrequencyRaw = 10000000000 / PeriodAverage;
  if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {
    FrequencyRaw = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }
  FrequencyReal = FrequencyRaw / 10000;
  RPM = FrequencyRaw / PulsesPerRevolution * 60;
  RPM = RPM / 10000;
  total = total - readings[readIndex];
  readings[readIndex] = RPM;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
  speed = (wheelCircumference * RPM) / 60;
  // Prepare CAN message
  struct can_frame canMsg;
  canMsg.can_id  = 0x100;             // CAN ID
  canMsg.can_dlc = sizeof(double);     // Data Length Code (DLC) - 8 bytes for a double
  // Copy the float into the CAN message data array
  memcpy(canMsg.data, &speed, sizeof(double));
  // Send CAN message
  if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message");
  }
  Serial.print("RPM: ");
  Serial.print(RPM);
  Serial.print(", Speed: ");
  Serial.print(speed);
  Serial.println(" cm/s");
  delay(80);
}
void Pulse_Event() {
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;
  LastTimeWeMeasured = micros();
  if (PulseCounter >= AmountOfReadings)  {
    PeriodAverage = PeriodSum / AmountOfReadings;
    PulseCounter = 1;
    PeriodSum = PeriodBetweenPulses;
    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
    AmountOfReadings = RemapedAmountOfReadings;
  } else {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses;
  }
}
