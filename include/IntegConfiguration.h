/***************************************************************
 * Name:      IntegConfiguration.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define IntegConfiguration class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef INTEGCONFIGURATION_H
#define INTEGCONFIGURATION_H

#include "RunConfiguration.h"


class IntegConfiguration : public RunConfiguration
{
public:
    IntegConfiguration();
    virtual ~IntegConfiguration();

protected:

private:
};

#endif // INTEGCONFIGURATION_H
