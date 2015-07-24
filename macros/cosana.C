#define cosana_cxx
#include "cosana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void cosana::Loop()
{
   if (fChain == 0) return;

   TH1F *_nhit = new TH1F("nhit","nhit",5,-0.5,4.5);
   TH1F *_cost = new TH1F("cost","cost",1000,-1,1);
   TH1F *_dist = new TH1F("dist","dist",1000,0.,10.);
   TH2F *_xy0 = new TH2F("xy0","xy0",500,-500,500,500,-500,500);
   TH2F *_xy1 = new TH2F("xy1","xy1",500,-500,500,500,-500,500);
   TH2F *_xy2 = new TH2F("xy2","xy2",500,-500,500,500,-500,500);
   TH2F *_xy3 = new TH2F("xy3","xy3",500,-500,500,500,-500,500);

   TH2F *_ip0_yz = new TH2F("yz0","yz0",100,-200,200,100,-200,200);
   TH2F *_ip0_yz_cut = new TH2F("yz0_cut","yz0_cut",100,-200,200,100,-200,200);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   Double_t xx[2],yy[2],zz[2];
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
      if(jentry%1000000 == 0) cout << "processed "<<jentry<<" events"<<endl;
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
         for(int i=0; i<3; i++)  t0[i]/=L0;
           
         // track 2
         x1[0] = xp->at(2);
         x1[1] = yp->at(2);
         x1[2] = zp->at(2);
         t1[0] = xp->at(3) - xp->at(2);
         t1[1] = yp->at(3) - yp->at(2);
         t1[2] = zp->at(3) - zp->at(2);
         Double_t L1 = sqrt(t1[0]*t1[0]+t1[1]*t1[1]+t1[2]*t1[2]);
         for(int i=0; i<3; i++) t1[i]/=L1;

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
         _cost->Fill(t0_dot_t1);
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
//         cout << "detM = "<<detM <<endl;
         if(detM > 1e-20){
           mmi[0][0] =  mm[1][1]/detM;
           mmi[0][1] = -mm[0][1]/detM;
           mmi[1][0] = -mm[1][0]/detM;
           mmi[1][1] =  mm[0][0]/detM;
            
           s0 = mmi[0][0]*gg[0]+mmi[0][1]*gg[1];
           s1 = mmi[1][0]*gg[0]+mmi[1][1]*gg[1];
           //cout << s0 <<" " <<s1<<endl;
         } 
         xx[0] = s0*t0[0]+x0[0];
         yy[0] = s0*t0[1]+x0[1];
         zz[0] = s0*t0[2]+x0[2];

         xx[1] = s1*t1[0]+x1[0];
         yy[1] = s1*t1[1]+x1[1];
         zz[1] = s1*t1[2]+x1[2];
         Double_t dist = sqrt(pow(xx[0]-xx[1],2)+pow(yy[0]-yy[1],2)+pow(zz[0]-zz[1],2));
         _dist->Fill(dist);
         //cout << "T0:: x = "<<xx[0]<<" y = "<<yy[0]<<" z = "<<zz[0]<<endl; 
         //cout << "T1:: x = "<<xx[1]<<" y = "<<yy[1]<<" z = "<<zz[1]<<endl; 
         // point of closest approach
         _ip0_yz->Fill((yy[0]+yy[1])/2,(zz[0]+zz[1])/2.);
         //if(t0_dot_t1 <0.995) _ip0_xz_cut->Fill((xx[0]+xx[1])/2,(zz[0]+zz[1])/2.);
         if(t0_dot_t1<0.9995 && dist<0.2) {
            _ip0_yz_cut->Fill((yy[0]+yy[1])/2,(zz[0]+zz[1])/2.);
            _xy0->Fill(xp->at(0),yp->at(0));
            _xy1->Fill(xp->at(1),yp->at(1));
            _xy2->Fill(xp->at(2),yp->at(2));
            _xy3->Fill(xp->at(3),yp->at(3));
         }
         
      }
   }
}
