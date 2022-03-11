#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "FBK_2x8_8E14_combined_v13"; //This defines names of all output files
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2020_02_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v5/confInfo/";
	mp.debug=false;
	//Define run range
	mp.run_start = new vector<int>{27821,27976};  // d1 : 27821  d2 : 28019
	mp.run_end = new vector<int>{27892,28124};    // d1 : 27892  d2 : 28124
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{0.98,0.98}; //degrees  //1.9 for ~v2, v3 : 1.89 v4 : 1.85, v5 : 1.8, v6:1.7 v7 : 1.6 v8 : 1.5 v9:1.4 v10:1.55 v11:1.45
	mp.x_offset= new vector<float>{0.,0.2}; //mm d1 : 0 -> d2 : 0.2 
	mp.y_offset= new vector<float>{0.012,0.012}; //mm
	
	mp.residMeanX={2.4,0.};
	mp.residRangeX={500,500.};
	mp.residMeanY={38.,670.};
	mp.residRangeY={500,500.};

	//Define xy binning and ranges [mm]
    mp.nbinsX=100; //133
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
	mp.photekMin=50;
	mp.photekMax=100;

	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={12.5,14.5}; //10.2 9.2
	mp.xSliceMax={13.2,15.2}; //11.2 10.2
	mp.ySliceMin={29.7,31.7};
	mp.ySliceMax={30.4,32.4};

	//Specify range for colz maps. -1 is ignored.
	mp.zMinEff=0.; mp.zMaxEff=1.; 
	mp.zMinGain=24.; mp.zMaxGain=54; 
	mp.zMinSigmat=-1; mp.zMaxSigmat=-1; 
	mp.zMinDeltat=1.5e-9; mp.zMaxDeltat=-1.; 


	//Do everything
    mp.makeMaps();
    

}
# endif
