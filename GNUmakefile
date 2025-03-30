# GNUMakefile

TARGET = x86_64-w64-mingw32
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
OBJCOPY = $(TARGET)-objcopy

# Directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Output EFI application
EFI_APP = $(BUILD_DIR)/TinyUEFI.efi

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Compiler flags
CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -fno-stack-protector -fno-stack-check \
         -fno-strict-aliasing -fpic -fno-builtin -Wall -Wextra -Werror -Wno-unused-parameter \
         -I$(SRC_DIR) -std=c11

# Linker flags
LDFLAGS = -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main

# Default target
all: dirs $(EFI_APP)

# Create build directories
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

# Build EFI application
$(EFI_APP): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run in QEMU with OVMF
run: all
	qemu-system-x86_64 -bios OVMF.fd -net nic -net user -drive file=fat:rw:$(BUILD_DIR),format=raw

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Install to USB drive (update /dev/sdX with your drive)
# CAUTION: This can damage your system if used incorrectly!
install: all
	@echo "WARNING: This will format your USB drive!"
	@echo "Make sure /dev/sdX is the correct device."
	@echo "Press Enter to continue or Ctrl+C to abort..."
	@read dummy
	sudo mkfs.fat -F32 /dev/sdX
	sudo mount /dev/sdX /mnt
	sudo mkdir -p /mnt/EFI/BOOT
	sudo cp $(EFI_APP) /mnt/EFI/BOOT/BOOTX64.EFI
	sudo umount /mnt
	@echo "Installation complete. The USB drive should be bootable on UEFI systems."

# Download OVMF for testing if needed
ovmf:
	@echo "Downloading OVMF.fd for QEMU testing..."
	wget -O OVMF.fd https://github.com/rust-osdev/ovmf-prebuilt/raw/master/OVMF-pure-efi.fd || \
	curl -L -o OVMF.fd https://github.com/rust-osdev/ovmf-prebuilt/raw/master/OVMF-pure-efi.fd
	@echo "OVMF.fd downloaded."

# Generate stub debug symbols
debug_info: all
	$(OBJCOPY) --add-gnu-debuglink=$(EFI_APP) $(EFI_APP)

.PHONY: all dirs clean run install ovmf debug_info
