// Pi.c: Entry point for SEC(Security).

#include <PiPei.h>

#include <Pi/PiHob.h>
#include <Library/DebugLib.h>
#include <Library/PrePiLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/ArmLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/DebugAgentLib.h>
#include <Ppi/GuidedSectionExtraction.h>
#include <Guid/LzmaDecompress.h>
#include <Library/SerialPortLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FrameBufferSerialPortLib.h>

#include <PiDxe.h>
// #include <PreInitializeVariableInfo.h>

#ifndef _BGRA8888_COLORS_
#define _BGRA8888_COLORS_
#define BGRA8888_BLACK          0xff000000
#define BGRA8888_WHITE          0xffffffff
#define BGRA8888_CYAN           0xff00ffff
#define BGRA8888_BLUE           0xff0000ff
#define BGRA8888_SILVER         0xffc0c0c0
#define BGRA8888_YELLOW         0xffffff00
#define BGRA8888_ORANGE         0xffffa500
#define BGRA8888_RED            0xffff0000
#define BGRA8888_GREEN          0xff00ff00
#endif

#ifndef _FB_ADDRESS_
#define _FB_ADDRESS_
#define FB_ADDR                 0x400000
#endif

STATIC VOID
UartInit
(
    VOID
)
{
   
    SerialPortInitialize();
    DEBUG ((EFI_D_ERROR, "\nTianoCore on Nokia Lumia 930 (ARM)\n"));
    DEBUG ((EFI_D_ERROR, "UEFI"));
}

VOID
Main
(
    IN VOID  *StackBase,
    IN UINTN StackSize
)
{

    // Initialize UART.
    UartInit();

    // We are done
    CpuDeadLoop();
}

VOID
CEntryPoint
(
    IN  VOID  *StackBase,
    IN  UINTN StackSize
)
{
    Main(StackBase, StackSize);
}
