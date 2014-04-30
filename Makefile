PROGRAM_NAME=frame_usage
obj-m += $(PROGRAM_NAME).o

.PHONY:all
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

.PHONY:clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


.PHONY:up
up:
	sudo insmod $(PROGRAM_NAME).ko

.PHONY:down
down:
	sudo rmmod $(PROGRAM_NAME).ko

.PHONY:re-up
re-up:
	make down
	make up

.PHONY:test
test:
	make
	sudo dmesg -c > /tmp/null
	make down
	make up
	-dmesg | grep rss
	-dmesg | grep sum
