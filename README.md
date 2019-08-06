# Lumia930Pkg
WIP Custom ARM UEFI firmware for Lumia930

![alt text](https://blog.rickliu.im/wp-content/uploads/2019/04/IMG_20190308_220745.jpg "Boot manager on Lumia930")

# Current Status
EMMC MMU PMIC GPIO Working

# Linux Notes
Linux kernel do boots but only recognizes one core and kernel may crash. Implementation of MpPark is needed in the firmware to make all four cores work.\
You will need a device tree with the kernel.\
Some development of the device specific device tree is needed.

# TO-DO
MpPark
