
#include <vector>
#include <Riostream.h>
#include <string>
#include <TH1D.h>
#include <TEfficiency.h>
#include <TH2D.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMarker.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>


    
    TFile *hfileHPK2 = new TFile("output/HPK2_2x2_February2021/HPK2_2x2_February2021.root","READ");
    
    TFile *interpad = new TFile("interpad.root", "RECREATE");

    TGraphAsymmErrors* slice0= dynamic_cast<TGraphAsymmErrors*>(hfileHPK2->Get("efficiency_slice0_all_chan"));
    TGraphAsymmErrors* slice1= dynamic_cast<TGraphAsymmErrors*>(hfileHPK2->Get("efficiency_slice1_all_chan"));
    TGraphAsymmErrors* slice2= dynamic_cast<TGraphAsymmErrors*>(hfileHPK2->Get("efficiency_slice2_all_chan"));  
    TGraphAsymmErrors* slice3= dynamic_cast<TGraphAsymmErrors*>(hfileHPK2->Get("efficiency_slice3_all_chan")); 
    


std::vector<double> fit_erf_func(TGraphAsymmErrors* slice, int slicenum, TCanvas* canvas ,double rgcurve1min, double rgcurve1max, double rgcurve2min, double rgcurve2max, double rgcurve3min, double rgcurve3max, double rgcurve4min, double rgcurve4max, double hfcurve1, double hfcurve2, double hfcurve3, double hfcurve4)

{
   

    
    //erf: left edge 
    //erfc (error function complement): right edge
 
    TF1* ff1=new TF1("ff1","TMath::Erf((x-[0])/[1])*[2]+[3]",rgcurve1min,rgcurve1max);
    TF1* ff2=new TF1("ff2","TMath::Erfc((x-[0])/[1])*[2]+[3]",rgcurve2min,rgcurve2max);
    TF1* ff3=new TF1("ff3","TMath::Erf((x-[0])/[1])*[2]+[3]",rgcurve3min,rgcurve3max);
    TF1* ff4=new TF1("ff4","TMath::Erfc((x-[0])/[1])*[2]+[3]",rgcurve4min,rgcurve4max);
 
    ff1->SetParameter(0,hfcurve1); //50% of the s-curve
    ff1->SetParameter(1,0.010); //expected resolution
    ff1->SetParameter(2,0.5); //height of the curve
    ff1->SetParameter(3,0.5); //baseline
    ff2->SetParameter(0,hfcurve2);
    ff2->SetParameter(1,0.010); 
    ff2->SetParameter(2,0.5); 
    ff2->SetParameter(3,0);
    ff3->SetParameter(0,hfcurve3);
    ff3->SetParameter(1,0.010); 
    ff3->SetParameter(2,0.5); 
    ff3->SetParameter(3,0.5);
    ff4->SetParameter(0,hfcurve4);
    ff4->SetParameter(1,0.010); 
    ff4->SetParameter(2,0.5); 
    ff4->SetParameter(3,0);
  
    


    slice->Fit("ff1","R");
    slice->Fit("ff2","R");
    slice->Fit("ff3","R");
    slice->Fit("ff4","R");

 
    canvas->cd();
 
    slice->Draw();
    slice->SetTitle(("Efficiency All Chan "+to_string(slicenum)).c_str());
    slice->SetLineColor(kBlue);
    ff1->Draw("SAME");
    ff2->Draw("SAME");
    ff3->Draw("SAME");
    ff4->Draw("SAME");


    double x1=(ff1->GetParameter(1));
    double x2=(ff2->GetParameter(1));
    double x3=(ff3->GetParameter(1));
    double x4=(ff4->GetParameter(1));
    double xaverage=(x1+x2+x3+x4)/4;

    double x1uncert=(ff1->GetParError(1));
    double x2uncert=(ff2->GetParError(1));
    double x3uncert=(ff3->GetParError(1));
    double x4uncert=(ff4->GetParError(1));

    cout<<"resolution 1"<< "  "<<x1<<"+/-"<<x1uncert<<endl;
    cout<<"resolution 2"<< "  "<<x2<<"+/-"<<x2uncert<<endl;
    cout<<"resolution 3"<< "  "<<x3<<"+/-"<<x3uncert<<endl;
    cout<<"resolution 4"<< "  "<<x4<<"+/-"<<x4uncert<<endl;
    cout<<"average resolution"<< "  "<<xaverage<<endl;


    double erfmidpoint1=(ff1->GetParameter(0));
    double erfmidpoint2=(ff2->GetParameter(0));
    double erfmidpoint3=(ff3->GetParameter(0));
    double erfmidpoint4=(ff4->GetParameter(0));

    cout<<"slice" <<slicenum <<" midpoints are"<< "  "<<erfmidpoint1<<" "<<erfmidpoint2<<" "<<erfmidpoint3<<" "<<erfmidpoint4<<endl;

    std::vector<double> sliceerfmidpointvalues{erfmidpoint1,erfmidpoint2,erfmidpoint3,erfmidpoint4};
    return sliceerfmidpointvalues;
}


double midpointlinfit(TCanvas* canvas, int curvenumber, double erfmidpointxvalues[4], double erfmidpointyvalues[4]){
    canvas->cd();
    TGraph* curvemidpointgraph =new TGraph(4,erfmidpointxvalues,erfmidpointyvalues);
    curvemidpointgraph->Draw();

    TF1* fflin=new TF1(("fflin"+ to_string(curvenumber)).c_str(),"[0]*x+[1]",0,25);
    curvemidpointgraph->Fit(("fflin"+ to_string(curvenumber)).c_str(),"R");
    fflin->Draw("SAME");
    double slope= (fflin->GetParameter(0));
    return slope;

}


void testbeammacro(){
    

    TCanvas* c1= new TCanvas("c1","c1",1200,800);
    TCanvas* c2= new TCanvas("c2","c2",1200,800);
    TCanvas* c3= new TCanvas("c3","c3",1200,800);
    TCanvas* c4= new TCanvas("c4","c4",1200,800);
    TCanvas* c5= new TCanvas("c5","c5",1200,800);
    TCanvas* c6= new TCanvas("c6","c6",1200,800);
    TCanvas* c7= new TCanvas("c7","c7",1200,800);
    TCanvas* c8= new TCanvas("c8","c8",1200,800);
    
    std::vector<double> erfmidpointslice0=fit_erf_func(slice0, 0, c1, -1.3, -1, -0.05, 0.105, 0.105, 0.25, 1.3, 1.4, -1.15, 0.06, 0.17, 1.37);
    std::vector<double> erfmidpointslice1=fit_erf_func(slice1, 1, c2, -1.3, -1, -0.05, 0.105, 0.105, 0.25, 1.3, 1.4, -1.15, 0.06, 0.17, 1.37);
    std::vector<double> erfmidpointslice2=fit_erf_func(slice2, 2, c3, -1.5, -1, -0.05, 0.105, 0.105, 0.25, 1.3, 1.5, -1.15, 0.06, 0.17, 1.37);
    std::vector<double> erfmidpointslice3=fit_erf_func(slice3, 3, c4, -1.3, -1, -0.05, 0.105, 0.105, 0.25, 1.3, 1.4, -1.15, 0.06, 0.17, 1.37);

    double merfmidpointcurve1[4]={static_cast<double>(erfmidpointslice0[0]),static_cast<double>(erfmidpointslice1[1]),static_cast<double>(erfmidpointslice2[0]),static_cast<double>(erfmidpointslice3[0])};
    double merfmidpointcurve2[4]={static_cast<double>(erfmidpointslice0[1]),static_cast<double>(erfmidpointslice1[1]),static_cast<double>(erfmidpointslice2[1]),static_cast<double>(erfmidpointslice3[1])};
    double merfmidpointcurve3[4]={static_cast<double>(erfmidpointslice0[2]),static_cast<double>(erfmidpointslice1[2]),static_cast<double>(erfmidpointslice2[2]),static_cast<double>(erfmidpointslice3[2])};
    double merfmidpointcurve4[4]={static_cast<double>(erfmidpointslice0[3]),static_cast<double>(erfmidpointslice1[3]),static_cast<double>(erfmidpointslice2[3]),static_cast<double>(erfmidpointslice3[3])};
  
    double erfmidpointyvalues[4]={19.65,20.1,21,21.45};
    double slope1= midpointlinfit(c5, 1, merfmidpointcurve1, erfmidpointyvalues);
    double slope2= midpointlinfit(c6, 2, merfmidpointcurve2, erfmidpointyvalues);
    double slope3= midpointlinfit(c7, 3, merfmidpointcurve3, erfmidpointyvalues);
    double slope4= midpointlinfit(c8, 4, merfmidpointcurve4, erfmidpointyvalues);    
    double avgslope= (slope1+slope2+slope3+slope4)/4;


    cout<<"average slope"<< "  "<<avgslope<<endl;

    double angle=(TMath::RadToDeg()*(TMath::ATan(avgslope))+90.0);

    cout<<"angle "<<angle<<endl;



    interpad->Write();
}
  





