// The bump is caused due to H->gamma Z

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/FastJet3.h"
#include <iostream>
#include <fstream>
using namespace Pythia8;
int main(){
	int nEvent    = 100000;
	double R       = 0.6;    // JetSize
	double pTMin   = 20.0;    // Min jet pT.
	double etaMax  = 2.5;    // Pseudorapidity range of detector.

  	Pythia pythia;	
  	Event& event = pythia.event;
  	// Process selection
	pythia.readString("HiggsSM:gg2H = on");

	pythia.readString("HadronLevel:Hadronize = on"); // Hadronisation
	pythia.readString("HadronLevel:Decay = on"); // Decay
	pythia.readString("PartonLevel:FSR = off");
	pythia.readString("PartonLevel:ISR = off");
	pythia.readString("PartonLevel:MPI = off");

	pythia.readString("Beams:frameType = 4");
	pythia.readString("Beams:LHEF = /home/dhruvg/calchep_3.8.7/cpp/results/events_ssm14tev.lhe");
	pythia.readString("35:name=h2");
	pythia.init();
	pythia.process.list(); // prints the list of hard process
	pythia.info.list();
	event.list();

	// fastjet::JetDefinition jetDef(fastjet::genkt_algorithm, R, power);
	fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, R);
	// fastjet::JetDefinition jetDef(fastjet::kt_algorithm, R);
	// fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, R);
	std::vector <fastjet::PseudoJet> fjInputs;

	// ofstream myfile1("invmbbarClean.dat");
	ofstream myfile1("ssmjetmult.dat");
	ofstream myfile2("ssm1jet.dat");
	ofstream myfile3("ssm2jet.dat");
	ofstream myfile4("ssm3jet.dat");
	ofstream myfile5("ssm4jet.dat");
	ofstream myfile6("ssm1m.dat");
	ofstream myfile7("ssm2m.dat");
	ofstream myfile8("ssm3m.dat");
	ofstream myfile9("ssm4m.dat");
	for(int iEvent=0;iEvent<nEvent;iEvent++) 
	{
		pythia.next();
		fjInputs.resize(0);
		for (int i = 0; i < event.size(); ++i) if(event[i].isFinal())
		{
			if (event[i].isVisible())
			{
				if (abs(event[i].eta()) < etaMax)
				{
					fastjet::PseudoJet particleTemp = event[i];
					fjInputs.push_back( particleTemp);
				}
			}			
		}
		vector <fastjet::PseudoJet> inclusiveJets, sortedJets;
		fastjet::ClusterSequence clustSeq(fjInputs, jetDef);
    	inclusiveJets = clustSeq.inclusive_jets(pTMin);
    	sortedJets = sorted_by_pt(inclusiveJets);
    	myfile1<<int(sortedJets.size())<<endl;

    	if(int(sortedJets.size())==1)
    	{
    		myfile2<<sortedJets[0].perp()<<endl;
    		myfile6<<sortedJets[0].m()<<endl;
    	}
    	else if(int(sortedJets.size())==2)
    	{
    		myfile2<<(sortedJets[0]).perp()<<endl;
    		myfile3<<sortedJets[1].perp()<<endl;
    		myfile7<<(sortedJets[0]+sortedJets[1]).m()<<endl;
    	}
    	else if(int(sortedJets.size())==3)
    	{
    		myfile2<<(sortedJets[0]).perp()<<endl;
    		myfile3<<sortedJets[1].perp()<<endl;
    		myfile4<<sortedJets[2].perp()<<endl;
    		myfile8<<(sortedJets[0]+sortedJets[1]+sortedJets[2]).m()<<endl;
    	}
    	else if(int(sortedJets.size())==4)
    	{
    		myfile2<<(sortedJets[0]).perp()<<endl;
    		myfile3<<sortedJets[1].perp()<<endl;
    		myfile4<<sortedJets[2].perp()<<endl;
    		myfile5<<sortedJets[3].perp()<<endl;
    		myfile9<<(sortedJets[0]+sortedJets[1]+sortedJets[2]+sortedJets[3]).m()<<endl;
    	}

	}
	pythia.stat();
	myfile1.close();
	myfile2.close();
	myfile3.close();
	myfile4.close();
	myfile5.close();
	myfile6.close();
	myfile7.close();
	myfile8.close();
	myfile9.close();
	return 0;
}
