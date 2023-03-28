#include <FastLED.h>

#define DATA_PIN 6 // On Trinket or Gemma, suggest changing this to 1

#define NUM_LEDS 50 // Popular NeoPixel ring size

int     button_switch =                       2; // external interrupt pin

#define switched                            true // value if the button switch has been pressed
#define triggered                           true // controls interrupt handler
#define interrupt_trigger_type            RISING // interrupt triggered on a RISING input
#define debounce                              10 // time to wait in milli secs
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define C9_Red    0xB80400
#define C9_Orange 0x902C02
#define C9_Green  0x046002
#define C9_Blue   0x070758
#define C9_White  0x606820

#define PALLET_SIZE  5

int blinky = 6;

int milli[] = {120, 80};
int state[] = {1,1};

CRGB leds[NUM_LEDS];
CRGB tungsten_yellow = CRGB( 124,  64,   0);
CRGB tungsten_blue   = CRGB(   0,  100, 90);
CRGB tungsten_green  = CRGB(  30,  86,  11);
CRGB tungsten_orange = CRGB( 120,  40,   2);
CRGB tungsten_red    = CRGB( 90,   0,   0);
CRGB tungsten_white  = CRGB( 255, 119,  10);

CRGB pallet[] = {
  tungsten_yellow,
  tungsten_blue,
  tungsten_green,
  tungsten_orange,
  tungsten_red
  //tungsten_white
};

CRGB holly_pallet[] = {
  tungsten_green,
  tungsten_green,
  tungsten_red,
  tungsten_green,
  tungsten_red
  //tungsten_white
};


/*
#define PALLET_SIZE  5
CRGB pallet[] = {
  C9_Red,
  C9_Orange,
  C9_Green,
  C9_Blue,
  C9_White
};
*/

volatile  bool interrupt_process_status = {
  !triggered
};
bool initialisation_complete =            false;


void button_interrupt_handler() {
  if (initialisation_complete == true) {
    if (interrupt_process_status == !triggered) {
      if (digitalRead(button_switch) == HIGH) {
        interrupt_process_status = triggered;
      }
    }
  }
}

bool read_button() {
  int button_reading;
  static bool     switching_pending = false;
  static long int elapse_timer;
  if (interrupt_process_status == triggered) {
    button_reading = digitalRead(button_switch);
    if (button_reading == HIGH) {
      switching_pending = true;
      elapse_timer = millis();
    }
    if (switching_pending && button_reading == LOW) {
      if (millis() - elapse_timer >= debounce) {
        switching_pending = false;
        interrupt_process_status = !triggered;
        return switched;
      }
    }
  }
  return !switched;
}

void all_white() {
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = tungsten_white;
    }
    FastLED.show();
}

void white_blink() {
    EVERY_N_MILLIS(750) {
      if(state[0] == 1) {
        for(int i = 0; i < NUM_LEDS; i += 2) {
          leds[i] = CRGB(0,0,0);
        }
        for(int i = 1; i < NUM_LEDS; i += 2) {
          leds[i] = tungsten_white;
        }
        state[0] = 0;
      } else {
        for(int i = 0; i < NUM_LEDS; i += 2) {
          leds[i] = tungsten_white;
        }
        for(int i = 1; i < NUM_LEDS; i += 2) {
          leds[i] = CRGB(0,0,0);
        }
        state[0] = 1;
      }
      FastLED.show();
    }
}

void all_nostalgia() {
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = pallet[i % PALLET_SIZE];
    }
    FastLED.show();
}

void nostalgia_odd() {
    EVERY_N_MILLIS(10) {
      if(milli[0] <= 0) {
        if(state[0] == 1) {
          for(int i = 0; i < NUM_LEDS / 2; i += 1) {
            leds[i] = CRGB(0,0,0);
          }
          state[0] = 0;
        } else {
          for(int i = 0; i < NUM_LEDS / 2; i += 1) {
            leds[i] = pallet[i % PALLET_SIZE];
          }
          state[0] = 1;
        }
        milli[0] = random(50, 200);
      } else {
        milli[0]--;
      }
  
      if(milli[1] <= 0) {
        if(state[1] == 1) {
          for(int i = NUM_LEDS / 2; i < NUM_LEDS; i += 1) {
            leds[i] = CRGB(0,0,0);
          }
          state[1] = 0;
        } else {
          for(int i = NUM_LEDS / 2; i < NUM_LEDS; i += 1) {
            leds[i] = pallet[i % PALLET_SIZE];
          }
          state[1] = 1;
        }
        milli[1] = random(50, 200);
      } else {
        milli[1]--;
      }
      
      FastLED.show();
    }
}

void nostalgia_twinkle() {
    EVERY_N_MILLIS(100) {

      if(random(0,100) >= 90) {
        int target = random(0,NUM_LEDS);
        CRGB was = leds[target];

        for(int loops = 0; loops <= 2; loops++) {
          leds[target] = tungsten_white;
          FastLED.show();
          delay(10);    
          leds[target] = was;
          FastLED.show();
          delay(100);  
        }
                  
        for(int i = 0; i < NUM_LEDS; i++) {
          leds[i] = pallet[i % PALLET_SIZE];  
        }
        FastLED.show();
      }
    }
}

void all_holly() {
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = holly_pallet[i % PALLET_SIZE];
    }
    FastLED.show(); 
}

void holly_twinkle() {
    EVERY_N_MILLIS(2000) {
        for(int i = 0; i < NUM_LEDS; i++) {
          if(random(0,100) <= 25) {
            leds[i] = tungsten_red;  
          } else {
            leds[i] = tungsten_green;
          }
        }
        FastLED.show();
      }    
}

void holly_sage() {
    EVERY_N_MILLIS(1000) {
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = tungsten_green;
      }
      leds[random(0,NUM_LEDS)] = tungsten_red;
      FastLED.show();    
   }
}

void nostalgia_half() {
    EVERY_N_MILLIS(10) {
      if(milli[0] <= 0) {
        if(state[0] == 1) {
          for(int i = 0; i < NUM_LEDS; i += 2) {
            leds[i] = CRGB(0,0,0);
          }
          state[0] = 0;
        } else {
          for(int i = 0; i < NUM_LEDS; i += 2) {
            leds[i] = pallet[i % PALLET_SIZE];
          }
          state[0] = 1;
        }
        milli[0] = random(50, 200);
      } else {
        milli[0]--;
      }
  
      if(milli[1] <= 0) {
        if(state[1] == 1) {
          for(int i = 1; i < NUM_LEDS; i += 2) {
            leds[i] = CRGB(0,0,0);
          }
          state[1] = 0;
        } else {
          for(int i = 1; i < NUM_LEDS; i += 2) {
            leds[i] = pallet[i % PALLET_SIZE];
          }
          state[1] = 1;
        }
        milli[1] = random(50, 200);
      } else {
        milli[1]--;
      }      
      FastLED.show();
    }
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { all_white, white_blink, all_nostalgia, nostalgia_twinkle, nostalgia_odd, nostalgia_half, all_holly, holly_twinkle, holly_sage};

void setup() {
  attachInterrupt(digitalPinToInterrupt(button_switch),
                  button_interrupt_handler,
                  interrupt_trigger_type);
  initialisation_complete = true; // open interrupt processing for business
  
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(70);

  gPatterns[blinky]();
  FastLED.show();   // Send the updated pixel colors to the hardware.
}


void loop() {
  if (read_button() == switched) {
    blinky = (blinky + 1) % ARRAY_SIZE( gPatterns);
  }

  gPatterns[blinky]();
}
