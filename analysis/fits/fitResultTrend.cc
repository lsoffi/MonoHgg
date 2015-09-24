#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <iostream>

using namespace std;

void fitResultTrend() {

  // mass
  float mass[3]    = {750., 1500., 5000.};
  float massErr[3] = {0., 0., 0.};

  // -------------------------------------------------------
  // hardcoded, from fit results
  float mean_cat0[3]    = { -1.3548e-03, -3.5387e-03, -1.1220e-02};
  float meanErr_cat0[3] = { 7.43e-05, 6.75e-05, 8.79e-05 };

  float sigma_cat0[3]    = { 5.5079e-03, 5.6970e-03, 8.3333e-03};
  float sigmaErr_cat0[3] = { 7.93e-05, 6.78e-05, 8.44e-05};

  float alphaCBneg_cat0[3]    = { 7.5933e-01, 7.1451e-01, 5.7497e-01};
  float alphaCBnegErr_cat0[3] = { 2.14e-02, 1.51e-02, 2.21e-02 };

  float alphaCBpos_cat0[3]    = { -5.7440e-01, -5.6471e-01, -1.1929e+00};
  float alphaCBposErr_cat0[3] = { 5.86e-02, 4.40e-02, 7.46e-02};

  float frac_cat0[3]    = { 8.3627e-01, 8.6378e-01, 8.0545e-01};
  float fracErr_cat0[3] = { 2.18e-02, 1.35e-02, 2.65e-02};

  // --------
  float mean_cat1[3]    = { -1.9333e-03, -2.9052e-03, -9.5690e-03 };
  float meanErr_cat1[3] = { 1.08e-04,  1.19e-04, 2.81e-04};

  float sigma_cat1[3]    = { 5.6399e-03, 5.4152e-03, 7.7163e-03};
  float sigmaErr_cat1[3] = { 1.29e-04, 1.21e-04, 2.90e-04};

  float alphaCBneg_cat1[3]    = { 5.3076e-01, 5.6857e-01, 3.9229e-01 };
  float alphaCBnegErr_cat1[3] = { 4.36e-02, 2.97e-02, 4.95e-02 };
  
  float alphaCBpos_cat1[3]    = { -9.6597e-01, -7.1691e-01, -9.6334e-01 };
  float alphaCBposErr_cat1[3] = { 7.60e-02, 7.61e-02, 1.34e-01};

  float frac_cat1[3]    = { 5.6007e-01, 7.5510e-01, 7.7249e-01};
  float fracErr_cat1[3] = { 5.52e-02,  3.93e-02, 5.13e-02};

  // --------
  float mean_cat2[3]    = { -2.0695e-03, -3.3043e-03, -6.2078e-03};
  float meanErr_cat2[3] = { 2.13e-04, 1.78e-04, 3.79e-04 };

  float sigma_cat2[3]    = { 1.1803e-02, 1.1912e-02, 1.3495e-02};
  float sigmaErr_cat2[3] = {  2.78e-04, 2.12e-04,  3.96e-04};

  float alphaCBneg_cat2[3]    = { 6.9874e-01, 7.3995e-01, 7.9587e-01};
  float alphaCBnegErr_cat2[3] = { 9.05e-02, 8.67e-02, 1.57e-01 };

  float alphaCBpos_cat2[3]    = { -8.1731e-01, -1.0278e+00, -1.2837e+00  };
  float alphaCBposErr_cat2[3] = { 1.34e-01, 1.33e-01, 3.02e-01};

  float frac_cat2[3]    = { 5.9778e-01, 6.3696e-01, 7.1512e-01};
  float fracErr_cat2[3] = { 8.01e-02, 7.85e-02, 1.52e-01};

  // --------
  float mean_cat3[3]    = { -3.1023e-03, -3.4559e-03, -5.1902e-03};
  float meanErr_cat3[3] = { 3.80e-04, 2.93e-04, 1.00e-03};

  float sigma_cat3[3]    = { 1.1237e-02, 1.1506e-02, 1.1892e-02};
  float sigmaErr_cat3[3] = { 5.50e-04, 3.66e-04, 1.07e-03};

  float alphaCBneg_cat3[3]    = { 6.1251e-02, 4.2169e-01, 5.7963e-01};
  float alphaCBnegErr_cat3[3] = { 5.15e-01, 2.37e-01, 4.98e-01 };

  float alphaCBpos_cat3[3]    = { -1.2763e+00, -1.3787e+00, -1.3526e+00};
  float alphaCBposErr_cat3[3] = { 1.26e-01, 1.66e-01, 8.11e-01};

  float frac_cat3[3]    = { 4.8370e-01, 4.1258e-01, 6.8332e-01};
  float fracErr_cat3[3] = { 3.97e-01, 1.12e-01, 7.56e-01 };



  // -------------------------------------------------------
  // Graphs
  gStyle->SetOptStat(0);

  // fraction
  TH2F *myHfrac = new TH2F("myHfrac","Fraction",100, 200,5500,100,0,1); 
  myHfrac->GetXaxis()->SetTitle("mass [GeV]"); 
  myHfrac->GetYaxis()->SetTitle("frac"); 
  TGraphErrors G_frac_cat0(3, mass, frac_cat0, massErr, fracErr_cat0); 
  G_frac_cat0.SetMarkerStyle(20);
  G_frac_cat0.SetMarkerColor(2);
  TGraphErrors G_frac_cat1(3, mass, frac_cat1, massErr, fracErr_cat1); 
  G_frac_cat1.SetMarkerStyle(20);
  G_frac_cat1.SetMarkerColor(3);
  TGraphErrors G_frac_cat2(3, mass, frac_cat2, massErr, fracErr_cat2); 
  G_frac_cat2.SetMarkerStyle(20);
  G_frac_cat2.SetMarkerColor(4);
  TGraphErrors G_frac_cat3(3, mass, frac_cat3, massErr, fracErr_cat3); 
  G_frac_cat3.SetMarkerStyle(20);
  G_frac_cat3.SetMarkerColor(7);

  // alpha pos
  TH2F *myHalphaCBpos = new TH2F("myHalphaCBpos","#alpha CB positive",100, 200,5500,100,-1.5,0.); 
  myHalphaCBpos->GetXaxis()->SetTitle("mass [GeV]"); 
  myHalphaCBpos->GetYaxis()->SetTitle("#alpha pos"); 
  TGraphErrors G_alphaCBpos_cat0(3, mass, alphaCBpos_cat0, massErr, alphaCBposErr_cat0); 
  G_alphaCBpos_cat0.SetMarkerStyle(20);
  G_alphaCBpos_cat0.SetMarkerColor(2);
  TGraphErrors G_alphaCBpos_cat1(3, mass, alphaCBpos_cat1, massErr, alphaCBposErr_cat1); 
  G_alphaCBpos_cat1.SetMarkerStyle(20);
  G_alphaCBpos_cat1.SetMarkerColor(3);
  TGraphErrors G_alphaCBpos_cat2(3, mass, alphaCBpos_cat2, massErr, alphaCBposErr_cat2); 
  G_alphaCBpos_cat2.SetMarkerStyle(20);
  G_alphaCBpos_cat2.SetMarkerColor(4);
  TGraphErrors G_alphaCBpos_cat3(3, mass, alphaCBpos_cat3, massErr, alphaCBposErr_cat3); 
  G_alphaCBpos_cat3.SetMarkerStyle(20);
  G_alphaCBpos_cat3.SetMarkerColor(7);

  // alpha neg
  TH2F *myHalphaCBneg = new TH2F("myHalphaCBneg","#alpha CB negative",100, 200,5500,100,0.,1.5); 
  myHalphaCBneg->GetXaxis()->SetTitle("mass [GeV]"); 
  myHalphaCBneg->GetYaxis()->SetTitle("#alpha neg"); 
  TGraphErrors G_alphaCBneg_cat0(3, mass, alphaCBneg_cat0, massErr, alphaCBnegErr_cat0); 
  G_alphaCBneg_cat0.SetMarkerStyle(20);
  G_alphaCBneg_cat0.SetMarkerColor(2);
  TGraphErrors G_alphaCBneg_cat1(3, mass, alphaCBneg_cat1, massErr, alphaCBnegErr_cat1); 
  G_alphaCBneg_cat1.SetMarkerStyle(20);
  G_alphaCBneg_cat1.SetMarkerColor(3);
  TGraphErrors G_alphaCBneg_cat2(3, mass, alphaCBneg_cat2, massErr, alphaCBnegErr_cat2); 
  G_alphaCBneg_cat2.SetMarkerStyle(20);
  G_alphaCBneg_cat2.SetMarkerColor(4);
  TGraphErrors G_alphaCBneg_cat3(3, mass, alphaCBneg_cat3, massErr, alphaCBnegErr_cat3); 
  G_alphaCBneg_cat3.SetMarkerStyle(20);
  G_alphaCBneg_cat3.SetMarkerColor(7);

  // mean
  TH2F *myHmean = new TH2F("myHmean","Mean",100, 200,5500,100,-0.015,0.0);
  myHmean->GetXaxis()->SetTitle("mass [GeV]");
  myHmean->GetYaxis()->SetTitle("mean");
  TGraphErrors G_mean_cat0(3, mass, mean_cat0, massErr, meanErr_cat0); 
  G_mean_cat0.SetMarkerStyle(20);
  G_mean_cat0.SetMarkerColor(2);
  TGraphErrors G_mean_cat1(3, mass, mean_cat1, massErr, meanErr_cat1); 
  G_mean_cat1.SetMarkerStyle(20);
  G_mean_cat1.SetMarkerColor(3);
  TGraphErrors G_mean_cat2(3, mass, mean_cat2, massErr, meanErr_cat2); 
  G_mean_cat2.SetMarkerStyle(20);
  G_mean_cat2.SetMarkerColor(4);
  TGraphErrors G_mean_cat3(3, mass, mean_cat3, massErr, meanErr_cat3); 
  G_mean_cat3.SetMarkerStyle(20);
  G_mean_cat3.SetMarkerColor(7);

  // sigma
  TH2F *myHsigma = new TH2F("myHsigma","Sigma",100, 200,5500,100,0.0001,0.015);
  myHsigma->GetXaxis()->SetTitle("mass [GeV]");
  myHsigma->GetYaxis()->SetTitle("sigma");
  TGraphErrors G_sigma_cat0(3, mass, sigma_cat0, massErr, sigmaErr_cat0); 
  G_sigma_cat0.SetMarkerStyle(20);
  G_sigma_cat0.SetMarkerColor(2);
  TGraphErrors G_sigma_cat1(3, mass, sigma_cat1, massErr, sigmaErr_cat1); 
  G_sigma_cat1.SetMarkerStyle(20);
  G_sigma_cat1.SetMarkerColor(3);
  TGraphErrors G_sigma_cat2(3, mass, sigma_cat2, massErr, sigmaErr_cat2); 
  G_sigma_cat2.SetMarkerStyle(20);
  G_sigma_cat2.SetMarkerColor(4);
  TGraphErrors G_sigma_cat3(3, mass, sigma_cat3, massErr, sigmaErr_cat3); 
  G_sigma_cat3.SetMarkerStyle(20);
  G_sigma_cat3.SetMarkerColor(7);



  // -------------------------------------------------------
  // Fits
  /*
  // TFitResultPtr rFrac_cat0 = G_frac_cat0.Fit("pol1","S");
  // rFrac_cat0->Print("V");   
  cout << "Fraction - cat0" << endl;
  G_frac_cat0.Fit("pol1");
  (G_frac_cat0.GetFunction("pol1"))->SetLineColor(2);
  cout << "Fraction - cat1" << endl;
  G_frac_cat1.Fit("pol1");
  (G_frac_cat1.GetFunction("pol1"))->SetLineColor(3);
  cout << "Fraction - cat2" << endl;
  G_frac_cat2.Fit("pol1");
  (G_frac_cat2.GetFunction("pol1"))->SetLineColor(4);
  cout << "Fraction - cat3" << endl;
  G_frac_cat3.Fit("pol1");
  (G_frac_cat3.GetFunction("pol1"))->SetLineColor(7);
  */

  // -------------------------------------------------------
  // plots
  TCanvas c00("c00","",1);
  myHfrac->Draw();
  G_frac_cat0.Draw("Psame");
  G_frac_cat1.Draw("Psame");
  G_frac_cat2.Draw("Psame");
  G_frac_cat3.Draw("Psame");
  c00.SaveAs("frac.png");

  TCanvas c03("c03","",1);
  myHalphaCBpos->Draw();
  G_alphaCBpos_cat0.Draw("Psame");
  G_alphaCBpos_cat1.Draw("Psame");
  G_alphaCBpos_cat2.Draw("Psame");
  G_alphaCBpos_cat3.Draw("Psame");
  c03.SaveAs("alphaCBpos.png");

  TCanvas c04("c04","",1);
  myHalphaCBneg->Draw();
  G_alphaCBneg_cat0.Draw("Psame");
  G_alphaCBneg_cat1.Draw("Psame");
  G_alphaCBneg_cat2.Draw("Psame");
  G_alphaCBneg_cat3.Draw("Psame");
  c04.SaveAs("alphaCBneg.png");

  TCanvas c05("c05","",1);
  myHmean->Draw();
  G_mean_cat0.Draw("Psame");
  G_mean_cat1.Draw("Psame");
  G_mean_cat2.Draw("Psame");
  G_mean_cat3.Draw("Psame");
  c05.SaveAs("mean.png");

  TCanvas c06("c06","",1);
  myHsigma->Draw();
  G_sigma_cat0.Draw("Psame");
  G_sigma_cat1.Draw("Psame");
  G_sigma_cat2.Draw("Psame");
  G_sigma_cat3.Draw("Psame");
  c06.SaveAs("sigma.png");
}
