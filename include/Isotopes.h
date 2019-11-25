/***************************************************************
 * Name:      Isotopes.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define Element and Isotope classes.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef ISOTOPES_H
#define ISOTOPES_H
#include "csv.h"

class Element {
    short unsigned int Z;
    std::string symbol;
    std::string el_name;
};

class Isotope : Element {
    short unsigned int A;
};

#endif