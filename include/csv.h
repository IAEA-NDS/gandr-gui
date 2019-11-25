/***************************************************************
 * Name:      csv.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Provides routines needed to parse character separeted data.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef CSV_H
#define CSV_H
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>    // std::transform

 /*
 Split the input string by whitespace and return a
 vector of strings.
 */
std::vector<std::string> split(std::string const &input);

template <class T>
std::vector<T> inline valFromVector(std::vector<std::string>& str) {
    std::vector<T> convertedVector(str.size());
    std::transform(str.begin(), str.end(), convertedVector.begin(),
        [](const std::string& val) { return std::stod(val); });
    return convertedVector;
}

template<class T>
std::vector<T> inline valFromString(std::string str) {
    auto str_vec = split(str);
    return valFromVector<T>(str_vec);
}

#endif