#include <ntddndis.h>

#ifndef __NTDDPACKET
#define __NTDDPACKET 1

#include <devioctl.h>

#include <packon.h>

typedef struct _PACKET_OID_DATA {

    ULONG           Oid;
    ULONG           Length;
    ULONG           Data[10];
	UCHAR			StringData[256];

}   PACKET_OID_DATA, *PPACKET_OID_DATA;

#include <packoff.h>

#define FILE_DEVICE_PROTOCOL        0x8000



#define IOCTL_PROTOCOL_SET_OID      CTL_CODE(FILE_DEVICE_PROTOCOL, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_QUERY_OID    CTL_CODE(FILE_DEVICE_PROTOCOL, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PROTOCOL_RESET        CTL_CODE(FILE_DEVICE_PROTOCOL, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif
