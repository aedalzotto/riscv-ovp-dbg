ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined, please setup Imperas/OVP environment")
endif
IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

#
# Makefile to build the test applications and run platform simulation
#

all: library applications 

applications:
	$(V) $(MAKE) -C application


ifeq ($(VLNVROOT),)
  IMPERAS_WARNING := $(warning "VLNVROOT not specified: This should be set to full path in which components are to be built. Defaulting to $(CURDIR)/lib")
  VLNVROOT:=$(CURDIR)/lib
endif

VLNVSRC?= components

# Define the module to load from the VLNV library
MODULEVENDOR=pucrs.br
MODULELIBRARY=module
MODULENAME=BuggyRiscv
MODULEVERSION=0.1


vlnvroot:
	@mkdir -p $(VLNVROOT)
	
# Build component library
library: vlnvroot
	$(V) $(MAKE) -f $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.library OPT=0 VLNVSRC=$(VLNVSRC) VLNVROOT=$(VLNVROOT) VERBOSE=$(VERBOSE)

# Start simulaiton
run: library applications
	@echo "Running Imperas Simulation Platform"
	$(V) harness.exe --vlnvroot $(VLNVROOT) \
	                 --modulevendor $(MODULEVENDOR) --modulelibrary $(MODULELIBRARY) --modulename $(MODULENAME) --moduleversion $(MODULEVERSION) \
	                 --program $(MODULENAME)/cpu0=application/app1.RISCV32.elf \
	                 --program $(MODULENAME)/cpu1=application/app2.RISCV32.elf \
                     --output imperas.log

# Launch Imperas in interactive mode 
debug: library applications
	@echo "Launching Imperas Simulation Platform (MPD Console)"
	$(V) harness.exe --vlnvroot $(VLNVROOT) \
                     --modulevendor $(MODULEVENDOR) --modulelibrary $(MODULELIBRARY) --modulename $(MODULENAME) --moduleversion $(MODULEVERSION) \
	                 --program $(MODULENAME)/cpu0=application/app1.RISCV32.elf \
	                 --program $(MODULENAME)/cpu1=application/app2.RISCV32.elf \
                     --output imperas.log \
                     --mpdconsole

gui: library applications
	@echo "Launching Imperas Simulation Platform (iGui)"
	$(V) harness.exe --vlnvroot $(VLNVROOT) \
	                 --modulevendor $(MODULEVENDOR) --modulelibrary $(MODULELIBRARY) --modulename $(MODULENAME) --moduleversion $(MODULEVERSION) \
	                 --program $(MODULENAME)/cpu0=application/app1.RISCV32.elf \
	                 --program $(MODULENAME)/cpu1=application/app2.RISCV32.elf \
                     --output imperas.log \
                     --mpdigui

clean:
	- rm -f idebug.log
	- $(MAKE) -C application clean
	- $(MAKE) -f $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.library OPT=0 VLNVSRC=$(VLNVSRC) VLNVROOT=$(VLNVROOT) VERBOSE=$(VERBOSE) clean


help:
	@echo The following targets are available to start simulation
	@echo debug   : run simulation using $(MODULEVENDOR)/$(MODULELIBRARY)/$(MODULENAME)/$(MODULEVERSION), starting in MPD console mode
	@echo gui     : run simulation using $(MODULEVENDOR)/$(MODULELIBRARY)/$(MODULENAME)/$(MODULEVERSION), starting eGui
