#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "IHEP_17_W7_1e15_2x2"; //This defines names of all output files
	mp.sensor_name="IHEP 2x2 17 W7 1e15 FNAL Board"; //Must be contained in sensor name as defined in AirTable. This determines which scope channels to consider.
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/SensorBeam2022/LecroyScope/RecoData/TimingDAQRECO/RecoWithTracks/v10/";
	mp.debug=false;

	//Define run range
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)
	mp.run_end = new vector<int>{61129};
	mp.run_start = new vector<int>{61016};


	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{0}; //degrees
	mp.x_offset= new vector<float>{0.0}; //mm
	mp.y_offset= new vector<float>{0.0}; //mm
	
	mp.dut_index = 7; //center of telescope
	mp.maxTrackChi2 = 50;

	//Define xy binning and ranges [mm]
	mp.nbinsX=40;//30;
	mp.nbinsY=40;//10;
	mp.rebinFactor=1; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=-6.0; mp.maxX=-1;
	mp.minY=-5.0; mp.maxY=0;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =380.;
	mp.nbinsAmp=50;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=0; mp.maxTime=2500; //picoseconds
	mp.delta_t_sign=1; //
	
	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,40.);
	mp.photekMin=200;
	mp.photekMax=350;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={-3.5,-2.2};
	mp.xSliceMax={-3.3,-1.8};
	mp.ySliceMin={-3.2,-1.7};
	mp.ySliceMax={-2.7,-1.4};

	//Force range for colz maps. -1 is ignored.
	// mp.zMinEff=0.6; mp.zMaxEff=-1.; 
	mp.zMinEff=0.; mp.zMaxEff=-1.; 
	mp.zMinGain=80.; mp.zMaxGain=-1.;  // Gain means MPV amplitude in mV, here...
	// mp.zMinSigmat=0.01e-9; mp.zMaxSigmat=0.08e-9; 
	mp.zMinSigmat=10; mp.zMaxSigmat=50; 
	mp.zMinDeltat=-1; mp.zMaxDeltat=-1;
	// mp.zMinDeltat=-300; mp.zMaxDeltat=-1.;


	//Do everything
    mp.makeMaps();
    

}
# endif
