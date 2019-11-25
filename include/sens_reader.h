/***************************************************************
 * Name:      sens_reader.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Provides routines needed to parse character separeted data.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2019-11-18
 * License:   GPL-3
 **************************************************************/
#ifndef SENS_READER_H
#define SENS_READER_H

#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <wx/wx.h> // remove later

enum class SensType {
	DICE_FORMAT,
	GANDR_FORMAT
};

class sens {
public:
	std::vector<double> values;
	std::string name;
	sens(std::string name);
	sens() {};
	template <class Archive>
	void serialize(Archive& ar) {
		ar(name, values);
	}
};

class sensData {
public:
	SensType type;
	void readSens(std::string fileName);
	void writeSens(std::string fileName, wxArrayInt *checked);
	std::vector<sens> sensitivities;
	std::vector<double> energyScale;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(sensitivities, energyScale);
	}
};

#endif