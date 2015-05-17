//-------------------------------------------------------------------
//  xml.h   --   DAX: DAQ for XENON
// 
//  the xml library contains functions to read the XML settings file
//
//  04.12.2007 MS  (18.07.2008)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// reads the XML settings file
int xml_readsettings(	char filename[100], 
			crate& crt, 
			digitizer& adc, 
			scaler& s,
			multipurposemodule& mp, 
			timeandtrigger& t, 
			var_graphics& graphics,
			u_int32_t& NoE, 
			int& EventsPerFile, 
			char *path,
			int& WriteToFile);

//-------------------------------------------------------------------
// check XML file for discriminator settings
int xml_setdiscr(	char filename[100]);
	
//-------------------------------------------------------------------
// stores a copy of the XML file with the data
int xml_storefile(	char *filename, 
			char *OutFileName);
	
