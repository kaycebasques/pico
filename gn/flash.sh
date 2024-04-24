export ELF=./out/rp2040.size_optimized/obj/src/bin/$1.elf
picotool reboot -f -u
sleep 3
picotool load -x $ELF
