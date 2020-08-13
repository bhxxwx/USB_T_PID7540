/*
 * Servers.h
 *
 *  Created on: 2020年4月26日
 *      Author: WangXiang
 */

#ifndef SERVERS_H_
#define SERVERS_H_
#include "UserConfig.h"
#include "DATAstruct/DATAstruct.h"



void Get_USB_Message(unsigned char* buffer);
void Control_light_device(char addh, char addl, char mode, char uart);
void Control_Scenes_device(char scence_num, char uart);
void Control_Curtain_device(char addh, char addl, char mode,char uart);
void Control_Music_device(char addh, char addl, char mode, char uart);
void Deal_port_1();
void Deal_port_2();


#endif /* SERVERS_H_ */
