// efi_network_protocol.h
#ifndef TINYUEFI_NETWORK_PROTOCOL_H
#define TINYUEFI_NETWORK_PROTOCOL_H

#include "uefi_types.h"

// MAC Address structure
typedef struct {
    uint8_t Addr[32];
} EFI_MAC_ADDRESS;

// Network statistics structure
typedef struct {
    uint64_t RxTotalFrames;
    uint64_t RxGoodFrames;
    uint64_t RxUndersizeFrames;
    uint64_t RxOversizeFrames;
    uint64_t RxDroppedFrames;
    uint64_t RxUnicastFrames;
    uint64_t RxBroadcastFrames;
    uint64_t RxMulticastFrames;
    uint64_t RxCrcErrorFrames;
    uint64_t TxTotalFrames;
    uint64_t TxGoodFrames;
    uint64_t TxUndersizeFrames;
    uint64_t TxOversizeFrames;
    uint64_t TxDroppedFrames;
    uint64_t TxUnicastFrames;
    uint64_t TxBroadcastFrames;
    uint64_t TxMulticastFrames;
    uint64_t TxCrcErrorFrames;
    uint64_t TxCollisionCount;
    uint64_t UnsupportedProtocol;
} EFI_NETWORK_STATISTICS;

// Network state
typedef enum {
    EfiSimpleNetworkStopped,
    EfiSimpleNetworkStarted,
    EfiSimpleNetworkInitialized,
    EfiSimpleNetworkMaxState
} EFI_SIMPLE_NETWORK_STATE;

// Network mode data
typedef struct {
    uint32_t State;
    uint32_t HwAddressSize;
    uint32_t MediaHeaderSize;
    uint32_t MaxPacketSize;
    uint32_t NvRamSize;
    uint32_t NvRamAccessSize;
    uint32_t ReceiveFilterMask;
    uint32_t ReceiveFilterSetting;
    uint32_t MaxMCastFilterCount;
    uint32_t MCastFilterCount;
    EFI_MAC_ADDRESS MCastFilter[16];
    EFI_MAC_ADDRESS CurrentAddress;
    EFI_MAC_ADDRESS BroadcastAddress;
    EFI_MAC_ADDRESS PermanentAddress;
    uint8_t IfType;
    bool MacAddressChangeable;
    bool MultipleTxSupported;
    bool MediaPresentSupported;
    bool MediaPresent;
} EFI_SIMPLE_NETWORK_MODE;

// Forward declaration
typedef struct _EFI_SIMPLE_NETWORK_PROTOCOL EFI_SIMPLE_NETWORK_PROTOCOL;

// Network protocol function types
typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_START)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_STOP)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_INITIALIZE)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    uint64_t ExtraRxBufferSize,
    uint64_t ExtraTxBufferSize
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_RESET)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    bool ExtendedVerification
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_SHUTDOWN)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_RECEIVE_FILTERS)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    uint32_t Enable,
    uint32_t Disable,
    bool ResetMCastFilter,
    uint64_t MCastFilterCnt,
    EFI_MAC_ADDRESS *MCastFilter
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_STATION_ADDRESS)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    bool Reset,
    EFI_MAC_ADDRESS *New
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_STATISTICS)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    bool Reset,
    uint64_t *StatisticsSize,
    EFI_NETWORK_STATISTICS *StatisticsTable
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_MCAST_IP_TO_MAC)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    bool IPv6,
    EFI_IP_ADDRESS *IP,
    EFI_MAC_ADDRESS *MAC
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_NVDATA)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    bool ReadWrite,
    uint64_t Offset,
    uint64_t BufferSize,
    void *Buffer
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_GET_STATUS)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    uint32_t *InterruptStatus,
    void **TxBuf
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_TRANSMIT)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    uint64_t HeaderSize,
    uint64_t BufferSize,
    void *Buffer,
    EFI_MAC_ADDRESS *SrcAddr,
    EFI_MAC_ADDRESS *DestAddr,
    uint16_t *Protocol
);

typedef EFI_STATUS (*EFI_SIMPLE_NETWORK_RECEIVE)(
    EFI_SIMPLE_NETWORK_PROTOCOL *This,
    uint64_t *HeaderSize,
    uint64_t *BufferSize,
    void *Buffer,
    EFI_MAC_ADDRESS *SrcAddr,
    EFI_MAC_ADDRESS *DestAddr,
    uint16_t *Protocol
);

// Simple network protocol structure
struct _EFI_SIMPLE_NETWORK_PROTOCOL {
    uint64_t Revision;
    EFI_SIMPLE_NETWORK_START Start;
    EFI_SIMPLE_NETWORK_STOP Stop;
    EFI_SIMPLE_NETWORK_INITIALIZE Initialize;
    EFI_SIMPLE_NETWORK_RESET Reset;
    EFI_SIMPLE_NETWORK_SHUTDOWN Shutdown;
    EFI_SIMPLE_NETWORK_RECEIVE_FILTERS ReceiveFilters;
    EFI_SIMPLE_NETWORK_STATION_ADDRESS StationAddress;
    EFI_SIMPLE_NETWORK_STATISTICS Statistics;
    EFI_SIMPLE_NETWORK_MCAST_IP_TO_MAC MCastIpToMac;
    EFI_SIMPLE_NETWORK_NVDATA NvData;
    EFI_SIMPLE_NETWORK_GET_STATUS GetStatus;
    EFI_SIMPLE_NETWORK_TRANSMIT Transmit;
    EFI_SIMPLE_NETWORK_RECEIVE Receive;
    EFI_EVENT WaitForPacket;
    EFI_SIMPLE_NETWORK_MODE *Mode;
};

// GUID for simple network protocol
extern EFI_GUID gEfiSimpleNetworkProtocolGuid;

// Helper functions
EFI_STATUS GetNetworkProtocol(EFI_SIMPLE_NETWORK_PROTOCOL **SimpleNetwork);
EFI_STATUS InitializeNetwork(EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork);
EFI_STATUS SendPacket(EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork, void *Data, 
                     uint64_t DataSize, EFI_MAC_ADDRESS *DestMacAddress);
EFI_STATUS ReceivePacket(EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork, void *Buffer, 
                        uint64_t *BufferSize);
void PrintMacAddress(EFI_MAC_ADDRESS *MacAddress, uint32_t Size);

#endif // TINYUEFI_NETWORK_PROTOCOL_H