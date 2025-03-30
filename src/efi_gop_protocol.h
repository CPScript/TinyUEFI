// efi_gop_protocol.h
#ifndef TINYUEFI_GOP_PROTOCOL_H
#define TINYUEFI_GOP_PROTOCOL_H

#include "uefi_types.h"

// Pixel format
typedef enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

// Pixel bitmask
typedef struct {
    uint32_t RedMask;
    uint32_t GreenMask;
    uint32_t BlueMask;
    uint32_t ReservedMask;
} EFI_PIXEL_BITMASK;

// Video mode information
typedef struct {
    uint32_t Version;
    uint32_t HorizontalResolution;
    uint32_t VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
    EFI_PIXEL_BITMASK PixelInformation;
    uint32_t PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

// BLT operation
typedef enum {
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

// Pixel definition
typedef struct {
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

// Mode structure
typedef struct {
    uint32_t MaxMode;
    uint32_t Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    uint64_t SizeOfInfo;
    EFI_PHYSICAL_ADDRESS FrameBufferBase;
    uint64_t FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

// Forward declaration
typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;

// Graphics protocol function types
typedef EFI_STATUS (*EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    uint32_t ModeNumber,
    uint64_t *SizeOfInfo,
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
);

typedef EFI_STATUS (*EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    uint32_t ModeNumber
);

typedef EFI_STATUS (*EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer,
    EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
    uint64_t SourceX,
    uint64_t SourceY,
    uint64_t DestinationX,
    uint64_t DestinationY,
    uint64_t Width,
    uint64_t Height,
    uint64_t Delta
);

// Graphics protocol structure
struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
};

// GUID for graphics output protocol
extern EFI_GUID gEfiGraphicsOutputProtocolGuid;

// Helper functions
EFI_STATUS GetGraphicsOutputProtocol(EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop);
EFI_STATUS SetBestGraphicsMode(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop);
EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Color);
EFI_STATUS DrawRectangle(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, 
                        uint64_t X, uint64_t Y, uint64_t Width, uint64_t Height, 
                        EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Color);
EFI_STATUS DrawBitmap(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, 
                     uint64_t X, uint64_t Y, uint64_t Width, uint64_t Height, 
                     EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Bitmap);
void GetPixelForRGB(uint8_t Red, uint8_t Green, uint8_t Blue, 
                    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Pixel);

#endif // TINYUEFI_GOP_PROTOCOL_H