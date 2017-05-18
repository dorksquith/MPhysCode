//Code to produce Higss decays & find bb decay info

// Headers and Namespaces.
#include "Pythia8/Pythia.h" // Include Pythia headers.
using namespace Pythia8; // Let Pythia8:: be implicit.

int main()  // Begin main program.
{
	
	// Set up generation.
	// Declare Pythia object
	Pythia pythia; 
	
	//config for printing out events
	int numEvents = 5000;
	
	// Switch on process.
	//Higgs switched on 
	pythia.readString("HiggsSM:gg2H = on");
	
	//Decay paths on or off 
	//commented out for branching ratio, so it can decay via any path
	pythia.readString("25:onMode = off"); //turns all H(25) decays off
	pythia.readString("25:onIfAny = 5"); //switches on the decay for b
	pythia.readString("25:onIfAny = -5"); //switches on the decay for bbar
	
	//to list more events rather than clearing and refilling for every event
	pythia.readString("Next:numberShowEvent = 0");  
	
	// 8 TeV CM energy.
	pythia.readString("Beams:eCM = 13000."); 
	
	// Initialize; incoming pp beams is default.
	pythia.init(); 
	
	
	//global variables 
	int iH = 0;
	double PassFilter=0;
	double bDecays=0;
	double bbarDecays=0;
	double bPass=0;
	double bbarPass=0;
	double totalH=0;
	double TotalbbDecay=0;
	
	// Generate event(s).
	//event loop to create 5 events
	for (int iEvent = 0; iEvent < numEvents; ++iEvent) 
	{
	
		// Generate an(other) event. Fill event record.
		pythia.next();
		//set up booleans
		bool Hb = false;
		bool Hbbar=false;
		bool HbFilter = false;
		bool HbbarFilter=false;
		//set up constants
		double beta=0;
		double bphi=0;
		double bpT=0;
		double brap=0;
		double bpx=0;
		double bpy=0;
		double bpz=0;
		double be=0;
		double bMotherMass=0;
		
		double bbareta=0;
		double bbarphi=0;
		double bbarpT=0;
		double bbarrap=0;
		double bbarpx=0;
		double bbarpy=0;
		double bbarpz=0;
		double bbare=0;
		double bbarMotherMass=0;
		double bUncutpT=0;
		double bbarUncutpT=0;

		
		//particle loop-can access the properties of each particle pythia.event[i].
		for (int i = 0; i < pythia.event.size(); ++i) 
		{
		 
		 //tally of H produced to compare bb to other decays
			if ((pythia.event[i].id() == 25) && (pythia.event[pythia.event[i].mother1()].id() != 25))
			{
				totalH++;
				iH=i;
			}
		 
			//coditions to locate b/bbar (5/-5) from a higgs decay (25)
			if (pythia.event[i].id() == 5 && pythia.event[pythia.event[i].mother1()].id() == 25)// && pythia.event[i].pT() > 25) // cuts out background noise at low pT
			{
				++bDecays;
				Hb = true;
				bUncutpT=pythia.event[i].pT();
				
				if (pythia.event[i].pT() > 25)
				{
					++bPass;
					HbFilter = true;
					beta =pythia.event[i].eta();
					bphi =pythia.event[i].phi();
					bpT =pythia.event[i].pT();
					brap =pythia.event[i].y();
					bpx =pythia.event[i].px();
					bpy =pythia.event[i].py();
					bpz =pythia.event[i].pz();
					be =pythia.event[i].e();
					bMotherMass=pythia.event[pythia.event[i].mother1()].m();
				
				}	
			}
			
			if (pythia.event[i].id() == -5 && pythia.event[pythia.event[i].mother1()].id() == 25)// && pythia.event[i].pT() > 25)
			{
				++bbarDecays;
				Hbbar=true;
				bbarUncutpT=pythia.event[i].pT();
				
				if(pythia.event[i].pT() > 25)
				{
					++bbarPass;
					HbbarFilter = true;
					bbareta =pythia.event[i].eta();
					bbarphi =pythia.event[i].phi();
					bbarpT =pythia.event[i].pT();
					bbarrap =pythia.event[i].y();
					bbarpx =pythia.event[i].px();
					bbarpy =pythia.event[i].py();
					bbarpz =pythia.event[i].pz();
					bbare =pythia.event[i].e();
					bbarMotherMass=pythia.event[pythia.event[i].mother1()].m();
					
				}
			}
			
			//What to do when a bb higs decay is found
			// when a Hs decay is found keep tally 
			if ((Hb == true) && (Hbbar == true))
			{
				TotalbbDecay++;
				Hb = false;
				Hbbar = false;
				//cout<<"b"<<" "<<bUncutpT<<endl;
				//cout<<"bbar"<<" "<<bbarUncutpT<<endl;
				
			}
			
			if ((HbFilter == true) && (HbbarFilter == true))
			{
				++PassFilter;
				/*
				cout<<" "<<endl;
				cout<<"H MEASUREMENTS"<<endl;
				cout<<"b measurements"<<endl;	
				cout<<"eta ="<< beta <<endl;
				cout<<"phi ="<< bphi <<endl;
				cout<<"pT ="<< bpT <<endl;
				cout<<"rap ="<< brap <<endl;
				cout<<"px ="<< bpx <<endl;
				cout<<"py ="<< bpy <<endl;
				cout<<"pz ="<< bpz <<endl;
				cout<<"e ="<< be <<endl;
				cout<<"b Mother particle Mass ="<< bMotherMass <<endl;
		
		
				cout<<"bbar measurements"<<endl;	
				cout<<"eta ="<< bbareta <<endl;
				cout<<"phi ="<< bbarphi <<endl;
				cout<<"pT ="<< bbarpT <<endl;
				cout<<"rap ="<< bbarrap <<endl;
				cout<<"px ="<< bbarpx <<endl;
				cout<<"py ="<< bbarpy <<endl;
				cout<<"pz ="<< bbarpz <<endl;
				cout<<"e ="<< bbare <<endl;
				cout<<"bbar Mother particle Mass ="<< bbarMotherMass <<endl;
				cout<<" "<<endl;
				*/
				
				//dR calc 
				double dphi2 = (bphi-bbarphi)*(bphi-bbarphi);
				double deta2 = (beta-bbareta)*(beta-bbareta);
				double dr = sqrt(deta2+dphi2);
				double Largest=0;
				
				if (bpT>=bbarpT)
				{
					Largest = bpT;
				}
				
				if (bpT<bbarpT)
				{
					Largest = bbarpT;
				}
				
				
				
				//print outs (This format for CSV)
				cout<<"b"<<" "<<beta<<" "<<bphi<<" "<<bpT<<" "<<Largest<<" "<<brap<<" "<<bpx<<" "<<bpy<<" "<<bpz<<" "<<be<<" "<<bMotherMass<<" "<<dphi2<<" "<<deta2<<" "<<dr<<endl;
				cout<<"bbar"<<" "<<bbareta<<" "<<bbarphi<<" "<<bbarpT<<" "<<Largest<<" "<<bbarrap<<" "<<bbarpx<<" "<<bbarpy<<" "<<bbarpz<<" "<<bbare<<" "<<bbarMotherMass<<" "<<dphi2<<" "<<deta2<<" "<<dr<<endl;
				
				HbFilter = false;
				HbbarFilter = false;
				
			}
			
		} 
		
	
	}	
	
		cout<<"Total b decays ="<< bDecays <<endl;
		cout<<"Total bbar decays ="<< bbarDecays <<endl;
		cout<<"Total bb decays ="<< TotalbbDecay <<endl;
		cout<<"b Decays that pass filter ="<< bPass <<endl;
		cout<<"bbar Decays that pass filter ="<< bbarPass <<endl;
		cout<<"Decays passing boolean ="<< PassFilter <<endl;
		double FilterRatio =PassFilter/bDecays;
		cout<<"Decays that pass the filter ="<< FilterRatio*100<<"%" <<endl;
		cout<<"Total H created ="<< totalH <<endl;
		double DecayRatio= TotalbbDecay/totalH;
		cout<<"bb to other decay ratio ="<< DecayRatio*100<< "%" <<endl;
		
return 0;
} 
