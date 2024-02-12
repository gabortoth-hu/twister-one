#include <LiquidCrystal_I2C.h> // Library for LCD


struct rotary {

  char CLK;
  char DT;
  char SW;

  int counter ;
  int currentStateCLK;
  int lastStateCLK;

};

#define initCLK(rotary) rotary.lastStateCLK = digitalRead(rotary.CLK)

rotary menuSelector = { 2, 3, 4, 0, 0, 0};
rotary quantitySelector = {5, 6, 7, 0, 0, 0};

unsigned long lastButtonPress = 0;

String menu[] = {
  "LFO RATE",
  "LFO AMOUNT",
  "FILTER CUTOFF",
  "FILTER RESONANCE",
  "ENV / ATTACK",
  "ENV / RELEASE",
  "* STORE SETTINGS *"
};

const int menuLen = sizeof(menu) / sizeof(menu[0]);

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void setup() {
  // put your setup code here, to run once:
  // Set encoder pins as inputs
	pinMode(menuSelector.CLK, INPUT);
	pinMode(menuSelector.DT, INPUT);
	pinMode(menuSelector.SW, INPUT_PULLUP);

  pinMode(quantitySelector.CLK, INPUT);
	pinMode(quantitySelector.DT, INPUT);
	pinMode(quantitySelector.SW, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);
  Serial.println("start");

  lcd.init(); //initialize the lcd
  //lcd.backlight(); //open the backlight 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("READY!");

  // Read the initial state of CLK
	initCLK(menuSelector);
  initCLK(quantitySelector);
}


void loop() {

  // menu selector changed
  if(updateEncoder(&menuSelector))
  {
  
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("Direction: ");
    lcd.print(menu[abs(menuSelector.counter) % menuLen]);
    //lcd.setCursor(0, 1);
    //lcd.print(menuSelector.counter);
  }

  if(updateEncoder(&quantitySelector)) {
    lcd.setCursor(0, 1);
    lcd.print(quantitySelector.counter);
  }
	// Put in a slight delay to help debounce the reading
	delay(1);
  
}

bool updateEncoder(struct rotary *r) 
{
  bool isChanged = false;

  
  // Read the current state of CLK
	r->currentStateCLK = digitalRead(r->CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (r->currentStateCLK != r->lastStateCLK && r->currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(r->DT) != r->currentStateCLK) {
			r->counter --;
		} else {
			// Encoder is rotating CW so increment
			r->counter ++;
		}

		Serial.print(" | Counter: ");
		Serial.println(r->counter);
    isChanged = true;
	}

	// Remember last CLK state
	r->lastStateCLK = r->currentStateCLK;

/*
	// Read the button state
	int btnState = digitalRead(menuSelector.SW);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
		}

		// Remember last button press event
		lastButtonPress = millis();
	}
  */

  return isChanged;
}
