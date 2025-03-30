// efi_file_protocol.h
#ifndef TINYUEFI_FILE_PROTOCOL_H
#define TINYUEFI_FILE_PROTOCOL_H

#include "uefi_types.h"  // Core UEFI types

// File system information structure
typedef struct {
    uint64_t Size;
    uint64_t ReadOnly;
    uint64_t VolumeSize;
    uint64_t FreeSpace;
    uint32_t BlockSize;
    char16_t VolumeLabel[100];
} EFI_FILE_SYSTEM_INFO;

// File information structure
typedef struct {
    uint64_t Size;
    uint64_t FileSize;
    uint64_t PhysicalSize;
    EFI_TIME CreateTime;
    EFI_TIME LastAccessTime;
    EFI_TIME ModificationTime;
    uint64_t Attribute;
    char16_t FileName[];
} EFI_FILE_INFO;

// File attributes
#define EFI_FILE_READ_ONLY            0x0000000000000001ULL
#define EFI_FILE_HIDDEN               0x0000000000000002ULL
#define EFI_FILE_SYSTEM               0x0000000000000004ULL
#define EFI_FILE_RESERVED             0x0000000000000008ULL
#define EFI_FILE_DIRECTORY            0x0000000000000010ULL
#define EFI_FILE_ARCHIVE              0x0000000000000020ULL
#define EFI_FILE_VALID_ATTR           0x0000000000000037ULL

// File open modes
#define EFI_FILE_MODE_READ            0x0000000000000001ULL
#define EFI_FILE_MODE_WRITE           0x0000000000000002ULL
#define EFI_FILE_MODE_CREATE          0x8000000000000000ULL

// Forward declarations
typedef struct _EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

// File protocol function types
typedef EFI_STATUS (*EFI_FILE_OPEN)(
    EFI_FILE_PROTOCOL *This,
    EFI_FILE_PROTOCOL **NewHandle,
    const char16_t *FileName,
    uint64_t OpenMode,
    uint64_t Attributes
);

typedef EFI_STATUS (*EFI_FILE_CLOSE)(
    EFI_FILE_PROTOCOL *This
);

typedef EFI_STATUS (*EFI_FILE_DELETE)(
    EFI_FILE_PROTOCOL *This
);

typedef EFI_STATUS (*EFI_FILE_READ)(
    EFI_FILE_PROTOCOL *This,
    uint64_t *BufferSize,
    void *Buffer
);

typedef EFI_STATUS (*EFI_FILE_WRITE)(
    EFI_FILE_PROTOCOL *This,
    uint64_t *BufferSize,
    void *Buffer
);

typedef EFI_STATUS (*EFI_FILE_GET_POSITION)(
    EFI_FILE_PROTOCOL *This,
    uint64_t *Position
);

typedef EFI_STATUS (*EFI_FILE_SET_POSITION)(
    EFI_FILE_PROTOCOL *This,
    uint64_t Position
);

typedef EFI_STATUS (*EFI_FILE_GET_INFO)(
    EFI_FILE_PROTOCOL *This,
    EFI_GUID *InformationType,
    uint64_t *BufferSize,
    void *Buffer
);

typedef EFI_STATUS (*EFI_FILE_SET_INFO)(
    EFI_FILE_PROTOCOL *This,
    EFI_GUID *InformationType,
    uint64_t BufferSize,
    void *Buffer
);

typedef EFI_STATUS (*EFI_FILE_FLUSH)(
    EFI_FILE_PROTOCOL *This
);

// File protocol structure
struct _EFI_FILE_PROTOCOL {
    uint64_t Revision;
    EFI_FILE_OPEN Open;
    EFI_FILE_CLOSE Close;
    EFI_FILE_DELETE Delete;
    EFI_FILE_READ Read;
    EFI_FILE_WRITE Write;
    EFI_FILE_GET_POSITION GetPosition;
    EFI_FILE_SET_POSITION SetPosition;
    EFI_FILE_GET_INFO GetInfo;
    EFI_FILE_SET_INFO SetInfo;
    EFI_FILE_FLUSH Flush;
};

// File system protocol function types
typedef EFI_STATUS (*EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
    EFI_FILE_PROTOCOL **Root
);

// File system protocol structure
struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    uint64_t Revision;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;
};

// Protocol GUIDs - extern declarations only
extern const EFI_GUID gEfiSimpleFileSystemProtocolGuid;
extern const EFI_GUID gEfiFileInfoGuid;
extern const EFI_GUID gEfiFileSystemInfoGuid;

// Helper functions
EFI_STATUS GetFileSystemProtocol(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **FileSystem);
EFI_STATUS OpenRootVolume(EFI_FILE_PROTOCOL **Root);
EFI_STATUS OpenFile(EFI_FILE_PROTOCOL *Root, const char16_t *FileName, 
                   EFI_FILE_PROTOCOL **File, uint64_t OpenMode);
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL *File, void *Buffer, uint64_t *BufferSize);
EFI_STATUS WriteFile(EFI_FILE_PROTOCOL *File, void *Buffer, uint64_t BufferSize);
EFI_STATUS ReadFileInfo(EFI_FILE_PROTOCOL *File, EFI_FILE_INFO **FileInfo);
EFI_STATUS ReadDirectory(EFI_FILE_PROTOCOL *Directory, EFI_FILE_INFO **EntryInfo);

#endif // TINYUEFI_FILE_PROTOCOL_H