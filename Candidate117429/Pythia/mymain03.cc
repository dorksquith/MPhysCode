//Z bosson bb decay

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

	//Z switch on
	pythia.readString("WeakSingleBoson:ffbar2gmZ = on");    //ff → ZZ
	
	//commented out for branching ratio, so it can decay via any path
	pythia.readString("23:onMode = off"); //turns all Z(23) decays off
	pythia.readString("23:onIfAny = 5"); //switches on the decay for b
	pythia.readString("23:onIfAny = -5"); //switches on the decay for bbar
	
	//needed for branching ratio to limit virtual Z 
	//pythia.readString("23:mMin = 89.");
	//pythia.readString("23:mMax = 94.");
	
	//to list more events rather than clearing and refilling for every event
	//pythia.readString("Next:numberShowProcess = 100");  //same as number of events
	pythia.readString("Next:numberShowEvent = 0");
	
	
	// 8 TeV CM energy.
	pythia.readString("Beams:eCM = 13000."); 
	
	// Initialize; incoming pp beams is default.
	pythia.init(); 
	
	int iZ = 0;
	double PassFilter=0;
	double bDecays=0;
	double bbarDecays=0;
	double bPass=0;
	double bbarPass=0;
	double totalZ=0;
	double TotalbbDecay=0;
	
	
	// Generate event(s).
	//event loop to create 5 events
	for (int iEvent = 0; iEvent < numEvents; ++iEvent) 
	{
		// Generate an(other) event. Fill event record.
		pythia.next();
		//set up booleans
		bool ZbFilter = false;
		bool ZbbarFilter = false;
		bool Zb = false;
		bool Zbbar = false;
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
			
			//tally of Z produced to compare bb to other decays
			if ((pythia.event[i].id() == 23) && (pythia.event[pythia.event[i].mother1()].id() != 23))
			{
				iZ=i;
				totalZ++;
			}
		 
			//coditions to locate b/bbar (5/-5) from Z decay (25)
			if ((pythia.event[i].id() == 5) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
			{
				Zb = true;
				bDecays++;
				bUncutpT=pythia.event[i].pT();
				if (pythia.event[i].pT() > 25)
				{
					bPass++;
					ZbFilter = true;
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
			
			if ((pythia.event[i].id() == -5) && (pythia.event[pythia.event[i].mother1()].id() == 23))
			{
				Zbbar = true;
				bbarDecays++;
				bbarUncutpT=pythia.event[i].pT();
				if (pythia.event[i].pT() > 25)
				{
					bbarPass++;
					ZbbarFilter = true;
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
			
			// when a Zs decay is found keep tally 
			if ((Zb == true) && (Zbbar == true))
			{
				TotalbbDecay++;
				Zb = false;
				Zbbar = false;
				//cout<<"b"<<" "<<bUncutpT<<endl;
				//cout<<"bbar"<<" "<<bbarUncutpT<<endl;
				
			}
			
			
			//What to do when a bb Z decay is found that passes filter
			if ((ZbFilter == true) && (ZbbarFilter == true))
			{
				PassFilter++;
				/*
				cout<<" "<<endl;
				//cout<<"Z MEASUREMENTS"<<endl;
				//cout<<"b measurements"<<endl;
					
				cout<<"beta ="<< beta <<endl;
				cout<<"bphi ="<< bphi <<endl;
				cout<<"bpT ="<< bpT <<endl;
				cout<<"brap ="<< brap <<endl;
				cout<<"bpx ="<< bpx <<endl;
				cout<<"bpy ="<< bpy <<endl;
				cout<<"bpz ="<< bpz <<endl;
				cout<<"be ="<< be <<endl;
				cout<<"bMumMass ="<< bMotherMass <<endl;
		
				//cout<<"bbar measurements"<<endl;	
				cout<<"bbareta ="<< bbareta <<endl;
				cout<<"bbarphi ="<< bbarphi <<endl;
				cout<<"bbarpT ="<< bbarpT <<endl;
				cout<<"bbarrap ="<< bbarrap <<endl;
				cout<<"bbarpx ="<< bbarpx <<endl;
				cout<<"bbarpy ="<< bbarpy <<endl;
				cout<<"bbarpz ="<< bbarpz <<endl;
				cout<<"bbare ="<< bbare <<endl;
				cout<<"bbarMumMass ="<< bbarMotherMass <<endl;
				cout<<" "<<endl;
				*/
				
				//print out for excell 
				//Delta R calculation
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
				
		//Printed out as a list as is useful for creating a CSV file
				cout<<"b"<<" "<<beta<<" "<<bphi<<" "<<bpT<<" "<<Largest<<" "<<brap<<" "<<bpx<<" "<<bpy<<" "<<bpz<<" "<<be<<" "<<bMotherMass<<" "<<dphi2<<" "<<deta2<<" "<<dr<<endl;
				cout<<"bbar"<<" "<<bbareta<<" "<<bbarphi<<" "<<bbarpT<<" "<<Largest<<" "<<bbarrap<<" "<<bbarpx<<" "<<bbarpy<<" "<<bbarpz<<" "<<bbare<<" "<<bbarMotherMass<<" "<<dphi2<<" "<<deta2<<" "<<dr<<endl;
				
				ZbFilter = false;
				ZbbarFilter = false;
				
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
		cout<<"Total Z created ="<< totalZ <<endl;
		double DecayRatio= TotalbbDecay/totalZ;
		cout<<"bb to other decay ratio ="<< DecayRatio*100<< "%" <<endl;
		
		
return 0;
} 
