#include "public.h"  
#include "uart.h"
#include "tftlcd.h"
#include "touch.h"
#include "gui.h"


//GUI测试
void GUI_Test(void)
{
	FRONT_COLOR=WHITE;
	LCD_ShowFontHZ(tftlcd_data.width/2-2*24-12, 0,"普中科技");
	LCD_ShowString(tftlcd_data.width/2-7*12,30,tftlcd_data.width,tftlcd_data.height,24,"www.prechin.cn");
	LCD_ShowString(tftlcd_data.width/2-7*8,55,tftlcd_data.width,tftlcd_data.height,24,"GUI Test");
	
	FRONT_COLOR=RED;
	gui_draw_bigpoint(10,55,GREEN);
	gui_draw_bline(10,80,120,80,10,GREEN);
	gui_draw_rectangle(10,95,30,30,GREEN);
	gui_draw_arcrectangle(50,95,30,30,5,1,BLUE,GREEN);
	gui_fill_rectangle(90,95,30,30,GREEN);
	//gui_fill_circle(90,120,20,GREEN);
	gui_fill_ellipse(30,150,20,10,GREEN);

	//更多的GUI等待大家来发掘和编写
	
	delay_ms(2000);
	LCD_Clear(BACK_COLOR);		
}

void main()
{

	UART_Init();
	TFTLCD_Init();
		
	FRONT_COLOR=WHITE;
	LCD_ShowFontHZ(tftlcd_data.width/2-2*24-12, 0,"普中科技");
	LCD_ShowString(tftlcd_data.width/2-7*12,30,tftlcd_data.width,tftlcd_data.height,24,"www.prechin.cn");

		
	while(1)
	{
		
	}					
}
