A lightweight UEFI application framework that provides a structured approach to firmware development without the complexity of EDK2.

## Features

- Minimal UEFI type definitions (no EDK2 dependency)
- Robust error handling with `EFI_CALL` macro
- Memory allocation/deallocation helpers
- Text output utilities with color support
- Cursor positioning and screen clearing
- Decimal and hexadecimal numeric output
- Key input handling
- String and memory utility functions
- QEMU-compatible build system
- USB installation support
- Clean, documented structure

### Implemented Protocols

- **File System Access** - Complete EFI_FILE_PROTOCOL implementation for file operations
- **Graphics Support** - Graphics Output Protocol (GOP) for display control and rendering
- **Network Functionality** - EFI_SIMPLE_NETWORK_PROTOCOL for basic networking
- **Protocol Discovery** - Framework for locating and utilizing UEFI system protocols

## Requirements

- x86_64 MinGW cross-compiler (`x86_64-w64-mingw32-gcc`)
- GNU Make
- QEMU (for testing)
- OVMF firmware (downloadable via `make ovmf`)

## Building

```bash
make
```

If you need OVMF firmware for testing in QEMU:

```bash
make ovmf
```

## Running in QEMU

```bash
make run
```

## Creating a Bootable USB Drive

**CAUTION: This will format your USB drive!**

```bash
# Replace /dev/sdX with your USB drive
make install
```

## Examples

The framework includes two example applications:

1. **Minimal Example** (`main.c`): Demonstrates basic console output, color manipulation, and memory allocation
2. **Extended Example** (`example.c`): Shows comprehensive usage including file operations, networking, graphics, and protocol discovery

### File System Example

```c
EFI_FILE_PROTOCOL *Root = NULL;
EFI_FILE_PROTOCOL *File = NULL;

// Open root volume
OpenRootVolume(&Root);

// Create and write to a file
OpenFile(Root, u"sample.txt", &File, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_WRITE);
WriteFile(File, content_buffer, content_size);
File->Close(File);

// Read a file
OpenFile(Root, u"sample.txt", &File, EFI_FILE_MODE_READ);
ReadFile(File, buffer, &buffer_size);
File->Close(File);
Root->Close(Root);
```

### Graphics Example

```c
EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop = NULL;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL BackgroundColor;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL RectangleColor;

// Get graphics protocol and set best mode
GetGraphicsOutputProtocol(&Gop);
SetBestGraphicsMode(Gop);

// Define colors and clear screen
GetPixelForRGB(0, 0, 0x80, &BackgroundColor); // Dark blue
GetPixelForRGB(0xFF, 0xFF, 0xFF, &RectangleColor); // White
ClearScreen(Gop, &BackgroundColor);

// Draw a rectangle
DrawRectangle(Gop, 50, 50, 200, 100, &RectangleColor);
```

### Network Example

```c
EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork = NULL;

// Get network protocol and initialize
GetNetworkProtocol(&SimpleNetwork);
InitializeNetwork(SimpleNetwork);

// Display MAC address
PrintMacAddress(&SimpleNetwork->Mode->CurrentAddress, 
                SimpleNetwork->Mode->HwAddressSize);

// Send and receive packets
SendPacket(SimpleNetwork, data_buffer, data_size, destination_mac);
ReceivePacket(SimpleNetwork, receive_buffer, &buffer_size);
```

## Customizing

1. Modify `main.c` or start from `example.c` to implement your UEFI application
2. Extend existing protocol implementations as needed
3. Add more UEFI protocol definitions to expand functionality
4. Update the Makefile for your specific toolchain if necessary

## Notes for Use

1. The UEFI system table and boot services are only valid during boot time
2. Call `ExitBootServices` before transitioning to runtime code
3. UEFI applications have limited resources available
4. Consider driver development for complex hardware interaction
5. Test thoroughly in QEMU before deploying to hardware

## Project Organization

```
TinyUEFI/
├── src/
│   ├── main.c                   # Minimal example application
│   ├── example.c                # Extended example with all protocols
│   ├── uefi_types.h             # Core UEFI type definitions
│   ├── uefi_helpers.h           # Common utilities and helper functions
│   ├── efi_file_protocol.h      # File system protocol interface
│   ├── efi_file_protocol.c      # File system implementation
│   ├── efi_gop_protocol.h       # Graphics output protocol interface
│   ├── efi_gop_protocol.c       # Graphics implementation
│   ├── efi_network_protocol.h   # Network protocol interface
│   ├── efi_network_protocol.c   # Network implementation
│   ├── efi_protocol_discovery.h # Protocol discovery interface
│   └── efi_protocol_discovery.c # Protocol discovery implementation
├── build/
│   ├── obj/                     # Object files
│   └── TinyUEFI.efi             # Output EFI application
├── GNUmakefile                  # Build system
└── OVMF.fd                      # UEFI firmware for QEMU (downloaded)
```

## License

Open source - see source files for license details.
