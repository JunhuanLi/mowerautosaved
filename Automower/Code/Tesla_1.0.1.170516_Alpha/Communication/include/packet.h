/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	main.h
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/13
Description		:	main function
*******************************************************************************/

#ifndef _PACKET_H_
#define _PACKET_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"
//#include "packet.h"
//#include "protocol.h"
#include <stdbool.h>


/* macros *********************************************************************/
#define MAX_LENGTH		100						
#define	SYNC1			0Xfa			
#define SYNC2			0Xfb			

typedef enum
{
	STATE_SYNC1 = 0,
	STATE_SYNC2,
	STATE_LENGTH,
	STATE_ACQUIRE_DATA
}TSerialState;

typedef struct
{
	char buf[MAX_LENGTH];
	u16 read_length;
	u16	length;
	bool valid;
}TSerialPacket;


void createPacket(TSerialPacket *packet, u8 cmd_id, char *buf, u8 len);
void setPacketBuf(TSerialPacket *packet, char *buf, u8 len);
void clearPacket(TSerialPacket *packet);
char *getBuf(TSerialPacket *packet);

TSerialPacket copyPacket(TSerialPacket *packet);


u8 getLength(TSerialPacket *packet);
u8 getDataLength(TSerialPacket *packet);
u8 getFooterLength(TSerialPacket *packet);
u8 getHeaderLength(TSerialPacket *packet);
u8 getDataReadLength(TSerialPacket *packet);


void setPacketValid(TSerialPacket *packet, bool valid);
bool isValid(TSerialPacket *packet);

u8 getPacketID(TSerialPacket *packet);
void setPacketID(TSerialPacket *packet, u8 id);

void setLength(TSerialPacket *packet, u8 length);

u16 calcCheckSum(TSerialPacket *packet);
bool veryfyCheckSum(TSerialPacket *packet);

bool isNextGood(TSerialPacket *packet, int bytes);
bool hasWriteCapacity(TSerialPacket *packet, int bytes);
void finalizePacket(TSerialPacket *packet);


void appendPacket(TSerialPacket *packet, char *buf, u8 size);
void byteToBuf(TSerialPacket *packet, s8 val);
void byte2ToBuf(TSerialPacket * packet, s16 val);
void byte4ToBuf(TSerialPacket * packet, s32 val);


void uByteToBuf(TSerialPacket *packet, u8 val);
void uByte2ToBuf(TSerialPacket * packet, u16 val);
void uByte4ToBuf(TSerialPacket * packet, u32 val);



bool readPacket(TSerialPacket *packet, char *data, u8 size);
s8 bufToByte(TSerialPacket *packet);
s16 bufToByte2(TSerialPacket *packet);
s32 bufToByte4(TSerialPacket *packet);

u8 bufToUByte(TSerialPacket *packet);
u16 bufToUByte2(TSerialPacket *packet);
u32 bufToUByte4(TSerialPacket *packet);

void bufToStr(TSerialPacket *packet,  char *buf, int len);


#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
