obj-m := pix_mod.o
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/vinhpn/RaspberryPi/branch-4.14.y/linux M=$(PWD) modules
clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/vinhpn/RaspberryPi/branch-4.14.y/linux M=$(PWD) clean