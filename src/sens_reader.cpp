/***************************************************************
 * Name:      sens_reader.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Provides routines needed to read sensitivities 
 *			  in whitespace separated CSV or in GANDR format
 *            (one sensitivity per line).
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2019-11-18
 * License:   GPL-3
 **************************************************************/

#include<sens_reader.h>
#include "csv.h"
#include "gen_utils.h"
#include "GanrunConfiguration.h"
#include <iomanip>
#include <limits>
#include "wx/wx.h"


/* Reads sensitivities in GANDR format or DICE 
format and returns a structure with column information */

void sensData::readSens(std::string fileName)
{
	std::ifstream inFile(fileName);
	std::string line;
	/* Read first line to detect format. 
	If it contains the word "Energy", assume GANDR format. */
	std::getline(inFile, line);
	std::transform(line.begin(), line.end(), line.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (line.find("ev") == std::string::npos){
		type = SensType::DICE_FORMAT;
		wxMessageBox("File in DICE format, names will not be provided");
	}
	else {
		wxMessageBox("Sensitivities in GANDR format.");
		type = SensType::GANDR_FORMAT;
	}
	
	if (type == SensType::DICE_FORMAT) {
		auto vals = valFromString<double>(line);
		sensitivities.assign(vals.size() - 1, sens("Name me"));
		inFile.seekg(0);
		while (std::getline(inFile, line)) {
			if (!line.size())
				break;
			std::vector<double> vals = valFromString<double>(line);
			energyScale.push_back(vals.at(0));
			size_t i = 0;
			for (auto it = std::next(vals.begin()); it != vals.end(); ++it) {
				sensitivities.at(i).values.push_back(*it);
				i++;
			}
		}
	}
	else if (type == SensType::GANDR_FORMAT) {
		while (std::getline(inFile, line)) {
			if (isDouble(line))
				break;
			sensitivities.push_back(sens(line));
		}

		size_t counter = 0;
		double val;

		/* Scroll to the start of file and skip the 
		   reaction names. */
		inFile.seekg(0);
		for (int i = 0; i < (1+ sensitivities.size()); i++)
			std::getline(inFile, line);

		while (std::getline(inFile, line)) {
			size_t local_ind = counter % (sensitivities.size()+1);
			try {
				val = std::stod(line);
			}
			catch (...) { 
				wxMessageBox("Could not read value " + line);
				val = -1;
				break;
			}
			if (local_ind == 0)
				energyScale.push_back(stod(line));
			else
				sensitivities.at(local_ind - 1).values.push_back(stod(line));
			counter++;
		}
	}


}

void sensData::writeSens(std::string fileName, std::vector<int> *checked)
{
	std::ofstream oFile(fileName);

	// Write header with descriptions
	oFile << "Energy [eV]" << std::endl;
	for (size_t j = 0; j < sensitivities.size(); j++) {
		if (std::find(checked->begin(), checked->end(), j) != checked->end())
			oFile << sensitivities.at(j).name << std::endl;
	}

	// Write the actual data
	for (size_t i = 0; i < energyScale.size(); i++) {
		oFile << std::scientific << std::setprecision(6)
			<< energyScale.at(i) << std::endl;
		for (size_t j = 0; j < checked->size(); j++) {
			if (checked->at(j))
				oFile << std::scientific << std::setprecision(6)  <<
					sensitivities.at(j).values.at(i) << std::endl;
		}
	}
	oFile.close();
}
sens::sens(std::string name) : name(name) {
}

