/***************************************************************
 * Name:      GlobalConfiguration.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Singleton header for global configuration, 
 *            such as paths.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2019-01-08
 * License:   GPL-3
 **************************************************************/
#ifndef GLOBAL_CONFIGURATION_H
#define GLOBAL_CONFIGURATION_H
#include <iostream>
#include "wx/wx.h"

class Singleton
{
private:
    /* Private constructor to prevent instancing. */
    Singleton();

public:
    /* Static access method. */
    static Singleton* getInstance();
    wxString zottDir = "";
    wxString gandrDir = "";
    bool runGandr = true;
};

#endif