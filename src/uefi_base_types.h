// uefi_base_types.h
#ifndef TINYUEFI_BASE_TYPES_H
#define TINYUEFI_BASE_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Define char16_t if not available in standard
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#include <uchar.h>  // C11 standard char16_t
#else
typedef uint16_t char16_t;  // Pre-C11 compatibility
#endif

// Fundamental UEFI types
typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef void *EFI_EVENT;
typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef uint64_t EFI_VIRTUAL_ADDRESS;

// GUID structure - fundamental to all UEFI protocols
typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} EFI_GUID;

// Status code definitions
#define EFI_SUCCESS                     0x0000000000000000ULL
#define EFI_LOAD_ERROR                  0x8000000000000001ULL
#define EFI_INVALID_PARAMETER           0x8000000000000002ULL
#define EFI_UNSUPPORTED                 0x8000000000000003ULL
#define EFI_BAD_BUFFER_SIZE             0x8000000000000004ULL
#define EFI_BUFFER_TOO_SMALL            0x8000000000000005ULL
#define EFI_NOT_READY                   0x8000000000000006ULL
#define EFI_DEVICE_ERROR                0x8000000000000007ULL
#define EFI_WRITE_PROTECTED             0x8000000000000008ULL
#define EFI_OUT_OF_RESOURCES            0x8000000000000009ULL
#define EFI_VOLUME_CORRUPTED            0x800000000000000AULL
#define EFI_VOLUME_FULL                 0x800000000000000BULL
#define EFI_NO_MEDIA                    0x800000000000000CULL
#define EFI_MEDIA_CHANGED               0x800000000000000DULL
#define EFI_NOT_FOUND                   0x800000000000000EULL
#define EFI_ACCESS_DENIED               0x800000000000000FULL
#define EFI_TIMEOUT                     0x8000000000000010ULL

// Error checking macro
#define EFI_ERROR(Status)               ((int64_t)(Status) < 0)

#endif // TINYUEFI_BASE_TYPES_H