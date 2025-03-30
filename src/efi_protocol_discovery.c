// efi_protocol_discovery.c
#include "efi_protocol_discovery.h"
#include "uefi_helpers.h"

// Locate a protocol by GUID
EFI_STATUS LocateProtocol(EFI_GUID *Protocol, void **Interface) {
    if (Protocol == NULL || Interface == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    EFI_LOCATE_PROTOCOL LocateProtocol = (EFI_LOCATE_PROTOCOL)ST->BootServices->LocateProtocol;
    return LocateProtocol(Protocol, NULL, Interface);
}

// Locate all handles that support a specified protocol
EFI_STATUS LocateHandles(EFI_GUID *Protocol, EFI_HANDLE **HandleBuffer, uint64_t *HandleCount) {
    EFI_STATUS Status;
    uint64_t BufferSize = 0;
    
    if (Protocol == NULL || HandleBuffer == NULL || HandleCount == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    EFI_LOCATE_HANDLE LocateHandle = (EFI_LOCATE_HANDLE)ST->BootServices->LocateHandleBuffer;
    
    // Get the size needed for handles
    Status = LocateHandle(ByProtocol, Protocol, NULL, &BufferSize, NULL);
    if (Status != EFI_BUFFER_TOO_SMALL) {
        *HandleCount = 0;
        *HandleBuffer = NULL;
        return EFI_NOT_FOUND;
    }
    
    // Allocate memory for handles
    *HandleBuffer = (EFI_HANDLE*)AllocatePool(BufferSize);
    if (*HandleBuffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    // Get all handles
    Status = LocateHandle(ByProtocol, Protocol, NULL, &BufferSize, *HandleBuffer);
    if (EFI_ERROR(Status)) {
        FreePool(*HandleBuffer);
        *HandleBuffer = NULL;
        *HandleCount = 0;
        return Status;
    }
    
    *HandleCount = BufferSize / sizeof(EFI_HANDLE);
    return EFI_SUCCESS;
}

// Open a protocol on a specific handle
EFI_STATUS OpenProtocolOnHandle(EFI_HANDLE Handle, EFI_GUID *Protocol, void **Interface) {
    if (Handle == NULL || Protocol == NULL || Interface == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    EFI_OPEN_PROTOCOL OpenProtocol = (EFI_OPEN_PROTOCOL)ST->BootServices->OpenProtocol;
    return OpenProtocol(
        Handle,
        Protocol,
        Interface,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
}

// Close a previously opened protocol
EFI_STATUS CloseProtocolOnHandle(EFI_HANDLE Handle, EFI_GUID *Protocol) {
    if (Handle == NULL || Protocol == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    EFI_CLOSE_PROTOCOL CloseProtocol = (EFI_CLOSE_PROTOCOL)ST->BootServices->CloseProtocol;
    return CloseProtocol(
        Handle,
        Protocol,
        ImageHandle,
        NULL
    );
}

// Print a GUID in standard format
void PrintGUID(EFI_GUID *Guid) {
    if (Guid == NULL) {
        PRINT(u"NULL-GUID");
        return;
    }
    
    PrintHex(Guid->Data1);
    PRINT(u"-");
    PrintHex(Guid->Data2);
    PRINT(u"-");
    PrintHex(Guid->Data3);
    PRINT(u"-");
    
    for (int i = 0; i < 2; i++) {
        PrintHex(Guid->Data4[i]);
    }
    
    PRINT(u"-");
    
    for (int i = 2; i < 8; i++) {
        PrintHex(Guid->Data4[i]);
    }
}