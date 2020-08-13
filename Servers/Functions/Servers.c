/*
 * Servers.c
 *
 *      Author: WangXiang
 */
#include "Servers.h"
#include "UserConfig.h"
#include <stdio.h>
#include <stddef.h>

char begin_status = 0;				//variable for UART
char end_status = 0;
int i = 0;
extern DeviceControl DeviceControlMessage;

#define port_receive_num 15												//定义每一个串口缓冲区条数
#define port_receive_size 13											//定义每一个串口缓冲区的大小

char port_receive_1[port_receive_num][port_receive_size];
int port_1_read = 0;														//串口1读指针
int port_1_write = 0;														//串口1写指针
char port_receive_2[port_receive_num][port_receive_size];
int port_2_read = 0;
int port_2_write = 0;
char port_receive_3[port_receive_num][port_receive_size];
int port_3_read = 0;
int port_3_write = 0;

//variable for UART_1
int messagr_len_1 = 0;												//串口中断使用(0场景模式:9位 , 1其他模式:11位)
int order_message_1 = 0;														//标记位，1表示下一位为模式位
char begin_status_1 = 0;
int i_1 = 0;

//variable for UART_2
int messagr_len_2 = 0;												//串口中断使用(0场景模式:9位 , 1其他模式:11位)
int order_message_2 = 0;														//标记位，1表示下一位为模式位
char begin_status_2 = 0;
int i_2 = 0;

//variable for UART_3
int messagr_len_3 = 0;												//串口中断使用(0场景模式:9位 , 1其他模式:11位)
int order_message_3 = 0;														//标记位，1表示下一位为模式位
char begin_status_3 = 0;
int i_3 = 0;

/* *********************************************
 * name:Get_USB_Message
 * function: USB receive control messages
 * variable:USB message of control messages
 * time:2020.6.17
 * Author: ZRH
 * *********************************************/
void Get_USB_Message(uint8_t buffer[200])
{
	if (USB_RxRead(buffer, 10) > 0)
	{
		DeviceControlMessage.DeviceNumber = buffer[2];
		DeviceControlMessage.ControlMode = buffer[3];
		DeviceControlMessage.addh = buffer[4];
		DeviceControlMessage.addl = buffer[5];
		DeviceControlMessage.function = buffer[6];
		DeviceControlMessage.status = 0x01;
//		USB_printf("%s",buffer);
	}
}

/* *********************************************
 * name:Control_485_device
 * function: After receiving the USB message, control the 485 device
 * variable:\
  * time:2020.6.17
 * Author: ZRH
 * *********************************************/
void Control_485_device()
{
	if (DeviceControlMessage.DeviceNumber == 0x01)
	{

	}
}

/* *********************************************
 * name:USART1_IRQHandler
 * function: USB device Send and receive messages
 * variable��\
 * time:2020.6.9
 * Author: ZRH
 * *********************************************/
//void USART1_IRQHandler(void)
//{
//	char x = USART_ReceiveData(USART1);
//
//	if (x == 0x55)
//	{
//		begin_status++;
////		USB_control.USB_buffer[0] = x;
//	} else if (x != 0x55 && begin_status == 1) //begin_status==2 means complete receive message
//	{
//		begin_status = 0;
////		USB_control.USB_buffer[0] = 0x00;
//	}
//	if (x == 0x0D)								//消息以0D,0A结尾
//	{
//		end_status++;
//	} else if (x != 0x0A && end_status == 1)
//	{
//		end_status = 0;
//	}else if (x == 0x0A && end_status == 1)
//	{
//		end_status++;
//	}
//
//	if (USB_control.status == 0 && begin_status > 1)
//	{
//		USB_control.USB_buffer[i]=x;
//		i++;
//		if(end_status>1)
//		{
//			i=0;//variable init
//			end_status = 0;
//			begin_status = 0;
//			USB_control.status = 1;//buffer init
//		}
//	}
//}
void USART1_IRQHandler(void)
{
	char x = USART_ReceiveData(USART1);
	if (x == 0x55)
	{
		begin_status++;
	} else if (x != 0x55 && begin_status == 1) //begin_status==2 means complete receive message
	{
		begin_status = 0;
	}
	if (begin_status > 1)
	{
		USB_printf("%c", 0x01);
		begin_status = 0;
	}
}
/* *********************************************
 * name:USART2_IRQHandler
 * function: 485 device Send and receive messages
 * variable:\
 * time:2020.6.9
 * Author: ZRH
 * *********************************************/
void USART2_IRQHandler(void)
{
	char x = USART_ReceiveData(USART2);
	if (x == 0x55)
	{
		begin_status++;
	} else if (x != 0x55 && begin_status == 1) //begin_status==2 means complete receive message
	{
		begin_status = 0;
	}
	if (begin_status > 1)
	{
		USB_printf("%c", 0x01);
		begin_status = 0;
	}
}

void USART3_IRQHandler(void)
{
	char x = USART_ReceiveData(USART3);
	if (x == 0x55)
	{
		begin_status++;
	} else if (x != 0x55 && begin_status == 1) //begin_status==2 means complete receive message
	{
		begin_status = 0;
	}
	if (begin_status > 1)
	{
		USB_printf("%c", 0x01);
		begin_status = 0;
	}
}

/* *********************************************
 * name:Control_light_device
 * function: Control light switch
 * variable: addh:Address high
 * 			 addl:Address low
 * 			 mode:Light switch mode
 * 			 uart:UART number
 * time:2020.6.1
 * Author: ZRH
 * *********************************************/
void Control_light_device(char addh, char addl, char mode, char uart)
{
	IWDG_Feed();
	char Cumulative_sum = 0x55 + 0x55 + 0x08 + 0x01 + 0x30 + 0x41 + 0x01 + addh + addl + mode;
	digitalWriteA(GPIO_Pin_1, HIGH);
	digitalWriteA(GPIO_Pin_8, HIGH);
	digitalWriteC(GPIO_Pin_4, HIGH);
	delay_us(1500);
	if (uart == 0x01)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x08);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x01);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x30);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x41);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x01);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, addh);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, addl);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, mode);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, Cumulative_sum);
	} else if (uart == 0x02)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x08);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x01);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x30);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x41);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x01);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, addh);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, addl);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, mode);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, Cumulative_sum);
	} else if (uart == 0x03)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x08);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x01);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x30);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x41);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x01);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, addh);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, addl);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, mode);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, Cumulative_sum);
	}

	delay_us(1500);
	digitalWriteA(GPIO_Pin_1, LOW);
	digitalWriteA(GPIO_Pin_8, LOW);
	digitalWriteC(GPIO_Pin_4, LOW);
}

/* *********************************************
 * name:Control_Scenes_device
 * function: Control light switch
 * variable:scence_num:Scene number
 * 			uart:UART number
 * time:2020.6.1
 * Author: ZRH
 * *********************************************/
void Control_Scenes_device(char scence_num, char uart)
{
	IWDG_Feed();
	char Cumulative_sum = 0x55 + 0x55 + 0x06 + 0x01 + 0x30 + 0x40 + 0x00 + scence_num;
	digitalWriteA(GPIO_Pin_1, HIGH);
	digitalWriteA(GPIO_Pin_8, HIGH);
	digitalWriteC(GPIO_Pin_4, HIGH);
	delay_us(1500);

	if (uart == 0x01)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x06);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x01);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x30);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x40);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x00);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, scence_num);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, Cumulative_sum);
	} else if (uart == 0x02)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x06);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x01);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x30);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x40);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x00);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, scence_num);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, Cumulative_sum);
	} else if (uart == 0x03)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x06);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x01);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x30);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x40);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x00);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, scence_num);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, Cumulative_sum);
	}

	delay_us(1500);
	digitalWriteA(GPIO_Pin_1, LOW);
	digitalWriteA(GPIO_Pin_8, LOW);
	digitalWriteC(GPIO_Pin_4, LOW);
}
/* *********************************************
 * name:Control_Curtain_device
 * function: Control music mode
 * variable��addh:Address high
 * 			 addl:Address low
 * 			 mode:Music switch mode
 * 			 uart:UART number
 * time:2020.6.1
 * Author: ZRH
 * *********************************************/
void Control_Curtain_device(char addh, char addl, char mode, char uart)
{
	IWDG_Feed();
	char Cumulative_sum = 0x55 + 0x55 + 0x08 + 0x01 + 0x30 + 0x43 + 0x03 + addh + addl + mode;
	digitalWriteA(GPIO_Pin_1, HIGH);
	digitalWriteA(GPIO_Pin_8, HIGH);
	digitalWriteC(GPIO_Pin_4, HIGH);
	delay_us(1500);

	if (uart == 0x01)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x08);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x01);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x30);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x43);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x03);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, addh);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, addl);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, mode);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, Cumulative_sum);
	} else if (uart == 0x02)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x08);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x01);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x30);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x43);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x03);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, addh);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, addl);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, mode);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, Cumulative_sum);
	} else if (uart == 0x03)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x08);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x01);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x30);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x43);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x03);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, addh);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, addl);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, mode);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, Cumulative_sum);
	}

	delay_us(1500);
	digitalWriteA(GPIO_Pin_1, LOW);
	digitalWriteA(GPIO_Pin_8, LOW);
	digitalWriteC(GPIO_Pin_4, LOW);
}
/* *********************************************
 * name:Control_Music_device
 * function: Control music mode
 * variable��addh:Address high
 * 			 addl:Address low
 * 			 mode:Music switch mode
 * 			 uart:UART number
 * time:2020.6.1
 * Author: ZRH
 * *********************************************/
void Control_Music_device(char addh, char addl, char mode, char uart)
{
	IWDG_Feed();
	char Cumulative_sum = 0x55 + 0x55 + 0x08 + 0x01 + 0x30 + 0x44 + 0x04 + addh + addl + mode;
	digitalWriteA(GPIO_Pin_1, HIGH);
	digitalWriteA(GPIO_Pin_8, HIGH);
	digitalWriteC(GPIO_Pin_4, HIGH);
	delay_us(1500);

	if (uart == 0x01)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x55);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x08);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x01);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x30);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x44);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, 0x04);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, addh);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, addl);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, mode);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, Cumulative_sum);
	} else if (uart == 0x02)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x55);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x08);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x01);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x30);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x44);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, 0x04);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, addh);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, addl);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, mode);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART2, Cumulative_sum);
	} else if (uart == 0x03)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x55);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x08);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x01);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x30);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x44);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, 0x04);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, addh);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, addl);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, mode);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, Cumulative_sum);
	}

	delay_us(1500);
	digitalWriteA(GPIO_Pin_1, LOW);
	digitalWriteA(GPIO_Pin_8, LOW);
	digitalWriteC(GPIO_Pin_4, LOW);
}


void Deal_port_1()											//别墅一楼(设备号0x01，串口1)
{
	if (port_1_write != port_1_read)
	{
		char temp[9] = { 0x55, 0x55, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x0A };
		int i = 0, j;
		if (port_receive_1[port_1_read][2] == 0x08)			//其他模式
		{
			temp[3] = 0xFF;
			temp[4] = port_receive_1[port_1_read][7];
			temp[5] = port_receive_1[port_1_read][8];
			temp[6] = port_receive_1[port_1_read][9];
		} else												//场景模式
		{
			temp[5] = port_receive_1[port_1_read][7];
		}
		for (j = 0; j < 9; j++)
		{
			USB_printf("%c", temp[j]);
		}
		for (j = 0; j < port_receive_size; j++)
		{
			port_receive_1[port_1_read][j] = 0x00;
		}
		port_1_read++;
		if (port_1_read == port_receive_num)
		{
			port_1_read = 0;
		}
	} else
	{
		return;
	}
}

void Deal_port_2()											//别墅二楼(设备号0x02,串口2)
{
	if (port_2_write != port_2_read)
	{
		char temp[9] = { 0x55, 0x55, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x0A };
		int i = 0, j;
		if (port_receive_2[port_2_read][2] == 0x08)			//其他模式
		{
			temp[3] = 0xFF;
			temp[4] = port_receive_2[port_2_read][7];
			temp[5] = port_receive_2[port_2_read][8];
			temp[6] = port_receive_2[port_2_read][9];
		} else												//场景模式
		{
			temp[5] = port_receive_2[port_2_read][7];
		}
		for (j = 0; j < 9; j++)
		{
			USB_printf("%c", temp[j]);
		}
		for (j = 0; j < port_receive_size; j++)
		{
			port_receive_2[port_2_read][j] = 0x00;
		}
		port_2_read++;
		if (port_2_read == port_receive_num)
		{
			port_2_read = 0;
		}
	} else
	{
		return;
	}
}

void Deal_port_3()											//别墅三楼(设备号0x03,串口3)
{
	if (port_3_write != port_3_read)
	{
		char temp[9] = { 0x55, 0x55, 0x03, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x0A };
		int i = 0, j;
		if (port_receive_3[port_3_read][2] == 0x08)			//其他模式
		{
			temp[3] = 0xFF;
			temp[4] = port_receive_3[port_3_read][7];
			temp[5] = port_receive_3[port_3_read][8];
			temp[6] = port_receive_3[port_3_read][9];
		} else												//场景模式
		{
			temp[5] = port_receive_3[port_3_read][7];
		}
		for (j = 0; j < 9; j++)
		{
			USB_printf("%c", temp[j]);
		}
		for (j = 0; j < port_receive_size; j++)
		{
			port_receive_3[port_3_read][j] = 0x00;
		}
		port_3_read++;
		if (port_3_read == port_receive_num)
		{
			port_3_read = 0;
		}
	} else
	{
		return;
	}
}
