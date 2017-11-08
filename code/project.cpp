#include "tempTrender.h"
#include <string>

using namespace std; 


void hotCold(tempTrender& lulea, tempTrender& karlstad, tempTrender& boras, tempTrender& falsterbo, tempTrender& falun, tempTrender& lund, tempTrender& soderarm, tempTrender& umea, tempTrender& visby){
	lund.hotCold();
}


void project(int choice) {
	string dataPathLulea = "../datasets/smhi-opendata_Lulea.csv"; 
	int startingLineLulea = 11;
	string dataPathKarlstad = "../datasets/smhi-opendata_Karlstad.csv";
	int startingLineKarlstad = 12;
	string dataPathBoras = "../datasets/smhi-opendata_Boras.csv";
	int startingLineBoras = 11;
	string dataPathFalsterbo = "../datasets/smhi-opendata_Falsterbo.csv";
	int startingLineFalsterbo = 11;		
	string dataPathFalun = "../datasets/smhi-opendata_Falun.csv";
	int startingLineFalun = 13;				
	string dataPathLund = "../datasets/smhi-opendata_Lund.csv";
	int startingLineLund = 13;
	string dataPathSoderarm = "../datasets/smhi-opendata_Soderarm.csv";
	int startingLineSoderarm = 12;
	string dataPathUmea = "../datasets/smhi-opendata_Umea.csv";
	int startingLineUmea = 13;
	string dataPathVisby = "../datasets/smhi-opendata_Visby.csv"; 
	int startingLineVisby = 11; 
	
	tempTrender lulea(dataPathLulea, startingLineLulea);
	lulea.readDataFile();
	
	tempTrender karlstad(dataPathKarlstad, startingLineKarlstad);
	karlstad.readDataFile();

	tempTrender boras(dataPathBoras, startingLineBoras);
	boras.readDataFile();
		
	tempTrender falsterbo(dataPathFalsterbo, startingLineFalsterbo);
	falsterbo.readDataFile();
			
	tempTrender falun(dataPathFalun, startingLineFalun);
	falun.readDataFile();
	
	tempTrender lund(dataPathLund, startingLineLund);
	lund.readDataFile();
	
	tempTrender soderarm(dataPathSoderarm, startingLineSoderarm);
	soderarm.readDataFile();
		
	tempTrender umea(dataPathUmea, startingLineUmea);
	umea.readDataFile();
	
	tempTrender visby(dataPathVisby, startingLineVisby);
	visby.readDataFile(); 
	

	switch(choice){
		case 1 : lulea.tempOnDay(3,3); 
				 break;
		case 2 : cout << "Davids arbete" << endl;
				 break;
		case 3 : hotCold(lulea, karlstad, boras, falsterbo, falun, lund, soderarm, umea, visby);
				 break;
		case 4 : cout << "Alexanders arbete" << endl;
	}
	

	
	  
	
	

}
