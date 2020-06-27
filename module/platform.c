#include "op/op.h"

#define APP_LDR_CTRL    OP_LDR_SET_START|OP_LDR_FAIL_IS_ERROR

static void constructPlatform(optModuleP mi)
{
	/* Get the processor path */
	const char *riscvModel = opVLNVString(
		0, // use the default VLNV path
		"riscv.ovpworld.org",
		"processor",
		"riscv",
		"1.0",
		OP_PROCESSOR,
		1 // report errors
	);

	/* Create the processor bus */
	optBusP bus0 = opBusNew(mi, "bus0", 32, 0, 0);

	/* Create the processor instance */
	optProcessorP processor0 = opProcessorNew(
		mi,
		riscvModel,
		"cpu0",
		/* Connect instruction and data to the same bus */
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "INSTRUCTION"),
				OP_BUS_CONNECT(bus0, "DATA")
			)
		),
		OP_PARAMS(
			OP_PARAM_STRING_SET("variant", "RV32I")
		)
		
	);

	/* Create the processor bus */
	optBusP bus1 = opBusNew(mi, "bus1", 32, 0, 0);

	optProcessorP processor1 = opProcessorNew(
		mi,
		riscvModel,
		"cpu1",
		/* Connect instruction and data to the same bus */
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus1, "INSTRUCTION"),
				OP_BUS_CONNECT(bus1, "DATA")
			)
		),
        OP_PARAMS(
			OP_PARAM_STRING_SET("variant", "RV32I")
		)
	);

	/* Get the semihost file */
	const char *riscvSemihost = opVLNVString(
		0, // use the default VLNV path
		0,
		0,
		"pk",
		0,
		OP_EXTENSION,
		1 // report errors
	);

	/* Set the CPU semihost */
	opProcessorExtensionNew(
		processor0,
		riscvSemihost,
		"pk_0",
		0
	);

	opProcessorExtensionNew(
		processor1,
		riscvSemihost,
		"pk_1",
		0
	);

	//optBusP bus_s = opBusNew(mi, "bus_s", 32, 0, 0);
	
	// Create and connect shared memory 
	
	opMemoryNew(
		mi,
		"shared",
		OP_PRIV_RWX,
		(0x00040000ULL)-(0x00020000ULL),
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "sp0", .slave=1, .addrLo=0x00020000ULL, .addrHi=0x00040000ULL),
				OP_BUS_CONNECT(bus1, "sp1", .slave=1, .addrLo=0x00020000ULL, .addrHi=0x00040000ULL)
			)
		),
		0
	);
	
	/* Create and connect processor0 memory */
	opMemoryNew(
		mi,
		"local0",
		OP_PRIV_RWX,
		(0x0001ffffULL)-(0x0ULL),
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "mp1", .slave=1, .addrLo=0x0ULL, .addrHi=0x0001ffffULL)
			)
		),
		0
	);

	/* Create and connect processor0 stack memory */
	
	opMemoryNew(
		mi,
		"stack0",
		OP_PRIV_RWX,
		0x00000fff,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "mp2", .slave=1, .addrLo=0xfffff000ULL, .addrHi=0xffffffffULL)
			)
		),
		0
	);
	
	/* Create and connect processor1 memory */
	opMemoryNew(
		mi,
		"local1",
		OP_PRIV_RWX,
		(0x0001ffffULL)-(0x0ULL),
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus1, "mp1", .slave=1, .addrLo=0x0ULL, .addrHi=0x0001ffffULL)
			)
		),
		0
	);

 	/* Create and connect processor1 stack memory */
 	
	opMemoryNew(
		mi,
		"stack1",
		OP_PRIV_RWX,
		0x00000fff,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus1, "mp2", .slave=1, .addrLo=0xfffff000ULL, .addrHi=0xffffffffULL)
			)
		),
		0
	);
	

	opProcessorApplicationLoad(processor0, "application/app1.RISCV32.elf", APP_LDR_CTRL, 0);
	opProcessorApplicationLoad(processor1, "application/app2.RISCV32.elf", APP_LDR_CTRL, 0);

}

int main(int argc, const char *argv[])
{
	/* Init OP */
	opSessionInit(OP_VERSION);

	/* Default parser. --program argument sets the elf */
	opCmdParseStd(argv[0], OP_AC_ALL, argc, argv);

	/* Init root module enabling verbose mode to get statistics */
	optParamP paramList = 0;
	paramList = opParamBoolSet(paramList, OP_FP_VERBOSE, 1);
	paramList = opParamBoolSet(paramList, OP_FP_STOPONCONTROLC, 1);
	optModuleP mi = opRootModuleNew(0, 0, paramList);

	constructPlatform(mi);

	opRootModuleSimulate(mi);
	opSessionTerminate();

	return 0;
} 