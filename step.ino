//Declare pin functions on Redboard
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6
#define lim 8

//Declare variables for functions
char user_input;
int x;
int y;
int state;
int currentPos = -1;
int endPos = 200;

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(lim, INPUT_PULLUP);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  if (digitalRead(lim) != LOW){
    Serial.println("**Please home stepper motor**\n");
  }
  Serial.println("Enter number for control option:");
  Serial.println("1. Turn at default microstep mode.");
  Serial.println("2. Reverse direction at default microstep mode.");
  Serial.println("3. Turn at 1/8th microstep mode.");
  Serial.println("4. Step forward and reverse directions.");
  Serial.println("5. Home stepper motor.");
  Serial.println("6. Go to end.");
  Serial.println();
}

//Main loop
void loop() {
  while(Serial.available()){
  user_input = Serial.read(); //Read user input and trigger appropriate function
  digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
  if (user_input =='1')
  {
    StepForwardDefault();
  }
  else if(user_input =='2')
  {
    ReverseStepDefault();
  }
  else if(user_input =='3')
  {
    SmallStepMode();
  }
  else if(user_input =='4')
  {
    ForwardBackwardStep();
  }
  else if(user_input =='5')
  {
    HomeStepper();
  }
  else if(user_input =='6')
  {
    GoToEnd();
  }
  else
  {
    Serial.println("Invalid option entered.");
  }
  resetEDPins();
  }
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void StepForwardDefault()
{
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 1; x<140; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
    currentPos++;
  }
  Serial.println("Enter new option");
  Serial.println();
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<1000; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

// 1/8th microstep foward mode function
void SmallStepMode()
{
  Serial.println("Stepping at 1/8th microstep mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2, HIGH);
  for(x= 1; x<endPos; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

//Forward/reverse stepping function
void ForwardBackwardStep()
{
  Serial.println("Alternate between stepping forward and reverse.");
  for(x= 1; x<5; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    //Read direction pin state and change it
    state=digitalRead(dir);
    if(state == HIGH)
    {
      digitalWrite(dir, LOW);
    }
    else if(state ==LOW)
    {
      digitalWrite(dir,HIGH);
    }
  
    for(y=1; y<endPos; y++)
    {
      digitalWrite(stp,HIGH); //Trigger one step
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }
  }
  Serial.println("Enter new option:");
  Serial.println();
}

void HomeStepper()
{
  Serial.println("Homing stepper motor.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2, HIGH);
  while(digitalRead(lim) != LOW){
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  delay(100);
  digitalWrite(dir, HIGH);
  for(int i = 0; i < 40; i++){
    digitalWrite(stp,HIGH);
    delay(4);
    digitalWrite(stp,LOW);
    delay(4);
  }
  currentPos = 0;
  Serial.println("Stepper homed.");
  Serial.println("Enter new option");
  Serial.println();
}

void GoToEnd()
{
  Serial.println("current pos: ");
  Serial.println(currentPos);
  Serial.println("End pos: ");
  Serial.println(endPos);
  Serial.println("Going to end.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1, LOW); 
  digitalWrite(MS2, LOW);
  while(currentPos < endPos)
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
    currentPos++;
    Serial.println(currentPos);
  }
  Serial.println("Stepper at end.");
  Serial.println("Enter new option");
  Serial.println();
}

