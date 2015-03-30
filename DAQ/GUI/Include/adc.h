//-------------------------------------------------------------------
//  adc.h   --   DAX: DAQ for XENON100 
// 
//  the adc library contains functions to handle the CAEN v1724 fADCs
//
//  29.11.2007 MS  (09.04.08)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// performs a single VME write
//   returns -1 on failure, 0 on success
int adc_writereg(crate& crt, 		// crate 
		digitizer& adc,		// adc
		int i,			// the module to set
		u_int32_t addr,	// the register to write to
		u_int32_t data);	// the value to set

//-------------------------------------------------------------------
// performs a single VME read
//   returns -1 on failure, 0 on success
int adc_readreg(crate& crt, 		// crate 
		digitizer& adc,		// adc
		int i,			// the module to set
		u_int32_t addr,	// the register to write to
		u_int32_t& data);	// the value to read

//-------------------------------------------------------------------
// read the registers of the ADCs and set some variables
int adc_init( 	crate& crt,		// crate infos 
		digitizer& adc,		// adc infos 
		int& EnableInt, 	// interrupts enabled?
		int& EnableOLIrq, 	// OLIrq enabled?
		int& EnableBerr, 	// BERR enabled?
		int& EvAlign);		// EvAlign?

//-------------------------------------------------------------------
// enable ADCs
int adc_enable( crate& crt, 		// crate 
		digitizer& adc);	// adc

//-------------------------------------------------------------------
// disable ADCs
int adc_disable(crate& crt, 		// crate
		digitizer& adc);	// adc
				
//-------------------------------------------------------------------
// make MBLT from one ADC module
//   returns -1 if not succesful, otherwise the number of bytes read
int adc_mblt( 	crate& crt, 		// crate 
		digitizer& adc,		// adc
		int i,			// the module to read
		u_int32_t *buff);	// pointer to store the data

//-------------------------------------------------------------------
// determine baseline (in baseline-mode):
//   Board must be configured in non ZLE mode
//   internal trigger
//   calculate baseline 
int adc_getbaseline(crate& crt,		// crate
		digitizer& adc);	// ADC

//-------------------------------------------------------------------
// analyze baseline (mean, RMS, spikes)
int adc_analyzebaseline(crate& crt,	// crate
		digitizer& adc);	// ADC

//-------------------------------------------------------------------
// Close connection to VME crate(s)
int adc_closecrates(crate& crt);	// crate		

