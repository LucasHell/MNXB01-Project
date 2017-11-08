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
		//printf("%f\n", tempDate);

		return tempDate;
	}
	
	int convertMonthToDays(int l) { // function to calculate the number of days in the months before the given month
		int monthDaysNormal[13] = { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 }; // arrays to store number of days
		int monthDaysLeap[13] = { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
		
		if((year.at(k) % 4 == 0) && (year.at(k) % 100 != 0)){ // leap year check
			daysMonth = monthDaysLeap[l];
		}
		else if(year.at(k) % 100 == 0){
			daysMonth = monthDaysNormal[l];
		}
		else if(year.at(k) % 400 == 0){
			daysMonth = monthDaysLeap[l];
		}
		else{
			daysMonth = monthDaysNormal[l];
		}
		return daysMonth;
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
	
	int GetMonth(int dataPoint){return month.at(dataPoint);}
	int GetDay(int dataPoint){return day.at(dataPoint);}
	int GetHour(int dataPoint){return hour.at(dataPoint);}
	float GetTemperature(int dataPoint){return temperature.at(dataPoint);}
	double GetDate(int dataPoint){return decimalYear.at(dataPoint);}
	

	void hotCold() {
		int currentYear = year.at(0); // Set starting values for year, month and day
		int currentMonth = month.at(0);
		int currentDay = day.at(0);
		float coldest = 100; // arbitrary number which is far above the coldest temperature
		float hottest = 0; // arbitrary number with is far below the hottest temperature
		
		TH1D* hotHist = new TH1D("Hot", "Histogram of hottest days over all years; day; Counts",  // Histogram from hottest days
			366, 1, 367);
		TH1D* coldHistEarly = new TH1D("ColdEarly", "Histogram of coldest days over all years; day; Counts",  // Histogram for coldest days in the beginning of the year
			366, 1, 367);
			
		TH1D* coldHistLate = new TH1D("ColdLate", "Histogram of coldest days over all years; day; Counts", // Histogram for coldest days in the end of the year
			366, 1, 367);
		
		for (k = 0; (unsigned)k < (year.size()-1); k++) { 
			if (temperature.at(k) > hottest){ // if the temperature of a given day is higher than the previously highest temperature it will update the variables
				hottest = temperature.at(k);
				hottestDay = day.at(k);
				hottestMonth = month.at(k);
			}
			
			if (temperature.at(k) < coldest){ // if the temperature of a given day is below the previously coldest temperature it will update the variables
				coldest = temperature.at(k);
				coldestDay = day.at(k);
				coldestMonth = month.at(k);
			}
			
			if (currentYear != year.at(k+1)){ // at the end of the year the hottest and coldest temperatures are pushed into two seperate vectors and coldest and hottest variables are reset
				currentYear = year.at(k+1);
				currentMonth = month.at(k+1);
				currentDay = day.at(k+1);
				hotDays.push_back(convertMonthToDays(hottestMonth) + hottestDay);
				coldDays.push_back(convertMonthToDays(coldestMonth) + coldestDay);
				coldest = 100;
				hottest = 0;
				
			} else if ( year.at(k) == year[year.size() - 1] && month.at(k) == month[month.size() - 1] && day.at(k) == day[day.size() - 1] ){ // if statements which saves the variables for the last year
				hotDays.push_back(convertMonthToDays(hottestMonth) + hottestDay);
				coldDays.push_back(convertMonthToDays(coldestMonth) + coldestDay);
			}
		}
	
	
		for (int j = 0; (unsigned)j < hotDays.size(); j++) { // add the values from the vectors to histograms
			hotHist->Fill(hotDays.at(j));
			if (coldDays.at(j) < 200){ // add the values for days in the beginning of the year to a seperate vector for fitting
				coldHistEarly->Fill(coldDays.at(j));
			} else { // add the values for days in the end of the year to a seperate vector for fitting
				coldHistLate->Fill(coldDays.at(j));
			}
			
		}
		TCanvas* hc = new TCanvas("hc", "Hot cold canvas", 900, 600); // Create canvas for plot
		
		hotHist->SetLineColor(2);
		hotHist->SetFillColor(2);
		hotHist->Draw(); // Draw the histogram containing hot days
		
		coldHistEarly->SetLineColor(4);
		coldHistEarly->SetFillColor(4);
		coldHistEarly->Draw("SAME"); // Draw the histogram containing early cold days in the same canvas

		coldHistLate->SetLineColor(4);
		coldHistLate->SetFillColor(4);
		coldHistLate->Draw("SAME"); // Draw the histogram containing late cold days in the same canvas
		gPad->RedrawAxis(); // Redraw axis because it became same color as cold histogram
		
		TF1* fitHot = new TF1("fitFuncHot", "gaus", 1, 367);
		fitHot->SetParameter(5, 200);
		fitHot->SetLineColor(1);
		fitHot->SetLineStyle(1);
		fitHot->SetLineWidth(2);
		hotHist->Fit(fitHot, "QMLL"); // fit the guassian to the values for hottest days
		
		TF1* fitColdEarly = new TF1("fitFuncColdEarly", "gaus", 1, 367);
		fitColdEarly->SetParameter(5, 200);
		fitColdEarly->SetLineColor(1);
		fitColdEarly->SetLineStyle(2);
		fitColdEarly->SetLineWidth(2);
		coldHistEarly->Fit(fitColdEarly, "QLLM"); // fit the guassian to the values for the early coldest days
		
		
		TF1* fitColdLate = new TF1("fitFuncColdLate", "gaus", 1, 367);
		fitColdLate->SetParameter(5, 200);
		fitColdLate->SetLineColor(1);
		fitColdLate->SetLineStyle(2);
		fitColdLate->SetLineWidth(2);
		coldHistLate->Fit(fitColdLate, "QLLM"); // fit the guassian to the values for the late coldest days
		
		
		//Print mean and uncertainty
		cout << "Cold: " << endl;
		cout << "The mean is " << fitColdEarly->GetParameter(1) << endl;
		cout << "Its uncertainty is " << fitColdEarly->GetParError(1) << endl << endl;
		
		cout << "Hot: " << endl;
		cout << "The mean is " << fitHot->GetParameter(1) << endl;
		cout << "Its uncertainty is " << fitHot->GetParError(1) << endl;
		
		hc->SaveAs("../report/img/hotColdDays.jpg");

	}	
		

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
					cout << tempVec.at(l) << endl;
					cout << year.at(y) << endl;
					l++;
				}else if (year.at(y) == year[year.size()-1] && month.at(y) == month[month.size()-1] && day.at(y) == day[day.size()-1]){
					tempYearAvg = tempYear/count;
					tempVec.push_back(tempYearAvg);
					
					//cout << tempVec.at(l) << endl;
					
					
					}
				
				
		}		
		Tfile.close();
		TH1D* histo = new TH1D("temperature", "Temperature;Temperature[#circC];Entries", 
			300, -40, 40);
			histo->SetFillColor(kRed);
			for (int k = 0; (unsigned)k<tempVec.size()-1; k++){
			//histo->Fill(0);
			}
			//double mean = histo->GetMean();
		cout << year.back() << endl;
		cout << "hej" << endl;
		
	
		
		//
		//cout << tempVec.size() << endl;
		/*for (int k = 0; (unsigned)k<tempVec.size(); k++){
			//histo->Fill(tempVec.at(k));
			}*/
		/*double mean = histo->GetMean(); 
		double stdev = histo->GetRMS(); 
		TCanvas* can = new TCanvas(); 
		histo->Draw(); */

		
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
	int k;
	int hottestDay;
	int hottestMonth;
	int coldestDay;
	int coldestMonth;
	vector<int> hotDays;
	vector<int> coldDays;
	int daysMonth;
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
