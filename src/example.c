// example.c
#include "uefi_types.h"
#include "uefi_helpers.h"
#include "efi_file_protocol.h"
#include "efi_network_protocol.h"
#include "efi_gop_protocol.h"
#include "efi_protocol_discovery.h"

// Example using file system protocol
void FileSystemExample() {
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL *Root = NULL;
    EFI_FILE_PROTOCOL *File = NULL;
    char16_t FileName[] = u"sample.txt";
    char16_t FileContent[] = u"Hello, TinyUEFI File System!";
    uint64_t ContentSize = StrLen(FileContent) * sizeof(char16_t);
    char16_t ReadBuffer[256];
    uint64_t ReadSize = sizeof(ReadBuffer);
    
    PRINTL(u"");
    PRINTL(u"*** File System Example ***");
    
    // Open root volume
    Status = OpenRootVolume(&Root);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Error opening root volume");
        return;
    }
    
    // Create and write to a file
    Status = OpenFile(Root, FileName, &File, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_WRITE);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Error creating file");
        Root->Close(Root);
        return;
    }
    
    Status = WriteFile(File, FileContent, ContentSize);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Error writing to file");
        File->Close(File);
        Root->Close(Root);
        return;
    }
    
    File->Close(File);
    PRINTL(u"File written successfully");
    
    // Now read the file back
    Status = OpenFile(Root, FileName, &File, EFI_FILE_MODE_READ);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Error opening file for reading");
        Root->Close(Root);
        return;
    }
    
    Status = ReadFile(File, ReadBuffer, &ReadSize);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Error reading file");
        File->Close(File);
        Root->Close(Root);
        return;
    }
    
    ReadBuffer[ReadSize / sizeof(char16_t)] = 0; // Null terminate
    PRINT(u"Read from file: ");
    PRINTL(ReadBuffer);
    
    // Clean up
    File->Close(File);
    Root->Close(Root);
}

// Example using network protocol
void NetworkExample() {
    EFI_STATUS Status;
    EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork = NULL;
    
    PRINTL(u"");
    PRINTL(u"*** Network Example ***");
    
    // Get network protocol
    Status = GetNetworkProtocol(&SimpleNetwork);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Network protocol not available");
        return;
    }
    
    // Initialize network
    Status = InitializeNetwork(SimpleNetwork);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Failed to initialize network");
        return;
    }
    
    // Display network information
    PRINT(u"MAC Address: ");
    PrintMacAddress(&SimpleNetwork->Mode->CurrentAddress, 
                   SimpleNetwork->Mode->HwAddressSize);
    PRINTL(u"");
    
    PRINT(u"Network State: ");
    switch (SimpleNetwork->Mode->State) {
        case EfiSimpleNetworkStopped:
            PRINTL(u"Stopped");
            break;
        case EfiSimpleNetworkStarted:
            PRINTL(u"Started");
            break;
        case EfiSimpleNetworkInitialized:
            PRINTL(u"Initialized");
            break;
        default:
            PRINTL(u"Unknown");
            break;
    }
    
    PRINT(u"Media Present: ");
    PRINTL(SimpleNetwork->Mode->MediaPresent ? u"Yes" : u"No");
}

// Example using graphics protocol
void GraphicsExample() {
    EFI_STATUS Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop = NULL;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL BackgroundColor;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL RectangleColor;
    
    PRINTL(u"");
    PRINTL(u"*** Graphics Example ***");
    
    // Get graphics protocol
    Status = GetGraphicsOutputProtocol(&Gop);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Graphics protocol not available");
        return;
    }
    
    // Set best graphics mode
    Status = SetBestGraphicsMode(Gop);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Failed to set graphics mode");
        return;
    }
    
    // Display current graphics mode information
    PRINT(u"Resolution: ");
    PrintDec(Gop->Mode->Info->HorizontalResolution);
    PRINT(u" x ");
    PrintDec(Gop->Mode->Info->VerticalResolution);
    PRINTL(u"");
    
    PRINT(u"Pixel Format: ");
    switch (Gop->Mode->Info->PixelFormat) {
        case PixelRedGreenBlueReserved8BitPerColor:
            PRINTL(u"RGB");
            break;
        case PixelBlueGreenRedReserved8BitPerColor:
            PRINTL(u"BGR");
            break;
        case PixelBitMask:
            PRINTL(u"BitMask");
            break;
        case PixelBltOnly:
            PRINTL(u"BltOnly");
            break;
        default:
            PRINTL(u"Unknown");
            break;
    }
    
    PRINTL(u"Drawing on screen...");
    
    // Define colors
    GetPixelForRGB(0, 0, 0x80, &BackgroundColor); // Dark blue background
    GetPixelForRGB(0xFF, 0xFF, 0xFF, &RectangleColor); // White rectangle
    
    // Clear screen with background color
    Status = ClearScreen(Gop, &BackgroundColor);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Failed to clear screen");
        return;
    }
    
    // Draw a rectangle
    Status = DrawRectangle(
        Gop,
        50, 50,
        200, 100,
        &RectangleColor
    );
    if (EFI_ERROR(Status)) {
        PRINTL(u"Failed to draw rectangle");
        return;
    }
    
    PRINTL(u"Graphics demo completed. Press any key to return to text mode...");
    WaitForKeyPress();
    
    // Return to text mode
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->EnableCursor(ST->ConOut, true);
}

// Example using protocol discovery
void ProtocolDiscoveryExample() {
    EFI_STATUS Status;
    EFI_HANDLE *Handles = NULL;
    uint64_t HandleCount = 0;
    EFI_GUID BlockIoProtocolGuid = {
        0x964e5b21, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
    };
    
    PRINTL(u"");
    PRINTL(u"*** Protocol Discovery Example ***");
    
    // Find all handles that support Block I/O protocol
    Status = LocateHandles(&BlockIoProtocolGuid, &Handles, &HandleCount);
    if (EFI_ERROR(Status)) {
        PRINTL(u"Failed to locate Block I/O handles");
        return;
    }
    
    // Print the number of found handles
    PRINT(u"Found ");
    PrintDec(HandleCount);
    PRINTL(u" handles that support Block I/O protocol");
    
    // Clean up
    if (Handles != NULL) {
        FreePool(Handles);
    }
}

// Main example function
EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    ImageHandle = image_handle;
    ST = system_table;
    
    // Reset console
    EFI_CALL(ST->ConOut->Reset(ST->ConOut, false));
    EFI_CALL(ST->ConOut->ClearScreen(ST->ConOut));
    
    // Set header
    SET_COLOR(EFI_WHITE, EFI_BACKGROUND_BLUE);
    PRINTL(u"TinyUEFI Extended Capabilities Demo");
    PRINTL(u"-----------------------------------");
    RESET_COLOR();
    
    // Run the examples
    FileSystemExample();
    NetworkExample();
    GraphicsExample();
    ProtocolDiscoveryExample();
    
    // Wait for user input before exiting
    PRINTL(u"");
    SET_COLOR(EFI_LIGHTGREEN, EFI_BACKGROUND_BLACK);
    PRINTL(u"Press any key to exit...");
    RESET_COLOR();
    
    WaitForKeyPress();
    
    CLEAR_SCREEN();
    PRINTL(u"Exiting TinyUEFI demo...");
    
    return EFI_SUCCESS;
}