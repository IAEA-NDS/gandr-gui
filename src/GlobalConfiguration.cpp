/***************************************************************
 * Name:      GlobalConfiguration.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Singleton for global configuration, such as paths.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2019-01-08
 * License:   GPL-3
 **************************************************************/
#include "GlobalConfiguration.h"

Singleton* Singleton::getInstance() {
    static Singleton instance;

    return &instance;
}

Singleton::Singleton() {
}
