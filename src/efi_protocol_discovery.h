// efi_protocol_discovery.h
#ifndef TINYUEFI_PROTOCOL_DISCOVERY_H
#define TINYUEFI_PROTOCOL_DISCOVERY_H

#include "uefi_types.h"

// GUID structure
typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} EFI_GUID;

// Protocol handler functions
typedef EFI_STATUS (*EFI_LOCATE_PROTOCOL)(
    EFI_GUID *Protocol,
    void *Registration,
    void **Interface
);

typedef EFI_STATUS (*EFI_LOCATE_HANDLE)(
    EFI_LOCATE_SEARCH_TYPE SearchType,
    EFI_GUID *Protocol,
    void *SearchKey,
    uint64_t *BufferSize,
    EFI_HANDLE *Buffer
);

typedef EFI_STATUS (*EFI_OPEN_PROTOCOL)(
    EFI_HANDLE Handle,
    EFI_GUID *Protocol,
    void **Interface,
    EFI_HANDLE AgentHandle,
    EFI_HANDLE ControllerHandle,
    uint32_t Attributes
);

typedef EFI_STATUS (*EFI_CLOSE_PROTOCOL)(
    EFI_HANDLE Handle,
    EFI_GUID *Protocol,
    EFI_HANDLE AgentHandle,
    EFI_HANDLE ControllerHandle
);

// Search types for LocateHandle
typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

// Protocol open attributes
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

EFI_STATUS LocateProtocol(EFI_GUID *Protocol, void **Interface);
EFI_STATUS LocateHandles(EFI_GUID *Protocol, EFI_HANDLE **HandleBuffer, uint64_t *HandleCount);
EFI_STATUS OpenProtocolOnHandle(EFI_HANDLE Handle, EFI_GUID *Protocol, void **Interface);
EFI_STATUS CloseProtocolOnHandle(EFI_HANDLE Handle, EFI_GUID *Protocol);
void PrintGUID(EFI_GUID *Guid);

#endif // TINYUEFI_PROTOCOL_DISCOVERY_H