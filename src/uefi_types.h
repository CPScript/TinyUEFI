#ifndef TINYUEFI_TYPES_H
#define TINYUEFI_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Define char16_t if not available (for older C standards)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#include <uchar.h>  // C11 and later
#else
typedef uint16_t char16_t;
#endif

// Basic UEFI types
typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void *EFI_EVENT;
typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef uint64_t EFI_VIRTUAL_ADDRESS;
typedef void *EFI_DEVICE_PATH_PROTOCOL;

// GUID structure
typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} EFI_GUID;

// Time structure
typedef struct {
    uint16_t Year;
    uint8_t  Month;
    uint8_t  Day;
    uint8_t  Hour;
    uint8_t  Minute;
    uint8_t  Second;
    uint8_t  Pad1;
    uint32_t Nanosecond;
    int16_t  TimeZone;
    uint8_t  Daylight;
    uint8_t  Pad2;
} EFI_TIME;

// IP address structures
typedef struct {
    uint8_t Addr[4];
} EFI_IPv4_ADDRESS;

typedef struct {
    uint8_t Addr[16];
} EFI_IPv6_ADDRESS;

typedef union {
    EFI_IPv4_ADDRESS v4;
    EFI_IPv6_ADDRESS v6;
} EFI_IP_ADDRESS;

// Memory types
typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

// Search types for LocateHandle
typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

// Table header
typedef struct {
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserved;
} EFI_TABLE_HEADER;

// Forward declarations
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;

// Input key data
typedef struct {
    uint16_t ScanCode;
    char16_t UnicodeChar;
} EFI_INPUT_KEY;

// Text input protocol
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_STATUS (*Reset)(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, bool ExtendedVerification);
    EFI_STATUS (*ReadKeyStroke)(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, EFI_INPUT_KEY *Key);
    EFI_EVENT WaitForKey;
};

// Text output mode
typedef struct {
    int32_t MaxMode;
    int32_t Mode;
    int32_t Attribute;
    int32_t CursorColumn;
    int32_t CursorRow;
    bool CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

// Text output protocol
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_STATUS (*Reset)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, bool ExtendedVerification);
    EFI_STATUS (*OutputString)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, const char16_t *String);
    EFI_STATUS (*TestString)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, const char16_t *String);
    EFI_STATUS (*QueryMode)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, uint64_t ModeNumber, 
                           uint64_t *Columns, uint64_t *Rows);
    EFI_STATUS (*SetMode)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, uint64_t ModeNumber);
    EFI_STATUS (*SetAttribute)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, uint64_t Attribute);
    EFI_STATUS (*ClearScreen)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
    EFI_STATUS (*SetCursorPosition)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, 
                                   uint64_t Column, uint64_t Row);
    EFI_STATUS (*EnableCursor)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, bool Visible);
    SIMPLE_TEXT_OUTPUT_MODE *Mode;
};

// System table
typedef struct {
    EFI_TABLE_HEADER Hdr;
    char16_t *FirmwareVendor;
    uint32_t FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    EFI_RUNTIME_SERVICES *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
    uint64_t NumberOfTableEntries;
    void *ConfigurationTable;
} EFI_SYSTEM_TABLE;

// Memory allocation function types
typedef EFI_STATUS (*EFI_ALLOCATE_POOL)(
    EFI_MEMORY_TYPE PoolType,
    uint64_t Size,
    void **Buffer
);

typedef EFI_STATUS (*EFI_FREE_POOL)(
    void *Buffer
);

typedef EFI_STATUS (*EFI_WAIT_FOR_EVENT)(
    uint64_t NumberOfEvents,
    EFI_EVENT *Event,
    uint64_t *Index
);

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

// Protocol open attributes
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

// Minimal boot services (only what we need)
struct _EFI_BOOT_SERVICES {
    EFI_TABLE_HEADER Hdr;
    
    // Task Priority Services
    void *RaiseTPL;
    void *RestoreTPL;
    
    // Memory Services
    void *AllocatePages;
    void *FreePages;
    void *GetMemoryMap;
    EFI_ALLOCATE_POOL AllocatePool;
    EFI_FREE_POOL FreePool;
    
    // Event & Timer Services
    void *CreateEvent;
    void *SetTimer;
    EFI_WAIT_FOR_EVENT WaitForEvent;
    void *SignalEvent;
    void *CloseEvent;
    void *CheckEvent;
    
    // Protocol Handler Services
    void *InstallProtocolInterface;
    void *ReinstallProtocolInterface;
    void *UninstallProtocolInterface;
    void *HandleProtocol;
    void *Reserved;
    void *RegisterProtocolNotify;
    void *LocateHandle;
    void *LocateDevicePath;
    void *InstallConfigurationTable;
    
    // Image Services
    void *LoadImage;
    void *StartImage;
    void *Exit;
    void *UnloadImage;
    void *ExitBootServices;
    
    // Miscellaneous Services
    void *GetNextMonotonicCount;
    void *Stall;
    void *SetWatchdogTimer;
    
    // DriverSupport Services
    void *ConnectController;
    void *DisconnectController;
    
    // Open and Close Protocol Services
    void *OpenProtocol;
    void *CloseProtocol;
    void *OpenProtocolInformation;
    
    // Library Services
    void *ProtocolsPerHandle;
    void *LocateHandleBuffer;
    void *LocateProtocol;
    void *InstallMultipleProtocolInterfaces;
    void *UninstallMultipleProtocolInterfaces;
    
    // 32-bit CRC Services
    void *CalculateCrc32;
    
    // Miscellaneous Services
    void *CopyMem;
    void *SetMem;
    void *CreateEventEx;
};

// UEFI Status codes
#define EFI_SUCCESS                     0x0000000000000000
#define EFI_LOAD_ERROR                  0x8000000000000001
#define EFI_INVALID_PARAMETER           0x8000000000000002
#define EFI_UNSUPPORTED                 0x8000000000000003
#define EFI_BAD_BUFFER_SIZE             0x8000000000000004
#define EFI_BUFFER_TOO_SMALL            0x8000000000000005
#define EFI_NOT_READY                   0x8000000000000006
#define EFI_DEVICE_ERROR                0x8000000000000007
#define EFI_WRITE_PROTECTED             0x8000000000000008
#define EFI_OUT_OF_RESOURCES            0x8000000000000009
#define EFI_VOLUME_CORRUPTED            0x800000000000000A
#define EFI_VOLUME_FULL                 0x800000000000000B
#define EFI_NO_MEDIA                    0x800000000000000C
#define EFI_MEDIA_CHANGED               0x800000000000000D
#define EFI_NOT_FOUND                   0x800000000000000E
#define EFI_ACCESS_DENIED               0x800000000000000F
#define EFI_TIMEOUT                     0x8000000000000010

// Text Output defines
#define EFI_BLACK                       0x00
#define EFI_BLUE                        0x01
#define EFI_GREEN                       0x02
#define EFI_CYAN                        0x03
#define EFI_RED                         0x04
#define EFI_MAGENTA                     0x05
#define EFI_BROWN                       0x06
#define EFI_LIGHTGRAY                   0x07
#define EFI_DARKGRAY                    0x08
#define EFI_LIGHTBLUE                   0x09
#define EFI_LIGHTGREEN                  0x0A
#define EFI_LIGHTCYAN                   0x0B
#define EFI_LIGHTRED                    0x0C
#define EFI_LIGHTMAGENTA                0x0D
#define EFI_YELLOW                      0x0E
#define EFI_WHITE                       0x0F

#define EFI_BACKGROUND_BLACK            0x00
#define EFI_BACKGROUND_BLUE             0x10
#define EFI_BACKGROUND_GREEN            0x20
#define EFI_BACKGROUND_CYAN             0x30
#define EFI_BACKGROUND_RED              0x40
#define EFI_BACKGROUND_MAGENTA          0x50
#define EFI_BACKGROUND_BROWN            0x60
#define EFI_BACKGROUND_LIGHTGRAY        0x70

#define EFI_ERROR(Status)               ((int64_t)(Status) < 0)

#endif
