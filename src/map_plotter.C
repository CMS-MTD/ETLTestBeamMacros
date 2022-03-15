#include "map_plotter.h"
using namespace std;
void map_plotter::makeMaps(){

	if(run_start->size()!=run_end->size() || run_start->size()!=x_offset->size() || run_start->size()!=y_offset->size() || run_start->size()!=angle->size()){
		cout<<"Run ranges, x and y offsets, and rotation angles must all be the same dimension."<<endl;
	}

	outDir = Form("output/%s",tag.Data());
	gSystem->mkdir(outDir);
	TString outFileName = Form("%s/%s.root",outDir.Data(),tag.Data());
	outRootFile = new TFile(outFileName,"recreate");

	// for(int i=0;i<16;i++) {
	// 	deltat_metal.push_back(new TH1F(Form("deltat_metal%i",i),"",25,500,550));
	// 	deltat_nometal.push_back(new TH1F(Form("deltat_nometal%i",i),"",25,500,550));
	// }
	
	// deltat_metal = new TH1F("deltat_metal","",25,500,550);
	// deltat_nometal = new TH1F("deltat_nometal","",25,500,550);
	

	channel_map = new TH2F("channel_map","channel map",nbinsX,minX,maxX,nbinsY,minY,maxY);
	for(uint ib = 0; ib < npad+1; ib++){
		if (ib>0) outRootFile->mkdir(Form("pad%i",ib));
		TString name = Form("h3_eff_%i",ib);
		v_h_eff.push_back(new TH3F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY,2,0,2));
		name = Form("h3_eff_timing_%i",ib);
		v_h_eff_timing.push_back(new TH3F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY,2,0,2));

		name = Form("h_map_eff_%i",ib);
		v_map_eff.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
		name = Form("h_map_eff_timing_%i",ib);
		v_map_eff_timing.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));

		name = Form("h_map_nhits_%i",ib);
		v_map_nhits.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));

		name = Form("h_eff_uniformity_%i",ib);
		v_eff_uniformity.push_back(new TH1F(name,name,90,0,1.));
		name = Form("h_den_uniformity_%i",ib);
		v_den_uniformity.push_back(new TH1F(name,name,40,0,500.));
	//		v_den_uniformity.back()->SetStatOverflows();

		name = Form("h3_amp_%i",ib);
		v_h_amp.push_back(new TH3F(name,name,nbinsX/rebinFactor,minX,maxX,nbinsY/rebinFactor,minY,maxY,nbinsAmp,minAmp,maxAmp));
		name = Form("h_map_amp_%i",ib);
		if (ib==0) v_map_amp.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
		else v_map_amp.push_back(new TH2F(name,name,nbinsX/rebinFactor,minX,maxX,nbinsY/rebinFactor,minY,maxY));

		name = Form("h3_run_%i",ib);
		v_h_run.push_back(new TH3F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY,40,2900,3700));
		
		name = Form("h3_time_%i",ib);
		v_h_time.push_back(new TH3F(name,name,nbinsX/rebinFactor,minX,maxX,nbinsY/rebinFactor,minY,maxY,nbinsTime,minTime,maxTime));
		name = Form("h_map_deltat_%i",ib);
		if (ib==0) {
			v_map_deltat.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
			map_deltat_normalized = new TH2F("map_deltat_normalized","Normalized delta T",nbinsX,minX,maxX,nbinsY,minY,maxY);
			map_deltat_normalized_aligned = new TH2F("map_deltat_normalized_aligned","Normalized delta T",nbinsX,minX,maxX,nbinsY,minY,maxY);
		}
		else v_map_deltat.push_back(new TH2F(name,name,nbinsX/rebinFactor,minX,maxX,nbinsY/rebinFactor,minY,maxY));
		name = Form("h_map_sigmat_%i",ib);
		if (ib==0) v_map_sigmat.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
		else v_map_sigmat.push_back(new TH2F(name,name,nbinsX/rebinFactor,minX,maxX,nbinsY/rebinFactor,minY,maxY));



		//1D projections
		TString title;
		vector<TH1F*> these_x_eff;
		vector<TH1F*> these_x_nhits;
		vector<TH1F*> these_x_amp;
		vector<TH1F*> these_x_deltat;
		vector<TH1F*> these_x_sigmat;
		for(int islice=0;islice<ySliceMin.size();islice++){
			name = Form("h_x_eff_%i_%i",ib,islice);
			// title = Form("%0.1f < y < %0.1f;x [mm]; Efficiency;",ySliceMin[islice],ySliceMax[islice]);
			title = Form(";x [mm]; Efficiency;");
			these_x_eff.push_back(new TH1F(name,title,nbinsX,minX,maxX));
			name = Form("h_x_nhits_%i_%i",ib,islice);
			these_x_nhits.push_back(new TH1F(name,name,nbinsX,minX,maxX));

			name = Form("h_x_amp_%i_%i",ib,islice);
			// title = Form("%0.1f < y < %0.1f;x [mm]; MPV [mV];",ySliceMin[islice],ySliceMax[islice]);
			title = Form(";x [mm]; MPV [mV];");
			if (ib==0) these_x_amp.push_back(new TH1F(name,title,nbinsX,minX,maxX));
			else these_x_amp.push_back(new TH1F(name,title,nbinsX/rebinFactor,minX,maxX));

			name = Form("h_x_deltat_%i_%i",ib,islice);
			// title = Form("%0.1f < y < %0.1f;x [mm]; DeltaT[s];",ySliceMin[islice],ySliceMax[islice]);
			title = Form(";x [mm]; DeltaT[s];");
			if (ib==0) these_x_deltat.push_back(new TH1F(name,title,nbinsX,minX,maxX));
			else these_x_deltat.push_back(new TH1F(name,title,nbinsX/rebinFactor,minX,maxX));

			name = Form("h_x_sigmat_%i_%i",ib,islice);
			// title = Form("%0.1f < y < %0.1f;x [mm]; Time resolution [s];",ySliceMin[islice],ySliceMax[islice]);
			title = Form(";x [mm]; Time resolution [s];");
			if (ib==0) these_x_sigmat.push_back(new TH1F(name,title,nbinsX,minX,maxX));
			else these_x_sigmat.push_back(new TH1F(name,title,nbinsX/rebinFactor,minX,maxX));

		}
		v_x_eff.push_back(these_x_eff);
		v_x_nhits.push_back(these_x_nhits);
		v_x_amp.push_back(these_x_amp);
		v_x_deltat.push_back(these_x_deltat);
		v_x_sigmat.push_back(these_x_sigmat);

		vector<TH1F*> these_y_eff;
		vector<TH1F*> these_y_nhits;
		vector<TH1F*> these_y_amp;
		vector<TH1F*> these_y_deltat;
		vector<TH1F*> these_y_sigmat;
		for(int islice=0;islice<xSliceMin.size();islice++){
			name = Form("h_y_eff_%i_%i",ib,islice);
			title = Form("%0.1f < x < %0.1f;y [mm]; Efficiency;",xSliceMin[islice],xSliceMax[islice]);
			these_y_eff.push_back(new TH1F(name,title,nbinsY,minY,maxY));
			name = Form("h_y_nhits_%i_%i",ib,islice);
			these_y_nhits.push_back(new TH1F(name,name,nbinsY,minY,maxY));

			name = Form("h_y_amp_%i_%i",ib,islice);
			title = Form("%0.1f < x < %0.1f;y [mm]; MPV [mV];",xSliceMin[islice],xSliceMax[islice]);
			if (ib==0) these_y_amp.push_back(new TH1F(name,title,nbinsY,minY,maxY));
			else these_y_amp.push_back(new TH1F(name,title,nbinsY/rebinFactor,minY,maxY));

			name = Form("h_y_deltat_%i_%i",ib,islice);
			title = Form("%0.1f < x < %0.1f;y [mm]; DeltaT [s];",xSliceMin[islice],xSliceMax[islice]);
			if (ib==0) these_y_deltat.push_back(new TH1F(name,title,nbinsY,minY,maxY));
			else these_y_deltat.push_back(new TH1F(name,title,nbinsY/rebinFactor,minY,maxY));

			name = Form("h_y_sigmat_%i_%i",ib,islice);
			title = Form("%0.1f < x < %0.1f;y [mm]; Time resolution [s];",xSliceMin[islice],xSliceMax[islice]);
			if (ib==0) these_y_sigmat.push_back(new TH1F(name,title,nbinsY,minY,maxY));
			else these_y_sigmat.push_back(new TH1F(name,title,nbinsY/rebinFactor,minY,maxY));
		}

		v_y_eff.push_back(these_y_eff);
		v_y_nhits.push_back(these_y_nhits);
		v_y_amp.push_back(these_y_amp);
		v_y_deltat.push_back(these_y_deltat);
		v_y_sigmat.push_back(these_y_sigmat);
	}
	
	int ngoodevents=0;
	int ngoodtimingevents=0;
	t = new TChain("pulse");
	for(int i_runrange=0;i_runrange<run_start->size();i_runrange++){
		for(int irun=run_start->at(i_runrange);irun<=run_end->at(i_runrange);irun++) t->Add(Form("%s/run%i_info.root",chainPath.Data(),irun));
		// for(int irun=run_start->at(i_runrange);irun<=run_end->at(i_runrange);irun++) t->Add(Form("%s/run_scope%i_info.root",chainPath.Data(),irun));
		
		InitBranches();
		uint nentries= t->GetEntries();
		cout<<"Loaded trees from runs "<<run_start->at(i_runrange)<<" through "<<run_end->at(i_runrange)<<", with "<<nentries<<" events."<<endl;
		if(debug) nentries=100000;

		for(int i=0;i<nentries;i++){
			t->GetEntry(i);
			if (i % 10000 == 0) {
				fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries);
			}
			fflush(stdout);

		//Skip events without exactly one good track
			// if(ntracks!=1 || npix < 1 || nback < 1 || chi2 > maxTrackChi2) continue;
			if(ntracks!=1 || nplanes<minTrackPlanes || npix<minTrackPix || chi2 > maxTrackChi2) continue;
			// float xResid = abs(xResidBack - residMeanX.at(i_runrange));
			// float yResid = abs(yResidBack - residMeanY.at(i_runrange));

			// if(xResid>residRangeX.at(i_runrange) || yResid>residRangeY.at(i_runrange)) continue;
			int ptkindex = indexGoodPhotekHit();
			if (ptkindex== -1) continue;
			// if( LP2_20[ptkindex] < -112e-9 || LP2_20[ptkindex]>-108e-9) continue;
	

			pair<int,int> nhits_and_channel =nLGADHitsAndChannel();
			int nhits= nhits_and_channel.first;
			int channel = nhits_and_channel.second;

			ngoodevents++;
			if(debug){
				cout<<"Event number "<<i<<endl;
				cout<<"channel "<<channel<<endl;
				if(channel>=0){
					cout<<"amp "<<amp[channel]<<endl;
					cout<<"pad_index "<<pads->at(channel)<<endl;
					cout<<"nhits "<<nhits<<endl;
				}
			}

			

		//Get amplitude, time of real hit
		//Fill hists

		//Allow for rotation & offset of coordinates
			pair<float,float> rotated = Rotate(x_dut[dut_index],y_dut[dut_index],angle->at(i_runrange));
			float x_adjust = rotated.first + x_offset->at(i_runrange);
			float y_adjust = rotated.second + y_offset->at(i_runrange);


			if(nhits>=1)
			{	//Record hit for hit channel.
				nhits=1;
			int pad_index = pads->at(channel);//getPadIndex(channel);
			//Efficiency for simple threshold.
			v_h_eff[pad_index]->Fill(x_adjust,y_adjust,nhits); //should be nhits
			v_h_amp[pad_index]->Fill(x_adjust,y_adjust,amp[channel]);
			v_h_run[pad_index]->Fill(x_adjust,y_adjust,run);
				
			
			//Additional selection for timing measurement
			if(ptkindex>=0){
				
				if (LP2_20[channel] !=0 && amp[channel] < saturation){ //There is a good event for timing

					// float delta_t = -LP2_20[channel]+LP2_40[ptkindex]; //fix
					float delta_t = LP2_20[channel]-LP2_40[ptkindex]; //fix
					// delta_t += 16.1e-9;
					delta_t *=1e12;
					delta_t *=delta_t_sign;
					// delta_t -=7500;
					// delta_t -=510;

					v_h_time[pad_index]->Fill(x_adjust,y_adjust,delta_t);
					v_h_eff_timing[pad_index]->Fill(x_adjust,y_adjust,1);

				}
				else{//There is a hit based on amplitude, and a photek hit, but LGAD signal not adequate for timing
						//Record as miss for all channels.
					for(int iscope_chan=0; iscope_chan<nchan;iscope_chan++){
						int pad_index = pads->at(iscope_chan);
						if(sensors->at(iscope_chan).find("Photek")==std::string::npos  && sensors->at(iscope_chan).find(sensor_name)!=std::string::npos){ //not a photek channel)
							v_h_eff_timing[pad_index]->Fill(x_adjust,y_adjust,0);
						}
					}
				}
				
			}
		}
		else if(nhits==0)
		{ 	//If there is no hit, record it as a miss for all channels included this run.
			for(int iscope_chan=0; iscope_chan<nchan;iscope_chan++)
			{
				int pad_index = pads->at(iscope_chan);
				if(sensors->at(iscope_chan).find("Photek")==std::string::npos  && sensors->at(iscope_chan).find(sensor_name)!=std::string::npos){ //not a photek channel)
				v_h_eff[pad_index]->Fill(x_adjust,y_adjust,nhits);
				if(ptkindex>=0) v_h_eff_timing[pad_index]->Fill(x_adjust,y_adjust,0); //Don't penalize for bad photek hits
				}
			}
		}
		

	}//end of one run range
	t->Reset();
	cout<<"Processed "<<nentries<<" events, "<<ngoodevents<<" good events."<<endl;
}//end of all run ranges


		
		//Construct maps from 3D hists
		for(uint ie = 1; ie < v_h_eff.size(); ie++){
			outRootFile->Cd(Form("%s:/pad%i",outFileName.Data(),ie));
			ConvertMap(v_h_eff[ie],v_map_eff[ie],4,ie);
			ConvertMap(v_h_eff_timing[ie],v_map_eff_timing[ie],4,ie);
			ConvertMap(v_h_eff[ie],v_map_nhits[ie],2,ie);

			Convert1D(v_h_eff[ie],v_x_eff[ie],4,true,ie);
			Convert1D(v_h_eff[ie],v_x_nhits[ie],2,true,ie);

			Convert1D(v_h_eff[ie],v_y_eff[ie],4,false,ie);
			Convert1D(v_h_eff[ie],v_y_nhits[ie],2,false,ie);
			cout<<"pad number "<<ie<<": "<<v_map_nhits[ie]->Integral()<<" hits"<<endl;	
			v_h_eff[ie]->Write();
			v_h_eff_timing[ie]->Write();
			v_map_eff[ie]->Write();
			v_map_nhits[ie]->Write();
			// v_x_eff[ie]->Write();
			// v_x_nhits[ie]->Write();
			// v_y_eff[ie]->Write();
			// v_y_nhits[ie]->Write();
		}


		cout<<"Finished efficiency maps"<<endl;
		//Assign each x-y bin to one pad based on their efficiency.

		for(uint ie = 1; ie < v_h_amp.size(); ie++){
			outRootFile->Cd(Form("%s:/pad%i",outFileName.Data(),ie));
			ConvertMap(v_h_amp[ie],v_map_amp[ie],3,ie);
			Convert1D(v_h_amp[ie],v_x_amp[ie],3,true,ie);
			Convert1D(v_h_amp[ie],v_y_amp[ie],3,false,ie);
			v_h_amp[ie]->Write();
			v_map_amp[ie]->Write();
			// v_x_amp[ie]->Write();
			// v_y_amp[ie]->Write();
		}
		cout<<"Finished amp maps"<<endl;
		for(uint ie = 1; ie < v_h_time.size(); ie++){
			outRootFile->Cd(Form("%s:/pad%i",outFileName.Data(),ie));
			v_h_time[ie]->Write();
			ConvertMap(v_h_time[ie],v_map_deltat[ie],0,ie);
			v_map_deltat[ie]->Write();
			ConvertMap(v_h_time[ie],v_map_sigmat[ie],1,ie);
			v_map_sigmat[ie]->Write();
			Convert1D(v_h_time[ie],v_x_deltat[ie],0,true,ie);
			//v_x_deltat[ie]->Write();
			Convert1D(v_h_time[ie],v_x_sigmat[ie],1,true,ie);

			Convert1D(v_h_time[ie],v_y_deltat[ie],0,false,ie);
			//v_y_deltat[ie]->Write();
			Convert1D(v_h_time[ie],v_y_sigmat[ie],1,false,ie);
			//v_y_sigmat[ie]->Write();

		}
		cout<<"Finished time maps"<<endl;
		outRootFile->Cd(Form("%s:/",outFileName.Data()));
		cout<<"Calculating channel map"<<endl;
		FillChannelMap(channel_map, v_map_eff);
		channel_map->Write();

		CosmeticMap(v_map_eff[0],Form("Hit efficiency, %.0f mV threshold",hitThres[0]));
		FillSummaryMap(v_map_eff,channel_map);
		
		CosmeticMap(v_map_eff_timing[0],Form("Timestamp reco efficiency, %.0f mV",hitThres[0]));
		FillSummaryMap(v_map_eff_timing,channel_map);
		
		CosmeticMap(v_map_amp[0],"Most probable amplitude [mV]");
		FillSummaryMapCoarse(v_map_amp[0],v_map_amp,v_map_eff[0],channel_map);
		
		CosmeticMap(v_map_sigmat[0],"Time resolution [ps]");
		FillSummaryMapCoarse(v_map_sigmat[0],v_map_sigmat,v_map_eff[0],channel_map);
		
		CosmeticMap(v_map_deltat[0],"#DeltaT(LGAD - MCP) [ps]");
		FillSummaryMapCoarse(v_map_deltat[0],v_map_deltat,v_map_eff[0],channel_map);

		CosmeticMap(map_deltat_normalized,"#DeltaT(LGAD - MCP) [ps]");
		FillSummaryMapCoarse(map_deltat_normalized, v_map_deltat,v_map_eff[0],channel_map,true);
		
		

		FillSummary1D(v_x_eff,channel_map,true); 
		FillSummary1D(v_x_nhits,channel_map,true); 
		FillSummary1DCoarse(v_x_amp,v_x_eff,channel_map,true);
		FillSummary1DCoarse(v_x_sigmat,v_x_eff,channel_map,true);
		FillSummary1DCoarse(v_x_deltat,v_x_eff,channel_map,true);


		FillSummary1D(v_y_eff,channel_map,false);
		FillSummary1D(v_y_nhits,channel_map,false);
		FillSummary1DCoarse(v_y_amp,v_y_eff,channel_map,false);
		FillSummary1DCoarse(v_y_sigmat,v_y_eff,channel_map,false);
		FillSummary1DCoarse(v_y_deltat,v_y_eff,channel_map,false);


		PrintSummaryMap(v_map_eff[0],"map_efficiency",zMinEff,zMaxEff);
		PrintSummaryMap(v_map_amp[0],"map_mpv",zMinGain,zMaxGain);
		PrintSummaryMap(v_map_sigmat[0],"map_sigmat",zMinSigmat,zMaxSigmat);
		PrintSummaryMap(v_map_deltat[0],"map_deltat",zMinDeltat,zMaxDeltat);
		PrintSummaryMap(map_deltat_normalized,"map_deltat_norm",zMinDeltat,zMaxDeltat);
		
		ConvertTH1toTGraphAsymmErrors(v_x_eff[0],v_x_nhits[0],v_x_eff_graph,"all_chan");
		ConvertTH1toTGraphAsymmErrors(v_x_eff[4],v_x_nhits[4],v_x_eff_graph,"chan4");
		ConvertTH1toTGraphAsymmErrors(v_x_eff[5],v_x_nhits[5],v_x_eff_graph,"chan5");
		ConvertTH1toTGraphAsymmErrors(v_x_eff[3],v_x_nhits[3],v_x_eff_graph,"chan3");
		ConvertTH1toTGraphAsymmErrors(v_x_eff[6],v_x_nhits[6],v_x_eff_graph,"chan6");
		for(int i=0;i<xSliceMin.size();i++){
			PrintSummary1D(v_x_eff[0][i],Form("x_efficiency%i",i));
			// cout<<"Printing graph"<<endl;
			// PrintSummaryGraph(v_x_eff_graph[i],Form("x_efficiency_gr%i",i));

			PrintSummary1D(v_x_amp[0][i],Form("x_mpv%i",i));
			PrintSummary1D(v_x_sigmat[0][i],Form("x_sigmat%i",i));
			PrintSummary1D(v_x_deltat[0][i],Form("x_deltat%i",i));
		}

		CosmeticMap(map_deltat_normalized_aligned,"#DeltaT(LGAD - MCP) [ps]");
		FillAligned(map_deltat_normalized_aligned,map_deltat_normalized, 30, 10, 18, 25);
		PrintSummaryMap(map_deltat_normalized_aligned,"map_deltat_norm_aligned",510,535);


		//Fill distributions for boxes 
		// for(uint ie = 0; ie < v_h_amp.size(); ie++){
		// 	for(uint ibox=0;ibox<box_x_lo.size();ibox++ ){
		// 		FillBox(v_h_amp[ie],v_amp_dists[ie*box_x_lo.size()+ibox], ibox);
		// 		v_amp_dists[ie*box_x_lo.size()+ibox]->Write();
		// 		FillBox(v_h_run[ie],v_run_dists[ie*box_x_lo.size()+ibox], ibox);
		// 		v_run_dists[ie*box_x_lo.size()+ibox]->Write();

		// 	}
		// }

	
		// TLine line = TLine();
		// line.SetLineStyle(3);


			//Fill uniformity distribution

		//possible fit function: TF1 * f4 = new TF1("f4","[2](TMath::Binomial([0],x*[0])*pow([1],x*[0])*pow(1-[1],[0]-x*[0]))",0,1)
		//	for(uint ie = 0; ie < v_eff_uniformity.size(); ie++){
		// for(uint ib = 0; ib < base_sel.size(); ib++){
		// 	for(uint ie = 0; ie < eff_sel.size(); ie++){
		// 		int index = ie + (eff_sel.size())*ib;
		// 		for(int ix=0;ix<npad_x;ix++){
		// 			for(int iy=0;iy<npad_y;iy++){
		// 				for(int ix_div=0;ix_div<3;ix_div++){
		// 					for(int iy_div=0;iy_div<3;iy_div++){
		// 						//Find boundaries
		// 						int x_lo =  v_h_eff[index]->GetXaxis()->FindBin(x_div[ix_div]+pad_width * ix);
		// 						int x_hi = v_h_eff[index]->GetXaxis()->FindBin(x_div[ix_div+1]  + pad_width * ix);
		// 						int y_lo = v_h_eff[index]->GetYaxis()->FindBin(y_div[iy_div] - pad_width * iy);
		// 						int y_hi = v_h_eff[index]->GetYaxis()->FindBin(y_div[iy_div+1]  - pad_width * iy);

		// 						//Offset high edge by one to avoid double counting bins at run_boundaries, except for last subdivision of each pad.
		// 						if(ix_div!=3) x_hi-=1;
		// 						if(iy_div!=3) y_hi-=1;

		// 						v_eff_uniformity[index]->Fill(GetEff(v_h_eff[index],x_lo,x_hi,y_lo,y_hi));
		// 						v_den_uniformity[index]->Fill(GetEff(v_h_eff[index],x_lo,x_hi,y_lo,y_hi,1));

		// 						//Draw vertical lines
		// 						if(iy==0 && iy_div==0){ line.DrawLine(v_h_eff[index]->GetXaxis()->GetBinLowEdge(x_lo),18,v_h_eff[index]->GetXaxis()->GetBinLowEdge(x_lo),22);
		// 							if(ix_div==2) line.DrawLine(v_h_eff[index]->GetXaxis()->GetBinLowEdge(x_hi+1),18,v_h_eff[index]->GetXaxis()->GetBinLowEdge(x_hi+1),22);
		// 						}

		// 						//Draw horizontal lines
		// 						if(ix==0 && ix_div==0){ line.DrawLine(0,v_h_eff[index]->GetYaxis()->GetBinLowEdge(y_lo),16.8,v_h_eff[index]->GetYaxis()->GetBinLowEdge(y_lo));
		// 							if(iy_div==2) line.DrawLine(0,v_h_eff[index]->GetYaxis()->GetBinLowEdge(y_hi+1),16.8,v_h_eff[index]->GetYaxis()->GetBinLowEdge(y_hi+1));
		// 						}

		// 					} //loop over y pad subdivision
		// 				} //loop over x pad subdivision
		// 			} //loop over pads in y
		// 		}//loop over pads in x
		// 		v_eff_uniformity[index]->Write();
		// 		v_den_uniformity[index]->Write();

		// 	}//loop over efficiency selections
		// }// loop over baseline selections

		// c1.Print("grid_map.pdf");

		// TCanvas c4("","",800,600);
		// c4.SetLeftMargin(0.11);
		
		// c4.SetBottomMargin(0.13);
		// Cosmetic1D(v_eff_uniformity[0]);
		// v_eff_uniformity[0]->GetXaxis()->SetRangeUser(0.7,1.);
		// v_eff_uniformity[0]->SetTitle(";Efficiency; N_{subpads}");
		// v_eff_uniformity[0]->Draw();
		// TF1* f1  = new TF1("eff_uni","gaus",0,1.);
		// v_eff_uniformity[0]->Fit("eff_uni","RQ");
		// DrawCMS(true);
		// c4.Print("eff_uniformity.pdf");


	outRootFile->Close();
}
void map_plotter::PrintSummaryMap(TH2F * h2,TString name, float min, float max){


	TCanvas c1("","",800,700);
	c1.SetLeftMargin(0.13);
	c1.SetRightMargin(0.2);
	c1.SetBottomMargin(0.13);
	h2->GetYaxis()->SetTitleOffset(1);
	h2->GetZaxis()->SetTitleOffset(1);
	if(min>=0) h2->SetMinimum(min);
	if(max>=0) h2->SetMaximum(max);
	h2->Draw("colz");
	// DrawCMS();
	DrawProton();
	// DrawTemp();
	c1.Print(Form("%s/%s_%s.pdf",outDir.Data(),name.Data(),tag.Data()));
	c1.Print(Form("%s/%s_%s.root",outDir.Data(),name.Data(),tag.Data()));

}
void map_plotter::PrintSummary1D(TH1F * h,TString name){

	TCanvas c1("","",1050,500);
	c1.SetLeftMargin(0.1);
	c1.SetRightMargin(0.08);
	c1.SetBottomMargin(0.13);
	h->SetMarkerStyle(20);
	h->SetMarkerSize(0.6);
	h->SetMarkerColor(kBlue+2);
	// if(min>=0) h2->SetMinimum(min);
	// if(max>=0) h2->SetMaximum(max);
	// h->SetTitle("");
	h->Draw();
	// DrawCMS();
	DrawProton(true);
	DrawTemp(true);
	c1.Print(Form("%s/%s_%s.pdf",outDir.Data(),name.Data(),tag.Data()));
	c1.Print(Form("%s/%s_%s.root",outDir.Data(),name.Data(),tag.Data()));

}
void map_plotter::PrintSummaryGraph(TGraphAsymmErrors * g,TString name){

	TCanvas c1("","",1050,500);
	c1.SetLeftMargin(0.1);
	c1.SetRightMargin(0.08);
	c1.SetBottomMargin(0.13);
	TH1F * hfiller = new TH1F("filler","",nbinsX,minX,maxX);
	Cosmetic1D(hfiller);
	hfiller->SetTitle(";x [mm];Efficiency");
	hfiller->Draw();
	// if(min>=0) h2->SetMinimum(min);
	// if(max>=0) h2->SetMaximum(max);
	// g->SetTitle("");
	g->SetLineWidth(1);

	g->SetMarkerStyle(20);
	g->SetMarkerSize(0.6);
	g->SetMarkerColor(kBlue+2);	
	g->SetLineColor(kBlue+2);
	g->Draw("epz same");
	// DrawCMS();
	DrawProton(true);
	DrawTemp(true);
	c1.Print(Form("%s/%s_%s.pdf",outDir.Data(),name.Data(),tag.Data()));
	c1.Print(Form("%s/%s_%s.root",outDir.Data(),name.Data(),tag.Data()));

}

void map_plotter::CleanMap(TH2F * map, float xmin, float xmax, float ymin, float ymax, bool scale){
	for(uint ix=0;ix<map->GetNbinsX();ix++){
		float thisx = map->GetXaxis()->GetBinLowEdge(ix);
		for(uint iy=0;iy<map->GetNbinsY();iy++){
			float thisy = map->GetYaxis()->GetBinLowEdge(iy);
			if(thisx<xmin || thisx>xmax || thisy<ymin || thisy>ymax){
				map->SetBinContent(ix,iy,0);
			}
			else{
				float this_content = map->GetBinContent(ix,iy);
				if (scale) map->SetBinContent(ix,iy,this_content/0.95);
			}
		}
	}
}


void map_plotter::FillChannelMap(TH2F * channel_map, vector<TH2F*> v_map_eff){
	for(uint ix=0;ix<channel_map->GetNbinsX();ix++){
		for(uint iy=0;iy<channel_map->GetNbinsY();iy++){
			float maxeff=0.;
			int maxpad=0;
			for(uint ipad =1; ipad<= npad; ipad++){
				if(v_map_eff[ipad]->GetBinContent(ix,iy) > maxeff){
					maxeff = v_map_eff[ipad]->GetBinContent(ix,iy);
					maxpad = ipad;
				}
			}
			channel_map->SetBinContent(ix,iy,maxpad);
		}
	}
}

void map_plotter::FillSummaryMap(vector<TH2F*> v_map, TH2F * channel_map){
	//Fill the zeroth map with the correct channel for each xy bin, according to channel_map.
	for(uint ix=0;ix<v_map[0]->GetNbinsX();ix++){
		float x = v_map[0]->GetXaxis()->GetBinCenter(ix);
		for(uint iy=0;iy<v_map[0]->GetNbinsY();iy++){
			float y = v_map[0]->GetYaxis()->GetBinCenter(iy);
			int bin_map = channel_map->FindBin(x,y);
			if(channel_map->GetBinContent(bin_map) > 0){
				v_map[0]->SetBinContent(ix,iy,v_map[channel_map->GetBinContent(bin_map)]->GetBinContent(ix,iy));
			}
		}
	}
	v_map[0]->Write();
}

void map_plotter::FillAligned(TH2F * destination, TH2F * source, int dx, int dy, int minx, int miny){
	//Fill the zeroth map with the correct channel for each xy bin, according to channel_map.
	TH2F * dummy = (TH2F*)destination->Clone();
	for(uint ix=0;ix<source->GetNbinsX();ix++){		
		int ix_dest = ix;
		while(ix_dest >= minx+dx){
			ix_dest -= dx;
		}
		for(uint iy=0;iy<source->GetNbinsY();iy++){
			int iy_dest = iy;
			while(iy_dest >= miny+dy){
				iy_dest -= dy;
			}
			float x = destination->GetXaxis()->GetBinCenter(ix_dest);
			float y = destination->GetYaxis()->GetBinCenter(iy_dest);
			destination->Fill(x,y,source->GetBinContent(ix,iy));
			if(source->GetBinContent(ix,iy) > 0) dummy->Fill(x,y); 
		}
	}

	destination->Divide(dummy);
	destination->Write();
}

void map_plotter::FillSummaryMapCoarse(TH2F* h_target, vector<TH2F*> v_map, TH2F* effmap, TH2F * channel_map, bool normalize){
	float scale_factor[npad+1];
	float mean;
	for(int i=1;i<=npad;i++){
		if(normalize && v_map[i]->GetEntries() > 0){
			scale_factor[i] = v_map[i]->Integral()/v_map[i]->GetEntries();
			mean+=scale_factor[i];
		}
		else scale_factor[i]=1.;
	}
	if(normalize){
		mean = mean/npad;
		for(int i=1;i<=npad;i++){
			scale_factor[i] = mean/scale_factor[i];
		}
	}

	//Fill the zeroth map with the correct channel for each xy bin, according to channel_map.
	float binwidthX = (maxX-minX)/nbinsX;
	float binwidthY = (maxY-minY)/nbinsY;
	for(uint ix=0;ix<h_target->GetNbinsX();ix++){
		float x = h_target->GetXaxis()->GetBinCenter(ix);
		for(uint iy=0;iy<h_target->GetNbinsY();iy++){
			//For hpk3p1 paper:
			// if(iy>=64) continue;
			float y = h_target->GetYaxis()->GetBinCenter(iy);
			int bin_map = channel_map->FindBin(x,y);
			if(channel_map->GetBinContent(bin_map) > 0){
				int ipad = channel_map->GetBinContent(bin_map);
				int source_bin=v_map[ipad]->FindBin(x,y);
				int eff_bin=effmap->FindBin(x,y);
				if(effmap->GetBinContent(eff_bin)>0.9){

					h_target->SetBinContent(ix,iy,scale_factor[ipad]*v_map[ipad]->GetBinContent(source_bin));
				}
				// else{//if efficiency < 50%, allow if both of the neighbors along x or y are > 50%
					
				// 	int left = effmap->FindBin(x-binwidthX,y);
				// 	int right = effmap->FindBin(x+binwidthX,y);
				// 	int top = effmap->FindBin(x,y+binwidthY);
				// 	int bottom = effmap->FindBin(x,y-binwidthY);
				// 	if((effmap->GetBinContent(left)>0.5 && effmap->GetBinContent(right)>0.5) || (effmap->GetBinContent(top)>0.5 && effmap->GetBinContent(bottom)>0.5))
				// 		h_target->SetBinContent(ix,iy,v_map[channel_map->GetBinContent(bin_map)]->GetBinContent(source_bin));
				// }
			}
		}
	}
	h_target->Write();
}

void map_plotter::FillSummary1DCoarse(vector<vector<TH1F*> > v_1D, vector<vector<TH1F*>> eff1d, TH2F * channel_map, bool isX){
	//Fill the zeroth map with the correct channel for each x or y bin, according to channel_map.
	int nslices=0;
	if (isX) {nslices=ySliceMin.size();}
	else {nslices=xSliceMin.size();}

	for(int islice=0;islice<nslices;islice++){
	for(uint ix=0;ix<v_1D[0][islice]->GetNbinsX();ix++){
		float x,y;
		int eff_bin;
		float coarse_bin;

		if(isX){
			x = v_1D[0][islice]->GetXaxis()->GetBinCenter(ix);
			y = (ySliceMin[islice] + ySliceMax[islice])/2.;
			eff_bin = eff1d[0][islice]->FindBin(x);
			coarse_bin = v_1D[1][islice]->FindBin(x);

		}
		else{
			x = (xSliceMin[islice] + xSliceMax[islice])/2.;
			y = v_1D[0][islice]->GetXaxis()->GetBinCenter(ix);
			eff_bin = eff1d[0][islice]->FindBin(y);
			coarse_bin = v_1D[1][islice]->FindBin(y);
		}
		int bin_map = channel_map->FindBin(x,y);
		if(eff1d[0][islice]->GetBinContent(eff_bin)>0.15 && channel_map->GetBinContent(bin_map) > 0){
			v_1D[0][islice]->SetBinContent(ix,v_1D[channel_map->GetBinContent(bin_map)][islice]->GetBinContent(coarse_bin));
			v_1D[0][islice]->SetBinError(ix,v_1D[channel_map->GetBinContent(bin_map)][islice]->GetBinError(coarse_bin));
		}
		
	}
	Cosmetic1D(v_1D[0][islice]);
	v_1D[0][islice]->Write();
	}
}


void map_plotter::FillSummary1D(vector<vector<TH1F*> > v_1D, TH2F * channel_map, bool isX){
	//Fill the zeroth map with the correct channel for each x or y bin, according to channel_map.
	int nslices=0;
	if (isX) {nslices=ySliceMin.size();}
	else {nslices=xSliceMin.size();}

	for(int islice=0;islice<nslices;islice++){
	for(uint ix=0;ix<v_1D[0][islice]->GetNbinsX();ix++){
		float x,y;
		if(isX){
			x = v_1D[0][islice]->GetXaxis()->GetBinCenter(ix);
			y = (ySliceMin[islice] + ySliceMax[islice])/2.;
		}
		else{
			x = (xSliceMin[islice] + xSliceMax[islice])/2.;
			y = v_1D[0][islice]->GetXaxis()->GetBinCenter(ix);
		}
		int bin_map = channel_map->FindBin(x,y);
		if(channel_map->GetBinContent(bin_map) > 0){
			v_1D[0][islice]->SetBinContent(ix,v_1D[channel_map->GetBinContent(bin_map)][islice]->GetBinContent(ix));
			v_1D[0][islice]->SetBinError(ix,v_1D[channel_map->GetBinContent(bin_map)][islice]->GetBinError(ix));
		}
		
	}
	Cosmetic1D(v_1D[0][islice]);
	v_1D[0][islice]->Write();
	}
}


float map_plotter::GetEff(TH3F * h3, int x_lo, int x_hi, int y_lo, int y_hi, int den){
	TH1D * htemp = new TH1D("temp","",2,0,2);
	h3->ProjectionZ("temp",x_lo, x_hi, y_lo, y_hi);
	float eff;
	if(den!=1) eff = htemp->GetBinContent(2)/htemp->GetEntries();
	else eff = htemp->GetEntries();
	delete htemp;
	return eff;
}

void map_plotter::Convert1D(TH3F * h3, vector<TH1F *> h1, int type, bool isX, int pad){

	int nbins=0; int nslices=0;
	if (isX) {nbins = h3->GetNbinsX();nslices=ySliceMin.size();}
	else {nbins = h3->GetNbinsY();nslices=xSliceMin.size();}

	for(int islice=0;islice<nslices;islice++){
	for(int ix=1;ix<=nbins;ix++){
		TH1F * h;
		if (isX) h = (TH1F *) h3->ProjectionZ("temp",ix,ix,h3->GetYaxis()->FindBin(ySliceMin[islice]),h3->GetYaxis()->FindBin(ySliceMax[islice]));
		else h = (TH1F *) h3->ProjectionZ("temp",h3->GetXaxis()->FindBin(xSliceMin[islice]),h3->GetXaxis()->FindBin(xSliceMax[islice]),ix,ix);

		if(type==0){
			if(h->GetEntries() > 10){
				h1[islice]->SetBinContent(ix,h->GetMean());
				h1[islice]->SetBinError(ix,h->GetMeanError());
			}
		}
		if(type==1){
			//time resolution (gauss width)
			if(h->GetEntries() > 20){
				pair<float,float> sig_and_err = GetSigmaT(h,minTime,maxTime);
				if( sig_and_err.second/sig_and_err.first > 0.3) continue;
				h1[islice]->SetBinContent(ix,sig_and_err.first);
				h1[islice]->SetBinError(ix,sig_and_err.second);
			}
		}
			//Count yields from efficiency numerator
		if(type==2){
			h1[islice]->SetBinContent(ix,h->GetBinContent(2));
		}
			//Landau MPV
		if(type==3){
			if(h->GetEntries()>20){
				pair<float,float> mpv_and_err = GetMPV(h,minAmp,maxAmp,hitThres[pad]);
				if(mpv_and_err.second/mpv_and_err.first > 0.3) continue;
				h1[islice]->SetBinContent(ix,mpv_and_err.first);
				h1[islice]->SetBinError(ix,mpv_and_err.second);
			}
		}
			//Efficiency calculation
		if(type==4){
			if(h->GetEntries()>10){
				float eff = h->GetBinContent(2)/h->GetEntries();
				float unc = sqrt(eff*(1-eff)*h->GetEntries())/h->GetEntries();
				h1[islice]->SetBinContent(ix,eff);
				h1[islice]->SetBinError(ix,unc);
			}
		}

			h->Delete();
		}
	h1[islice]->Write();
	}
}

void map_plotter::ConvertTH1toTGraphAsymmErrors(vector<TH1F*> v_h, vector<TH1F*> v_nhits, vector<TGraphAsymmErrors*> v_g, TString name){
	
	for(uint islice =0;islice < v_h.size(); islice++){

    int n = v_h[islice]->GetNbinsX();
	float x[500]; float y[500]; float exl[500]; float exh[500]; float eyl[500]; float eyh[500];
	for(int i=1;i<=n;i++){
		int npass = v_nhits[islice]->GetBinContent(i);
		int ntotal = npass/v_h[islice]->GetBinContent(i);
		float width = v_nhits[islice]->GetBinWidth(i);
		x[i-1] = v_h[islice]->GetBinCenter(i);
		if(npass>0){
			vector<float> effs = GetEffandError(npass, ntotal);

			y[i-1] = effs[0];
			eyl[i-1] = effs[1];
			eyh[i-1]= effs[2];
		}
		else{
			y[i-1] = 0;
			eyl[i-1] = 0;
			eyh[i-1]= 0;
		}

		exl[i-1]=0.5*width;
		exh[i-1]=0.5*width;
	}
	TGraphAsymmErrors * g = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
	g->SetName(Form("efficiency_slice%i_%s",islice,name.Data()));
	TString title = Form("%0.1f < y < %0.1f;x [mm]; Efficiency;",ySliceMin[islice],ySliceMax[islice]);
	g->SetTitle(title);
	g->Write();
	PrintSummaryGraph(g,Form("effgraph_slice%i_%s",islice,name.Data()));


	v_g.push_back(g);
}
}

void map_plotter::ConvertMap(TH3F * h3, TH2F * h2, int type, int pad){
	TString hist_tag;
	if(type==1) hist_tag = "dt";
	else if (type==3) hist_tag = "amp";
	else hist_tag ="tmp";
	for(uint ix=1;ix<=h3->GetNbinsX();ix++){
		for(uint iy=1;iy<=h3->GetNbinsY();iy++){
			TH1F * h = (TH1F *) h3->ProjectionZ(Form("%s_%i_%i",hist_tag.Data(),ix,iy),ix,ix,iy,iy);
			if(type==0){
				if(h->GetEntries() > 10){ 
					h2->SetBinContent(ix,iy,h->GetMean());
					h2->SetBinError(ix,iy,h->GetMeanError());
				
				}
			}
			if(type==1){
				//time resolution (gauss width)
				if(h->GetEntries() > 20){
					pair<float,float> sig_and_err = GetSigmaT(h,minTime,maxTime);
					if( sig_and_err.second/sig_and_err.first > 0.3) continue;
					h2->SetBinContent(ix,iy,sig_and_err.first);
					h2->SetBinError(ix,iy,sig_and_err.second);
					h->Write();
				}
			}
			if(type==2){
				h2->SetBinContent(ix,iy,h->GetBinContent(2));
			}
			//Landau MPV
			if(type==3){
				if(h->GetEntries()>20){
					pair<float,float> mpv_and_err = GetMPV(h, minAmp,maxAmp,hitThres[pad]);
					if(mpv_and_err.second/mpv_and_err.first > 0.3) continue;
					h2->SetBinContent(ix,iy,mpv_and_err.first);
					h2->SetBinError(ix,iy,mpv_and_err.second);
					h->Write();
				}
			}
			//Efficiency calculation
			if(type==4){
				if(h->GetEntries()>10){
					float eff = h->GetBinContent(2)/h->GetEntries();
					float unc = sqrt(eff*(1-eff)*h->GetEntries())/h->GetEntries();
					h2->SetBinContent(ix,iy,eff);
					h2->SetBinError(ix,iy,unc);
				}
			}

			h->Delete();
		}
	}
}


pair<int,int> map_plotter::nLGADHitsAndChannel(){
	//Return number of hits in non-photek channels
	//return index of hit
	int nhits=0;
	int ch=-1;
	for(int j=0;j<nchan;j++){
	//	if(j!=2) continue;//fix
		if(sensors->at(j).find("Photek")==std::string::npos && sensors->at(j).find(sensor_name)!=std::string::npos){ //not a photek channel
			if(amp[j] > hitThres[pads->at(j)]){
				nhits++;
				if(nhits == 1) ch=j;
				else if(nhits >=2 && amp[j] > amp[ch]) ch=j;
			}
		}
	}
	return pair<int,int> {nhits,ch};
}

int map_plotter::indexGoodPhotekHit(){
	//returns -1 if no good photek hit.
	int index=-1;
	for(int j=nchan-1;j>=0;j--){
		if(sensors->at(j).find("Photek")!=std::string::npos){ 
			if(amp[j] > photekMin && amp[j]<photekMax && LP2_20[j]!=0){
				index = j;
				break;
			}
		}
	}
	return index;

}

void map_plotter::InitBranches(){


	t->SetBranchStatus("*", 0);
	//analog info
	t->SetBranchStatus("amp", 1); t->SetBranchAddress("amp", &amp);
	t->SetBranchStatus("LP2_20", 1); t->SetBranchAddress("LP2_20", &LP2_20);	
	t->SetBranchStatus("LP2_40", 1); t->SetBranchAddress("LP2_40", &LP2_40);	

	//Track info
	t->SetBranchStatus("ntracks", 1); t->SetBranchAddress("ntracks", &ntracks);
	t->SetBranchStatus("nback", 1); t->SetBranchAddress("nback", &nback);
	t->SetBranchStatus("nplanes", 1); t->SetBranchAddress("nplanes", &nplanes);
	t->SetBranchStatus("npix", 1); t->SetBranchAddress("npix", &npix);
	t->SetBranchStatus("chi2", 1); t->SetBranchAddress("chi2", &chi2);
	// t->SetBranchStatus("xResidBack", 1); t->SetBranchAddress("xResidBack", &xResidBack);
	// t->SetBranchStatus("yResidBack", 1); t->SetBranchAddress("yResidBack", &yResidBack);
	t->SetBranchStatus("x_dut", 1); t->SetBranchAddress("x_dut", &x_dut);
	t->SetBranchStatus("y_dut", 1); t->SetBranchAddress("y_dut", &y_dut);
	//Run conf info
	t->SetBranchStatus("run", 1); t->SetBranchAddress("run", &run);
	t->SetBranchStatus("gconf", 1); t->SetBranchAddress("gconf", &gconf);
	t->SetBranchStatus("sensors", 1); t->SetBranchAddress("sensors", &sensors);
	t->SetBranchStatus("pads", 1); t->SetBranchAddress("pads", &pads);


}

