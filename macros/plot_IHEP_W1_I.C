#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "IHEP_W1_I_2x2"; //This defines names of all output files
	mp.sensor_name="IHEP W1"; //Must be contained in sensor name as defined in AirTable. This determines which scope channels to consider.
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/SensorBeam2022/LecroyScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/";
	mp.debug=false;

	//Define run range
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)
	mp.run_end = new vector<int>{53163};
	mp.run_start = new vector<int>{52953};


	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{0}; //degrees
	mp.x_offset= new vector<float>{2.265}; //mm
	mp.y_offset= new vector<float>{4.05}; //mm
	
	mp.dut_index = 7; //center of telescope
	mp.maxTrackChi2 = 20;
	mp.minTrackPlanes=9;
	mp.minTrackPix=2;


	//Define xy binning and ranges [mm]
	mp.nbinsX=200;//30;
	mp.nbinsY=200;//10;
	mp.rebinFactor=1; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=-2; mp.maxX=2;
	mp.minY=-2; mp.maxY=2;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =380.;
	mp.nbinsAmp=50;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=0; mp.maxTime=1500; //picoseconds
	mp.delta_t_sign=-1; //
	
	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,50.);
	mp.photekMin=200;
	mp.photekMax=350;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={-0.75,0.55};
	mp.xSliceMax={-0.55,0.75};
	mp.ySliceMin={-0.8,0.55};
	mp.ySliceMax={-0.65,0.75};
	
	mp.channels={3,4,5,6};
	//Force range for colz maps. -1 is ignored.
	// mp.zMinEff=0.6; mp.zMaxEff=-1.; 
	mp.zMinEff=0.; mp.zMaxEff=-1.; 
	mp.zMinGain=120.; mp.zMaxGain=-1.;  // Gain means MPV amplitude in mV, here...
	// mp.zMinSigmat=0.01e-9; mp.zMaxSigmat=0.08e-9; 
	mp.zMinSigmat=10; mp.zMaxSigmat=50; 
	mp.zMinDeltat=-1; mp.zMaxDeltat=-1;
	// mp.zMinDeltat=-300; mp.zMaxDeltat=-1.;


	//Do everything
    mp.makeMaps();
    

}
# endif
