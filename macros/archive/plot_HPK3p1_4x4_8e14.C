#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
  map_plotter mp;
  mp.tag = "HPK3p1_4x4_8e14"; //This defines names of all output files
  mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/2019_04_April_CMSTiming/KeySightScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/confInfo/";
  mp.debug=false;
  //Define run range
  mp.run_start = new vector<int>{10473,11068};
  mp.run_end = new vector<int>{10602,11121};
  //Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)
  
  float angle = -3.1;
  //Define rotation angle and manual adjustments of x and y
  mp.angle = new vector<float>{angle,angle}; //degrees
  mp.x_offset= new vector<float>{0.040,0.040}; //mm
  mp.y_offset= new vector<float>{0.0,0.0}; //mm
  
  //Define xy binning and ranges [mm]
  mp.nbinsX=120;//160;//30;
  mp.nbinsY=60;//80;//10;
  mp.rebinFactor=3; //coarser binning for maps of gain, timing (finer for efficiency)
  mp.minX=4; mp.maxX=18;
  mp.minY=30; mp.maxY=36;

  //Define amplitude, time binning and range, and scope saturation.
  mp.saturation = 200.;
  mp.nbinsAmp=60;
  mp.minAmp=0; mp.maxAmp=mp.saturation;
  mp.nbinsTime=60;
  //mp.minTime=7.2e-9; mp.maxTime=8.8e-9;
  mp.minTime=5.e-9; mp.maxTime=8.0e-9;

  //define threshold for LGAD hits, and range for photek
  mp.hitThres=vector<float>(mp.npad+1,12.);
  //mp.hitThres[1]=12;
  //mp.hitThres[2]=14;
  //mp.hitThres[3]=12;
  //mp.hitThres[4]=14;
  mp.hitThres[5]=9.;
  //mp.hitThres[6]=13;
  mp.hitThres[7]=10;
  //mp.hitThres[8]=13;
  mp.hitThres[9]=10.;
  mp.hitThres[10]=10.;
  //mp.hitThres[11]=13;
  mp.hitThres[12]=10.;
  mp.hitThres[13]=10.;
  mp.hitThres[14]=10.;
  mp.hitThres[15]=10.;
  mp.hitThres[16]=11.;


	// Define geometric boundaries for 1D "slices"
	mp.xSliceMin={5.2,8.2,11.2,14.2};
	mp.xSliceMax={7.4,10.4,13.4,16.4};
	mp.ySliceMin={31.4,32.4,33.4,34.4};
	mp.ySliceMax={31.8,32.8,33.8,34.8};


  //Specify range for colz maps. -1 is ignored.
  mp.zMinEff=-1.; mp.zMaxEff=-1.; 
  mp.zMinGain=13.; mp.zMaxGain=22.; 
  mp.zMinSigmat=0.04e-9; mp.zMaxSigmat=0.08e-9; 
  mp.zMinDeltat=6.55e-9; mp.zMaxDeltat=6.9e-9; 
  
  
  //Do everything
  mp.makeMaps();
  
}
# endif
