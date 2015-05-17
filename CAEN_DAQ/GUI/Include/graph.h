//-------------------------------------------------------------------
//  graph.h   --   DAX: DAQ for XENON100 
// 
//  the graph library contains functions to plot the waveforms
//
//  10.12.2007 MS  (18.01.08)
//-------------------------------------------------------------------


#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>

//-------------------------------------------------------------------
// Initialize graphical Output
int graph_init(	TApplication *theApp,	// ...
		TCanvas *win,		// ...
		TH1D *g,		// Histogram
		var_graphics& graphics);// contains display settings

//-------------------------------------------------------------------
// Reconstruct waveform and display histogramm
int graph_display(TH1D *g, 		// Histogram			
		u_int32_t *buff,	// data to restore
		int channel);		// channel to display

int graph_displayNoZLE(TH1D *g, 
		u_int32_t *buff, 
		int channel);

//-------------------------------------------------------------------
// prepare graph for display
int graph_edit( TH1D *g,		// ...
		var_graphics& graphics, // ...
		digitizer& adc);	// ...

//-------------------------------------------------------------------
// keys for Oscilloscope mode
int graph_checkkey(var_graphics& graphics, 
		digitizer& adc,
		char c,
		TH1D *g,
		char *OutFileName);


