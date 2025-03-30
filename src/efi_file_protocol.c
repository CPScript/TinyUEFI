// efi_file_protocol.c
#include "efi_file_protocol.h"
#include "uefi_helpers.h"
#include "efi_protocol_discovery.h"

// Define GUIDs - using const to prevent inadvertent modification
const EFI_GUID gEfiSimpleFileSystemProtocolGuid = {
    0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
};

const EFI_GUID gEfiFileInfoGuid = {
    0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
};

const EFI_GUID gEfiFileSystemInfoGuid = {
    0x09576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}
};


// Get the file system protocol
EFI_STATUS GetFileSystemProtocol(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **FileSystem) {
    return LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, (void**)FileSystem);
}

// Open the root volume
EFI_STATUS OpenRootVolume(EFI_FILE_PROTOCOL **Root) {
    EFI_STATUS Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    
    // Get the file system protocol
    Status = GetFileSystemProtocol(&FileSystem);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // Open the volume
    Status = FileSystem->OpenVolume(FileSystem, Root);
    return Status;
}

// Open a file from the given root directory
EFI_STATUS OpenFile(EFI_FILE_PROTOCOL *Root, const char16_t *FileName, 
                   EFI_FILE_PROTOCOL **File, uint64_t OpenMode) {
    if (Root == NULL || FileName == NULL || File == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    return Root->Open(Root, File, FileName, OpenMode, 0);
}

// Read data from a file
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL *File, void *Buffer, uint64_t *BufferSize) {
    if (File == NULL || Buffer == NULL || BufferSize == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    return File->Read(File, BufferSize, Buffer);
}

// Write data to a file
EFI_STATUS WriteFile(EFI_FILE_PROTOCOL *File, void *Buffer, uint64_t BufferSize) {
    if (File == NULL || Buffer == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    uint64_t Size = BufferSize;
    return File->Write(File, &Size, Buffer);
}

// Read file information
EFI_STATUS ReadFileInfo(EFI_FILE_PROTOCOL *File, EFI_FILE_INFO **FileInfo) {
    EFI_STATUS Status;
    uint64_t InfoSize = 0;
    
    if (File == NULL || FileInfo == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Get the size needed for file info
    Status = File->GetInfo(File, &gEfiFileInfoGuid, &InfoSize, NULL);
    if (Status != EFI_BUFFER_TOO_SMALL) {
        return Status;
    }
    
    // Allocate memory for file info
    *FileInfo = (EFI_FILE_INFO*)AllocatePool(InfoSize);
    if (*FileInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    // Get the file info
    Status = File->GetInfo(File, &gEfiFileInfoGuid, &InfoSize, *FileInfo);
    if (EFI_ERROR(Status)) {
        FreePool(*FileInfo);
        *FileInfo = NULL;
    }
    
    return Status;
}

// Read the next directory entry
EFI_STATUS ReadDirectory(EFI_FILE_PROTOCOL *Directory, EFI_FILE_INFO **EntryInfo) {
    EFI_STATUS Status;
    uint64_t BufferSize = 256; // Initial buffer size
    
    if (Directory == NULL || EntryInfo == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Allocate memory for directory entry
    *EntryInfo = (EFI_FILE_INFO*)AllocatePool(BufferSize);
    if (*EntryInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    // Read the directory entry
    Status = Directory->Read(Directory, &BufferSize, *EntryInfo);
    
    // If we need more buffer space
    if (Status == EFI_BUFFER_TOO_SMALL) {
        FreePool(*EntryInfo);
        *EntryInfo = (EFI_FILE_INFO*)AllocatePool(BufferSize);
        if (*EntryInfo == NULL) {
            return EFI_OUT_OF_RESOURCES;
        }
        Status = Directory->Read(Directory, &BufferSize, *EntryInfo);
    }
    
    // If we reached the end of the directory or had an error
    if (EFI_ERROR(Status) || BufferSize == 0) {
        if (BufferSize == 0) {
            Status = EFI_NOT_FOUND; // End of directory
        }
        FreePool(*EntryInfo);
        *EntryInfo = NULL;
    }
    
    return Status;
}