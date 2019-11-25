/***************************************************************
 * Name:      ExforReader.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for parsing Exfor files with experimental data.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "ExforReader.h"
#include "csv.h"

ExforReader::ExforReader(std::string fileName) : fileName(fileName) {
}


ExforReader::~ExforReader() {
}

void ExforExperiment::from_title(std::string title) {
    author = title.substr(0, 25);
	try {
		isotope_name = title.substr(41, 9);
		int Z = std::stoi(isotope_name.substr(0, 3));
		int A = std::stoi(isotope_name.substr(6, 3));
		// The "ZA" number is calculated as 1000*Z+A
		isotope = 1000 * Z + A;
		mf = std::stoi(title.substr(52, 2));
		mt = std::stoi(title.substr(56, 3));
	}
	catch (...) {
		return;
	}
}

void ExforExperiment::data_from_line(std::string line) {
    auto float_res = valFromString<double>(line);
    if (float_res.size() == 6) {
        energy.push_back(float_res.at(0));
        d_energy.push_back(std::max(float_res.at(1), float_res.at(2)));
        value.push_back(float_res.at(3));
        d_value.push_back(std::max(float_res.at(4), float_res.at(5)));
    }
}

void ExforReader::read() {
    std::ifstream exfile(fileName);
    std::string line;
    ExforExperiment *current = new ExforExperiment();
    current->author = "";
    std::string author_last = "";
    bool new_exp = false;

    while (std::getline(exfile, line)) {
        if (line.size()) {
            if (line.size() > 67 || new_exp) {
                if (current->author.size()) {
                    experiments.push_back(*current);
                    delete current;
                    current = new ExforExperiment();
                    new_exp = false;
                }
                author_last = current->author;
                current->from_title(line);
            }
            else {
                current->data_from_line(line);
            }
        }
        else {
            new_exp = true;
        }
    }
    if (current->author.size()) {
        experiments.push_back(*current);
        delete current;
    }
    if (!experiments.size()) {
        wxMessageBox(wxT("No experiments read"), wxT("EXFOR Reader"), wxICON_INFORMATION);
        throw std::invalid_argument("No experiemnts read");
    }

	if (experiments.at(0).mf == 0) {
		wxMessageBox("Invalid header, missing info");
	}
}

wxArrayString ExforReader::getTitles() {
    wxArrayString titles;
    for (auto e : experiments) {
		if (e.mf != 0) {
			titles.Add(e.author + " MF " + std::to_string(e.mf)
				+ " MT " + std::to_string(e.mt));
		}
		// In case of non-standard headers
		else {
			titles.Add(e.author + " MF " + "N/A"
				+ " MT " + "N/A");
		}
    }
    return titles;
}

std::vector<int> ExforReader::getLengths() {
    std::vector<int> lengths;
    for (auto e : experiments)
        lengths.push_back(e.energy.size());
    return lengths;
}

std::vector<int> ExforReader::getIndexes() {
    auto lengths = getLengths();
    std::vector<int> indexes;
    indexes.push_back(0);
    for (auto len : lengths)
        indexes.push_back(indexes.back() + len);
    indexes.erase(indexes.begin());
    return indexes;
}