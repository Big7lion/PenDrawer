//TIM2 PWM输出，由TIM4来控制其输出与停止
//frequency_tim2：TIM2 PWM输出周期：KHz
//duty_tim2：TIM2 PWM占空比  0-100
//period_tim4: TIM4控制TIM2总周期，单位0.1ms
//duty_tim4: TIM4控制TIM2输出时间，单位0.1ms
  
void TIM2_CH3_PA2_PWM_TIM4_CH4_GATE(unsigned short int frequency_tim2,char duty_tim2,unsigned short int period_tim4,unsigned short int duty_tim4)
{  
  
     GPIO_InitTypeDef          GPIO_InitStructure;
     TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
     TIM_OCInitTypeDef         TIM_OCInitStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2  RCC_APB1Periph_TIM4, ENABLE);    //使能定时器2时钟
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA   RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

     //设置该引脚为复用输出功能,输出TIM2 CH3的PWM脉冲波形    GPIOA.2
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM2_CH3
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
  
     //初始化TIM2
     TIM_TimeBaseStructure.TIM_Period = (1/frequency_tim2); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值,本身频率为1MHz，又分频器设置的下面一行
     TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //设置用来作为TIMx时钟频率除数的预分频值，72MHz/72=1MHz 
     TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
     TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
     
     //初始化TIM2 Channel3 PWM模式     
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
     TIM_OCInitStructure.TIM_Pulse=(10*duty_tim2/frequency_tim2);       //(TIM_Period)*(duty/100) --> (1/period)*(duty/100)
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
     TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3
 
     TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器 
     TIM_Cmd(TIM2, ENABLE);  //使能TIM2
 
     //初始化TIM4
     TIM_TimeBaseStructure.TIM_Period = period_tim4; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值,本身频率为1MHz，又分频器设置的下面一行
     TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值，72MHz/72=1MHz 
     TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
     
     //初始化TIM4 Channel4 PWM模式     
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
     TIM_OCInitStructure.TIM_Pulse=duty_tim4;       //(TIM_Period)*(duty/100) --> (1/period)*(duty/100)
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
     TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4
 
     TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR4上的预装载寄存器 
     TIM_Cmd(TIM4, ENABLE);  //使能TIM4
     
     TIM_SelectInputTrigger(TIM2, TIM_TS_ITR3);     //选择有效输入端 ,查表得知TIM4对于TIM2是ITR3       
     TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);  //配置为主从复位模式
     TIM_SelectOutputTrigger(TIM4,TIM_TRGOSource_OC4Ref ); 
     /* Enable timer's master/slave work mode */ 
     TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);   //Slave
     TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);   //Master
}