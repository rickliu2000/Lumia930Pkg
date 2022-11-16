#ifndef _DEVICE_MEMORY_MAP_H_
#define _DEVICE_MEMORY_MAP_H_

#include <Library/ArmLib.h>

#define MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT 64

/* Below flag is used for system memory */
#define SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES \
                EFI_RESOURCE_ATTRIBUTE_PRESENT |                 \
                EFI_RESOURCE_ATTRIBUTE_INITIALIZED |             \
                EFI_RESOURCE_ATTRIBUTE_TESTED |                  \
                EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |             \
                EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |       \
                EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | \
                EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |    \
                EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE

typedef enum { NoHob, AddMem, AddDev, HobOnlyNoCacheSetting, MaxMem } DeviceMemoryAddHob;

#define MEMORY_REGION_NAME_MAX_LENGTH 32

typedef struct {
	CHAR8                          Name[MEMORY_REGION_NAME_MAX_LENGTH];
	EFI_PHYSICAL_ADDRESS		   Address;
	UINT64						   Length;
	EFI_RESOURCE_TYPE			   ResourceType;
	EFI_RESOURCE_ATTRIBUTE_TYPE	   ResourceAttribute;
	ARM_MEMORY_REGION_ATTRIBUTES   ArmAttributes;
	DeviceMemoryAddHob			   HobOption;
	EFI_MEMORY_TYPE				   MemoryType;
} ARM_MEMORY_REGION_DESCRIPTOR_EX, *PARM_MEMORY_REGION_DESCRIPTOR_EX;

#define MEM_RES EFI_RESOURCE_MEMORY_RESERVED
#define MMAP_IO EFI_RESOURCE_MEMORY_MAPPED_IO
#define SYS_MEM EFI_RESOURCE_SYSTEM_MEMORY

#define SYS_MEM_CAP SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES
#define UNCACHEABLE EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE

#define Reserv EfiReservedMemoryType
#define Conv EfiConventionalMemory
#define BsData EfiBootServicesData
#define BsCode EfiBootServicesCode
#define RtData EfiRuntimeServicesData
#define RtCode EfiRuntimeServicesCode
#define MmIO EfiMemoryMappedIO
#define MaxMem EfiMaxMemoryType

#define WRITE_THROUGH ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH
#define DEVICE ARM_MEMORY_REGION_ATTRIBUTE_DEVICE
#define WRITE_BACK ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define UNCACHED_UNBUFFERED ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = 
{
	/*                        Address,	  Length,ResourceType,Resource Attribute, ARM MMU Attribute,HobOption, EFI Memory Type */

    {"New HLOS",             0x00000000, 0x07D00000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv },	

    //{"HLOS 0",               0x00100000, 0x00100000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv },
    //{"UEFI FD",              0x00200000, 0x00100000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv },
    //{"MPPark Code",          0x00300000, 0x00040000, MEM_RES, UNCACHEABLE, UNCACHED_UNBUFFERED, AddMem, RtCode },

    {"TZ Apps",              0x07D00000, 0x00300000, SYS_MEM, SYS_MEM_CAP, DEVICE, AddMem, Reserv },
    {"Subsystem Reserved 0", 0x08000000, 0x05900000, SYS_MEM, SYS_MEM_CAP, DEVICE, AddMem, Reserv },
    {"HLOS 3",               0x0D900000, 0x00300000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv},
    {"Subsystem Reserved 1", 0x0DC00000, 0x01400000, SYS_MEM, SYS_MEM_CAP, DEVICE, AddMem, Reserv },
    {"HLOS 4",               0x0F000000, 0x00500000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv },
    {"Subsystem Reserved 2", 0x0F580000, 0x00500000, SYS_MEM, SYS_MEM_CAP, DEVICE, AddMem, Reserv },
    {"SMEM",                 0x0FA00000, 0x00200000, MEM_RES, UNCACHEABLE, UNCACHED_UNBUFFERED, AddMem, MaxMem },
    {"NonRelocatable 0",     0x0FC00000, 0x00300000, SYS_MEM, SYS_MEM_CAP, DEVICE, AddMem, Reserv },
    {"FrameBuffer",          0x0FF00000, 0x00800000, MEM_RES, EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE, WRITE_THROUGH, AddMem, MaxMem },
    {"HLOS Memory 5",        0x10700000, 0x6C500000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv },
    {"Firmware location",    0x7CC00000, 0x00100000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, BsCode},
    {"UEFI MEM POOL",        0x7CD00000, 0x03300000, SYS_MEM, SYS_MEM_CAP, WRITE_BACK, AddMem, Conv },

    /* Other memory regions */
    {"IMEM Boot Base",       0xFE800000, 0x00040000, SYS_MEM, EFI_RESOURCE_ATTRIBUTE_INITIALIZED, DEVICE, NoHob, Conv },
    {"IMEM Cookie Base",     0xFC42B000, 0x00001000, MMAP_IO, EFI_RESOURCE_ATTRIBUTE_INITIALIZED, DEVICE, AddDev, Conv },	
    {"IMEM Cookie Base 2",   0xFE805000, 0x00001000, MMAP_IO, EFI_RESOURCE_ATTRIBUTE_INITIALIZED, DEVICE, AddDev, Conv },
  //{"RPM CODE_DATARAM Dump",0xFC100000, 0x00100000, MMAP_IO, EFI_RESOURCE_ATTRIBUTE_INITIALIZED, DEVICE, AddDev, Conv },

    /* Register regions */
    {"SOC REGISTER REG 1",   0xF9000000, 0x02000000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 2",   0xFC400000, 0x00002000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 3",   0xFC428000, 0x00008000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 4",   0xFC4AB000, 0x00001000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 5",   0xFC4C0000, 0x00040000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 6",   0xFD400000, 0x00D00000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 7",   0xFC4B8000, 0x00008000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },
    {"SOC REGISTER REG 8",   0xFC4A9000, 0x00001000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO },

    /* Terminator for MMU*/
    {"Terminator for MMU",   0},
    /* Terminator for LibMem */
    {"Terminator for LibMem",0xFFFFFFFF, 0x00001000, MMAP_IO, UNCACHEABLE, DEVICE, AddDev, MmIO }
};

#endif

