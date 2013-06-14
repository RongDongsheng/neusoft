MODULE_NAME :=testnl
obj-m   :=$(MODULE_NAME).o
PWD       := $(shell pwd)
#KERN_DIR = /home/rongdongsheng/develop/linux-kernel-2.6.32
#KERN_DIR = /usr/src/$(shell uname -r)  
KERN_DIR = /lib/modules/$(shell uname -r)/build  
all:  
	make -C $(KERN_DIR) M=$(PWD) 
#	gcc -o ${MODULE_NAME} testnl.c      
clean:
        make -C $(KERN_DIR) M=$(PWD) clean
obj-m += testnl.o  
