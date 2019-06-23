# This branch contains a not working source. There seems to be a overflow somewhere in the emmc dirver.
# Lumia930Pkg
WIP Custom ARM UEFI firmware for Lumia930

![alt text](https://blog.rickliu.im/wp-content/uploads/2019/04/IMG_20190308_220745.jpg "Boot manager on Lumia930")

# Current Status
MMU PMIC GPIO Working

# TO-DO
EMMC
Move memory regions that occupies the first 128mb to higher location to leave space for memory map of the linux kernel on arm\
Make mppark work by reparking the cores\
write new framebuffer location to 0xFD8C1214
