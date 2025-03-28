#ifndef TINYUEFI_HELPERS_H
#define TINYUEFI_HELPERS_H

#include "uefi_types.h"

extern EFI_SYSTEM_TABLE *ST;
extern EFI_HANDLE ImageHandle;

// Text macros
#define PRINT(msg)                  ST->ConOut->OutputString(ST->ConOut, (char16_t *)msg)
#define PRINTL(msg)                 { PRINT(msg); PRINT(u"\r\n"); }
#define PRINTERR(msg)               ST->StdErr->OutputString(ST->StdErr, (char16_t *)msg)
#define PRINTERRL(msg)              { PRINTERR(msg); PRINTERR(u"\r\n"); }

// Color output macros
#define SET_COLOR(fg, bg)           ST->ConOut->SetAttribute(ST->ConOut, (fg) | (bg))
#define RESET_COLOR()               ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGRAY)
#define CLEAR_SCREEN()              ST->ConOut->ClearScreen(ST->ConOut)
#define SET_CURSOR(col, row)        ST->ConOut->SetCursorPosition(ST->ConOut, col, row)

// Error handling macro
#define EFI_CALL(expr) { \
    EFI_STATUS status = (expr); \
    if (EFI_ERROR(status)) { \
        SET_COLOR(EFI_RED, EFI_BACKGROUND_BLACK); \
        PRINTERR(u"Error in "); \
        PRINTERR(u#expr); \
        PRINTERR(u" - Status: 0x"); \
        PrintHex(status); \
        PRINTERRL(u""); \
        RESET_COLOR(); \
        return status; \
    } \
}

// Memory management wrappers
static inline void* AllocatePool(uint64_t size) {
    void *buffer = NULL;
    ST->BootServices->AllocatePool(EfiLoaderData, size, &buffer);
    return buffer;
}

static inline void FreePool(void *buffer) {
    if (buffer != NULL) {
        ST->BootServices->FreePool(buffer);
    }
}

// Wait for key press
static inline EFI_STATUS WaitForKeyPress(void) {
    EFI_INPUT_KEY key;
    EFI_STATUS status;
    
    // Flush any existing keys in the buffer
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) != EFI_NOT_READY);
    
    // Wait for a new key press
    EFI_EVENT events[1];
    events[0] = ST->ConIn->WaitForKey;
    uint64_t index;
    
    status = ST->BootServices->WaitForEvent(1, events, &index);
    if (EFI_ERROR(status)) {
        return status;
    }
    
    // Read the key
    return ST->ConIn->ReadKeyStroke(ST->ConIn, &key);
}

// Utility: print a hex number
static inline void PrintHex(uint64_t value) {
    char16_t hex_str[17]; // 16 digits max for uint64_t + null terminator
    char16_t *hex_chars = u"0123456789ABCDEF";
    
    for (int i = 15; i >= 0; i--) {
        hex_str[i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    
    hex_str[16] = 0; // Null-terminate
    
    // Skip leading zeros
    char16_t *start = hex_str;
    while (*start == u'0' && *(start+1) != 0) {
        start++;
    }
    
    PRINT(start);
}

// Utility: print a decimal number
static inline void PrintDec(uint64_t value) {
    char16_t dec_str[21]; // 20 digits max for uint64_t + null terminator
    int pos = 20;
    
    dec_str[pos] = 0; // Null-terminate
    
    // Special case for zero
    if (value == 0) {
        PRINT(u"0");
        return;
    }
    
    while (value > 0 && pos > 0) {
        dec_str[--pos] = u'0' + (value % 10);
        value /= 10;
    }
    
    PRINT(&dec_str[pos]);
}

// Simple string utility functions
static inline uint64_t StrLen(const char16_t *str) {
    uint64_t len = 0;
    while (str[len] != 0) {
        len++;
    }
    return len;
}

static inline int StrCmp(const char16_t *str1, const char16_t *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

static inline void StrCpy(char16_t *dest, const char16_t *src) {
    while ((*dest++ = *src++) != 0);
}

static inline void MemSet(void *buffer, uint8_t value, uint64_t size) {
    uint8_t *buf = (uint8_t*)buffer;
    for (uint64_t i = 0; i < size; i++) {
        buf[i] = value;
    }
}

static inline void MemCpy(void *dest, const void *src, uint64_t size) {
    uint8_t *d = (uint8_t*)dest;
    const uint8_t *s = (const uint8_t*)src;
    for (uint64_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
}

#endif // TINYUEFI_HELPERS_H
