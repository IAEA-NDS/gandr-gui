/***************************************************************
 * Name:      gen_utls.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Headers for some helper functions used throught 
 *            the program.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2019-01-08
 * License:   GPL-3
 **************************************************************/

#pragma once
#ifndef GEN_UTILS_H
#define GEN_UTILS_H
#include <string>
#include <vector>
#include <array>
/*
Takes an array of numbers as input and returns a string in "short"
format. The tailing zeroes are discarded. 
For example, the array with elements [1, 2, 2, 2, 3, 3, 0, 0] 
is converted to a string "1 3*2 2*3".
*/
template<typename T, std::size_t SIZE>
std::string vector_to_short_string(std::array<T, SIZE> in_array) {
    auto last_el = in_array.at(0);
    int counter = 0;
    std::string out_string = "";

    for (auto &el : in_array) {
        if (el == last_el) {
            counter += 1;
        }
        else if (counter > 1) {
            out_string += " " + std::to_string(counter) +
                "*" + std::to_string(last_el);
            last_el = el;
            counter = 1;
        }
        else if (counter == 1) {
            out_string += " " + std::to_string(last_el);
            last_el = el;
        }
    }
    if (counter != 0 && last_el != 0)
        out_string += " " + std::to_string(counter) +
        "*" + std::to_string(last_el);
    return out_string;
}

/*
Takes an vector of numbers as input and returns a string in "short"
format. The tailing zeroes are discarded.
For example, the array with elements [1, 2, 2, 2, 3, 3, 0, 0]
is converted to a string "1 3*2 2*3".
*/
template<typename T>
std::string vector_to_short_string(std::vector<T> in_array) {
    auto last_el = in_array.at(0);
    int counter = 0;
    std::string out_string = "";

    for (auto &el : in_array) {
        if (el == last_el) {
            counter += 1;
        }
        else if (counter > 1) {
            out_string += " " + std::to_string(counter) +
                "*" + std::to_string(last_el);
            last_el = el;
            counter = 1;
        }
        else if (counter == 1) {
            out_string += " " + std::to_string(last_el);
            last_el = el;
        }
    }
    if (counter != 0 && last_el != 0)
        out_string += " " + std::to_string(counter) +
        "*" + std::to_string(last_el);
    return out_string;
}

bool isDouble(const std::string& s);

#endif