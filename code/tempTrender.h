#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
	
using namespace std; 

class tempTrender {
	public:
	tempTrender(string iniDataPath, int iniStartingLine); //Construct using the specified file
	~tempTrender() {} //Destructor
		
		
	void split(string stringToSplit, vector<string>& container, char delimiter){
		stringstream sts(stringToSplit);
		string token;
		while (getline(sts, token, delimiter)){
			container.push_back(token);
		}

	}
	double convertDateToYear(int i){
		double tempDate = 0.0;
		double normalYear = 365.0000000000; //days
		double leapYear = 366.0000000000; //days
		double hourToDay = 24.0;
		if((year.at(i) % 4 == 0) && (year.at(i) % 100 != 0)){
			tempDate = hour.at(i) / (hourToDay * leapYear) + day.at(i)/leapYear + month.at(i)/12 + year.at(i);
		}
		else if(year.at(i) % 100 == 0){
			tempDate = hour.at(i)/ (hourToDay * normalYear) + day.at(i)/normalYear + month.at(i)/12 + year.at(i);
		}
		else if(year.at(i) % 400 == 0){
			tempDate = hour.at(i) / (hourToDay * leapYear) + day.at(i)/leapYear + month.at(i)/12 + year.at(i);
		}
		else{
			tempDate = hour.at(i) / (hourToDay * normalYear) + day.at(i)/normalYear + month.at(i)/12 + year.at(i);
		}
		return tempDate;
	}
	void readDataFile(){
		ifstream dataFile(dataPath.c_str());
		
		for(int j = 1; j < startingLine; j++){
			getline(dataFile, stringDummy);
		}
		string segment;
		
		while(getline(dataFile, segment))
		{
			vector<string> stringSeparationVector;
			vector<string> dateDummy;
			vector<string> hourDummy;
			split(segment, stringSeparationVector, ';');
			
			//Splits the date yyyy-mm-dd into three vectors (year, month, day) with yyyy, mm, dd
			split(stringSeparationVector.at(0), dateDummy,'-');
			year.push_back(strtof(dateDummy.at(0).c_str(), 0));
			month.push_back(strtof(dateDummy.at(1).c_str(), 0));
			day.push_back(strtof(dateDummy.at(2).c_str(), 0));
			
			//Splits the time hh:mm:ss into hh only vector (no minutes and seconds are recorded in files)
			split(stringSeparationVector.at(1), hourDummy,'-');
			hour.push_back(strtof(hourDummy.at(0).c_str(), 0));

			//Writes the temperature into the vector
			temperature.push_back(strtod(stringSeparationVector.at(2).c_str(), 0));
			
			//Creates year in decimal and stores in a vector
			decimalYear.push_back(convertDateToYear(i));
			i++;
		}
		
		dataFile.close();
	}
	
	int GetYear(int dataPoint){return year.at(dataPoint);}
	int GetMonth(int dataPoint){return month.at(dataPoint);}
	int GetDay(int dataPoint){return day.at(dataPoint);}
	int GetHour(int dataPoint){return hour.at(dataPoint);}
	float GetTemperature(int dataPoint){return temperature.at(dataPoint);}
	double GetDate(int dataPoint){return decimalYear.at(dataPoint);}
	
	
	void tempOnDay(int monthToCalculate, int dayToCalculate){		
		
		ofstream Tfile("tredjeMars.csv");
		vector <int> yearVec; 
		vector <float> tempVec;
		for (int y = 0; (unsigned)y < (year.size()-1); y++){
			
				if (month.at(y) == monthToCalculate && day.at(y) == dayToCalculate){  
					//Tfile << year.at(y)<< " - "<< month.at(y)<< " - " << day.at(y)<< " - " << temperature.at(y) << endl;	
					Tfile << year.at(y) << " - " << temperature.at(y) << endl;
					yearVec.push_back(year.at(y)); 
					tempYear += temperature.at(y);
					count++;							
				}
				if (year.at(y) != year.at(y+1)){
					tempYearAvg = tempYear/count;
					tempVec.push_back(tempYearAvg);
					tempYear=0;
					count=0;
					//cout << tempVec.at(l) << endl; //funkar inte med denna 
					//cout << year.at(y) << endl;
					l++;
				}else if (year.at(y) == year[year.size()-1] && month.at(y) == month[month.size()-1] && day.at(y) == day[day.size()-1]){
					tempYearAvg = tempYear/count;
					tempVec.push_back(tempYearAvg);
					
					//cout << tempVec.at(l) << endl;
					
					
					}
						
		}
		
		
		Tfile.close();
		
		/*-------------------EJ KLAR : Creating Histogram--------------------
		TH1D* histo = new TH1D("temperature", "Temperature;Temperature[#circC];Entries", 
			300, -40, 40);
		histo->SetFillColor(kRed);
		for (int k = 0; (unsigned)k < tempVec.size(); k++){
			histo->Fill(tempVec.at(k));
			}
		double mean = histo->GetMean(); 
		double stdev = histo->GetRMS(); 
		TCanvas* can = new TCanvas(); 
		histo->Draw(); 	
		---------------Creating Histogram-----------------*/ 
		
		
		/*---------------------Mean Temperature------------------*/
		float meanT = 0; 	
		
		//for loop for calculating the sum ttt of all temperatures in tempVec 
		for(int tt = 0; (unsigned)tt < tempVec.size(); tt++){
			meanT += tempVec.at(tt);  
			count++;  
			}
		
		//dividing the sum by the amount of iterations to get the mean value ttt	
		meanT /= count;
		cout << "The mean temperature for march third for all years is: " << meanT << " degrees celcius."<< endl;
		/*----------------Mean Temperature-----------------*/
		
		
		/*---------------EJ KLAR : Standard Deviation-------------*/
		float diffT = 0; 
		float kvd; 
		vector <float> kvdV; 
		float meanKv = 0; 
		float sigma; 
		
		for(int s = 0; (unsigned)s < tempVec.size(); s++){
			diffT = tempVec.at(s) - meanT;
			kvd = pow(diffT,2);
			kvdV.push_back(kvd);
			
			//cout << kvdV[s] << endl; 
			}
			
		for(int r = 0; (unsigned)r < kvdV.size(); r++){
			meanKv += kvdV.at(r); 
			count++; 
			}
			
		meanKv /= count; 
		sigma = TMath::Sqrt(meanKv); 
		cout << "The standard deviation is: " << sigma << endl;	
		/*----------------Standard Deviation-----------*/
		
		
	}
		
		
		
	
			
		
		
	

	
	private:
	
	string dataPath;
	string stringDummy;
	int intDummy;
	float temperatureDummy;
	vector<float> temperature; //in celsius
	vector<double> decimalYear;
	vector<int> year;
	vector<int> month;
	vector<int> day;
	vector<int> hour;
	int startingLine;
	int i;
	float tempYear;
	int count;
	float tempYearAvg;
	int l;
	
	//void tempOnDay(int monthToCalculate, int dayToCalculate); //Make a histogram of the temperature on this day
	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

};

#endif
