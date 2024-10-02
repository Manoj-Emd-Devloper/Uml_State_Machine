enum event{
  ON,
  OFF
};

typedef enum {
  LIGHT_OFF,
  LIGHT_DIM,
  LIGHT_MED,
  LIGHT_FULL
}light_state_t;

light_state_t curr_state = LIGHT_OFF;

#define PIN_LED 9

#define LIGHT_BRIGHT_OFF 0
#define LIGHT_BRIGHT_DIM 25
#define LIGHT_BRIGHT_MED 85
#define LIGHT_BRIGHT_FULL 255

// Function prototypes (optional but recommended)
void run_entry_action(light_state_t state);
void light_state_machine(uint8_t event);
void light_change_density(uint8_t pin, uint8_t intensity);
void light_init(void);

void run_entry_action(light_state_t state) {
  switch (state) {
    case LIGHT_OFF:
      {
        light_change_density(PIN_LED, LIGHT_BRIGHT_OFF);
        break;
      }
    case LIGHT_DIM:
      {
        light_change_density(PIN_LED, LIGHT_BRIGHT_DIM);
        break;
      }
    case LIGHT_MED:
      {
        light_change_density(PIN_LED, LIGHT_BRIGHT_MED);
        break;
      }
    case LIGHT_FULL:
      {
        light_change_density(PIN_LED, LIGHT_BRIGHT_FULL);
        break;
      }
  }
}

void light_state_machine(uint8_t event) {
  light_state_t prev_state;
  prev_state = curr_state;

  switch (curr_state) {
    case LIGHT_OFF:
      switch (event) {
        case ON:
          curr_state = LIGHT_DIM;
          break;
      }
      break;
    case LIGHT_DIM:
      switch (event) {
        case ON:
          curr_state = LIGHT_MED;
          break;
        case OFF:
          curr_state = LIGHT_OFF;
          break;
      }
      break;
    case LIGHT_MED:
      switch (event) {
        case ON:
          curr_state = LIGHT_FULL;
          break;
        case OFF:
          curr_state = LIGHT_OFF;
          break;
      }
      break;
    case LIGHT_FULL:
      switch (event) {
        case ON:
          curr_state = LIGHT_DIM;
          break;
        case OFF:
          curr_state = LIGHT_OFF;
          break;
      }
      break;
  }
  if (prev_state != curr_state) {
    run_entry_action(curr_state);
  }
}

void light_change_density(uint8_t pin, uint8_t intensity) {
  analogWrite(pin, intensity);
}

void light_init(void) {
  curr_state = LIGHT_OFF;
  run_entry_action(LIGHT_OFF);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("-------------------");
  Serial.println("Send 'x' or '0' ");

  light_init();
}

void loop() {
  uint8_t event;
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    event = Serial.read();
    if (event == '0') {
      light_state_machine(ON);
    } else if (event == 'x') {
      light_state_machine(OFF);
    }
  }
}
