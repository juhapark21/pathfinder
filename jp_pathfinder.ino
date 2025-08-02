// Button mapping = # - 1
const int BUT1 = 0;
const int BUT2 = 1;
const int BUT3 = 2; 
const int BUT4 = 3; 

// LED mapping = # + 6 
const int LED1 = 7; 
const int LED2 = 8; 
const int LED3 = 9; 
const int LED4 = 10; 

// Default sequence 
int sequence[10] = {1, 4, 3, 3, 1, 2, 4, 3, 4, 4};

int delayms = 1000; 

void setup() {
  // put your setup code here, to run once:
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  pinMode(BUT4, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  long startTime = millis();

  // Randomly generate 10-button sequence, replace the default sequence 
  for (byte i = 0; i < 10; i = i + 1) {
    int randNum = random(1, 5);
    sequence[i] = randNum;
  }
}

int waitUntilPress() {
  while (true) {
    for (byte i = 0; i <= 3; i = i + 1) {
      if (digitalRead(i) == HIGH) {
        // debounce? 
        delay(20);  
        if (digitalRead(i) == HIGH) {
          // Still pressed 
          while (digitalRead(i) == HIGH) {
            delay(10);
          }

          // Button has been pressed and released 
          return (i + 1);
        }
      }
    }
  }
}

void waitUntilPress(int butNum) {
  while (true) {
    if (digitalRead(butNum - 1) == HIGH) {
      // debounce? 
      delay(20);
      if (digitalRead(butNum - 1) == HIGH) {
        // Still pressed 
        while (digitalRead(butNum - 1) == HIGH) {
          delay(10);
        }

        // Button has been pressed and released 
        return; 
      }
    }
  }
}

bool successfullyPressed(int butNum) {
  int pressed = waitUntilPress();
  return (pressed == butNum);
}

void turnOnLED(int LEDNum) {
  digitalWrite((LEDNum + 6), HIGH);
}

void turnOffAllLEDs() {
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}

void turnOnAllLEDs() {
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}

void turnOnJustThisLED(int LEDNum) {
  turnOffAllLEDs();
  turnOnLED(LEDNum);
}

void blinkAll() {
  for (byte i = 0; i < 4; i = i + 1) {
    turnOnAllLEDs();
    delay(100);
    turnOffAllLEDs();
    delay(100);
  }
}

// Shows the startup sequence 
void startHint() {
  for (byte j = 0; j < 3; j = j + 1) {
    for (byte i = 1; i <= 4; i = i + 1) {
      turnOnJustThisLED(i);
      delay(500);
    }
  }
}

// Won - turn on all LEDs for a bit, then quickly play the winning sequence 
void wonBlink() {
  turnOnAllLEDs(); 
  delay(1000);
  for (byte i = 0; i < 10; i = i + 1) {
    int num = sequence[i];
    turnOnJustThisLED(num);
    delay(100);
  }
  turnOnAllLEDs();
  delay(1000)
  turnOffAllLEDs();
}


bool lostYet = false; 

void loop() {
  // put your main code here, to run repeatedly:
  // memory game 

  lostYet = false;

  // press button 1->2->3->4 in order to turn on 
  startHint();
  // If you press 3->1->4->2, toggles hard mode (goes faster)
  // Still have to press 1->2->3->4 to begin the game 
  if ((waitUntilPress() == 3) && (waitUntilPress() == 1) && (waitUntilPress() == 4) && (waitUntilPress() == 2)) {
    if (delay == 100) {
      delay = 1000;
    } else if (delay == 1000) {
      delay = 100; 
    }
  }

  if ((waitUntilPress() == 1) && (waitUntilPress() == 2) && (waitUntilPress() == 3) && (waitUntilPress() == 4)) {
    // Start the game! 
    for (byte ind = 0; ind < 10; ind = ind + 1) {
        // Display the (partial) sequence 
        for (byte i = 0; i <= ind; i = i + 1) {
          int num = sequence[i];
          turnOnJustThisLED(num);
          delay(1000);
        }
        // Wait for player to enter correct sequence 
        for (byte j = 0; j <= ind; j = j + 1) {
          int num = sequence[j];
          if (successfullyPressed(num) == false) {
            // lost the game 
            lostYet = true; 
            break;
          }
        }

        // End of partial sequence 
        if (lostYet == true) {
          break; 
        }
        delay(500);
    }

    // Sequence done - won or lost? 
    if (lostYet == true) {
      blinkAll();
    } else {
      wonBlink();
    }

  } else {
    startHint(); 
  }

}
