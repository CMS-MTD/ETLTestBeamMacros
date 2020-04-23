#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "plot_FBK_2x8_8E14_Feb"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2020_02_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v5/confInfo/";
	mp.debug=false;
	//Define run range
	mp.run_start = new vector<int>{27821,27976,28277}; 
	mp.run_end = new vector<int>{27892,28124,28367};   
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{1.06,1.04,1.08};
	mp.x_offset= new vector<float>{0.04-1.,0.54-1.,-0.04-1.}; 
	mp.y_offset= new vector<float>{0.048+0.042,0.128+0.042,0.212+0.042}; //mm
	
	mp.residMeanX={2.4,0.,0.9};
	mp.residRangeX={500,500.,500};
	mp.residMeanY={38.,670.,110};
	mp.residRangeY={350,350,350};

	//Define xy binning and ranges [mm]
    mp.nbinsX=100; 
	mp.nbinsY=50;
	mp.rebinFactor=1; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=4; mp.maxX=24;
	mp.minY=26; mp.maxY=36;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =210.;
	mp.nbinsAmp=60;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=2.0e-9; mp.maxTime=5.0e-9; // swapped

	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,15.);
//	mp.hitThres=vector<float>(mp.npad+1,10.);
	mp.photekMin=50;
	mp.photekMax=100;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={8.5,14.8}; //10.2 9.2
	mp.xSliceMax={9.5,15.8}; //11.2 10.2
	mp.ySliceMin={29.7,31.7};
	mp.ySliceMax={30.4,32.4};

	//Specify range for colz maps. -1 is ignored.
	mp.zMinEff=0.1; mp.zMaxEff=1.; 
	mp.zMinGain=24.; mp.zMaxGain=54; 
	mp.zMinSigmat=-1; mp.zMaxSigmat=-1; 
//	mp.zMinSigmat=2.5e-9; mp.zMaxSigmat=3.5e-9; 
	mp.zMinDeltat=3e-9; mp.zMaxDeltat=3.5e-9; 
//	mp.zMinDeltat=1.5e-9; mp.zMaxDeltat=-1.; 


	//Do everything
    mp.makeMaps();
    

}
# endif
