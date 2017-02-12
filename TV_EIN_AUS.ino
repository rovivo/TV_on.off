// switch next line on if you need debug messages
//#define DEBUG

const int LEDTV1 = 2;
const int LEDBOXON = 4;
const int LEDBOXOFF = 5;

const int WarnLED = 3;
const int BOXPower = 12;
const int TVPower = 13; 

unsigned long previousMillis = 0;
const long interval = 1800000; // 1 800 000 milisekunden = 30min
const long WarnungStart = 60000;
long interval2 = 0;

int LTV1 = 0;
//int LTV2 = 0;
int LBOXON = 0;
int LBOXOFF = 0;

boolean timesup = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LEDTV1, INPUT);
  //pinMode(LEDTV2, INPUT);
  pinMode(LEDBOXON, INPUT);
  pinMode(LEDBOXOFF, INPUT);
  pinMode(BOXPower, OUTPUT);
  pinMode(TVPower, OUTPUT);
  pinMode(WarnLED, OUTPUT);
  digitalWrite(LEDBOXON, HIGH); // Pull Up Widerstand aktivieren
  digitalWrite(LEDBOXOFF, HIGH); // Pull Up Widerstand aktivieren
  //digitalWrite(LEDTV1, HIGH); // Pull Up Widerstand aktivieren
  //digitalWrite(LEDTV2, HIGH); // Pull Up Widerstand aktivieren

  digitalWrite(WarnLED, LOW);
  
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis();
  interval2 = interval - WarnungStart;
  
  LTV1 = digitalRead(LEDTV1);
  //LTV2 = digitalRead(LEDTV2);
  LBOXON = digitalRead(LEDBOXON);
  LBOXOFF = digitalRead(LEDBOXOFF);
  
  #ifdef DEBUG
     Serial.print("LEDTV1 ");
     Serial.println(LTV1);
     //Serial.print("LEDTV2 ");
     //Serial.println(LTV2);
     Serial.print("LEDBOX ON ");
     Serial.println(LBOXON);
     Serial.print("LEDBOX OFF ");
     Serial.println(LBOXOFF);
  #endif

// Box and TV is off = LED also off and stop counting
  if (LBOXOFF == HIGH && LBOXON == LOW) 
  {
    timesup = LOW;
    currentMillis = 0;
    previousMillis = 0;
    digitalWrite (WarnLED, LOW);
  }

// switch off BOX after the given time
  if (timesup == HIGH && LBOXON == HIGH) 
  {
    BOXONOFF();
  }

// bring the tv in the same state as the box
  if (LBOXON == HIGH && (LTV1 == LOW)) 
  {
    TVONOFF();
    previousMillis = currentMillis;
  }

// bring the tv in the same state as the box
  if (LBOXON == LOW && (LTV1 == HIGH)) 
  {
    TVONOFF();
  }

// restart counting down because of pressing any button on the remote
  if (LBOXON == HIGH && LBOXOFF == HIGH)
  {
      previousMillis = currentMillis;
      digitalWrite (WarnLED, LOW);
  }
  else  //if not pressed any button, check if time is up
  {
      if (currentMillis - previousMillis >= interval) 
	  {
          timesup = HIGH;
	  }   
  }

// if box on and time is up, switch box and tv off
  if (timesup == HIGH && LBOXON == HIGH) 
  {
    BOXONOFF();
    TVONOFF();
  }

// check time again ?!
  if (LBOXON == HIGH && timesup == LOW) 
  {
    #ifdef DEBUG
      Serial.println("halb drin");
    #endif
    if (currentMillis - previousMillis >= interval) 
	  {
      previousMillis = currentMillis;
      #ifdef DEBUG
        Serial.println("drin");
      #endif
	  BOXONOFF();
      TVONOFF();
      timesup = HIGH;
    }
  }

// lets blink the warn led 60s before off with interval 2
  if (LBOXON == HIGH && timesup == LOW) 
  {
    #ifdef DEBUG
      Serial.println("halb drin blink");
    #endif
	 if (currentMillis - previousMillis >= interval2) 
	 {
       #ifdef DEBUG
        Serial.println("drin blink");
       #endif
       if (digitalRead (WarnLED) == LOW) 
	   {
         digitalWrite (WarnLED, HIGH);
       }
       else 
	   {
         digitalWrite (WarnLED, LOW);
       }
	 }
	 else
	 {
		 digitalWrite(WarnLED, LOW);
	 }
   } 

  #ifdef DEBUG
    Serial.print("Current Millis ");
    Serial.println(currentMillis);
    Serial.print("Previous Millis ");
    Serial.println(previousMillis);
    Serial.print("Intervall ");
    Serial.println(interval);
    Serial.print("TimesUP: ");
    Serial.println(timesup);
    Serial.print("WarnLED: ");
    Serial.println(digitalRead (WarnLED));
    Serial.print("interval2: ");
    Serial.println(interval2);
    Serial.println("");
  #endif
  
  delay (250);
}

// command structure to switch on or off the tv
void TVONOFF() {
  digitalWrite(TVPower, LOW);
  delay(250);
  digitalWrite(TVPower, HIGH);
  #ifdef DEBUG
  Serial.println("TV EIN / AUS");
  #endif
  delay(5000);  
}

// command structure to switch on or off the box
void BOXONOFF() {
  digitalWrite(BOXPower, HIGH);
  delay(250);
  digitalWrite(BOXPower, LOW);
  #ifdef DEBUG
  Serial.println("BOX EIN / AUS");
  #endif
}


