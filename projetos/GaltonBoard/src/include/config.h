#ifndef CONFIG_H
#define CONFIG_H

// --------------------------- Configuração ---------------------------

#define JOYSTICK_N_BINS 8
#define JOYSTICK_EXTEND_THRESHOLD 0.9f
#define JOYSTICK_RETRACT_THRESHOLD 0.8f

#define MAX_FPS 60

#define INITIAL_SIMULATION_UPDATE_FREQUENCY 3
#define MAX_SIMULATION_UPDATE_FREQUENCY MAX_FPS*5
#define MIN_SIMULATION_UPDATE_FREQUENCY 3

#define INITIAL_SCALE 4
#define INITIAL_N_LINES 7
#define INITIAL_SIMULATION_TICK_US 1<<18

// --------------------------- Pinagem ---------------------------

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Joystick
#define JOY_X_PIN 26
#define JOY_Y_PIN 27

// Buzzer
#define BUZZER_PIN 21

// Buttons
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22

#endif
