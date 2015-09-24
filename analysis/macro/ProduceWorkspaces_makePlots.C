#include "HighMass-HggFitter_mgg_makePlots.cc"
//

void ProduceWorkspaces_makePlots(){
  gSystem->SetIncludePath("-I/afs/cern.ch/cms/slc5_amd64_gcc462/lcg/roofit/5.32.00-cms5/include");
  gSystem->Load("libRooFit");
  runfits(); 
}
