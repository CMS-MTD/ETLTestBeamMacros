#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "FBK_2x8_8E14"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2020_02_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/confInfo/";
	mp.debug=false;
	//Define run range
	mp.run_start = new vector<int>{27821};
	mp.run_end = new vector<int>{27892};
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{-2.}; //degrees
	mp.x_offset= new vector<float>{0.0}; //mm
	mp.y_offset= new vector<float>{0.0}; //mm
	
	//Define xy binning and ranges [mm]
	mp.nbinsX=100;
	mp.nbinsY=50;
	mp.rebinFactor=2; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=4; mp.maxX=24;
	mp.minY=25; mp.maxY=38;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =210.;
	mp.nbinsAmp=60;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=-5.0e-9; mp.maxTime=-2.0e-9;

	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,15.);
	mp.photekMin=50;
	mp.photekMax=100;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={10.2,9.2};
	mp.xSliceMax={11.2,10.2};
	mp.ySliceMin={30.0,32.3};
	mp.ySliceMax={30.7,33.0};

	//Specify range for colz maps. -1 is ignored.
	mp.zMinEff=0.; mp.zMaxEff=1.; 
	mp.zMinGain=0.; mp.zMaxGain=-1; 
	mp.zMinSigmat=-1; mp.zMaxSigmat=-1; 
	mp.zMinDeltat=-1.; mp.zMaxDeltat=-1.; 


	//Do everything
    mp.makeMaps();
    

}
# endif
