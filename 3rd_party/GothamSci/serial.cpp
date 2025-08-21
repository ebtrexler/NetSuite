//*****************************************************************************
//
// serial.cpp : Implementation of simple serial port data communications.
//
// Author:  Reed Bement 04/09/2004
//
//    Notes:
//    ----
//
//    Simple polling based serial data communications with no event or
//    handshaking. 
//
//*****************************************************************************

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "serial.h"

#pragma package(smart_init)

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************

DWORD g_dwValidBaudRates[] = 
{ 
   CBR_110,    CBR_300,    CBR_600,   CBR_1200,  CBR_2400,   CBR_4800,   
   CBR_9600,   CBR_14400,  CBR_19200, CBR_38400, CBR_56000,  CBR_57600, 
   CBR_115200, CBR_128000, CBR_256000
};

//*****************************************************************************
//
// MapComErrorToString - .
//
// Paramaters:
//
//    pszErr   - Pointer to string to receive error string.
//    dwSize   - Size of receiving string buffer in characters.
//    dwErr    - Error mask from ClearCommError.
//
// Returns - Nothing.
//
//*****************************************************************************

void MapComErrorToString(PSZ pszErr, DWORD dwSize, DWORD dwErr)
{
   #define MAX_COM_ERR_STR 16
   
   pszErr[0] = '\0';

   if (dwSize > MAX_COM_ERR_STR) {
      if (dwErr & CE_BREAK)      strcpy(pszErr, "Break ");
      if (dwErr & CE_DNS)        strcat(pszErr, "Parallel DNS ");
      if (dwErr & CE_FRAME)      strcat(pszErr, "Frame ");
      if (dwErr & CE_IOE)        strcat(pszErr, "I/O Error ");
      if (dwErr & CE_MODE)       strcat(pszErr, "Mode ");
      if (dwErr & CE_OOP)        strcat(pszErr, "Paper ");
      if (dwErr & CE_OVERRUN)    strcat(pszErr, "Buffer Overrun ");
      if (dwErr & CE_PTO)        strcat(pszErr, "Parallel TO ");
      if (dwErr & CE_RXOVER)     strcat(pszErr, "Receive Overrun ");
      if (dwErr & CE_RXPARITY)   strcat(pszErr, "Parity ");
      if (dwErr & CE_TXFULL)     strcat(pszErr, "Transmit Full ");
   }
   else {
      strncpy(pszErr, "pszErr too small", dwSize);
      pszErr[dwSize-1] = '\0';
   }
}

//*****************************************************************************
//
// SetSerialError - Set an error in the serial context.
//
// Paramaters:
//
//    psc       - Pointer to the ser comm. context.
//    dwError   - Error number to set.
//
// Returns - Nothing.
//
//*****************************************************************************

void SetSerialError(PSERIAL_CONTEXT psc, DWORD dwError, PSZ pszFuncName, DWORD dwParam)
{
   char pszErr[64];
   
   psc->dwError = dwError;
   
   switch (dwError) {
      
      case BAD_PORT_NUM:
		 sprintf(psc->szError, "Bad COM port requested: %d", psc->dwComPort);
         break;

      case BAD_BAUD_RATE:
         sprintf(psc->szError, "Bad baud rate requested: %d", psc->dwBaudRate);
         break;

      case W32_COMM_ERR: {
         DWORD dwLastErr = GetLastError();

         sprintf(psc->szError, "Error calling %s on COM%d:, LastError: %d, 0x%X", pszFuncName, psc->dwComPort, dwLastErr, dwLastErr);
         break;
      }

      case COMM_ERR: {
         MapComErrorToString(pszErr, 64, dwParam);
         sprintf(psc->szError, "Comm error: %s detected during: %s", pszErr, pszFuncName);
         break;
      }
   
      case SERIAL_UNINIT:
         sprintf(psc->szError, "Serial context not initialized during: %s", pszFuncName);
         break;
      
      case READ_SIZE_ERR: {
         sprintf(psc->szError, "ReadFile bytes read doesn't match requested: %d", dwParam);
         break;
      }

      case BLOCK_TOO_BIG: {
         sprintf(psc->szError, "Request block larger than driver buffer: %d", dwParam);
         break;
      }
  
      default:
         sprintf(psc->szError, "Unknown serial error: %d", dwError);
   }
}

//*****************************************************************************
//
// CheckCommStatus - Check the status of the comm port.
//
// Paramaters:
//
//    psc          - Pointer to the ser comm. context.
//    pdwRxCount   - Pointer to DWORD to receive buffer count.
//    pdwTxCount   - Pointer to DWORD to transmit buffer count.
//
// Returns - TRUE on success, FALSE on failure.
//
//*****************************************************************************

BOOL CheckCommStatus(PSERIAL_CONTEXT psc, PDWORD pdwRxCount, PDWORD pdwTxCount)
{
   if (pdwRxCount) *pdwRxCount = 0;
   if (pdwTxCount) *pdwTxCount = 0;

   if (psc->hCom != INVALID_HANDLE_VALUE) {
      
      DWORD       dwErrCode;
      COMSTAT     cs;

      if (ClearCommError(psc->hCom, &dwErrCode, &cs)) {

         if (!dwErrCode) {

            if (pdwRxCount) *pdwRxCount = cs.cbInQue;
            if (pdwTxCount) *pdwTxCount = cs.cbOutQue;

            return TRUE;
         }
         else {

            PurgeComm(psc->hCom, PURGE_RXCLEAR | PURGE_TXCLEAR);
      
            SetSerialError(psc, COMM_ERR, "CheckCommStatus", dwErrCode);
         }
      }
      else {
         SetSerialError(psc, W32_COMM_ERR, "ClearCommError", 0);
      }
   }
   else {
      SetSerialError(psc, SERIAL_UNINIT, "CheckCommStatus", 0);
   }
   return FALSE;
}

//*****************************************************************************
//
// InitSerial - Initialize context and open a serial port.
//
// Paramaters:
//
//    psc         - Pointer to the ser comm. context.
//    dwComPort   - Port number up to MAX_PORTS.
//    dwBaudRate  - May any one of the CBR_* constant baud rate values.
//
// Returns - TRUE on success, FALSE on failure.
//
//*****************************************************************************

BOOL InitSerial(PSERIAL_CONTEXT psc, DWORD dwComPort, DWORD dwBaudRate)
{
   // Clear context.
   memset(psc, 0, sizeof(PSERIAL_CONTEXT));

   psc->dwSize       = sizeof(PSERIAL_CONTEXT);
   psc->hCom         = INVALID_HANDLE_VALUE;
   psc->dwComPort    = dwComPort;
   psc->dwBaudRate   = dwBaudRate;

   // Parameter validation
   if ((dwComPort > 0) && (dwComPort <= MAX_PORTS)) {

      BOOL bFound = FALSE;
      
	  for (unsigned int i = 0; i < (sizeof(g_dwValidBaudRates)/sizeof(DWORD)); i++) {
         if (g_dwValidBaudRates[i] == dwBaudRate) {
            bFound = TRUE;
            break;
         }
      }
      if (!bFound) {
         SetSerialError(psc, BAD_BAUD_RATE, 0, 0);
         return FALSE;
      }
   }
   else {
      SetSerialError(psc, BAD_PORT_NUM, 0, 0);
      return FALSE;
   }
   
//   sprintf(psc->szPortName, "COM%d", dwComPort);
   sprintf(psc->szPortName, "\\\\.\\COM%d", dwComPort);   // ebt 7/2012 per http://support.microsoft.com/?id=115831

   psc->hCom = CreateFile(UnicodeString(psc->szPortName).c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0,         
                          NULL,      
                          OPEN_EXISTING,
                          0,
                          NULL);

   if (psc->hCom != INVALID_HANDLE_VALUE) {
   
      // Omit the call to SetupComm to use the default queue sizes.
   
      // Get the current configuration.
   
      if (GetCommState(psc->hCom, &psc->dcb)) {
         
         // Fill in the DCB.
      
         psc->dcb.BaudRate         = psc->dwBaudRate;
         psc->dcb.ByteSize         = 8;
         psc->dcb.Parity           = NOPARITY;
         psc->dcb.StopBits         = ONESTOPBIT;
         psc->dcb.fAbortOnError    = 1;
         psc->dcb.fDsrSensitivity  = 0;
         psc->dcb.fDtrControl      = 0;
         psc->dcb.fOutxCtsFlow     = 0;
         psc->dcb.fOutxDsrFlow     = 0;
         psc->dcb.fRtsControl      = 0;

         if (SetCommState(psc->hCom, &psc->dcb)) {

            // Set timeouts
         
            if (GetCommTimeouts(psc->hCom, &psc->cto)) {
         
               psc->cto.ReadIntervalTimeout        = 2000;
               psc->cto.ReadTotalTimeoutConstant   = 2000;
               psc->cto.ReadTotalTimeoutMultiplier = 10;
         
               if (SetCommTimeouts(psc->hCom, &psc->cto)) {

                  if (SetCommMask(psc->hCom, 0)) {

                     COMMPROP cp;

                     if (GetCommProperties(psc->hCom, &cp)) {

                        psc->dwMaxBuffer = cp.dwCurrentRxQueue;

                        return TRUE;
                     }
                     else {
                        SetSerialError(psc, W32_COMM_ERR, "GetCommProperties", 0);
                     }
                  }
                  else {
                     SetSerialError(psc, W32_COMM_ERR, "SetCommMask", 0);
                  }
               }
               else {
                  SetSerialError(psc, W32_COMM_ERR, "SetCommTimeouts", 0);
               }
            }
            else {
               SetSerialError(psc, W32_COMM_ERR, "GetCommTimeouts", 0);
            }
         }
         else {
            SetSerialError(psc, W32_COMM_ERR, "SetCommState", 0);
         }
      }
      else {
         SetSerialError(psc, W32_COMM_ERR, "GetCommState", 0);
      }
      CloseHandle(psc->hCom);
   }
   else {
      SetSerialError(psc, W32_COMM_ERR, "CreateFile", 0);
   }
   return FALSE;
}

//*****************************************************************************
//
// CloseSerial - Close the serial port and clear the context.
//
// Paramaters:
//
//    psc - Pointer to the ser comm. context.
//
// Returns - Nothing.
//
//*****************************************************************************

void CloseSerial(PSERIAL_CONTEXT psc)
{
   if (psc->hCom != INVALID_HANDLE_VALUE) {
      CloseHandle(psc->hCom);
   }

   // Clear context.
   memset(psc, 0, sizeof(PSERIAL_CONTEXT));

   psc->dwSize = sizeof(PSERIAL_CONTEXT);
   psc->hCom   = INVALID_HANDLE_VALUE;
}

//*****************************************************************************
//
// SendSerialByte - Send a byte out the serial port. Blocks.
//
// Paramaters:
//
//    psc - Pointer to the ser comm. context.
//    b   - Byte to be sent.
//
// Returns - TRUE on success, FALSE on failure.
//
//*****************************************************************************

BOOL SendSerialByte(PSERIAL_CONTEXT psc, BYTE b)
{
   return SendSerialBlock(psc, &b, 1);
}

//*****************************************************************************
//
// RecvSerialByte - Receive a byte from the serial port. Blocks.
//
// Paramaters:
//
//    psc - Pointer to the ser comm. context.
//    pb  - Pointer to location for received byte.
//
// Returns - TRUE on success, FALSE on failure.
//
//*****************************************************************************

BOOL RecvSerialByte(PSERIAL_CONTEXT psc, PBYTE pb, unsigned int wait)
{
   return RecvSerialBlock(psc, pb, 1, wait);
}

//*****************************************************************************
//
// SendSerialBlock - Send a block of bytes to the serial port. Blocks.
//
// Paramaters:
//
//    psc         - Pointer to the ser comm. context.
//    pb          - Pointer to block.
//    dwByteCount - Number of bytes to send.
//
// Returns - TRUE on success, FALSE on failure.
//
//*****************************************************************************

BOOL SendSerialBlock(PSERIAL_CONTEXT psc, PBYTE pb, DWORD dwByteCount)
{
   if (psc->hCom != INVALID_HANDLE_VALUE) {
      
      if (dwByteCount <= psc->dwMaxBuffer) {
         
         DWORD dwWritten;

         if (WriteFile(psc->hCom, pb, dwByteCount, &dwWritten, NULL)) {
            return TRUE;
         }
         else {
   
            // See if there's a Comm error, CheckCommStatus will SetSerialError if so.
            
            if (CheckCommStatus(psc, NULL, NULL)) {
   
               SetSerialError(psc, W32_COMM_ERR, "WriteFile", 0);
            }
         }
      }
      else {
         SetSerialError(psc, BLOCK_TOO_BIG, "RecvSerialBlock", psc->dwMaxBuffer);
      }
   }
   else {
      SetSerialError(psc, SERIAL_UNINIT, "SendSerialBlock", 0);
   }
   return FALSE;
}

//*****************************************************************************
//
// RecvSerialBlock - Receive a block of bytes from the serial port. Blocks.
//
// Paramaters:
//
//    psc         - Pointer to the ser comm. context.
//    pb          - Pointer to block.
//    dwByteCount - Number of bytes to send.
//
// Returns - TRUE on success, FALSE on failure.
//
//*****************************************************************************

BOOL RecvSerialBlock(PSERIAL_CONTEXT psc, PBYTE pb, DWORD dwByteCount, unsigned int wait)
{
   if (psc->hCom != INVALID_HANDLE_VALUE) {
      
      if (dwByteCount <= psc->dwMaxBuffer) {
         
         DWORD dwRecvCount;

//         DWORD dwHardCodedTimeout = 10; //ebt added, to catch when no communication
         DWORD dwTimeOutCount = 0;
   
         // Check to see if there is any data waiting in the buffer.
   
         while (CheckCommStatus(psc, &dwRecvCount, NULL)) {

            if (dwRecvCount) {
               
               DWORD dwRead;
               
               if (ReadFile(psc->hCom, pb, dwByteCount, &dwRead, NULL)) {
         
                  if (dwByteCount == dwRead) {
                     return TRUE;
                  }
                  else {
                     SetSerialError(psc, READ_SIZE_ERR, "RecvSerialBlock", dwByteCount);
                  }
               }
               else {
                  SetSerialError(psc, W32_COMM_ERR, "ReadFile", 0);
               }
               break;
            }
            else {
               if (wait > 0){  // SET WAIT = -1 FOR NO TIMEOUT
                  dwTimeOutCount++;
				  if (dwTimeOutCount >= wait) return false;
                  // No data ready, wait 1ms.
                  Sleep(1);
               }
            }
         }
      }
      else {
         SetSerialError(psc, BLOCK_TOO_BIG, "RecvSerialBlock", psc->dwMaxBuffer);
      }
   }
   else {
      SetSerialError(psc, SERIAL_UNINIT, "RecvSerialBlock", 0);
   }
   return FALSE;
}

