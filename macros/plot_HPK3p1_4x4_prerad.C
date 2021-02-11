#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "HPK3p1_4x4_prerad"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2019_04_April_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/confInfo/";
	mp.debug=false;
	//Define run range
	mp.run_start = new vector<int>{12416,12500};
	mp.run_end = new vector<int>{12499,12552};
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{-2.5,-2.5}; //degrees
	mp.x_offset= new vector<float>{0.040,0.040}; //mm
	mp.y_offset= new vector<float>{0.0,0.0}; //mm
	
	//Define xy binning and ranges [mm]
	mp.nbinsX=160;//30;
	mp.nbinsY=80;//10;
	mp.rebinFactor=2; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=4; mp.maxX=20;
	mp.minY=30; mp.maxY=38;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =640.;
	mp.nbinsAmp=50;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=0; mp.maxTime=1200;
	// mp.minTime=7.2e-9; mp.maxTime=8.8e-9;
	// mp.minTime=-800; mp.maxTime=800;

	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,40.);
	mp.photekMin=25;
	mp.photekMax=55;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={6,9,12,15};
	mp.xSliceMax={8.2,11.2,14.2,17.2};
	mp.ySliceMin={32.6,33.6,34.6,35.6};
	mp.ySliceMax={33,34,35,36};

	//Specify range for colz maps. -1 is ignored.
	// mp.zMinEff=0.6; mp.zMaxEff=-1.; 
	mp.zMinEff=0.; mp.zMaxEff=-1.; 
	mp.zMinGain=70.; mp.zMaxGain=110.; 
	// mp.zMinSigmat=0.01e-9; mp.zMaxSigmat=0.08e-9; 
	mp.zMinSigmat=10; mp.zMaxSigmat=80; 
	mp.zMinDeltat=350; mp.zMaxDeltat=725;
	// mp.zMinDeltat=-300; mp.zMaxDeltat=-1.;


	//Do everything
    mp.makeMaps();
    

}
# endif
