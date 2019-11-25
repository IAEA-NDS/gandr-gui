/***************************************************************
* Name:      csv.cpp
* Desc. : This code is part of the software GANDR GUI,
*developed to provide a user friendly interface to
*            some of the functions of GANDR software.
* Purpose : code for the split routine.
* Author : Jan(jan.malec@gmail.com)
* Created : 2018 - 09 - 12
* License:   GPL - 3
* *************************************************************/

#include "csv.h"
#include <wx/wx.h> // remove later


std::vector<std::string> split(std::string const &input) {
    std::istringstream buffer(input);
    std::vector<std::string> ret{ std::istream_iterator<std::string>(buffer),
                                 std::istream_iterator<std::string>() };
    return ret;
}