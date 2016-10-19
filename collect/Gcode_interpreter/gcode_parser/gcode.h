/**
  ******************************************************************************
  * @file    
  * @Author: 
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
**/
#ifndef INIT_H_
#define INIT_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f10x_gpio.h"
/* Definition ----------------------------------------------------------------*/
typedef enum {false = 0, true = !false} bool;
typedef unsigned    char   uint8_t;
typedef unsigned    char   byte;
typedef unsigned    short  uint16_t;
typedef bool               boolean;


// define the parameters of our machine.
#define X_STEPS_PER_INCH 5080
#define X_STEPS_PER_MM   100
#define X_MOTOR_STEPS    200

#define Y_STEPS_PER_INCH 5080
#define Y_STEPS_PER_MM   100
#define Y_MOTOR_STEPS    200

#define Z_STEPS_PER_INCH 5080
#define Z_STEPS_PER_MM   200
#define Z_MOTOR_STEPS    200

//our maximum feedrates
#define FAST_XY_FEEDRATE 1000.0
#define FAST_Z_FEEDRATE  50.0

// Units in curve section
#define CURVE_SECTION_INCHES 0.019685
#define CURVE_SECTION_MM 0.5

// Set to one if endstop outputs are inverting (ie: 1 means open, 0 means closed)
// RepRap opto endstops are *not* inverting.
#define ENDSTOPS_INVERTING 0
// Optionally disable max endstops to save pins or wiring
#define ENDSTOP_X_MIN_ENABLED true
#define ENDSTOP_X_MAX_ENABLED false
#define ENDSTOP_Y_MIN_ENABLED true
#define ENDSTOP_Y_MAX_ENABLED false
#define ENDSTOP_Z_MIN_ENABLED false
#define ENDSTOP_Z_MAX_ENABLED true

// How many temperature samples to take.  each sample takes about 100 usecs.
#define TEMPERATURE_SAMPLES 5

// The *_ENABLE_PIN signals are active high as default. Define this
// to one if they should be active low instead (e.g. if you're using different
// stepper boards).
// RepRap stepper boards are *not* inverting.
#define INVERT_ENABLE_PINS 1

// If you use this firmware on a cartesian platform where the
// stepper direction pins are inverted, set these defines to 1
// for the axes which should be inverted.
// RepRap stepper boards are *not* inverting.
#define INVERT_X_DIR false
#define INVERT_Y_DIR true
#define INVERT_Z_DIR true

// Defines in which logical direction to move when using the G30 command
// (home to physical reference switches). 1 is positive, 0 is negative
#define REFERENCE_X_DIR false
#define REFERENCE_Y_DIR false
#define REFERENCE_Z_DIR true

/****************************************************************************************
* digital i/o pin assignment
*  
* this uses the undocumented feature of Arduino - pins 14-19 correspond to analog 0-5
****************************************************************************************/

//cartesian bot pins
#define X_STEP_PIN    GPIO_Pin_1
#define X_DIR_PIN     GPIO_Pin_2
#define X_ENABLE_PIN  GPIO_Pin_15
#define X_MIN_PIN     GPIO_Pin_11
#define X_MAX_PIN     GPIO_Pin_12

#define Y_STEP_PIN    GPIO_Pin_1
#define Y_DIR_PIN     GPIO_Pin_2
#define Y_ENABLE_PIN  GPIO_Pin_15
#define Y_MIN_PIN     GPIO_Pin_11
#define Y_MAX_PIN     GPIO_Pin_12


#define ENABLE_Z_Axis              0
/*//z axis
#define Z_STEP_PIN 29
#define Z_DIR_PIN 30
#define Z_ENABLE_PIN 31
#define Z_MIN_PIN 2
#define Z_MAX_PIN 1
*/
/*//extruder pins
#define EXTRUDER_MOTOR_SPEED_PIN   14
#define EXTRUDER_MOTOR_DIR_PIN     10
#define EXTRUDER_HEATER_PIN        12
#define EXTRUDER_FAN_PIN           3
#define EXTRUDER_THERMISTOR_PIN    4  //NB! analog pin, -1 disables thermistor readings
#define EXTRUDER_THERMOCOUPLE_PIN  -1 //NB! analog pin, -1 disables thermocouple readings
*/
// Enable/disable features
#define ENABLE_ARCS                0

typedef enum Axis
{
  X_AXIS = 0,  
  Y_AXIS = 1,
  Z_AXIS = 2
}Axis;

typedef struct AxisConfig 
{
  uint16_t step_pin;
  uint16_t dir_pin;
  uint16_t min_pin;
  uint16_t max_pin;
  uint16_t enable_pin;
  bool invert_dir;
  bool reference_dir;
  bool min_endstop_enabled;
  bool max_endstop_enabled;
  uint16_t steps_per_inch;
  uint16_t steps_per_mm;
  uint16_t motor_steps;
}AxisConfig;

typedef struct LongPoint
{
  long p[3];
}LongPoint;

typedef struct FloatPoint
{
  float p[3];
} FloatPoint;


typedef struct GcodeParser
{
  unsigned int seen;
  int G;
  int M;
  float P;
  float X;
  float Y;
  float Z;
  float I;
  float J;
  float F;
  float S;
  float R;
  float Q;
}GcodeParser;


/* Exported Functions --------------------------------------------------------*/
void init_steppers(void);
void process_string(char instruction[], int size);
int scan_int(char *str, int *valp, unsigned int *seen, unsigned int flag);
int scan_float(char *str, float *valp, unsigned int *seen, unsigned int flag);
void do_step(Axis AXIS,bool dir );
void cmd_G01(FloatPoint start ,FloatPoint end);
void cmd_G02(FloatPoint start ,FloatPoint end,float r_x,float r_y);
void cmd_G03(FloatPoint start ,FloatPoint end,float r_x,float r_y);
void do_led(Axis AXIS,bool dir);
//void home_axis(Axis axis);
#endif // INIT_H_

/*********************************END OF FILE**********************************/
