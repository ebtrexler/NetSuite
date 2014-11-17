//*****************************************************************************
//
// serial.h : Declarations and definitions for serial data communications
//    support.
//
// Author:  Reed Bement 04/09/2004
//
//*****************************************************************************

#ifndef SERIAL_H
#define SERIAL_H

#define MAX_ERROR       255
#define MAX_PORTNAME    64
#define MAX_PORTS       256

// Serial error constants

#define BAD_PORT_NUM       0
#define BAD_BAUD_RATE      1
#define W32_COMM_ERR       2
#define COMM_ERR           3
#define SERIAL_UNINIT      4
#define READ_SIZE_ERR      5
#define BLOCK_TOO_BIG      6
#define LAST_SER_ERR       7

// The serial communications context.

typedef struct 
{
   DWORD          dwSize;
   DWORD          dwComPort;
   DWORD          dwBaudRate;
   DCB            dcb;
   HANDLE         hCom;
   DWORD          dwError;
   COMMTIMEOUTS   cto;
   char           szPortName[MAX_PORTNAME];
   char           szError[MAX_ERROR];
   DWORD          dwMaxBuffer;
} SERIAL_CONTEXT, *PSERIAL_CONTEXT; 


// Public function declarations

BOOL PACKAGE InitSerial(PSERIAL_CONTEXT psc, DWORD dwComPort, DWORD dwBaudRate);
void PACKAGE CloseSerial(PSERIAL_CONTEXT psc);
BOOL PACKAGE SendSerialByte(PSERIAL_CONTEXT psc, BYTE b);
BOOL PACKAGE RecvSerialByte(PSERIAL_CONTEXT psc, PBYTE pb, unsigned int wait = 1);
BOOL PACKAGE SendSerialBlock(PSERIAL_CONTEXT psc, PBYTE pb, DWORD dwByteCount);
BOOL PACKAGE RecvSerialBlock(PSERIAL_CONTEXT psc, PBYTE pb, DWORD dwByteCount, unsigned int wait = 1);
BOOL PACKAGE CheckCommStatus(PSERIAL_CONTEXT psc, PDWORD pdwRxCount, PDWORD pdwTxCount);

#endif //#define SERIAL_H
