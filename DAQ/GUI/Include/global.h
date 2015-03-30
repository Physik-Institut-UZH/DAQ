//-------------------------------------------------------------------
//  global.h   --   miniDAX: DAQ for XENON
// 
//  contains global #defines and variable definitions
//
//  25.01.2008 MS (changed 24.06.2008)
//-----------------
//  26.08.2008 MS	miniDAX version
//  29.11.2010 MS	CustomLength DAQ window
//-------------------------------------------------------------------

#include <sys/types.h>

// maximal number of ADCs and Crates
#define ADC_MAX	32	// max. possible value: 8x20=160
#define CRATE_MAX 2	// max. possible value: 8

// Digitizer Register Map (the list in uncomplete)
#define BoardInfoReg                0x8140
#define SoftTriggerReg              0x8108
#define AcquisitionControlReg       0x8100
#define PostTriggerReg              0x8114
#define ChannelEnableMaskReg        0x8120
#define TriggerSourceMaskReg        0x810C
#define ChannelFWRevision           0x108C
#define MotherBoardFWRevision       0x8124
#define EventSizeReg                0x814C
#define BlockOrganizationReg        0x800C
#define ChannelConfigReg            0x8000
#define VMEControlReg               0xEF00
#define BltEvNumReg                 0xEF1C
#define FrontPanelIOData	    0x811C
#define MonitorDacReg               0x8138
#define MonitorModeReg              0x8144
#define ZeroThreshold 		    0x1024
#define ZeroPrePost		    0x1028
#define DACRegister		    0x1098
#define SoftwareTrigger		    0x8108
#define ZLEThreshold		    0x8024
#define SingleZLEThresh		    0x1024		
#define LookBackForward		    0x8028
#define AnalogMonitorSettings	    0x8150
#define S1Threshold		    0x8080
#define CustomWindow		    0x8020

// Board Types
#define V1724   0
#define V1721   1
#define V1731   2
#define V1720   3

// definitions
#define FILLER       	0xFFFFFFFF
#define NO_DATA_TIME 	120		// DAQ will stop if no counts are recorded for this time (sec) 
#define SC_FILENAME	"miniDAX2sc"	// Path to Slow Control Communication File
#define FILE_ID		"mDAX"		// file identifier to create filename

//-------------------------------------------------------------------
// Variable to store information on ADCs
typedef struct
{
  int NumberOfADCs;		// total number of ADCs
  u_int32_t Address[ADC_MAX]; 	// Base Addresses
  int Id[ADC_MAX];		// Board Identifier
  int Crate[ADC_MAX];		// Crate in which Board sits
  int BltSize;			// Size of a Block Transfer
  int BoardInfo;		// Show only Board Infos?
  u_int32_t ExpectedEvSize; 	// ...
  int BufferSize;		// max. buffer to store data of one module
  int NbSamplesEvt;		// max. lenght of waveform
  int ZLE;			// zero length encoding activated? (1/0)
  int baseline;			// position of baselines
  int trig;			// 1: S1 trigger, 2: S2 trigger
  int busy;			// activate front panel to read busy status (1/0)?
  u_int32_t trigmask;		// trigger mask: which modules send signal to AnalogSum Output?
  int pmt[ADC_MAX][8];		// Store Pmt to Channel mapping: pmt[Module][Channel]=PMT_Number
  int mask[ADC_MAX];		// store mask of channels present on a board
  int pmtmax;			// store highest PMT number
  int NbChannels;		// how many channels are used
  int ch0;			// channel 0, DM
  int ch1;			// channel 1
  int ch2;			// channel 2
  int ch3;			// channel 3
  int ch4;			// channel 4
  int ch5;			// channel 5
  int ch6;			// channel 6
  int ch7;			// channel 7
  int channel[8];			// channels
  // properties of the ADC
  float voltage_range;		// in Volts
  int sample_size;		// in bits
  int sampling_freq;		// in Hz
  int TriggerLatency;
  int EventLength;              // in case a non-standard event window should be used
} digitizer;

//-------------------------------------------------------------------
// Variable to store information on Crates
typedef struct
{
  int NumberOfCrates;		// total Number of Crates
  int handle[CRATE_MAX];	// device handle
} crate;

//-------------------------------------------------------------------
// Variable to store time information (time, triggers, etc)
typedef struct
{
//    u_int64_t totnb; 		// total number of bytes read
//    u_int64_t last_totnb; 	// to calculate the difference
    u_int32_t totnb;            // total number of bytes read
    u_int32_t last_totnb;       // to calculate the difference
    int32_t CurrentTime;	// timing
    int32_t PreviousTime;	// ...
    u_int32_t tottrg;		// total number of triggers
    u_int32_t last_tottrg;  	// to calculate the difference
    u_int32_t last_ave;     	// to calculate the average
    int store10[10];		// to calculate 10s trigger average
    int MeasSeconds;		// the number of seconds the measurement is running    
    int DisplayTime;		// print to screen every x seconds
} timeandtrigger;

//-------------------------------------------------------------------
// Variable to store scaler information 
typedef struct
{
  int Crate;                    // crate controller identifier
  u_int32_t Address;        	// base address
  u_int32_t cnt[32];        	// counter values
  u_int32_t events;		// store number of recorded events per scaler cycle to logfile
  int timecnt;			// internal counter to check wether scaler have to be read
  int scalertime;		// scalers are read every <scalertime> seconds
  int32_t start;		// timing
  int32_t stop;			// timing
  int32_t old;			// timing
  double totaltime;		// total time (from external clock)
  double lifetime; 		// lifetime (ext. clock)
  double deadtime;		// deadtime (ext. clock)
  int totalcnt;			// scaler channels ...
  int lifecnt;
  int deadcnt;
} scaler;

//-------------------------------------------------------------------
// Variable for General Purpose Module
typedef struct
{
  int Crate;			// crate controller identifier
  u_int32_t Address;		// base address
  int HEVeto;			// High Energy Veto Status
} multipurposemodule;

//-------------------------------------------------------------------
// Variable to hold information for oscilloscope mode
typedef struct
{
  int on;			// graphics display switched on?
  int module;			// module to display
  int channel;			// channel to display
  int pmtNb;			// PMT to display
  int x_display;		// 0..all, 1..manual, 2..auto
  int y_display;		// 0..all, 1..manual, 2..auto
  int x_low;			// lower display border x-axis
  int x_high;			// upper display border x-axis
  int y_low;			// lower display border y-axis
  int y_high;			// upper display border y-axis
  int x_autorange;		// sets the scale for the x-axis auto-range

} var_graphics;

