#ifndef __FPAU_SWITCHES
#define __FPAU_SWITCHES

//------- HW Specific Switches ---------//
// Must be activated from top to bottom
#define RISCV_PROCESSOR  // Build for a RISC-V processor
#define UART             // Send string messages via UART
#define FPAU             // Include FPAU instructions

//------ Cycles Profiling Switches -----//
//#define PROFILING_NTT_INTT
//#define PROFILING_MAC
//#define PROFILING_ADD_SUB
#define PROFILING_STAGES

#ifdef STEEL                     // defined in compilation flags (makefile)
#define TICKS_REGISTER  "cycle"
#else
#define TICKS_REGISTER  "time"
#endif

//--- Testing Vectors Output Switches --//
// UART macro must be defined
//#define NTT_TESTING
//#define NTT_TESTING_HW
//#define INTT_TESTING_HW
//#define MAC_TESTING_HW
//#define SIGNATURE_TESTING_HW

#define HW_TESTING_SEEDINIT_TO_ZERO  // better always active


#endif //__FPAU_SWITCHES