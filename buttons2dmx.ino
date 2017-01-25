#include <DmxSimple.h>

#define DEBUG 1                 // Optimize speed by setting to zero
#define BROADCAST_MS 2000       //  How long in ms to send desired DMX value
#define LOOP_DELAY 250          // ms to wait between checking GPI.  Also DMX send interval !

                                // Arduino Pins to use
unsigned char pins[] = {5,6,7,8, 9,10,11,12};

                                // DMX address array
unsigned char addresses[] = {101, 102, 103, 104,  105, 106, 107, 108};

                                // DMX value array
unsigned char values[] =   {255, 255, 255, 255,  255, 255, 255, 255};

                                //  until timers for each button
unsigned long until[] = {0,0,0,0, 0,0,0,0};    


#define GPI_PIN_ON    LOW             // If voltage is LOW, the dip switch is in the ON position. 

void setup() {
#if DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif
  
  DmxSimple.usePin(3);
                                        // Initialize the input (contact closure) pins
  for (int i=0; i<sizeof(pins); i++)
  {
    pinMode(pins[i], INPUT);            // set pin to input
    digitalWrite(pins[i], HIGH);        // turn on pullup resistor
  }
}

void loop() {

#ifdef DEBUG
  char debug[128];
#endif
        //
        // Read GPI and interpret which button was pushed (Arduino pins 2-10)
        //
  for (int i=0; i<sizeof(pins); i++)
  {
    unsigned char val=0;
    
    val = digitalRead(pins[i]);
    
    if (val==GPI_PIN_ON){
      until[i] = millis() + BROADCAST_MS;
    } 

#if DEBUG
    sprintf(debug, "pin %02d = %d - Button %d - Send DMX value %03d to Address %03d Until ", 
                              pins[i], val, i, values[i], addresses[i]);
    Serial.print(debug);
    Serial.print(until[i]);
#endif
    
    if (until[i] > millis()){
#if DEBUG
      Serial.print(" *** SENDING values[i] NOW ***");
#endif
      DmxSimple.write(addresses[i], values[i]);
      
    } else if (until[i] > (millis() + BROADCAST_MS) ){   // After broadcast is over send Zeros for same interval
#if DEBUG
      Serial.print(" *** SENDING Zero NOW ***");
#endif
      DmxSimple.write(addresses[i], 0);     // default to zero
    }

#if DEBUG
    Serial.println(" ");
#endif
  }
#if DEBUG
  delay(250);
  Serial.print("\n------ Millisecond Timer is now ");
  Serial.println(millis());
#endif
  
}
