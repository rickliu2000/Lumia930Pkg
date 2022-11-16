# Lumia930Pkg
- WIP Custom ARM UEFI firmware for Lumia930

# Current Status
- Repairing something missing
- Working：EMMC MMU PMIC GPIO

# Linux Notes
- **[NOW]**: Some things need to be configured and repaired to ensure the start to touch in Linux
- Linux kernel do boots but crashes pretty soon.\
- All four cores are up after a recent update of bootshim\
- You will need a device tree with the kernel.\
- Some development of the device specific device tree is needed.\
- For a partially working kernel, see https://github.com/rickliu2000/linux_nokia_msm8974

# TO-DO
- We no longer need to implement MpPark since the recent update of bootshim by - -@imbushuo. A ram patch stops core 1-3 from being powered on before the old firmware exits
