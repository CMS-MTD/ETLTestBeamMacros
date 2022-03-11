#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "HPK3p1_4x4_4e14_400V"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2019_04_April_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/confInfo/";
	mp.debug=false;
	//Define run range
	mp.run_start = new vector<int>{13222,13224};
	mp.run_end = new vector<int>{13223,13244};
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	float dx = 12.;
  	float dy = 0.4;
  	float theta = 1.909; //TMath::ATan(dy/dx);
  	float costheta = TMath::Cos(theta);
  	float sintheta = TMath::Sin(theta);
	mp.angle = new vector<float>{-theta,-theta}; //degrees
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
	mp.nbinsAmp=60;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=1.0e-9; mp.maxTime=9.0e-9;

	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,10.);
	mp.photekMin=15;
	mp.photekMax=270;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={6,9,12,15};
	mp.xSliceMax={8.2,11.2,14.2,17.2};
	mp.ySliceMin={32.6,33.6,34.6,35.6};
	mp.ySliceMax={33,34,35,36};

	//Specify range for colz maps. -1 is ignored.
	mp.zMinEff=-1.; mp.zMaxEff=-1.; 
	mp.zMinGain=70.; mp.zMaxGain=110.; 
	mp.zMinSigmat=0.01e-9; mp.zMaxSigmat=0.08e-9; 
	mp.zMinDeltat=7.75e-9; mp.zMaxDeltat=-1.;


	//Do everything
    mp.makeMaps();
    

}
# endif
