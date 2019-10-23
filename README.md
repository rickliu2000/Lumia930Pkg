# Lumia930Pkg
WIP Custom ARM UEFI firmware for Lumia930

![alt text](https://blog.rickliu.im/wp-content/uploads/2019/04/IMG_20190308_220745.jpg "Boot manager on Lumia930")

# Current Status
EMMC MMU PMIC GPIO Working

# Linux Notes
![alt text](https://blog.rickliu.im/wp-content/uploads/2019/03/Four_Penguins.jpeg)
Linux kernel do boots but crashes pretty soon.\
All four cores are up after a recent update of bootshim\
You will need a device tree with the kernel.\
Some development of the device specific device tree is needed.

# TO-DO
We no longer need to implement MpPark since the recent update of bootshim by @imbushuo. A ram patch stops core 1-3 from being powered on before the old firmware exits
