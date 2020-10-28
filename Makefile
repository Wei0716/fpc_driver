obj-m := fpc.o
fpc-objs += module.o
fpc-objs += sys_hook.o
fpc-objs += hooks.o

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
