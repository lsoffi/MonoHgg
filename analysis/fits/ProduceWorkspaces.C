#include "HighMassGGFitter.cc"
//

void ProduceWorkspaces(int mass, std::string coupling){
  
  cout << "mass = " << mass << endl; 
  cout << "coupling = " << coupling << endl; 
  runfits(mass, coupling);
}
