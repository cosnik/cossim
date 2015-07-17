#define cosana_cxx
#include "cosana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void cosana::Loop()
{
//   In a ROOT session, you can do:
//      root> .L cosana.C
//      root> cosana t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   TH1F *_nhit = new TH1F("nhit","nhit",5,-0.5,4.5);
   TH2F *_ip0_xz = new TH2F("xz0","xz0",100,-200,200,100,-200,200);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   // track parameters for two tracks.....
   Double_t s0,s1;
   Double_t x0[3],t0[3];
   Double_t x1[3],t1[3];
   Double_t delta[3],delta_dot_t0,delta_dot_t1,t0_dot_t1;

   Double_t mm[2][2], mmi[2][2];
   Double_t gg[2];
 
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      Int_t nhit = xp->size();
      _nhit->Fill(nhit);

      if( nhit == 4){
         // track 1
         x0[0] = xp->at(0);
         x0[1] = yp->at(0);
         x0[2] = zp->at(0);
         t0[0] = xp->at(1) - xp->at(0);
         t0[1] = yp->at(1) - yp->at(0);
         t0[2] = zp->at(1) - zp->at(0);
         Double_t L0 = sqrt(t0[0]*t0[0]+t0[1]*t0[1]+t0[2]*t0[2]);
         for(int i=0; i<3; i++) {
            t0[i]/=L0;
            cout <<" "<<t1[i];
         }
         cout <<endl;
           
         // track 2
         x1[0] = xp->at(2);
         x1[1] = yp->at(2);
         x1[2] = zp->at(2);
         t1[0] = xp->at(3) - xp->at(2);
         t1[1] = yp->at(3) - yp->at(2);
         t1[2] = zp->at(3) - zp->at(2);
         Double_t L1 = sqrt(t1[0]*t1[0]+t1[1]*t1[1]+t1[2]*t1[2]);
         for(int i=0; i<3; i++) {
            t1[i]/=L1;
            cout <<" "<<t1[i];
         }
         cout <<endl;

         // Delta vector
         for(int i=0; i<3; i++) delta[i] = x0[i] - x1[i];
         // vector inproduct
         t0_dot_t1 = 0;
         delta_dot_t0 = 0;
         delta_dot_t1 = 0;
         for(int i=0; i<3; i++) {
            t0_dot_t1    += t0[i]*t1[i];
            delta_dot_t0 += delta[i]*t0[i];
            delta_dot_t1 += delta[i]*t1[i];
         }
         // fill matrix and vector
         mm[0][0] = 1.;
         mm[0][1] = -t0_dot_t1;
         mm[1][0] = -t0_dot_t1;
         mm[1][1] = 1.;
         gg[0] = -delta_dot_t0;
         gg[1] = +delta_dot_t1;
         
         
         Double_t detM = mm[0][0]*mm[1][1] - mm[0][1]*mm[1][0];
         s0 = 0;
         s1 = 0;
         cout << "detM = "<<detM <<endl;
         if(detM > 1e-20){
           mmi[0][0] =  mm[1][1]/detM;
           mmi[0][1] = -mm[0][1]/detM;
           mmi[1][0] = -mm[1][0]/detM;
           mmi[1][1] =  mm[0][0]/detM;
            
           s0 = mmi[0][0]*gg[0]+mmi[0][1]*gg[1];
           s1 = mmi[1][0]*gg[0]+mmi[1][1]*gg[1];
           cout << s0 <<" " <<s1<<endl;
         } 
         
         // point of closest approach
         _ip0_xz->Fill(s0*t0[0]+x0[0],s0*t0[2]+x0[2]);
         
      }
   }
}
