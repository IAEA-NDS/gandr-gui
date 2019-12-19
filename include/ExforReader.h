/***************************************************************
 * Name:      ExforReader.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for parsing Exfor files with experimental data.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/


#ifndef EXFOR_READER_H
#define EXFOR_READER_H

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <utility>
#include "wx/arrstr.h"
#include <wx/msgdlg.h>
#include "csv.h"

 /*
  ExforExperiment holds information about a signle experiment
  with differential data that can be used for fitting.
 */
class ExforExperiment {
public:
    std::string author;
    unsigned short int year;
    int isotope;
    unsigned short int mf;
    unsigned short int mt;
    std::string isotope_name;
    std::pair <int, int> num; //the two numbers at the end
    std::vector<double> energy;
    std::vector<double> d_energy;
    std::vector<double> value;
    std::vector<double> d_value;

    void from_title(std::string title);
    void data_from_line(std::string line);
};

/*
 ExforReader class provides routines needed to read EXFOR files in GANDR format.
*/
class ExforReader {
public:
    ExforReader(std::string _fileName = "92235.exfor");
    std::string fileName;
    std::vector<ExforExperiment> experiments;
    ~ExforReader();
    void read();
    wxArrayString getTitles();
    std::vector<int> getLengths();
    std::vector<int> getIndexes();
	std::vector<std::pair<double, double>> getEnergies();
};

#endif