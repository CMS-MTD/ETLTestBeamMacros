#include "map_plotter.C"

# ifndef __CINT__
int main(int argc, char **argv)
{
	map_plotter mp;
	mp.tag = "FBK_5x5_group1"; //This defines names of all output files
	mp.sensor_name="FBK 29"; //Must be contained in sensor name as defined in AirTable. This determines which scope channels to consider.
	mp.chainPath = "root://cmseos.fnal.gov//store/group/cmstestbeam/SensorBeam2022/LecroyScope/RecoData/TimingDAQRECO/RecoWithTracks/v1/";
	mp.debug=false;

	mp.npad=26;
	
	//Define run range
	//Note: will try to load every run in this range, even if it doesn't exist (so, expect some harmless complaints.)
	//conf 588
	mp.run_end = new vector<int>{54563}; 
	mp.run_start = new vector<int>{54364}; //54364

	//Define rotation angle and manual adjustments of x and y
	mp.angle = new vector<float>{-0.75}; //degrees
	mp.x_offset= new vector<float>{0.0}; //mm
	mp.y_offset= new vector<float>{0.0}; //mm
	
	mp.dut_index = 7; //center of telescope
	mp.maxTrackChi2 = 50;
	mp.minTrackPlanes=8;
	mp.minTrackPix=2;

	//Define xy binning and ranges [mm]
	mp.nbinsX=100;//30;
	mp.nbinsY=100;//10;
	mp.rebinFactor=1; //coarser binning for maps of gain, timing (finer for efficiency)
	mp.minX=-3; mp.maxX=5;
	mp.minY=-6; mp.maxY=2;

	//Define amplitude, time binning and range, and scope saturation.
	mp.saturation =380.;
	mp.nbinsAmp=50;
	mp.minAmp=0; mp.maxAmp=mp.saturation;
	mp.nbinsTime=60;
	mp.minTime=0; mp.maxTime=1500; //picoseconds
	mp.delta_t_sign=-1; //
	
	//define threshold for LGAD hits, and range for photek
	mp.hitThres=vector<float>(mp.npad+1,50.);
	mp.channelSF = {1,1.11,1,1, 1,1,1,1};
	mp.photekMin=200;
	mp.photekMax=350;

	// Define geometric boundaries for 1D "slices"
	mp.ySliceMin={-2.1};
	mp.ySliceMax={-1.9};
	mp.xSliceMin={0.9};
	mp.xSliceMax={1.1};

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
