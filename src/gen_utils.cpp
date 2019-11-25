/***************************************************************
 * Name:      gen_utls.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Sources for some helper functions used throught
 *            the program.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2019-01-08
 * License:   GPL-3
 **************************************************************/


#include "gen_utils.h"
 // Is string a float?
bool isDouble(const std::string& s) {
	try {
		std::stod(s);
		return true;
	}
	catch (...) {
		return false;
	}
}