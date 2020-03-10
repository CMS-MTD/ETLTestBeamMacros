#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip> // for setw()
#include <time.h>

#include "TSystem.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TBranch.h"
#include "TString.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TColor.h"

#include "utilities.C"
using namespace std;


class map_plotter{

public:

	pair<int,int> nLGADHitsAndChannel();
	int indexGoodPhotekHit();
	void InitBranches();
	void makeMaps();
	void FillChannelMap(TH2F * map, vector<TH2F*> effs);
	void FillSummaryMap(vector<TH2F*> v_map, TH2F * channel_map);
	void FillSummaryMapCoarse(TH2F* h_target, vector<TH2F*> v_map, TH2F* effmap, TH2F * channel_map, bool normalize=false);
	void FillSummary1D(vector<vector<TH1F*> > v_map, TH2F * channel_map, bool isX);
	void FillSummary1DCoarse(vector<vector<TH1F*> > v_map, vector<vector<TH1F*>> eff1d, TH2F * channel_map, bool isX);

	void FillAligned(TH2F* aligned, TH2F * separate, int dx, int dy, int minx, int miny);

	void ConvertMap(TH3F * h3, TH2F * h2, int type, int pad);
	void Convert1D(TH3F * h3, vector<TH1F*> h1, int type, bool isX, int pad);
	void FillBox(TH3F * h3, TH1D * h1, int ibox);
	void CleanMap(TH2F * map, float xmin, float xmax, float ymin, float ymax, bool scale);
	float GetEff(TH3F * h3, int x_lo, int x_hi, int y_lo, int y_hi, int den=0);

	void PrintSummaryMap(TH2F * h2,TString name,float min, float max);
	void PrintSummary1D(TH1F * h,TString name);
	TString tag;
	TString outDir;
	TString chainPath;
	bool debug;
	vector<int> * run_start;
	vector<int> * run_end;
	static const int nchan=4;
	static const int nchan_lgad=3;
	static const int npad=16;//3;//16;
	vector<float> hitThres;
	vector<float> residMeanX;
	vector<float> residRangeX;
	vector<float> residMeanY;
	vector<float> residRangeY;

	float photekMin;
	float photekMax;

	vector<float> * angle;
	vector<float> * x_offset;
	vector<float> * y_offset;
	float saturation;
	//Divide interior of each pad in subsections (3x3 grid)
	//Specify bounds on one corner, use avg pad width including dead region to extrapolate to other pads
	//Plot boundaries at end for sanity check.

	vector<float> y_div;
	vector<float> x_div;

	float npad_y;
	float npad_x;
	float pad_width;


	//efficiency
	int nbinsX;
	int nbinsY;
	int rebinFactor; //coarser binning for gain maps


	int minX;
	int maxX;
	float minY;
	float maxY;

	//// For 1D projections
	vector<float> xSliceMin;
	vector<float> xSliceMax;
	vector<float> ySliceMin;
	vector<float> ySliceMax;


	float nbinsAmp;
	float nbinsTime;
	float minAmp;
	float maxAmp;
	float minTime;
	float maxTime;

	float zMinEff;float zMaxEff;
	float zMinGain;float zMaxGain;
	float zMinDeltat;float zMaxDeltat;
	float zMinSigmat; float zMaxSigmat;


	//////


	TChain * t;
	TFile * outRootFile;

	Float_t amp[nchan];
	Float_t LP2_20[nchan];
	Float_t LP2_40[nchan];
	Float_t chi2;
	Float_t xResidBack;
	Float_t yResidBack;
	Int_t ntracks;
	Int_t nback;
	Int_t npix;
	Float_t y_dut[3];
	Float_t x_dut[3];

	Int_t run;
	Int_t gconf;
	vector<int> *pads = new vector<int>();
	vector<string> *sensors = new vector<string>();

	vector<TH3F*> v_h_eff;
	vector<TH3F*> v_h_eff_timing;
	vector<TH3F*> v_h_amp;
	vector<TH3F*> v_h_time;
	vector<TH3F*> v_h_run;

	TH2F * channel_map;

	vector<TH2F*> v_map_eff;
	vector<TH2F*> v_map_eff_timing;
	vector<TH2F*> v_map_nhits;
	vector<TH2F*> v_map_amp;
	vector<TH2F*> v_map_deltat;
	TH2F* map_deltat_normalized;
	TH2F* map_deltat_normalized_aligned;
	vector<TH2F*> v_map_sigmat;

	TH2F * cosmetic_map;

	vector<vector<TH1F*> > v_x_eff;
	vector<vector<TH1F*> > v_x_nhits;
	vector<vector<TH1F*> > v_x_amp;
	vector<vector<TH1F*> > v_x_deltat;
	vector<vector<TH1F*> > v_x_sigmat;

	vector<vector<TH1F*> > v_y_eff;
	vector<vector<TH1F*> > v_y_nhits;
	vector<vector<TH1F*> > v_y_amp;
	vector<vector<TH1F*> > v_y_deltat;
	vector<vector<TH1F*> > v_y_sigmat;

	vector<TH1D*> v_amp_dists;
	vector<TH1D*> v_run_dists;

	vector<TH1F*> v_eff_uniformity;
	vector<TH1F*> v_den_uniformity;


};


