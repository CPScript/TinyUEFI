// efi_network_protocol.c
#include "efi_network_protocol.h"
#include "uefi_helpers.h"
#include "efi_protocol_discovery.h"

// GUID for simple network protocol
EFI_GUID gEfiSimpleNetworkProtocolGuid = {
    0xA19832B9, 0xAC25, 0x11D3, {0x9A, 0x2D, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}
};

// Get the simple network protocol
EFI_STATUS GetNetworkProtocol(EFI_SIMPLE_NETWORK_PROTOCOL **SimpleNetwork) {
    return LocateProtocol(&gEfiSimpleNetworkProtocolGuid, (void**)SimpleNetwork);
}

// Initialize the network interface
EFI_STATUS InitializeNetwork(EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork) {
    EFI_STATUS Status;
    
    if (SimpleNetwork == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Check if the network is already started
    if (SimpleNetwork->Mode->State == EfiSimpleNetworkStarted) {
        // Initialize the network
        Status = SimpleNetwork->Initialize(SimpleNetwork, 0, 0);
        return Status;
    } else if (SimpleNetwork->Mode->State == EfiSimpleNetworkStopped) {
        // Start the network
        Status = SimpleNetwork->Start(SimpleNetwork);
        if (EFI_ERROR(Status)) {
            return Status;
        }
        
        // Initialize the network
        Status = SimpleNetwork->Initialize(SimpleNetwork, 0, 0);
        return Status;
    } else {
        // Network already initialized
        return EFI_SUCCESS;
    }
}

// Send a packet through the network interface
EFI_STATUS SendPacket(EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork, void *Data, 
                     uint64_t DataSize, EFI_MAC_ADDRESS *DestMacAddress) {
    if (SimpleNetwork == NULL || Data == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Check if the network is initialized
    if (SimpleNetwork->Mode->State != EfiSimpleNetworkInitialized) {
        return EFI_NOT_READY;
    }
    
    // Set EtherType to IPv4 (0x0800)
    uint16_t Protocol = 0x0800;
    
    // Send the packet
    return SimpleNetwork->Transmit(
        SimpleNetwork,
        0,          // HeaderSize (0 for default Ethernet header)
        DataSize,   // BufferSize
        Data,       // Buffer
        NULL,       // Use default source MAC
        DestMacAddress,  // Destination MAC
        &Protocol   // Protocol (EtherType)
    );
}

// Receive a packet from the network interface
EFI_STATUS ReceivePacket(EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork, void *Buffer, 
                        uint64_t *BufferSize) {
    EFI_STATUS Status;
    uint64_t HeaderSize = 0;
    EFI_MAC_ADDRESS SrcMac;
    EFI_MAC_ADDRESS DestMac;
    uint16_t Protocol;
    
    if (SimpleNetwork == NULL || Buffer == NULL || BufferSize == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Check if the network is initialized
    if (SimpleNetwork->Mode->State != EfiSimpleNetworkInitialized) {
        return EFI_NOT_READY;
    }
    
    // Receive a packet
    Status = SimpleNetwork->Receive(
        SimpleNetwork,
        &HeaderSize,
        BufferSize,
        Buffer,
        &SrcMac,
        &DestMac,
        &Protocol
    );
    
    return Status;
}

// Print a MAC address
void PrintMacAddress(EFI_MAC_ADDRESS *MacAddress, uint32_t Size) {
    if (MacAddress == NULL) {
        PRINT(u"NULL");
        return;
    }
    
    for (uint32_t i = 0; i < Size; i++) {
        PrintHex(MacAddress->Addr[i]);
        if (i < Size - 1) {
            PRINT(u":");
        }
    }
}