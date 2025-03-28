# TinyUEFI

A minimal yet production-ready UEFI application template that avoids the complexity of EDK2 while providing essential functionality.

## Features already added

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

## Features to be added

- File system access via EFI_FILE_PROTOCOL
- Network functionality via EFI_SIMPLE_NETWORK_PROTOCOL
- Graphics support using the GOP (Graphics Output Protocol)
- Protocol discovery for hardware device access

## Requirements

- x86_64 MinGW cross-compiler (GCC)
- GNU Make
- QEMU (for testing)

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

## Customizing

1. Modify `main.c` to implement your UEFI application
2. Extend `uefi_helpers.h` with additional utilities as needed
3. Add more UEFI protocol definitions to `uefi_types.h` as required
4. Update the Makefile for your specific toolchain if necessary

## Features

- **Robust Error Handling**: All UEFI calls are checked with detailed error reporting
- **Memory Management**: Proper allocation and deallocation with error checking
- **Console I/O**: Full text output with color support and cursor control
- **User Interaction**: Input handling with key event waiting
- **Cross-Platform Build**: Works with standard MinGW cross-compilers
- **Test Environment**: QEMU + OVMF configuration for quick testing

## Notes for Use

1. The UEFI system table and boot services are only valid during boot time
2. Call `ExitBootServices` before transitioning to runtime code
3. For file system access, implement the EFI_FILE_PROTOCOL interface
4. For network access, implement the EFI_SIMPLE_NETWORK_PROTOCOL
5. USB and PCIe device access requires additional protocol interfaces

## Example Extension: File System Access

To add file system support, you would:

1. Define the EFI_FILE_PROTOCOL and EFI_SIMPLE_FILE_SYSTEM_PROTOCOL in uefi_types.h
2. Add helper functions in uefi_helpers.h for opening volumes and files
3. Use the EFI_LOADED_IMAGE_PROTOCOL to locate your boot device
4. Implement file I/O functions in a new file like fs_helpers.h
