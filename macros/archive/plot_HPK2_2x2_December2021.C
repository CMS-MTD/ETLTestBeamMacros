#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "HPK2_2x2_December2021"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2021_CMSTiming_ETL/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v5/confInfo/";
	mp.debug=false;
	//Define run range
	// mp.run_start = new vector<int>{31220,31272};
	// mp.run_end = new vector<int>{31250,31283};
	// mp.run_start = new vector<int>{31201,31251};
	// mp.run_end = new vector<int>{31219,31271};
	mp.run_start = new vector<int>{31201};
	mp.run_end = new vector<int>{31283};

	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{0.345428}; //degrees
	mp.x_offset= new vector<float>{0.0}; //mm
	mp.y_offset= new vector<float>{0.0}; //mm
	
	mp.dut_index = 0; //center of telescope
	mp.maxTrackChi2 = 30;

	//Define xy binning and ranges [mm]
	mp.nbinsX=300;//30;
	mp.nbinsY=20;//10;
	mp.rebinFactor=1; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=17; mp.maxX=20;
	mp.minY=19.2; mp.maxY=22.2;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =350.;
	mp.nbinsAmp=50;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=2500; mp.maxTime=3500;
	// mp.minTime=7.2e-9; mp.maxTime=8.8e-9;
	// mp.minTime=-800; mp.maxTime=800;

	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,50.);
	mp.photekMin=60;
	mp.photekMax=130;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={17.5,19};
	mp.xSliceMax={18,19.5};
	mp.ySliceMin={19.65,20.1, 21,21.45};
	mp.ySliceMax={19.95,20.4, 21.3,21.75};

	//Specify range for colz maps. -1 is ignored.
	// mp.zMinEff=0.6; mp.zMaxEff=-1.; 
	mp.zMinEff=0.; mp.zMaxEff=-1.; 
	mp.zMinGain=-1.; mp.zMaxGain=-1.; 
	// mp.zMinSigmat=0.01e-9; mp.zMaxSigmat=0.08e-9; 
	mp.zMinSigmat=10; mp.zMaxSigmat=80; 
	mp.zMinDeltat=-1; mp.zMaxDeltat=-1;
	// mp.zMinDeltat=-300; mp.zMaxDeltat=-1.;


	//Do everything
    mp.makeMaps();
    

}
# endif
