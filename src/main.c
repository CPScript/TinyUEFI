#include "uefi_types.h"
#include "uefi_helpers.h"
#include "efi_file_protocol.h"
#include "efi_network_protocol.h"
#include "efi_gop_protocol.h"
#include "efi_protocol_discovery.h"

EFI_SYSTEM_TABLE *ST = NULL;

EFI_HANDLE ImageHandle = NULL;

EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    ImageHandle = image_handle;
    ST = system_table;
    
    // Reset
    EFI_CALL(ST->ConOut->Reset(ST->ConOut, false));
    EFI_CALL(ST->ConOut->ClearScreen(ST->ConOut));

    SET_COLOR(EFI_WHITE, EFI_BACKGROUND_BLUE);
    
    // header
    PRINTL(u"TinyUEFI Application - Minimal UEFI Template");
    PRINTL(u"--------------------------------------------");
    RESET_COLOR();
    PRINTL(u"");
    
    // Show firmware information
    PRINT(u"Firmware Vendor: ");
    PRINTL(ST->FirmwareVendor);
    
    PRINT(u"Firmware Revision: 0x");
    PrintHex(ST->FirmwareRevision);
    PRINTL(u"");
    
    // Demonstrate color output
    PRINTL(u"");
    PRINTL(u"Color output demonstration:");
    
    SET_COLOR(EFI_RED, EFI_BACKGROUND_BLACK);
    PRINTL(u"This text should be RED");
    
    SET_COLOR(EFI_GREEN, EFI_BACKGROUND_BLACK);
    PRINTL(u"This text should be GREEN");
    
    SET_COLOR(EFI_BLUE, EFI_BACKGROUND_BLACK);
    PRINTL(u"This text should be BLUE");
    
    SET_COLOR(EFI_YELLOW, EFI_BACKGROUND_BLACK);
    PRINTL(u"This text should be YELLOW");
    
    RESET_COLOR();
    PRINTL(u"");
    
    // Demonstrate memory allocation
    PRINTL(u"Memory allocation test:");
    char16_t *dynamic_buf = AllocatePool(128 * sizeof(char16_t));
    
    if (dynamic_buf) {
        for (int i = 0; i < 26; i++) {
            dynamic_buf[i] = u'A' + i;
        }
        dynamic_buf[26] = 0;
        
        PRINT(u"Allocated buffer content: ");
        PRINTL(dynamic_buf);
        
        // Free the memory
        FreePool(dynamic_buf);
        PRINTL(u"Memory freed successfully");
    } else {
        PRINTL(u"Memory allocation failed!");
    }
    
    PRINTL(u"");
    PRINTL(u"Cursor positioning demonstration:");
    
    for (int row = 12; row < 16; row++) {
        for (int col = 0; col < 40; col += 5) {
            SET_CURSOR(col, row);
            PrintDec(col);
            PRINT(u",");
            PrintDec(row);
        }
    }
    
    SET_CURSOR(0, 17);
    PRINTL(u"");
    PRINTL(u"");
    
    SET_COLOR(EFI_LIGHTGREEN, EFI_BACKGROUND_BLACK);
    PRINTL(u"Press any key to exit...");
    RESET_COLOR();
    
    WaitForKeyPress();
    
    CLEAR_SCREEN();
    PRINTL(u"Exiting TinyUEFI application...");
    
    return EFI_SUCCESS;
}
