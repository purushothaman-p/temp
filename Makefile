## [M1: point 1]
#  Assigning the variable 'MODULE' with the string ex3
#  ...
MODULE	 = ex3

## [M2: point 1]
#  The variable 'obj-m' is appended with the concatenation of 'MODULE' and '.o' - 'ex3.o' 
#  ...
obj-m += $(MODULE).o

## [M3: point 1]
# The ?= operator sets the KERNELDIR variable with the build directory only if its not previously assigned any value.
# else it takes the prior value. uname -r gives the build version of the present kernel.
#  ...
KERNELDIR ?= /lib/modules/$(shell uname -r)/build

## [M4: point 1]
#  PWD variable is set to hold the present working directory string from the shell command.
#  ...
PWD := $(shell pwd)

## [M5: point 1]
#  make recipie for 'all'. the variable 'MODULE' which contains 'ex3' is a pre-requisite for this recipie
#  The all recipie runs all the recipies listed in the makefile and is the default recipie.
all: $(MODULE)

## [M6: point 1]
#  % is a wildcard. %.0 represents .0 files in the present directory. each %.o file requires a %.c corresponding to it.
#  the echo statement prints the C compiler(denoted by $(CC)) command along with the source file(denoted by $<) and output file(denoted by $@)
#  the @ before echo, suppresses the printing of the echo command and just prints the result of the echo command.
%.o: %.c
	@echo "  CC      $<"
	@$(CC) -c $< -o $@

## [M7: point 1]
#  recipie for 'ex3' executes makes and compiles the module into a kernel object
#  -C argument takes in kernel directory's location and M argument denotes the external module's location
#  ...
$(MODULE):
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

## [M8: point 1]
# The clean recipie removes all the files in the kernel directory and module directory which was compiled 
# by the previous command.
#  ...
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
