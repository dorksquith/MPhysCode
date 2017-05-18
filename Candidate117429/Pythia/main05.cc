//Z bosson Branching ratios

// Headers and Namespaces.
#include "Pythia8/Pythia.h" // Include Pythia headers.
using namespace Pythia8; // Let Pythia8:: be implicit.

int main()  // Begin main program.
{
	// Set up generation.
	// Declare Pythia object
	Pythia pythia; 
	
       //config for printing out events
	int numEvents = 1000;

	//Z switch on
	pythia.readString("WeakSingleBoson:ffbar2gmZ = on");    //ff → ZZ
	pythia.readString("Next:numberShowEvent = 0");
	pythia.readString("Beams:eCM = 13000."); 
	pythia.readString("23:mMin = 89.");
	pythia.readString("23:mMax = 94.");
	pythia.init(); 
	

	int totalZ=0;

	int TotalbbDecay=0,TotalnutauDecay=0,TotalnumuDecay=0,TotalnueDecay=0;
	int TotalssDecay=0,TotalccDecay=0,TotalddDecay=0,TotaluuDecay=0;
	int TotaltauDecay=0,TotalmuDecay=0,TotaleDecay=0;
	
	// Generate event(s).
	//event loop 
	for (int iEvent = 0; iEvent < numEvents; ++iEvent) 
	{
		if( (iEvent%100)==0){
			cout<<"Event counter: "<<iEvent<<endl;
		}
		// Generate an(other) event. Fill event record.
		pythia.next();

		int Zb=0,Znue=0,Znumu=0,Znutau=0,Ze=0,Zmu=0,Ztau=0;
		int Zc=0,Zs=0,Zd=0,Zu=0;
		int Z=0;
		
		//particle loop-can access the properties of each particle pythia.event[i].
		for (int i = 0; i < pythia.event.size(); ++i) 
		{	
		
		  //we only care about Z bosons and b-quarks, so skip other particles for speed.
		  if(pythia.event[i].id() != 23 && pythia.event[i].id() != 5 && pythia.event[i].id() != -5){
		    continue;
		  }

		  // get the end-of-chain Z boson
		  if ( (pythia.event[i].id() == 23) && (pythia.event[pythia.event[i].mother1()].id() != 23)){
		    //double zwidth = pythia.event[i].mWidth();
		    //double zmass = pythia.event[i].m0();
		    //double minm = pythia.event[i].mMin();
		    //double maxm = pythia.event[i].mMax();
		    totalZ++;
		    Z++;
		  }
		  
		  // get the b / bbar from the Z
		  if ((abs(pythia.event[i].id()) == 5) && (pythia.event[pythia.event[i].mother1()].id() == 23)){
		    
		    Zb++;
		    
		  }
		  
		    if ((abs(pythia.event[i].id()) == 4) && (pythia.event[pythia.event[i].mother1()].id() == 23)){
		  
		  Zc++;
		    
		    }
		  if ((abs(pythia.event[i].id()) == 3) && (pythia.event[pythia.event[i].mother1()].id() == 23)){
		  
		  Zs++;
		    
		    }
		  if ((abs(pythia.event[i].id()) == 2) && (pythia.event[pythia.event[i].mother1()].id() == 23)){
		  
		  Zu++;
		    
		    }	
		  if ((abs(pythia.event[i].id()) == 1) && (pythia.event[pythia.event[i].mother1()].id() == 23)){
		  
		  Zd++;
		    
		    }	
		  if ((abs(pythia.event[i].id()) == 11) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
					{
			Ze++;
				
				}
			
			if ((abs(pythia.event[i].id()) == 13) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
			{
			Zmu++;
				
				}
			
			if ((abs(pythia.event[i].id()) == 15) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
			{
			Ztau++;
				
				}
			
			if ((abs(pythia.event[i].id()) == 12) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
			{
			Znue++;
				
				}
			
			if ((abs(pythia.event[i].id()) == 14) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
			{
			Znumu++;
				
				}
			
			if ((abs(pythia.event[i].id()) == 16) && (pythia.event[pythia.event[i].mother1()].id() == 23)) // cuts out background noise at low pT
			{
			Znutau++;
			}
		  

		}//particle loop

		if(Z!=1 || (Zb!=2 && Zb!=0))
		{
			cout<<"Found "<< Z<<" Z bosons and "<<Zb<<" b daughters in event "<<iEvent<<endl; 
		}
		
		if(Zb==2){	
			TotalbbDecay++;
		}
		
		if(Zc==2){	
			TotalccDecay++;
		}
		if(Zs==2){	
			TotalssDecay++;
		}
		if(Zd==2){	
			TotalddDecay++;
		}
		if(Zu==2){	
			TotaluuDecay++;
		}
		if(Znue==2){	
			TotalnueDecay++;
		}
		if(Znumu==2){	
			TotalnumuDecay++;
		}
		if(Znutau==2){	
			TotalnutauDecay++;
		}
		if(Ze==2){	
			TotaleDecay++;
		}
		if(Zmu==2){	
			TotalmuDecay++;
		}
		if(Ztau==2){	
			TotaltauDecay++;
		}
		
	}//event looop	
	
		
		
	//cout<<"Total bb decays ="<< TotalbbDecay <<endl;
	//cout<<"Total nunu (e) decays ="<< TotalnueDecay <<endl;
	//cout<<"Total Z created ="<< totalZ <<endl;
	double DecayRatio= double(TotalbbDecay)/double(totalZ);
	cout<<"bb decay ratio ="<< DecayRatio*100<< "%" <<endl;
	
	double DecayRatio9= double(TotalccDecay)/double(totalZ);
	cout<<"cc decay ratio ="<< DecayRatio9*100<< "%" <<endl;

	double DecayRatio8= double(TotalssDecay)/double(totalZ);
	cout<<"ss decay ratio ="<< DecayRatio8*100<< "%" <<endl;

	double DecayRatio7= double(TotalddDecay)/double(totalZ);
	cout<<"dd decay ratio ="<< DecayRatio7*100<< "%" <<endl;

	double DecayRatio6= double(TotaluuDecay)/double(totalZ);
	cout<<"uu decay ratio ="<< DecayRatio6*100<< "%" <<endl;

	double DecayRatio2= double(TotalnueDecay)/double(totalZ);
	cout<<"nunu (e) decay ratio ="<< DecayRatio2*100<< "%" <<endl;		

	double DecayRatio3= double(TotalnumuDecay)/double(totalZ);
	cout<<"nunu (mu) decay ratio ="<< DecayRatio3*100<< "%" <<endl;	

	double DecayRatio4= double(TotalnutauDecay)/double(totalZ);
	cout<<"nunu (tau) decay ratio ="<< DecayRatio4*100<< "%" <<endl;	

	double DecayRatio12= double(TotalmuDecay)/double(totalZ);
	cout<<"mumu decay ratio ="<< DecayRatio12*100<< "%" <<endl;		

	double DecayRatio13= double(TotaleDecay)/double(totalZ);
	cout<<"ee decay ratio ="<< DecayRatio13*100<< "%" <<endl;	

	double DecayRatio14= double(TotaltauDecay)/double(totalZ);
	cout<<"tauta decay ratio ="<< DecayRatio14*100<< "%" <<endl;

	double sumdecays=DecayRatio14+DecayRatio13+DecayRatio12+DecayRatio4+DecayRatio3+DecayRatio2
	+DecayRatio6+DecayRatio7+DecayRatio8+DecayRatio9+DecayRatio;
	cout<<"Sum: "<<sumdecays<<endl;			
	*/
	return 0;
} 
