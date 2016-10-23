#include "lcdmenu.h"
#include "ConfigurationStore.h"
#include "beep.h" 

/*******************************************************本程序开源仅供3D打印学习使用******************************************************
																												Dlion-3D打印主板
																												3D二进制创客---------技术论坛:www.3dbinmaker.com-----QQ技术交流群：523118188
																												文件说明：UI显示  版本：V02
																												Copyright(C)深圳洛众科技有限公司
																												All rights reserved
*****************************************************************************************************************************************/

//typedef void (*menuFunc_t)();
static uint32_t tp_scan_next_update_millis=0;
static uint32_t display_next_update_millis=0;

menuFunc_t currentMenu = main_screen;
menuFunc_t lastMenu;
bool redraw_screen = true;
u8 test_data_2=12;
int test_data_1=11;
bool windows_flag = false;

u8 ioc_key=0x00;
_data_input input_data;
_filelistbox_obj * flistbox;
_filelistbox_list * filelistx;

_btn_obj **screen_key_group;


float manual_move_length=6;
u32 manual_move_xy_speed=60;
u32 manual_move_z_speed=8;

float preheat_e0_length=1;
u32 preheat_e0_speed=5;

float heater_0_temp = 0;
float bed_temp = 0;
int fanSpeed_temp=0;


void lcd_update(void)
{ 
	if (tp_scan_next_update_millis < millis())
	{
	  tp_dev.scan(0);
	  in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);
	  tp_scan_next_update_millis = millis() + 10;
	  (*currentMenu)();
	}
}
void menu_action_gcode(char* pgcode)
{
    enquecommand(pgcode);
}

void start_screen (void)
{
  gui_show_strmid(0,60,lcddev.width,40,RED,16,(u8*)"Dlion-3D打印开源版本",1);
	gui_show_strmid(0,100,lcddev.width,40,RED,16,(u8*)"V02",1);
	gui_show_strmid(180,200,120,40,RED,16,(u8*)"By 3D Binmaker",1);
}

void main_screen (void)
{
		u8 selx=0XFF;
		u8 i;
	  u8 num = 2; 
	  char TempBuffer[32];

	  gui_phy.back_color=LIGHTGRAY;	

		if(redraw_screen==true)				
		{
			redraw_screen=false;			
		
			gui_fill_rectangle(0,0,lcddev.width,lcddev.height,LIGHTGRAY );
			gui_draw_rectangle(0,0,lcddev.width,lcddev.height,LIGHTGRAY);
			
			gui_show_strmid(180,200,120,40,RED,16,(u8*)"By 3D Binmaker",1);

			

			screen_key_group =(_btn_obj **)gui_memin_malloc(sizeof(_btn_obj *)*num);	
			if(screen_key_group)
			{
          screen_key_group[0]=btn_creat(40,160,100,40,0,4);
					screen_key_group[1]=btn_creat(190,160,100,40,1,4);
						for(i=0;i<num;i++)
						{
							screen_key_group[i]->bcfucolor=BLACK;
							screen_key_group[i]->bcfdcolor=WHITE;
							screen_key_group[i]->bkctbl[0]=CYAN;
							screen_key_group[i]->bkctbl[1]=RED;
							screen_key_group[i]->bkctbl[2]=CYAN;
							screen_key_group[i]->bkctbl[3]=RED;
						}

						screen_key_group[0]->caption="装卸耗材";
						screen_key_group[1]->caption="SD卡打印";
						
						for(i=0;i<num;i++)
						{
							btn_draw(screen_key_group[i]);
						}
			}

		}
		
		
			if(display_next_update_millis<millis())
			{
				display_next_update_millis = millis() + 500;
				
				sprintf(TempBuffer, "E0-TEMP:%3.0f/%-3.0f",degHotend(0),degTargetHotend(0));
				gui_show_strmid(0,50,lcddev.width,16,BLACK,16,(u8 *)TempBuffer,0);
				
				sprintf(TempBuffer, "BED-TEMP:%3.0f/%-3.0f",degBed(),degTargetBed());
				gui_show_strmid(0,100,lcddev.width,16,BLACK,16,(u8 *)TempBuffer,0);
			}
		

 		selx=screen_key_chk(screen_key_group,&in_obj,num);
		if(selx&(1<<6))
		{
			switch(selx & ~(3<<6))
					{   
				      case 0: 
								      currentMenu = preheat_screen;
						          redraw_screen=true;
											break;
							case 1: 
										  currentMenu = gecodelist_screen;
						          redraw_screen=true;
											break;									
					   	default: break;
					}
		}

	  if(redraw_screen)
		{
			for(i=0;i<num;i++)
			{	
				btn_delete(screen_key_group[i]);
			}
			gui_memin_free(screen_key_group);
			display_next_update_millis=0;	
		}
}




void preheat_screen (void)
{	
		u8 selx=0XFF;
		u8 i;
	  u8 num = 3; 
	 char TempBuffer[32];

	  gui_phy.back_color=LIGHTGRAY;	

		if(redraw_screen==true)				
		{
			redraw_screen=false;			
		
			gui_fill_rectangle(0,0,lcddev.width,lcddev.height,LIGHTGRAY );
			gui_fill_rectangle(0,0,lcddev.width,40,LIGHTBLUE);
			gui_draw_hline(0,40,lcddev.width,GRAY);
			gui_show_strmid(0,0,320,40,WHITE,16,(u8*)"装卸耗材",1);
			
			gui_show_strmid(180,200,120,40,RED,16,(u8*)"By 3D Binmaker",1);

			

			screen_key_group =(_btn_obj **)gui_memin_malloc(sizeof(_btn_obj *)*num);	
			if(screen_key_group)
			{
          screen_key_group[0]=btn_creat(40,140,100,40,0,4);
					screen_key_group[1]=btn_creat(190,140,100,40,1,4);
				  screen_key_group[2]=btn_creat(8,8,50,25,2,4);
						for(i=0;i<num;i++)
						{
							screen_key_group[i]->bcfucolor=BLACK;
							screen_key_group[i]->bcfdcolor=WHITE;
							screen_key_group[i]->bkctbl[0]=CYAN;
							screen_key_group[i]->bkctbl[1]=RED;
							screen_key_group[i]->bkctbl[2]=CYAN;
							screen_key_group[i]->bkctbl[3]=RED;
						}


						screen_key_group[0]->caption="加热";
						screen_key_group[1]->caption="挤出";
						screen_key_group[2]->caption="返回";
						
						for(i=0;i<num;i++)
						{
							btn_draw(screen_key_group[i]);
						}
			}

	 }
		
		
	if(display_next_update_millis<millis())
	{
		display_next_update_millis = millis() + 500;

 		sprintf(TempBuffer, "E0-TEMP:%3.0f/%-3.0f",degHotend(0),degTargetHotend(0));
				gui_show_strmid(0,80,lcddev.width,16,BLACK,16,(u8 *)TempBuffer,0);
	}
		
		
 		selx=screen_key_chk(screen_key_group,&in_obj,num);
		if(selx&(1<<6))
		{
			switch(selx & ~(3<<6))
					{   
				      case 0: 
								        if(target_temperature[0] == 0)
												{
													target_temperature[0] = 210;
													sprintf(TempBuffer, "M104 S%.1f",(float)target_temperature[0]);
													menu_action_gcode(TempBuffer);

													screen_key_group[0]->bkctbl[0]=RED;
													screen_key_group[0]->bkctbl[1]=CYAN;
													screen_key_group[0]->bkctbl[2]=RED;
													screen_key_group[0]->bkctbl[3]=CYAN;
													btn_draw(screen_key_group[0]);
												}
												else
												{
												  target_temperature[0] = 0;
													sprintf(TempBuffer, "M104 S%.1f",(float)target_temperature[0]);
													menu_action_gcode(TempBuffer);

													screen_key_group[0]->bkctbl[0]=CYAN;
													screen_key_group[0]->bkctbl[1]=RED;
													screen_key_group[0]->bkctbl[2]=CYAN;
													screen_key_group[0]->bkctbl[3]=RED;
													btn_draw(screen_key_group[0]);
												}
											 break;
							case 1: 
										   menu_action_gcode("G91");
											 sprintf(TempBuffer, "G1 E%.1f F%d",10.0,1200);
											 menu_action_gcode(TempBuffer);
											 menu_action_gcode("G90");
											 break;	
              case 2: 
								      target_temperature[0] = 0;
											sprintf(TempBuffer, "M104 S%.1f",(float)target_temperature[0]);
											menu_action_gcode(TempBuffer);

											screen_key_group[0]->bkctbl[0]=CYAN;
											screen_key_group[0]->bkctbl[1]=RED;
											screen_key_group[0]->bkctbl[2]=CYAN;
											screen_key_group[0]->bkctbl[3]=RED;
											btn_draw(screen_key_group[0]);
										  currentMenu = main_screen;
						          redraw_screen=true;
											break;							
					   	default: break;
					}
		}

	  if(redraw_screen)
		{
			for(i=0;i<num;i++)
			{	
				btn_delete(screen_key_group[i]);
			}
			gui_memin_free(screen_key_group);
			display_next_update_millis=0;	
		}
}



void gecodelist_screen(void)
{ 
	u8 selx=0XFF;
	u8 i;	
	u8 *pname=0;
 	_filelistbox_list * filelisttemp;	
 	if(windows_flag==false)
	{	 
		if(redraw_screen==true)			
		{ 
			redraw_screen=false;		

		
			gui_fill_rectangle(0,0,lcddev.width,lcddev.height,LIGHTGRAY );
			gui_fill_rectangle(0,0,lcddev.width,40,LIGHTBLUE);  
			gui_draw_hline(0,40,lcddev.width,GRAY);
			gui_show_strmid(0,0,320,40,WHITE,16,(u8*)"SD卡打印",1);
			//画背景				
			
			screen_key_group =(_btn_obj **)gui_memin_malloc(sizeof(_btn_obj *)*3);
			flistbox=filelistbox_creat(2,44,lcddev.width-4,192,1,16);
			if(screen_key_group&&flistbox)
				{		
              screen_key_group[0]=btn_creat(8,8,50,25,0,4);
							screen_key_group[0]->bcfucolor=BLACK;
							screen_key_group[0]->bcfdcolor=WHITE;
							screen_key_group[0]->bkctbl[0]=CYAN;
							screen_key_group[0]->bkctbl[1]=RED;
							screen_key_group[0]->bkctbl[2]=CYAN;
							screen_key_group[0]->bkctbl[3]=RED;
						  screen_key_group[0]->caption="返回";

					
						for(i=1;i<3;i++)
						{	
							screen_key_group[i]=btn_creat(95+(i-1)*80,125,60,40,i,2);
							
							screen_key_group[i]->bkctbl[0]=0X8452;
							screen_key_group[i]->bkctbl[1]=0XAD97;			
							screen_key_group[i]->bkctbl[2]=0XAD97;
							screen_key_group[i]->bkctbl[3]=0X8452;
						}
						screen_key_group[1]->caption=(u8*)"打印";
						screen_key_group[2]->caption=(u8*)"返回";
					
						flistbox->fliter=FLBOX_FLT_GCODE;
						flistbox->type=0;
						flistbox->path="0:\\GCODE";				
						filelistbox_scan_filelist(flistbox);
						filelistbox_draw_listbox(flistbox);		
						
						  for(i=0;i<1;i++)
							{	
								btn_draw(screen_key_group[i]);
							}
				}
		}


		selx=filelistbox_check(flistbox,&in_obj);		
		selx=screen_key_chk(screen_key_group,&in_obj,1);
		
		if(selx&(1<<6))
		{	switch(selx & ~(3<<6))//
					{   case 0: currentMenu = main_screen;
											redraw_screen=true ;
											for(i=0;i<3;i++)
											{	
												btn_delete(screen_key_group[i]);
											}
											gui_memin_free(screen_key_group); 
											filelistbox_delete(flistbox);
											
								break;			   
					   	default: break;
					}
		}
		else
		{
			if(flistbox->dbclick==0X81)
				{ flistbox->dbclick=0x00;
					redraw_screen=true;
					windows_flag=true;
				}
		}
	}
	else 
	{	if(redraw_screen==true)
		{ 
			redraw_screen=false;
		
			gui_draw_arcrectangle(75,80,180,40,6,1,GRAYBLUE,GRAYBLUE);
			gui_fill_rectangle(75,110,180,70,WHITE );//
			gui_draw_rectangle(75,110,180,70,GRAYBLUE);
			filelisttemp=filelist_search(flistbox->list,flistbox->selindex);
			gui_show_strmid(75,80,180,30,BLACK,16,filelisttemp->name,1);	
			
			if(screen_key_group)
				{ 					
					for(i=1;i<3;i++)
					{	btn_draw(screen_key_group[i]);
					}
				}
		}	
		selx=screen_key_chk(screen_key_group+1,&in_obj,2);
		if(selx&(1<<6))
		{	switch(selx & ~(3<<6))//
					{   case 0:	filelisttemp=filelist_search(flistbox->list,flistbox->selindex);
											strcpy((char *)card.filename,(const char*)filelisttemp->name);
											pname=gui_memin_malloc(strlen((const char*)filelisttemp->name)+strlen((const char*)flistbox->path)+2);
											if(pname)  
											{	
												pname=gui_path_name(pname,flistbox->path,filelisttemp->name);
												selx=f_open(&card.fgcode,(const TCHAR*)pname,FA_READ);
												gui_memin_free(pname);
												if (selx==FR_OK)
												{
													currentMenu = print_screen;
													redraw_screen=true;
													windows_flag=false;	
													for(i=0;i<3;i++)
													{	btn_delete(screen_key_group[i]);
													}
													gui_memin_free(screen_key_group);
													filelistbox_delete(flistbox);												
													card_startFileprint();
													starttime=millis();
												}
											}
								break;
							case 1:		windows_flag=false;
												filelistbox_draw_listbox(flistbox);
								break;						
					   	default: break;
					}
		}		
	}
	
}





void print_screen (void)
{
	u8 i;
	u8 selx=0XFF;
	char TempBuffer[32];
	u8 num=2;

	if(redraw_screen==true)
		{
			redraw_screen=false;
			display_next_update_millis=0;
      gui_phy.back_color=LIGHTGRAY;	
			gui_fill_rectangle(0,0,lcddev.width,lcddev.height,LIGHTGRAY );
			gui_fill_rectangle(0,0,lcddev.width,40,LIGHTBLUE);  
			gui_draw_hline(0,40,lcddev.width,GRAY);
			
       gui_show_strmid(0,0,lcddev.width,40,WHITE,16,"打印中",1);
		
			screen_key_group =(_btn_obj **)gui_memin_malloc(sizeof(_btn_obj *)*num);	
			if(screen_key_group)
			{
          screen_key_group[0]=btn_creat(40,180,100,40,0,4);
					screen_key_group[1]=btn_creat(190,180,100,40,1,4);
						for(i=0;i<num;i++)
						{
							screen_key_group[i]->bcfucolor=BLACK;
							screen_key_group[i]->bcfdcolor=WHITE;
							screen_key_group[i]->bkctbl[0]=CYAN;
							screen_key_group[i]->bkctbl[1]=RED;
							screen_key_group[i]->bkctbl[2]=CYAN;
							screen_key_group[i]->bkctbl[3]=RED;
						}

						screen_key_group[0]->caption="暂停";
						screen_key_group[1]->caption="急停";
						
						for(i=0;i<num;i++)
						{
							btn_draw(screen_key_group[i]);
						}
			}
		}
	if(display_next_update_millis<millis())
	{
		display_next_update_millis = millis() + 500;

		sprintf(TempBuffer, "X:%8.3fmm",current_position[X_AXIS]);
		 gui_show_strmid(20,70,120,16,BLACK,16,(u8 *)TempBuffer,0);
		sprintf(TempBuffer, "Y:%8.3fmm",current_position[Y_AXIS]);
		 gui_show_strmid(20,100,120,16,BLACK,16,(u8 *)TempBuffer,0);
		sprintf(TempBuffer, "Z:%8.3fmm",current_position[Z_AXIS]);
		 gui_show_strmid(20,130,120,16,BLACK,16,(u8 *)TempBuffer,0);
		
 		sprintf(TempBuffer, "E0-TEMP:%3.0f/%-3.0f",degHotend(0),degTargetHotend(0));
		gui_show_strmid(180,70,120,16,BLACK,16,(u8 *)TempBuffer,0);
		
 		sprintf(TempBuffer, "BED-TEMP:%3.0f/%-3.0f",degBed(),degTargetBed());
		gui_show_strmid(180,100,120,16,BLACK,16,(u8 *)TempBuffer,0);

		  
			 if(card_eof())
			 {
			  currentMenu = main_screen;
			  redraw_screen=true ;
			 }

	}
	  

		  selx=screen_key_chk(screen_key_group,&in_obj,num);

		  if(selx&(1<<6))
		  {
				switch(selx & ~(3<<6))
					{ case 0: 
										if(card.sdprinting == true)
										{
											screen_key_group[0]->caption="继续";
											btn_draw(screen_key_group[0]);
											card.sdprinting = false;
										}
										else
										{
											screen_key_group[0]->caption="暂停";
											btn_draw(screen_key_group[0]);
											card.sdprinting = true;
											TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);

										} 
										break;
						case 1: 
							      quickStop(); 
										card_closefile();
										starttime=0;
										card.sdprinting = false;

										autotempShutdown();
										setTargetHotend(0,active_extruder);
										heater_0_temp = 0;
                    bed_temp = 0;
 
						        disable_x(); 
										disable_y(); 
										disable_z(); 
										disable_e0(); 
										disable_e1(); 
						        menu_action_gcode("M84") ;
										redraw_screen=true;
							      currentMenu = main_screen;
										break;					
						default: break;
					}	
	    }
	if(redraw_screen)
	{
		for(i=0;i<num;i++)
		{btn_delete(screen_key_group[i]);
		}
		gui_memin_free(screen_key_group);
		display_next_update_millis=0;		
	}
}



