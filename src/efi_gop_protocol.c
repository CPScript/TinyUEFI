// efi_gop_protocol.c
#include "efi_gop_protocol.h"
#include "uefi_helpers.h"
#include "efi_protocol_discovery.h"

// GUID for graphics output protocol
EFI_GUID gEfiGraphicsOutputProtocolGuid = {
    0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
};

// Get the graphics output protocol
EFI_STATUS GetGraphicsOutputProtocol(EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop) {
    return LocateProtocol(&gEfiGraphicsOutputProtocolGuid, (void**)Gop);
}

// Set the best available graphics mode (highest resolution)
EFI_STATUS SetBestGraphicsMode(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop) {
    EFI_STATUS Status;
    uint32_t MaxWidth = 0;
    uint32_t MaxHeight = 0;
    uint32_t BestMode = 0;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info = NULL;
    uint64_t SizeOfInfo;
    
    if (Gop == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Find the best available mode
    for (uint32_t Mode = 0; Mode < Gop->Mode->MaxMode; Mode++) {
        Status = Gop->QueryMode(Gop, Mode, &SizeOfInfo, &Info);
        if (EFI_ERROR(Status)) {
            continue;
        }
        
        if (Info->HorizontalResolution > MaxWidth && 
            Info->VerticalResolution > MaxHeight) {
            MaxWidth = Info->HorizontalResolution;
            MaxHeight = Info->VerticalResolution;
            BestMode = Mode;
        }
        
        FreePool(Info);
    }
    
    // Set the best mode
    if (MaxWidth > 0 && MaxHeight > 0) {
        Status = Gop->SetMode(Gop, BestMode);
        return Status;
    }
    
    // If no suitable mode was found, keep the current mode
    return EFI_SUCCESS;
}

// Clear the screen with a specific color
EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Color) {
    if (Gop == NULL || Color == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    return Gop->Blt(
        Gop,
        Color,
        EfiBltVideoFill,
        0, 0,
        0, 0,
        Gop->Mode->Info->HorizontalResolution,
        Gop->Mode->Info->VerticalResolution,
        0
    );
}

// Draw a rectangle with a specific color
EFI_STATUS DrawRectangle(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, 
                        uint64_t X, uint64_t Y, uint64_t Width, uint64_t Height, 
                        EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Color) {
    if (Gop == NULL || Color == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    return Gop->Blt(
        Gop,
        Color,
        EfiBltVideoFill,
        0, 0,
        X, Y,
        Width, Height,
        0
    );
}

// Draw a bitmap to the screen
EFI_STATUS DrawBitmap(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, 
                     uint64_t X, uint64_t Y, uint64_t Width, uint64_t Height, 
                     EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Bitmap) {
    if (Gop == NULL || Bitmap == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    return Gop->Blt(
        Gop,
        Bitmap,
        EfiBltBufferToVideo,
        0, 0,
        X, Y,
        Width, Height,
        Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
    );
}

// Create a pixel from RGB values
void GetPixelForRGB(uint8_t Red, uint8_t Green, uint8_t Blue, 
                    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Pixel) {
    if (Pixel == NULL) {
        return;
    }
    
    Pixel->Red = Red;
    Pixel->Green = Green;
    Pixel->Blue = Blue;
    Pixel->Reserved = 0;
}