
#include <Time.h>
#include <TimeAlarms.h>

#define TIME_HEADER  "T" 
#define TIME_REQUEST 7

int led = 14;
int flag = false;  //To Display Main Routine
int showTime = false;  //Set to Show TIME
int toggle = true;  //To toggle ON/OFF when ALARM fires
String dateTime[10],_dateTime[10];

//Date time variables
int d,m,y,h,mi,s;
 
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete



void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(led, OUTPUT);
  setSyncProvider( requestSync);
  Serial1.write("Waiting for Time ...\n");
  Serial.write("Starting");
  setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011
  inputString.reserve(200);
}

void  loop(){ 
  
  digitalClockDisplay();
  Alarm.delay(500);
  serialEvent();
  if (stringComplete) {
          Serial.println(inputString); 
          if( inputString.startsWith("_SET")  ){
                 Serial.println("Seting time");
                 //int count = inputString.length();
                  
                  char str1[50];
                  inputString.toCharArray(str1, 50);
                  char str2[] = "_";
                  char * pnt;
                  pnt=strtok( str1, str2 );
                  int j = 0;
                  while( pnt!= NULL ) 
                   {
                     //Serial.println(  pnt );
                     dateTime[j] = pnt;
                     j++;
                     pnt = strtok( NULL, str2 );
                   }
                   for(int l=0;l<j;l++){
                    Serial.println(  dateTime[l] ); 
                   }
                 setTime(dateTime[1].toInt(),dateTime[2].toInt(),dateTime[3].toInt(),dateTime[4].toInt(),dateTime[5].toInt(),14);
         }
          else if( inputString.startsWith("_ALARM_")  ){
                  Serial.println("Seting ALARM");
                 char _str1[50];
                  inputString.toCharArray(_str1, 50);
                  char _str2[] = "_";
                  char * _pnt;
                  _pnt=strtok( _str1, _str2 );
                  int _j = 0;
                  while( _pnt!= NULL ) 
                   {
                     //Serial.println(  pnt );
                     _dateTime[_j] = _pnt;
                     _j++;
                     _pnt = strtok( NULL, _str2 );
                   }
                   for(int _l=0;_l<_j;_l++){
                    Serial.println(  _dateTime[_l] ); 
                   }
                   setAlarm(_dateTime[1].toInt());
                 
         }else{
         
                 Serial.println("NOT ALRM");
                   if( inputString == "1") {
                        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level) 
                      }else  if( inputString == "0"){
                        digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltagelevel) 
                      }
         }    
    inputString = "";
    stringComplete = false;
  }//check stringComplete
  
}//Main Loop Ends


void serialEvent() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read(); 
    // add it to the inputString:
   
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar != '@') {
       inputString += inChar;
    } else{
        stringComplete = true; 
    }
  }
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  //setTime(DEFAULT_TIME);
  if(Serial1.find(TIME_HEADER)) {
     pctime = Serial1.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
       Serial1.write("Time Updated");
     }
  }
}


// functions to be called when an alarm triggers:
void MorningAlarm(){
  Serial.println("Alarm: - turn lights off");    
}

void EveningAlarm(){
  Serial.println("Alarm: - turn lights on");           
}

void WeeklyAlarm(){
  Serial.println("Alarm: - its Monday Morning");      
}

void ExplicitAlarm(){
  Serial.println("Alarm: - this triggers only at the given date and time");       
}

void Repeats(){
  Serial.write("15 second timer");  
  if( toggle ){
    digitalWrite(led, HIGH);  
     toggle = false;
  }else {
    digitalWrite(led, LOW);  
    toggle = true;
  } 
 
}

void OnceOnly(){
  Serial.write("This timer only triggers once\n");  
  if( toggle ){
    digitalWrite(led, HIGH);  
     toggle = false;
  }else {
    digitalWrite(led, LOW);  
    toggle = true;
  }  
 
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year());
  Serial.print("--");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}



void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

void setAlarm(int i){
  Serial.write("Alarm Function called for ");
  Serial.write( i );
  Serial.write(" seconds.\n");
  Alarm.timerOnce(i, OnceOnly);
  //Alarm.timerRepeat(i, Repeats);  
}

    
