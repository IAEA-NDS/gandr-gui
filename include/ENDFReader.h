/***************************************************************
 * Name:      ENDFReader.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define ENDFReader, a class that can extract data
 *            from ENDF files.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef ENDFREADER_H
#define ENDFREADER_H

#include <vector>
#include <fstream>
#include <wx/wx.h> //debugging, remove later

struct cs_data {
    int len;
    unsigned int mt;
    unsigned int mf;
    unsigned int matnam;
    float NA;
    std::vector<double> data;
};

#pragma once
#include <string>

 /*
 ENDFReader provides routines necessary to exatract data
 from ENDF files.
 */
class ENDFReader
{
public:
    ENDFReader(std::string fileName = "92235.pendf");
    void read_cs(unsigned int mf, unsigned int mt, cs_data &data);
    bool isValid();
    ~ENDFReader();
    std::string fileName;
};

#endif