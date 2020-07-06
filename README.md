# riscv-ovp-dbg

Toy application for Imperas iGui MPD Debugger.

## Imperas MPD

The Imperas MPD is a GDB-based debugger with multiprocessor support for OVP. It supports heterogeneous processors and TCL hooks. It has a graphical interface for Eclipse IDE (eGui) and a standalone lightweight one (iGui).

## Test environment

The test environment is composed of 3 RISC-V RV32I processors with a shared memory going from address 0x0 to 0xFFFF, local private memory from 0x10000 to 0x4FFFF and a private stack from 0xFFFF0000 to 0xFFFFFFFF as the image below shows:

<p align="center">
  <img src="docs/memory.png"/>
</p>

Each processors runs an instance of a fibonacci application, which calculates its values, puts in a buffer located in a shared memory and then checks if the values are correctly stored.

## Running the application

Run:
```
make run
```

Should produce the following output:

```
app3 starting
app2 starting
app1 starting
app2: buffer[0] corrupt
```

This is because the application failed to verify its values. Check [application/app2.c](application/app2.c) lines 42:47:
```
for ( b = 0 ; b < 16 ; b++ ) {
	if ( buffer[b] != v + NUM ) {
		printf("app%u: buffer[%u] corrupt\n", NUM, b);
		return 1;
	}
}
```

This means something is writing to the buffer[0] of the app2.

## Debugging the application with iGui MPD

To debug the application with iGui tool, run:
```
make gui
```

Click Targets > View, and in the new window click the cpu1 (which is the core running the app2.c)

<p align="center">
  <img src="docs/target.png"/>
</p>

Add a watchpoint to buffer[0] to see when it gets modified and ignore legit access by cpu1:
```
(Command): watch buffer[0]
(Command): watchignore 1 cpu1
```

Continue execution until the watchpoint gets triggered by clicking 'Cont':

<p align="center">
  <img src="docs/cont.png"/>
</p>

The event got triggered by cpu0 on line 38. Check that the address is being accessed by the loop iterating for one more step than it should:

<p align="center">
  <img src="docs/debug.png"/>
</p>

