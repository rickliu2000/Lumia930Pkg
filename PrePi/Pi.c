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
#include <Library/PrePiHobListPointerLib.h>
#include <Library/DebugAgentLib.h>
#include <Ppi/GuidedSectionExtraction.h>
#include <Guid/LzmaDecompress.h>
#include <Library/SerialPortLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FrameBufferSerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/PerformanceLib.h>

#include <PiDxe.h>
#include "Pi.h"
// #include <PreInitializeVariableInfo.h>


#define IS_XIP() (((UINT64)FixedPcdGet64 (PcdFdBaseAddress) > mSystemMemoryEnd) || \
                  ((FixedPcdGet64 (PcdFdBaseAddress) + FixedPcdGet32 (PcdFdSize)) < FixedPcdGet64 (PcdSystemMemoryBase)))

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

#define FB_ADDR_REG             0xFD901E14
#define FB_NEW_ADDR             FixedPcdGet32(PcdMipiFrameBufferAddress)

UINT64 mSystemMemoryEnd = FixedPcdGet64(PcdSystemMemoryBase) +
                          FixedPcdGet64(PcdSystemMemorySize) - 1;

STATIC VOID
UartInit
(
    VOID
)
{
    // Move Framebuffer to the top
    MmioWrite32(FB_ADDR_REG,FB_NEW_ADDR);
    // Flush using CTL0_FLUSH and Flush VIG0
    MmioWrite32(0xfd900618,0x00000001);
    MmioWrite32(0xfd900718,0x00000001); 

    SerialPortInitialize();
    DEBUG ((EFI_D_ERROR, "\nTianoCore on Nokia Lumia 930 (ARM)\n"));
    DEBUG ((EFI_D_ERROR,  "Firmware version %s built %a %a\n\n",
	        (CHAR16*) PcdGetPtr(PcdFirmwareVersionString),
			        __TIME__,
				__DATE__
	));

  DEBUG((
        EFI_D_INFO | EFI_D_LOAD,
        "SRC0 at 0x%p\n",
        MmioRead32(0xFD901EA4)
  )); 
}

VOID
Main
(
    	IN VOID	    *StackBase,
	IN UINTN    StackSize,
	IN UINT64   StartTimeStamp
)
{
	EFI_HOB_HANDOFF_INFO_TABLE*   HobList;
    EFI_STATUS                    Status;

    UINTN                           MemoryBase = 0;
    UINTN                           MemorySize = 0;
    UINTN                           UefiMemoryBase = 0;
    UINTN 			    UefiMemorySize = 0;

    // Initialize UART.
    UartInit();

    // Declare UEFI region
    MemoryBase      = FixedPcdGet32(PcdSystemMemoryBase);
    MemorySize      = FixedPcdGet32(PcdSystemMemorySize);
    UefiMemoryBase  = MemoryBase + FixedPcdGet32(PcdPreAllocatedMemorySize);
    UefiMemorySize  = FixedPcdGet32(PcdUefiMemPoolSize);
    StackBase       = (VOID*) (UefiMemoryBase + UefiMemorySize - StackSize);

    //Declear the PI/UEFI memory region
    HobList = HobConstructor (
    (VOID*)UefiMemoryBase,
    UefiMemorySize,
    (VOID*)UefiMemoryBase,
    StackBase  // The top of the UEFI Memory is reserved for the stacks
    );
    //PrePeiSetHobList (HobList);

    StackSize = FixedPcdGet32 (PcdCoreCount) * PcdGet32 (PcdCPUCorePrimaryStackSize);

    DEBUG((
        EFI_D_INFO | EFI_D_LOAD,
        "UEFI Memory Base = 0x%p, Size = 0x%p, Stack Base = 0x%p, Stack Size = 0x%p\n",
        UefiMemoryBase,
        UefiMemorySize,
        StackBase,
        StackSize
    ));
    PrePeiSetHobList(HobList);
    // Initialize MMU and Memory HOBs (Resource Descriptor HOBs)
    Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Failed to configure MMU\n"));
        CpuDeadLoop();
    }else{
       DEBUG((EFI_D_INFO | EFI_D_LOAD, "MMU configured\n"));
    }

    // Initialize GIC
    Status = QGicPeim();
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "Failed to configure GIC\n"));
      CpuDeadLoop();
    }
    DEBUG((EFI_D_INFO | EFI_D_LOAD, "GIC configured\n"));

    DEBUG((
        EFI_D_INFO | EFI_D_LOAD,
        "SRC0_addrs 0= 0x%p, 1 = 0x%p, 2 = 0x%p, 3 = 0x%p\n 4 = 0x%p\n",
        MmioRead32(0xFD901E14),
        MmioRead32(0xFD902214),
        MmioRead32(0xFD902A14),
        MmioRead32(0xFD902E14),
        MmioRead32(0xFD990008)
  ));

  // Create the Stacks HOB (reserve the memory for all stacks)


  BuildStackHob ((UINTN)StackBase, StackSize);

  //TODO: Call CpuPei as a library
  BuildCpuHob (ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));
  // Store timer value logged at the beginning of firmware image execution
  //Performance.ResetEnd = GetTimeInNanoSecond (StartTimeStamp);

  // Build SEC Performance Data Hob
  //BuildGuidDataHob (&gEfiFirmwarePerformanceGuid, &Performance, sizeof (Performance));

  // Set the Boot Mode
  SetBootMode (ArmPlatformGetBootMode ());

  // Initialize Platform HOBs (CpuHob and FvHob)
  Status = PlatformPeim ();
  //ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Failed to Initialize Platform HOBS\n"));
    }else{
       DEBUG((EFI_D_INFO | EFI_D_LOAD, "Platform HOBS Initialized\n"));
  }

  // Now, the HOB List has been initialized, we can register performance information
  PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  // SEC phase needs to run library constructors by hand.
  ProcessLibraryConstructorList ();

  // Assume the FV that contains the SEC (our code) also contains a compressed FV.
  Status = DecompressFirstFv ();
  //ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "FV does not contains a compressed FV\n"));
    }else{
       DEBUG((EFI_D_INFO | EFI_D_LOAD, "FV contains a compressed FV\n"));
  }

  // Load the DXE Core and transfer control to it
  Status = LoadDxeCoreFromFv (NULL, 0);
  if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Failed to load DXE Core\n"));
    }else{
       DEBUG((EFI_D_INFO | EFI_D_LOAD, "Loading DXE Core\n"));
  }




    // We are done
    CpuDeadLoop();
}

VOID
CEntryPoint
(
    	IN  UINTN                     			  MpId,
	IN  VOID  					  *StackBase,
	IN  UINTN 					  StackSize
)
{
   UINT64 StartTimeStamp;
   ArmPlatformInitialize (MpId);

   if (ArmPlatformIsPrimaryCore (MpId) && PerformanceMeasurementEnabled ()) {
	       // Initialize the Timer Library to setup the Timer HW controller
		    TimerConstructor ();
		   // We cannot call yet the PerformanceLib because the HOB List has not been initialized
			StartTimeStamp = GetPerformanceCounter ();
   } else {
			StartTimeStamp = 0;
		  }

  // Data Cache enabled on Primary core when MMU is enabled.
  ArmDisableDataCache ();
  // Invalidate Data cache
  ArmInvalidateDataCache ();
  // Invalidate instruction cache
  ArmInvalidateInstructionCache ();
  // Enable Instruction Caches on all cores.
  ArmEnableInstructionCache ();

  // Define the Global Variable region when we are not running in XIP
  if (!IS_XIP()) {
    if (ArmPlatformIsPrimaryCore (MpId)) {
      if (ArmIsMpCore()) {
        // Signal the Global Variable Region is defined (event: ARM_CPU_EVENT_DEFAULT)
        ArmCallSEV ();
      }
    } else {
      // Wait the Primay core has defined the address of the Global Variable region (event: ARM_CPU_EVENT_DEFAULT)
      ArmCallWFE ();
    }
  }

   Main(StackBase, StackSize, StartTimeStamp);
}
