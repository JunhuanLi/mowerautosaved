/*******************************************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: queue.c
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: queue 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...    
	
*******************************************************************************/

/* includes *******************************************************************/
#include "packet.h"
#include <string.h>
#include <stdio.h>

/* macros *********************************************************************/

/* static variables ***********************************************************/
#define	FRAME_HEARDER_SIZE		2
#define	COMMAND_ID_SIZE			1
#define BUFFER_LEN_SIZE			1
#define CRC_SIZE				2
#define DEBUG_PACKET			0


/*------------------------------------------------------------------------------
Function		:   createPacket
Description		:	createPacket
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void createPacket(TSerialPacket *packet, u8 cmd_id, char *buf, u8 len)
{
	clearPacket(packet);
	setPacketID(packet, cmd_id);

	if(buf != NULL && len > 0)
	{
		setPacketBuf(packet, buf, len);
	}
	else
	{
		// the packet only has cmd id
	}

	finalizePacket(packet);
}

/*------------------------------------------------------------------------------
Function		: 	setPacketBuf
Description		:	setPacketBuf
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void setPacketBuf(TSerialPacket * packet, char * buf, u8 len)
{
	if(hasWriteCapacity(packet, len))
	{
		memcpy(packet->buf + getHeaderLength(packet), buf, len);
		packet->length = getHeaderLength(packet) + len;
	}
	else
	{
		// it is out of range
	}
}

/*------------------------------------------------------------------------------
Function		: 	getBuf
Description		:	getBuf
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
char *getBuf(TSerialPacket * packet)
{
	return packet->buf;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 getLength(TSerialPacket * packet)
{
	return packet->length;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void setLength(TSerialPacket * packet, u8 length)
{
	packet->length = length;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 getDataLength(TSerialPacket * packet)
{
	return packet->length - getHeaderLength(packet) - getFooterLength(packet);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u16 getReadLength(TSerialPacket *packet)
{
	return packet->read_length;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 getHeaderLength(TSerialPacket * packet)
{
	return (FRAME_HEARDER_SIZE + COMMAND_ID_SIZE + BUFFER_LEN_SIZE);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 getFooterLength(TSerialPacket * packet)
{
	return (CRC_SIZE);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 getDataReadLength(TSerialPacket * packet)
{
	return packet->read_length - getHeaderLength(packet);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void resetRead(TSerialPacket *packet)
{
	packet->read_length = getHeaderLength(packet);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void setPacketValid(TSerialPacket * packet, bool valid)
{
	packet->valid = valid;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void clearPacket(TSerialPacket * packet)
{
	packet->length = getHeaderLength(packet);
	packet->read_length = getHeaderLength(packet);
	packet->valid = FALSE;
	memset(packet->buf, 0, sizeof(char) * MAX_LENGTH);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void appendPacket(TSerialPacket * packet, char * buf, u8 size)
{
	if(!hasWriteCapacity(packet, size))
	{
		return;
	}

	memcpy(packet->buf + packet->length, buf, size);
	packet->length +=size;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void byteToBuf(TSerialPacket * packet, s8 val)
{
	appendPacket(packet, (char *)&val, 1);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void byte2ToBuf(TSerialPacket * packet, s16 val)
{
	appendPacket(packet, (char *)&val, 2);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void byte4ToBuf(TSerialPacket * packet, s32 val)
{
	appendPacket(packet, (char *)&val, 4);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void uByteToBuf(TSerialPacket * packet, u8 val)
{
	appendPacket(packet, (char *)&val, 1);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void uByte2ToBuf(TSerialPacket * packet, u16 val)
{
	appendPacket(packet, (char *)&val, 2);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void uByte4ToBuf(TSerialPacket * packet, u32 val)
{
	appendPacket(packet, (char *)&val, 4);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
bool readPacket(TSerialPacket * packet, char * data, u8 size)
{
	if(data == NULL)
	{
		return FALSE;
	}

	if(isNextGood(packet, size))
	{
		memcpy(data, packet->buf + packet->read_length, size);
		packet->read_length += size;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}	

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
s8 bufToByte(TSerialPacket * packet)
{
	s8 ret = 0;
#if DEBUG_PACKET
	if(isNextGood(packet, 1))
	{
		memcpy(&ret, packet->buf + packet->read_length, 1);
		packet->read_length += 1;
	}
#else
	readPacket(packet, (char*)&ret, 1);
#endif
	return ret;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
s16 bufToByte2(TSerialPacket * packet)
{
	s16 ret = 0;
#if DEBUG_PACKET
	unsigned char c1, c2;

	if(isNextGood(packet, 2))
	{
		memcpy(&c1, packet->buf + packet->read_length, 1);
		memcpy(&c2, packet->buf + packet->read_length+1, 1);
		ret = (c1 && 0xff) | (c2 << 8);
		packet->read_length += 2;
	}
#else
	readPacket(packet, (char*)&ret, 2);
#endif
	return ret;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
s32 bufToByte4(TSerialPacket * packet)
{
	s32 ret = 0;
#if DEBUG_PACKET
	unsigned char c1, c2, c3, c4;
	
	if(isNextGood(packet, 4))
	{
		memcpy(&c1, packet->buf + packet->read_length, 1);
		memcpy(&c2, packet->buf + packet->read_length+1, 1);
		memcpy(&c3, packet->buf + packet->read_length+2, 1);
		memcpy(&c4, packet->buf + packet->read_length+3, 1);
		ret = (c1 && 0xff) | (c2 << 8) | c3 << 16 | c4 << 24;
		packet->read_length += 4;
	}
#else
	readPacket(packet, (char*)&ret, 4);
#endif
	return ret;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 bufToUByte(TSerialPacket * packet)
{
	u8 ret = 0;
#if DEBUG_PACKET
	if(isNextGood(packet, 1))
	{
		memcpy(&ret, packet->buf + packet->read_length, 1);
		packet->read_length += 1;
	}
#else
	readPacket(packet, (char*)&ret, 1);
#endif
	return ret;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u16 bufToUByte2(TSerialPacket * packet)
{
	u16 ret = 0;
#if DEBUG_PACKET
	unsigned char c1, c2;

	if(isNextGood(packet, 2))
	{
		memcpy(&c1, packet->buf + packet->read_length, 1);
		memcpy(&c2, packet->buf + packet->read_length+1, 1);
		ret = (c1 && 0xff) | (c2 << 8);
		packet->read_length += 2;
	}
#else
	readPacket(packet, (char*)&ret, 2);
#endif
	return ret;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u32 bufToUByte4(TSerialPacket * packet)
{
	u32 ret = 0;
#if DEBUG_PACKET
	unsigned char c1, c2, c3, c4;
	
	if(isNextGood(packet, 4))
	{
		memcpy(&c1, packet->buf + packet->read_length, 1);
		memcpy(&c2, packet->buf + packet->read_length+1, 1);
		memcpy(&c3, packet->buf + packet->read_length+2, 1);
		memcpy(&c4, packet->buf + packet->read_length+3, 1);
		ret = (c1 && 0xff) | (c2 << 8) | c3 << 16 | c4 << 24;
		packet->read_length += 4;
	}
#else
	readPacket(packet, (char*)&ret, 4);
#endif
	return ret;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
bool isNextGood(TSerialPacket * packet, int bytes)
{
	if(bytes <= 0)
	{
		return FALSE;
	}

	if(packet->read_length + bytes <= packet->length - getFooterLength(packet))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
bool hasWriteCapacity(TSerialPacket * packet, int bytes)
{
	if(bytes < 0)
	{
		return FALSE;
	}
	
	if(packet->length + bytes <= MAX_LENGTH)
	{
		return TRUE;
	}
	else
	{
		packet->valid = FALSE;
		return FALSE;
	}
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
bool isValid(TSerialPacket * packet)
{
	return packet->valid;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 getPacketID(TSerialPacket * packet)
{
	if(packet->length >= 4)
	{
		return packet->buf[3];
	}
	else
	{
		return 255;//CMD_END(255)
	}
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void setPacketID(TSerialPacket * packet, u8 id)
{
	packet->buf[3] = id;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
bool veryfyCheckSum(TSerialPacket * packet)
{
	unsigned short chksum;
	unsigned char c1,c2;

	if(packet->length - 2 < getHeaderLength(packet))
	{
		return FALSE;
	}

	c2 = packet->buf[packet->length - 2];
	c1 = packet->buf[packet->length - 1];

	chksum = c1 | c2 << 8;

	if(chksum == calcCheckSum(packet))
		{
		return TRUE;
	}
	else
		{
		return FALSE;
	}
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
u16 calcCheckSum(TSerialPacket * packet)
{
	int i;
	unsigned char n;
	unsigned short c = 0;

	i = 3;
	n = packet->buf[2] -2;

	while(n > 1)
	{
		c += ((unsigned char)packet->buf[i] << 8 | (unsigned char)packet->buf[i+1]);
		c = c & 0xffff;
		n = n -2;
		i = i + 2;
	}

	if(n > 0)
	{
		c = c ^ (unsigned short)((unsigned char)packet->buf[i]);
	}
	return c;
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void finalizePacket(TSerialPacket * packet)
{
	s16 check_sum;
	s8 c1,c2;

	packet->buf[0] = (u8)(SYNC1);
	packet->buf[1] = (u8)(SYNC2);
	packet->buf[2] = (u8)(packet->length - getHeaderLength(packet) + COMMAND_ID_SIZE + CRC_SIZE);

	check_sum =calcCheckSum(packet);

	c1 = (check_sum >> 8) & 0xff;
	c2 = check_sum;

	byteToBuf(packet, c1);
	byteToBuf(packet, c2);

	setPacketValid(packet, TRUE);
}

/*------------------------------------------------------------------------------
Function		: 
Description		:
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
TSerialPacket copyPacket(TSerialPacket * packet)
{
	TSerialPacket temp;
	temp.valid = packet->valid;
	temp.length = packet->length;
	temp.read_length = packet->read_length;
	memcpy(&temp.buf, &packet->buf, packet->length);
	return temp;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
