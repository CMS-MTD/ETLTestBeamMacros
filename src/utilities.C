using namespace std;
#include "TEfficiency.h"


pair<float,float> Rotate(float x0, float y0, float angle);
void CosmeticMap(TH2F * map, TString zaxis);
void Cosmetic1D(TH1F * h);
void DrawCMS(bool is1D=false);
void DrawProton(bool is1D=false);
vector<float> GetEffandError(int pass, int total);
void DrawTemp(bool is1D=false);
TF1 *langaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF);
Double_t langaufun(Double_t *x, Double_t *par);
pair<float,float> GetMPV(TH1F * h);
pair<float,float> GetSigmaT(TH1F * h);


void Cosmetic1D(TH1F * h){
	h->SetLineWidth(1);
	h->SetStats(0);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleOffset(0.9);
	h->GetXaxis()->SetTitleOffset(1.1);
	h->GetXaxis()->SetLabelSize(0.045);
	h->GetYaxis()->SetLabelSize(0.045);
}

void CosmeticMap(TH2F * map, TString zaxis){
	map->SetStats(0);
	map->SetTitle("");
	map->SetTitle(Form(";x [mm];y [mm];%s",zaxis.Data()));
//	map->GetXaxis()->SetTitleOffset(0.95);
	map->GetXaxis()->SetTitleSize(0.05);
	map->GetYaxis()->SetTitleOffset(0.65);
	map->GetZaxis()->SetTitleOffset(0.8);
	map->GetYaxis()->SetTitleSize(0.05);
	map->GetZaxis()->SetTitleSize(0.055);
	
	map->GetZaxis()->SetLabelSize(0.045);


}

void DrawCMS(bool is1D){
	TLatex * tla = new TLatex();
    tla->SetTextSize(0.05);
	if(!is1D)tla->DrawLatexNDC(0.13,0.92,"#font[62]{CMS} #scale[0.8]{#font[52]{Phase-2 Preliminary}}");
	// else tla->DrawLatexNDC(0.11,0.92,"#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}");
}

void DrawProton(bool is1D){
  TLatex * tla = new TLatex();
  tla->SetTextSize(0.035);
  if(!is1D)tla->DrawLatexNDC(0.58,0.92,"FNAL 120 GeV proton beam");
  // else tla->DrawLatexNDC(0.1,0.92,"FNAL 120 GeV proton beam");
}

void DrawTemp(bool is1D){
  TLatex * tla = new TLatex();
    tla->SetTextSize(0.05);
  if(!is1D)tla->DrawLatexNDC(0.62,0.92,"HPK type 3.1, 195V, -20 C");
  // else tla->DrawLatexNDC(0.66,0.92,"HPK type 3.1, 195V, -20 C");
}

pair<float,float> Rotate(float x0, float y0, float angle){
	float rad_angle = angle*3.14159/180.;
 	float x_rot = x0*cos(rad_angle) + y0*sin(rad_angle);
 	float y_rot = y0*cos(rad_angle) - x0*sin(rad_angle);
 	return pair<float,float>{x_rot,y_rot};
}

pair<float,float> GetSigmaT(TH1F * h, float minTime, float maxTime){

	TF1* f1  = new TF1("f1","gaus",minTime,maxTime);
	h->Fit("f1","RQ");
	float sigma = f1->GetParameter(2);
	float e_sigma = f1->GetParError(2);
	float chi2pdf = f1->GetChisquare()/f1->GetNDF();
	f1->Delete();

	return pair<float,float> {sigma,e_sigma};

}

vector<float> GetEffandError(int pass, int total){
  TEfficiency teff("teff","",2,0,2);
  teff.SetStatisticOption(TEfficiency::kBUniform);
  teff.SetPosteriorMode();
  teff.SetTotalEvents(1,total);
  teff.SetPassedEvents(1,pass);
  vector<float> results;
  results.push_back(teff.GetEfficiency(1));
  results.push_back(teff.GetEfficiencyErrorLow(1));
  results.push_back(teff.GetEfficiencyErrorUp(1));

  return results;

}

vector<float> getStartingParameters(TH1F * h){
  float rms = h->GetRMS();
  float peakpos = h->GetXaxis()->GetBinCenter(h->GetMaximumBin());

  float startwidth = rms/5.;
  float startMPV = 0.8*h->GetMean();//peakpos;
  float startnorm = h->Integral();
  float startsigma = rms/10;

  vector<float> start_params = {startwidth,startMPV,startnorm,startsigma};
  return start_params;

}

float findLevel(float level, TH1F *h){
  int bin = 0;
  for(int i=1;i<=h->GetNbinsX();i++){
    if(h->GetBinContent(i) >= level){
      bin=i;
      break;
    }
  }
  if (bin==0) bin = h->GetNbinsX();
  return h->GetXaxis()->GetBinCenter(bin);
}

vector<float> getFitRange(TH1F *h){
  TH1F *h_cum = (TH1F*)h->GetCumulative(); //Underflow and overflow may not be included by default..
  h_cum->Scale(1./h->Integral());
  // h_cum->Set(h->GetNbinsX()+2,h->GetIntegral());
  float x_low = findLevel(0.05,h_cum);
  float x_high = findLevel(0.95,h_cum);
  vector<float> range = {x_low,x_high};
  return range;
}

pair<float,float> GetMPV(TH1F * h, float minAmp, float maxAmp, float hitThres){

    vector<float> start_parameters = getStartingParameters(h);
    // vector<float> range = getFitRange(h);
    double fr[2]={minAmp,maxAmp};
    // double st[4]={0.2*hitThres,2.5*hitThres,5000,hitThres/3.};
    // double fr[2]={minAmp,range[1]};
    double st[4]={start_parameters[0],start_parameters[1],start_parameters[2],start_parameters[3]};
    double pllo[4]={1.0,1.0*hitThres,1.0,1.0};
    double plhi[4]={2*hitThres,4.*hitThres,100000.0,hitThres/2.5};
    double fp[4],fpe[4];
    double CHI2;
    int NDF;

    TF1* f1 = langaufit(h ,fr , st, pllo, plhi, fp, fpe, &CHI2, &NDF);
    f1 -> SetLineColor(3);

    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1111);
  
    float mpv = f1->GetParameter(1);
	float e_mpv = f1->GetParError(1);
    
    
	return pair<float,float> {mpv,e_mpv};

}


Double_t langaufun(Double_t *x, Double_t *par) {

      Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      Double_t mpshift  = -0.22278298;       // Landau maximum location

      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;

      mpc = par[1] - mpshift * par[0];

      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];

      step = (xupp-xlow) / np;

      for(i=1.0; i<=np/2; i++) {
      	xx = xlow + (i-.5) * step;
      	fland = TMath::Landau(xx,mpc,par[0]) / par[0];
      	sum += fland * TMath::Gaus(x[0],xx,par[3]);

      	xx = xupp - (i-.5) * step;
      	fland = TMath::Landau(xx,mpc,par[0]) / par[0];
      	sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }

      return (par[2] * step * sum * invsq2pi / par[3]);
  }
  TF1 * langaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
  {

  	Int_t i;
  	Char_t FunName[100];

  	sprintf(FunName,"Fitfcn_%s",his->GetName());

  	TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
  	if (ffitold) delete ffitold;

  	TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
  	ffit->SetParameters(startvalues);
  	ffit->SetParNames("Width","MP","Area","GSigma");

  	for (i=0; i<4; i++) {
  		ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
  	}

   his->Fit(FunName,"RBQ");   // fit within specified range, use ParLimits, quiet

   ffit->GetParameters(fitparams);    // obtain fit parameters
   for (i=0; i<4; i++) {
      fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
  }
   ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
   NDF[0] = ffit->GetNDF();           // obtain ndf

   return (ffit);              // return fit function

}