/***************************************************************
 * Name:      ENDFReader.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for ENDFReader, a class that can extract data
 *            from ENDF files.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "ENDFReader.h"


ENDFReader::ENDFReader(std::string fileName) : fileName(fileName) {
}

void ENDFReader::read_cs(unsigned int mf, unsigned int mt, cs_data & data) {
    std::string line;
    std::ifstream enfile(fileName);
    cs_data temp_data;
    bool found_section = false;
    int line_no = 0;
    while (std::getline(enfile, line)) {
		wxMessageBox(line);
        line_no += 1;
        if (line_no == 1) {
            try {
                temp_data.NA = std::stof(line.substr(0, 11).c_str());
            }
            catch (...) {
                wxMessageBox("Could not enter material number");
                return;
            }
        }
        try {
            temp_data.matnam = std::stoi(line.substr(66, 4).c_str());
            temp_data.mf = std::stoi(line.substr(70, 2).c_str());
            temp_data.mt = std::stoi(line.substr(72, 3).c_str());
        }
        catch (...) {
            wxMessageBox("Could not read matnam");
            return;
        }
        if (temp_data.mf == mf && temp_data.mt == mt) {
            found_section = true;
            break;
        }
        if (temp_data.mf > mf && temp_data.mt > mt) {
            found_section = false;
            break;
        }
    }
    if (!found_section) {
        wxMessageBox("Could not find section in file");
        throw std::invalid_argument("Could not find section in file");
    }
    try {
        std::getline(enfile, line);
        std::getline(enfile, line);
        temp_data.len = std::stoi(line.substr(0, 12).c_str());
    }
    catch (...) {
        wxMessageBox("Could not read reaction data");
        throw std::invalid_argument("Could not read reaction data");
    }
    data = temp_data;
}

bool ENDFReader::isValid() {
    return false;
}


ENDFReader::~ENDFReader() {
}