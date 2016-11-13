//TIM2 PWM�������TIM4�������������ֹͣ
//frequency_tim2��TIM2 PWM������ڣ�KHz
//duty_tim2��TIM2 PWMռ�ձ�  0-100
//period_tim4: TIM4����TIM2�����ڣ���λ0.1ms
//duty_tim4: TIM4����TIM2���ʱ�䣬��λ0.1ms
  
void TIM2_CH3_PA2_PWM_TIM4_CH4_GATE(unsigned short int frequency_tim2,char duty_tim2,unsigned short int period_tim4,unsigned short int duty_tim4)
{  
  
     GPIO_InitTypeDef          GPIO_InitStructure;
     TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
     TIM_OCInitTypeDef         TIM_OCInitStructure;

     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2  RCC_APB1Periph_TIM4, ENABLE);    //ʹ�ܶ�ʱ��2ʱ��
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA   RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

     //���ø�����Ϊ�����������,���TIM2 CH3��PWM���岨��    GPIOA.2
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM2_CH3
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
  
     //��ʼ��TIM2
     TIM_TimeBaseStructure.TIM_Period = (1/frequency_tim2); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,����Ƶ��Ϊ1MHz���ַ�Ƶ�����õ�����һ��
     TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��72MHz/72=1MHz 
     TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
     TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
     
     //��ʼ��TIM2 Channel3 PWMģʽ     
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_Pulse=(10*duty_tim2/frequency_tim2);       //(TIM_Period)*(duty/100) --> (1/period)*(duty/100)
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
     TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3
 
     TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ��� 
     TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
 
     //��ʼ��TIM4
     TIM_TimeBaseStructure.TIM_Period = period_tim4; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,����Ƶ��Ϊ1MHz���ַ�Ƶ�����õ�����һ��
     TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��72MHz/72=1MHz 
     TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
     
     //��ʼ��TIM4 Channel4 PWMģʽ     
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_Pulse=duty_tim4;       //(TIM_Period)*(duty/100) --> (1/period)*(duty/100)
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
     TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC4
 
     TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ��� 
     TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
     
     TIM_SelectInputTrigger(TIM2, TIM_TS_ITR3);     //ѡ����Ч����� ,����֪TIM4����TIM2��ITR3       
     TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);  //����Ϊ���Ӹ�λģʽ
     TIM_SelectOutputTrigger(TIM4,TIM_TRGOSource_OC4Ref ); 
     /* Enable timer's master/slave work mode */ 
     TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);   //Slave
     TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);   //Master
}