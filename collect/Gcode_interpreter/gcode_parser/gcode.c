/**
  ******************************************************************************
  * @file    gcode.c
  * @Author: MQjehovah
  * @version v1.0.0
  * @date    2015-2-28
  * @brief   inspired by gcode_interpreter
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "gcode.h"
#include "stdlib.h"
#include <math.h>
#include "lcd.h"
/* Definition ----------------------------------------------------------------*/
AxisConfig axes[3] = {
	{X_STEP_PIN, X_DIR_PIN, X_MIN_PIN, X_MAX_PIN, X_ENABLE_PIN, INVERT_X_DIR, REFERENCE_X_DIR, ENDSTOP_X_MIN_ENABLED, ENDSTOP_X_MAX_ENABLED, X_STEPS_PER_INCH, X_STEPS_PER_MM, X_MOTOR_STEPS},
	{Y_STEP_PIN, Y_DIR_PIN, Y_MIN_PIN, Y_MAX_PIN, Y_ENABLE_PIN, INVERT_Y_DIR, REFERENCE_Y_DIR, ENDSTOP_Y_MIN_ENABLED, ENDSTOP_Y_MAX_ENABLED, Y_STEPS_PER_INCH, Y_STEPS_PER_MM, Y_MOTOR_STEPS},
#if ENABLE_Z_Axis
{Z_STEP_PIN, Z_DIR_PIN, Z_MIN_PIN, Z_MAX_PIN, Z_ENABLE_PIN, INVERT_Z_DIR, REFERENCE_Z_DIR, ENDSTOP_Z_MIN_ENABLED, ENDSTOP_Z_MAX_ENABLED, Z_STEPS_PER_INCH, Z_STEPS_PER_MM, Z_MOTOR_STEPS}
#endif
};


LongPoint start;
bool abs_mode = false; //0 = incremental; 1 = absolute
FloatPoint current_units;
FloatPoint target_units;
float feedrate = 0.0;

#define GCODE_G	(1<<0)
#define GCODE_M	(1<<1)
#define GCODE_P	(1<<2)
#define GCODE_X	(1<<3)
#define GCODE_Y	(1<<4)
#define GCODE_Z	(1<<5)
#define GCODE_I	(1<<6)
#define GCODE_J	(1<<7)
#define GCODE_K	(1<<8)
#define GCODE_F	(1<<9)
#define GCODE_S	(1<<10)
#define GCODE_Q	(1<<11)
#define GCODE_R	(1<<12)

#define TYPE_INT 1
#define TYPE_FLOAT 2

#define PARSE_INT(ch, instr, str, str_size, len, val, seen, flag)       \
  case ch:                                                              \
  len = scan_int(str, &val, &seen, flag);                               \
  break;

#define PARSE_FLOAT(ch, instr, str, str_size, len, val, seen, flag)     \
  case ch:                                                              \
  len = scan_float(str, &val, &seen, flag);                             \
  break;
	
/* Functions --------------------------------------------------------*/

/**
  ********************************************************************
  * @brief  init_steppers
  * @param  None
  * @retval None
  * @Note   None
  ********************************************************************
**/
void init_steppers(void)
{
	uint8_t i;
	   //if (a.min_endstop_enabled) pinMode(a.min_pin, INPUT);
     //if (a.max_endstop_enabled) pinMode(a.max_pin, INPUT);
	   GPIO_InitTypeDef GPIO_InitStructure;   
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;    
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
     GPIO_Init(GPIOA, &GPIO_InitStructure); 
		 GPIO_Init(GPIOB, &GPIO_InitStructure); 
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_15;
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	   GPIO_Init(GPIOA, &GPIO_InitStructure); 
		 GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
	for ( i=0;i<3;i++)
	{
		current_units.p[i] = 0.0;
		target_units.p[i] = 0.0;
	}

}


/**
  ********************************************************************
  * @brief  parse_string
  * @param  None
  * @retval None
  * @Note   None
  ********************************************************************
**/
int parse_string(struct GcodeParser * gc, char instruction[], int size)
{
  int ind;
  int len;
  gc->seen = 0;
  len=0;
  for (ind=0; ind<size; ind += (1+len)) {
    len = 0;
    switch (instruction[ind]) {
      PARSE_INT('G', instruction, &instruction[ind+1], size-ind, len, gc->G, gc->seen, GCODE_G);
      PARSE_INT('M', instruction, &instruction[ind+1], size-ind, len, gc->M, gc->seen, GCODE_M);
      PARSE_FLOAT('S', instruction, &instruction[ind+1], size-ind, len, gc->S, gc->seen, GCODE_S);
      PARSE_FLOAT('P', instruction, &instruction[ind+1], size-ind, len, gc->P, gc->seen, GCODE_P);
      PARSE_FLOAT('X', instruction, &instruction[ind+1], size-ind, len, gc->X, gc->seen, GCODE_X);
      PARSE_FLOAT('Y', instruction, &instruction[ind+1], size-ind, len, gc->Y, gc->seen, GCODE_Y);
      PARSE_FLOAT('Z', instruction, &instruction[ind+1], size-ind, len, gc->Z, gc->seen, GCODE_Z);
      PARSE_FLOAT('I', instruction, &instruction[ind+1], size-ind, len, gc->I, gc->seen, GCODE_I);
      PARSE_FLOAT('J', instruction, &instruction[ind+1], size-ind, len, gc->J, gc->seen, GCODE_J);
      PARSE_FLOAT('F', instruction, &instruction[ind+1], size-ind, len, gc->F, gc->seen, GCODE_F);
      PARSE_FLOAT('R', instruction, &instruction[ind+1], size-ind, len, gc->R, gc->seen, GCODE_R);
      PARSE_FLOAT('Q', instruction, &instruction[ind+1], size-ind, len, gc->Q, gc->seen, GCODE_Q);
    
    }
  }
	return 0;
}


/**
  ********************************************************************
  * @brief  process_string
  * @param  None
  * @retval None
  * @Note   None
  ********************************************************************
**/
void process_string(char instruction[], int size)
{
  GcodeParser gc;	
  if (instruction[0] == '/') 
	{
		USART_SendStr("note:");
    USART_SendStr(instruction);
    return;
  }
	
  parse_string(&gc, instruction, size);
	
  if (gc.seen & GCODE_G) {
   
    if (abs_mode) {
      if (gc.seen & GCODE_X)
        target_units.p[X_AXIS] = gc.X;
      if (gc.seen & GCODE_Y)
        target_units.p[Y_AXIS] = gc.Y;
      if (gc.seen & GCODE_Z)
        target_units.p[Z_AXIS] = gc.Z;
    }
    else {
      if (gc.seen & GCODE_X)
        target_units.p[X_AXIS] = current_units.p[X_AXIS]+gc.X;
      if (gc.seen & GCODE_Y)
        target_units.p[Y_AXIS] = current_units.p[Y_AXIS]+gc.Y;
      if (gc.seen & GCODE_Z)
        target_units.p[Z_AXIS] = current_units.p[Z_AXIS]+gc.Z;
    }

 
    if ( gc.seen & GCODE_F )
      feedrate = gc.F;
    switch (gc.G) {
      
    case 0:
    case 1:
//			start.p [X_AXIS]=(int)current_units.p[X_AXIS]+400;
//	    start.p [Y_AXIS]=(int)current_units.p[Y_AXIS]+300;
//      DDALine((int)current_units.p[X_AXIS],(int)current_units.p[Y_AXIS], (int)target_units.p[X_AXIS], (int)target_units.p[Y_AXIS],0xff);
		  cmd_G01(current_units ,target_units);
	  	current_units=target_units;
      break;
#if ENABLE_ARCS == 1
      //Clockwise arc
    case 2:
      //Counterclockwise arc
    case 3: 
			
      break;
#endif // ENABLE_ARCS
			
    case 4: //Dwell
      delay_us((int)(gc.P * 1000));
      break;

     
    case 20:     //Inches for Units
//      units[X_AXIS] = X_STEPS_PER_INCH;
//      units[Y_AXIS] = Y_STEPS_PER_INCH;
//      units[Z_AXIS] = Z_STEPS_PER_INCH;
//#if ENABLE_ARCS == 1
//      curve_section = CURVE_SECTION_INCHES;
//#endif


      break;

      
    case 21:   //mm for Units
//      units[X_AXIS] = X_STEPS_PER_MM;
//      units[Y_AXIS] = Y_STEPS_PER_MM;
//      units[Z_AXIS] = Z_STEPS_PER_MM;
//#if ENABLE_ARCS == 1
//      curve_section = CURVE_SECTION_MM;
//#endif
//      calculate_deltas();
//      break;

//      //go home.
//    case 28:
////      set_target(0.0, 0.0, 0.0);
////      dda_move(getMaxSpeed());
//      break;

    
    case 30:   // Home to physical switches
//      if (gc.seen & GCODE_Z) {
//       // home_axis(Z_AXIS);
//        current_units.p[Z_AXIS] = 0.0;
//      }
//      if (gc.seen & GCODE_Y) {
//       // home_axis(Y_AXIS);
//        current_units.p[Y_AXIS] = 0.0;
//      }
//      if (gc.seen & GCODE_X) {
//       // home_axis(X_AXIS);
//        current_units.p[X_AXIS] = 0.0;
//      }

//      // Move to given offset (fp is overwritten by home_axis)
//      if (gc.seen & GCODE_X) fp.p[X_AXIS] = gc.X;
//      else fp.p[X_AXIS] = current_units.p[X_AXIS];
//      if (gc.seen & GCODE_Y) fp.p[Y_AXIS] = gc.Y;
//      else fp.p[Y_AXIS] = current_units.p[Y_AXIS];
//      if (gc.seen & GCODE_Z) fp.p[Z_AXIS] = gc.Z;
//      else fp.p[Z_AXIS] = current_units.p[Z_AXIS];

//      set_target(fp.p[X_AXIS], fp.p[Y_AXIS], fp.p[Z_AXIS]);
//      feedrate_micros = calculate_feedrate_delay(feedrate);
//      dda_move(feedrate_micros);

      break;

      // Drilling canned cycles
    case 81: // Without dwell
    case 82: // With dwell
//    case 83: { // Peck drilling
//      float retract = gc.R;
//				
//      if (!abs_mode)
//        retract += current_units.p[Z_AXIS];

//      // Retract to R position if Z is currently below this
//      if (current_units.p[Z_AXIS] < retract) {
//        set_target(current_units.p[X_AXIS], current_units.p[Y_AXIS], retract);
//        dda_move(getMaxSpeed());
//      }

//      // Move to start XY
//      set_target(fp.p[X_AXIS], fp.p[Y_AXIS], current_units.p[Z_AXIS]);
//      dda_move(getMaxSpeed());

//      // Do the actual drilling
//    

//      // For G83 move in increments specified by Q code, otherwise do in one pass
//      if (gc.G == 83)
//        delta_z = gc.Q;
//      else
//        delta_z = retract - fp.p[Z_AXIS];

//      do {
//        // Move rapidly to bottom of hole drilled so far (target Z if starting hole)
//        set_target(fp.p[X_AXIS], fp.p[Y_AXIS], target_z);
//        dda_move(getMaxSpeed());

//        // Move with controlled feed rate by delta z (or to bottom of hole if less)
//        target_z -= delta_z;
//        if (target_z < fp.p[Z_AXIS])
//          target_z = fp.p[Z_AXIS];
//        set_target(fp.p[X_AXIS], fp.p[Y_AXIS], target_z);
//        if (feedrate > 0)
//          feedrate_micros = calculate_feedrate_delay(feedrate);
//        else
//          feedrate_micros = getMaxSpeed();
//        dda_move(feedrate_micros);

//        // Dwell if doing a G82
//        if (gc.G == 82)
//          delay_us((int)(gc.P * 1000));

//        // Retract
//        set_target(fp.p[X_AXIS], fp.p[Y_AXIS], retract);
//        dda_move(getMaxSpeed());
//      } while (target_z > fp.p[Z_AXIS]);
//    }
      break;

			
    case 90: //Absolute Positioning
     abs_mode = true;
      break;
			
    case 91: //Incremental Positioning
      abs_mode = false;
      break;

    case 92: //Set as home
      //set_position(0.0, 0.0, 0.0);
      break;

    default:
     USART_SendStr("Unsupported G-Code");
    }
   USART_SendStr("ok");
   return;
  }

  if (gc.seen & GCODE_M) {
    switch (gc.M) {
      //TODO: this is a bug because search_string returns 0.  gotta fix that.
    case 0:
      
      break;
      /*
        case 0:
        //todo: stop program
        break;

        case 1:
        //todo: optional stop
        break;

        case 2:
        //todo: program end
        break;
      */
      //turn extruder on, forward
    case 101:
      //extruder_set_direction(true);
     // extruder_set_speed(extruder_speed);
      break;

      //turn extruder on, reverse



    default:
      USART_SendStr("Unsupported M-Code");
     
    }
    return;
  }


}

int scan_float(char *str, float *valp, unsigned int *seen, unsigned int flag)
{
  float res;
  int len;
  char *end;

  res = (float)strtod(str, &end);
  len = end - str;

  if (len > 0) {
    *valp = res;
    *seen |= flag;
  }
  else {
    *valp = 0;
  }

  return len;	/* length of number */
}

int scan_int(char *str, int *valp, unsigned int *seen, unsigned int flag)
{
  int res;
  int len;
  char *end;

  res = (int)strtol(str, &end, 10);
  len = end - str;

  if (len > 0) {
    *valp = res;
    *seen |= flag;
  }
  else {
    *valp = 0;
  }

  return len;	/* length of number */
}

void cmd_G01(FloatPoint start ,FloatPoint end)
{
	   LongPoint step;
		 bool dir[3];
	   long e,f;
		 step.p[X_AXIS]=(long)((end.p[X_AXIS]-start.p[X_AXIS])*X_STEPS_PER_MM);
	   step.p[Y_AXIS]=(long)((end.p[Y_AXIS]-start.p[Y_AXIS])*Y_STEPS_PER_MM);
//   step.p[Z_AXIS]=(long)((end.p[Z_AXIS]-start.p[Z_AXIS])*Z_STEPS_PER_MM);
		 f=0;
	if(step.p[X_AXIS]>0)
	{
		if (step.p[Y_AXIS]>0){dir[X_AXIS]=true;dir[Y_AXIS]=true;}
		else{dir[X_AXIS]=true;dir[Y_AXIS]=false;}
	}
	else
	{
		if (step.p[Y_AXIS]>0){dir[X_AXIS]=false;dir[Y_AXIS]=true;}
		else{dir[X_AXIS]=false;dir[Y_AXIS]=false;}
	}
	
	step.p[X_AXIS]=abs(step.p[X_AXIS]);
	step.p[Y_AXIS]=abs(step.p[Y_AXIS]);
	e=step.p[X_AXIS]+step.p[Y_AXIS];
	while(e!=0)
	{
		if (f>=0)
		{
			do_step(X_AXIS,dir[X_AXIS]);
//			do_led(X_AXIS,dir[X_AXIS]);
			f-=step.p [Y_AXIS];
		}
		else
		{
			do_step(Y_AXIS,dir[Y_AXIS]);
	//		do_led(Y_AXIS,dir[Y_AXIS]);
			f+=step.p [X_AXIS];
		}
		e--;
	}
	
	
}

void cmd_G02(FloatPoint end ,FloatPoint start,float r_x,float r_y)
{
	LongPoint step;
	bool dir[3];
  long  f,e;
	   step.p[X_AXIS]=abs((long)((start.p[X_AXIS]-r_x)*X_STEPS_PER_MM));
	   step.p[Y_AXIS]=abs((long)((start.p[Y_AXIS]-r_y)*Y_STEPS_PER_MM));
//   step.p[Z_AXIS]=(long)((end.p[Z_AXIS]-start.p[Z_AXIS])*Z_STEPS_PER_MM);
if(start.p[X_AXIS]>=0)
	{
		if (start.p[Y_AXIS]>=0){dir[X_AXIS]=true;dir[Y_AXIS]=false;}
		else{dir[X_AXIS]=false;dir[Y_AXIS]=false;}
	}
	else
	{
		if (start.p[Y_AXIS]>0){dir[X_AXIS]=true;dir[Y_AXIS]=true;}
		else{dir[X_AXIS]=false;dir[Y_AXIS]=true;}
	}
	
	e=(long)(abs(end.p[X_AXIS]-start.p[X_AXIS])*X_STEPS_PER_MM+abs(end.p[Y_AXIS]-start.p[Y_AXIS])*Y_STEPS_PER_MM);
	f=0;
	while(e!=0)
	{
		if (f>=0)
		{
			do_step(X_AXIS,dir[X_AXIS]);
			f=f-2*step.p[X_AXIS]+1;
			step.p[X_AXIS]-=1;
		}
		else
		{
			do_step(Y_AXIS,dir[Y_AXIS]);
			f=f-2*step.p[Y_AXIS]+1;
			step.p[Y_AXIS]-=1; 
		}
		e--;
	}
}



void cmd_G03(FloatPoint start ,FloatPoint end,float r_x,float r_y)
{
	LongPoint step;
	bool dir[3];
  long  f,e;
	   step.p[X_AXIS]=abs((long)((start.p[X_AXIS]-r_x)*X_STEPS_PER_MM));
	   step.p[Y_AXIS]=abs((long)((start.p[Y_AXIS]-r_y)*Y_STEPS_PER_MM));
//   step.p[Z_AXIS]=(long)((end.p[Z_AXIS]-start.p[Z_AXIS])*Z_STEPS_PER_MM);
if(start.p[X_AXIS]>=0)
	{
		if (start.p[Y_AXIS]>=0){dir[X_AXIS]=false;dir[Y_AXIS]=true;}
		else{dir[X_AXIS]=true;dir[Y_AXIS]=true;}
	}
	else
	{
		if (start.p[Y_AXIS]>0){dir[X_AXIS]=true;dir[Y_AXIS]=false;}
		else{dir[X_AXIS]=true;dir[Y_AXIS]=false;}
	}
	
	e=(long)(abs(end.p[X_AXIS]-start.p[X_AXIS])*X_STEPS_PER_MM+abs(end.p[Y_AXIS]-start.p[Y_AXIS])*Y_STEPS_PER_MM);
	f=0;
	while(e!=0)
	{
		if (f>=0)
		{
			do_step(X_AXIS,dir[X_AXIS]);
			f=f-2*step.p[X_AXIS]+1;
			step.p[X_AXIS]-=1;
		}
		else
		{
			do_step(Y_AXIS,dir[Y_AXIS]);
			f=f-2*step.p[Y_AXIS]+1;
			step.p[Y_AXIS]-=1; 
		}
		e--;
	}
}
void do_step(Axis AXIS,bool dir)
{
	if (AXIS==X_AXIS)
	{
        if (dir){	GPIO_SetBits(GPIOA, X_DIR_PIN);}
				else{GPIO_ResetBits(GPIOA, X_DIR_PIN);}
				GPIO_ResetBits(GPIOA, X_STEP_PIN);
				delay_us(500);
				GPIO_SetBits(GPIOA, X_STEP_PIN);
				
	}
	else 
	{
		    if(AXIS==Y_AXIS)
	      {
		      if (dir){	GPIO_SetBits(GPIOB, Y_DIR_PIN);}
			   	else{GPIO_ResetBits(GPIOB, Y_DIR_PIN);}
				  GPIO_ResetBits(GPIOB, Y_STEP_PIN);
				  delay_us(500);
			  	GPIO_SetBits(GPIOB, Y_STEP_PIN);
	      }
   }
	
}


void do_led(Axis AXIS,bool dir)
{
	
	if (AXIS==X_AXIS)
	{
	//	DDALine(0,0,100,100,0xff) ;
        if (dir){	
					start.p[X_AXIS]+=1;
				 putpixel(start.p[X_AXIS],start.p[Y_AXIS],0xff);}
				else{
						start.p[X_AXIS]-=1;
				 putpixel(start.p[X_AXIS],start.p[Y_AXIS],0xff);}
			 
				
	}
	else 
	{
		    if(AXIS==Y_AXIS)
	      {
		       if (dir){	
						 start.p[Y_AXIS]+=1;
				 putpixel(start.p[X_AXIS],start.p[Y_AXIS],0xff);}
				else{
					start.p[Y_AXIS]-=1;
				putpixel(start.p[X_AXIS],start.p[Y_AXIS],0xff);}
	      }
   }
	
}
