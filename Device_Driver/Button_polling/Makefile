obj-m += button.o 
KDIR = /home/vinhpn/RaspberryPi/branch-4.14.y/linux
all:	
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KDIR)  M=$(shell pwd) modules
clean:	
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KDIR)  M=$(shell pwd) clean