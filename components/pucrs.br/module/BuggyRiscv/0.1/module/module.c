#include "op/op.h"

#define APP_LDR_CTRL    OP_LDR_SET_START|OP_LDR_FAIL_IS_ERROR
#define HARNESS_NAME "harness" 

static OP_CONSTRUCT_FN(moduleConstructor) 
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
	optProcessorP cpu0 = opProcessorNew(
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

	optProcessorP cpu1 = opProcessorNew(
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
		cpu0,
		riscvSemihost,
		"pk_0",
		0
	);

	opProcessorExtensionNew(
		cpu1,
		riscvSemihost,
		"pk_1",
		0
	);

	
	// Create and connect shared memory 
	opMemoryNew(
		mi,
		"shared",
		OP_PRIV_RWX,
		0x10000ULL-1,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "mp0", .slave=1, .addrLo=0x0ULL, .addrHi=0xFFFFULL),
				OP_BUS_CONNECT(bus1, "mp1", .slave=1, .addrLo=0x0ULL, .addrHi=0xFFFFULL)
			)
		),
		0
	);
	
	/* Create and connect processor0 memory */
	opMemoryNew(
		mi,
		"local0",
		OP_PRIV_RWX,
		0x40000ULL-1,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "mp0", .slave=1, .addrLo=0x10000ULL, .addrHi=0x4FFFFULL)
			)
		),
		0
	);

	/* Create and connect processor0 stack memory */
	
	opMemoryNew(
		mi,
		"stack0",
		OP_PRIV_RWX,
		0x10000ULL-1,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus0, "mp0", .slave=1, .addrLo=0xFFFF0000ULL, .addrHi=0xFFFFFFFFULL)
			)
		),
		0
	);
	
	/* Create and connect processor1 memory */
	opMemoryNew(
		mi,
		"local1",
		OP_PRIV_RWX,
		0x40000ULL-1,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus1, "mp1", .slave=1, .addrLo=0x10000ULL, .addrHi=0x4FFFFULL)
			)
		),
		0
	);

 	/* Create and connect processor1 stack memory */
	opMemoryNew(
		mi,
		"stack1",
		OP_PRIV_RWX,
		0x10000ULL-1,
		OP_CONNECTIONS(
			OP_BUS_CONNECTIONS(
				OP_BUS_CONNECT(bus1, "mp1", .slave=1, .addrLo=0xFFFF0000ULL, .addrHi=0xFFFFFFFFULL)
			)
		),
		0
	);
}

// int main(int argc, const char *argv[])
// {
// 	/* Init OP */
// 	opSessionInit(OP_VERSION);

// 	/* Default parser. --program argument sets the elf */
// 	opCmdParseStd(argv[0], OP_AC_ALL, argc, argv);

// 	/* Init root module enabling verbose mode to get statistics */
// 	optParamP paramList = 0;
// 	paramList = opParamBoolSet(paramList, OP_FP_VERBOSE, 1);
// 	paramList = opParamBoolSet(paramList, OP_FP_STOPONCONTROLC, 1);

// 	optModuleP mi = opRootModuleNew(0, 0, paramList);

// 	constructPlatform(mi);

// 	opRootModuleSimulate(mi);
// 	opSessionTerminate();

// 	return 0;
// }

optModuleAttr modelAttrs = {
 .versionString = OP_VERSION,
 .type = OP_MODULE,
 .name = HARNESS_NAME,
 .releaseStatus = OP_UNSET,
 .purpose = OP_PP_BAREMETAL,
 .visibility = OP_VISIBLE,
 .constructCB = moduleConstructor,
};