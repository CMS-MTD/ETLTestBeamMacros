#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "FBK_2x8"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2019_04_April_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/confInfo/";
	mp.debug=false;
	//Define run range
	mp.run_start = new vector<int>{11406,11484};
	mp.run_end = new vector<int>{11482,11550};
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{-2.5,-2.5}; //degrees
	mp.x_offset= new vector<float>{0.040-1.,0.040+9.}; //mm
	mp.y_offset= new vector<float>{0.0+1.,0.0+1.}; //mm
	
	//Define xy binning and ranges [mm]
	mp.nbinsX=260;
	mp.nbinsY=80;
	mp.rebinFactor=2; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=4; mp.maxX=30;
	mp.minY=30; mp.maxY=38;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =640.;
	mp.nbinsAmp=60;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=7.2e-9; mp.maxTime=8.8e-9;

	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,10.);
	mp.photekMin=25;
	mp.photekMax=55;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={10.2,9.2};
	mp.xSliceMax={11.2,10.2};
	mp.ySliceMin={32.4,34.5};
	mp.ySliceMax={33.4,35.5};

	//Specify range for colz maps. -1 is ignored.
	mp.zMinEff=0.8; mp.zMaxEff=1.; 
	mp.zMinGain=0.; mp.zMaxGain=50.; 
	mp.zMinSigmat=0.01e-9; mp.zMaxSigmat=0.08e-9; 
	mp.zMinDeltat=-1.; mp.zMaxDeltat=-1.; 


	//Do everything
    mp.makeMaps();
    

}
# endif
