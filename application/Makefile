IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined")
endif

CROSS?=OR1K
all: app1.$(CROSS).elf  app2.$(CROSS).elf

-include $(IMPERAS_HOME)/lib/$(IMPERAS_ARCH)/CrossCompiler/$(CROSS).makefile.include
ifeq ($($(CROSS)_CC),)
    IMPERAS_ERROR := $(error "Please install the $(CROSS)_CC toolchain")
endif

app1.$(CROSS).elf: app1.$(CROSS).o
	$(V) echo "Linking Application $@"
	$(V) $(IMPERAS_LINK) -g -o $@  $^  $(IMPERAS_LDFLAGS)

app1.$(CROSS).o: app1.c
	$(V) echo "Compiling Application $@"
	$(V) $(IMPERAS_CC) -g -c  $(IMPERAS_CFLAGS) -D$(CROSS)  -o $@  $<

app2.$(CROSS).elf: app2.$(CROSS).o
	$(V) echo "Linking Application $@"
	$(V) $(IMPERAS_LINK) -g -o $@  $^  $(IMPERAS_LDFLAGS)

app2.$(CROSS).o: app2.c
	$(V) echo "Compiling Application $@"
	$(V) $(IMPERAS_CC) -g -c  $(IMPERAS_CFLAGS) -D$(CROSS)  -o $@  $<


clean:
	- rm -f app1.$(CROSS).elf app1.$(CROSS).o  app2.$(CROSS).elf app2.$(CROSS).o
