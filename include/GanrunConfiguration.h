/***************************************************************
 * Name:      GanrunConfiguration.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define GunRunConfiguration class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef GANRUNCONFIGURATION_H
#define GANRUNCONFIGURATION_H

#include "RunConfiguration.h"
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <array>
#include <cmath>

#include "sens_reader.h"

#define NRMAX 25


enum imodes {
    STANDARDS = 10,
    MIXTURE = 11,
    CLASSIC = 12,
    PRODUCT = 13,
    C5 = 14,
    INTEGRAL = 15
};

enum idefs {
    REUSE = 0,
    LINEAR_COMBINATION = 1,
    FROM_EXFOR = 2
};

enum ic5s {
    COV_FROM_EXFOR = 0,
    COV_FROM_C5M = 1,
    COV_FROM_C5M_ACCEPT_NEG = -1 // like 1, but user
                                    //accepts negative eigs
};

enum {
    DIFFERENTIAL_EXP,
    INTEGRAL_EXP
};

struct reaction {
    int gan_no = 0;
    int mte = 0;
    std::string label;
    std::vector<int> composition;

    template <class Archive>
    void serialize(Archive &ar) {
        ar(gan_no, mte, label, composition);
    }
};

struct additional_cov_data {
    std::pair <int, int> range;
    double covfc = 0;
    bool irelfc = true;

    template <class Archive>
    void serialize(Archive &ar) {
        ar(range, covfc, irelfc);
    }
};

struct additional_sens_data {
    // card 20
    int nam; // gandr material name for j-th exp

    // card 21
    std::array<double, 25> xexary; // nrmax(25) factor giving dep of 
                         // ne-th exp on official gandr reaction for
                         // material j
    template <class Archive>
    void serialize(Archive &ar) {
        ar(nam, xexary);
    }
};

struct exfor_cov_data {
    float inca = 2.;
    float covmin = -4.;
    bool irelco = 1;
    bool isys = 0;
    bool iglobl = 0;
    int expno; // Not part of output, used internally. -1 means global
    std::vector<double> xsys;
    bool edited = false;

    bool operator==(const exfor_cov_data& rhs) {
        return (rhs.inca == inca && rhs.covmin == covmin &&
            rhs.irelco == irelco && rhs.isys == isys && rhs.iglobl == iglobl
            && rhs.xsys == xsys);
        return true; /* your comparison code goes here */
    }

    template <class Archive>
    void serialize(Archive &ar) {
        ar(inca, covmin, irelco, isys, iglobl, expno, xsys);
    }
};

struct standards_experiment {
    // card 16
    int mats = 9228; // mat number for ne-th exp
    int mts = 1; // mt number
    int nps = 0; // number of data paris in ne-th exp (e-sig example)
    int nexc = 0; // Number of "extra" convariance contributions
    int nlib = 0; // Number of materials with finite sens for ne-th exp
    bool ismg = DIFFERENTIAL_EXP; // 1 if multigroup data present, 0 default
    bool irm = 0; // Absolute or relative covariance contribution?

    // card 17
    int nampl = 0; // 5 character experiment name
    int mtpl = 0; // reaction identifier for ne-th exp

    // card 18, nexc records
    std::vector<additional_cov_data> cov_data;
    // User supplied covariance data

    // card 19
    std::vector<int> mgrid; // nps integers that id energy int. quant.

    std::vector<additional_sens_data> sens_data; 

    template <class Archive>
    void serialize(Archive &ar) {
        ar(mats, mts, nps, nexc, nlib, ismg, irm, nampl, mtpl, cov_data, mgrid, sens_data);
    }
};

struct intReac {
public:
	//CARD 39, imode 15, 
	int matna = 1;	     	   // Material name for n-th reac
	std::string rnam = "reac"; // User defiend string

	int za = 1;
	//CARD 40, imode 15,
	int mfd = 3;          // data type(file) to be processed
	int mtd = 1;          // reaction(section) to be processed
	double temp = 293.6;     // temperature in kelvin
	double sigz = 1.e10; // sigma zero value(usually 1.e10)

	//CARD 41, imode 15,
	int iusegp = 238;    // number of groups in the user group structure
	int igrtop = 0;    // number of energies in the gandr grid
	int igrid = 4;     // identifier of gandr grid type

	//CARD 42, imode 15,
	int iwt = 3;   //  flux weighting option
				   // = 1 if the weight function is to be read in
				   // = 2 if the weight function is a constant(1.)
				   // = 3 if the weight function is 1 / e
	int interp = 2;// interpolation "law" to be used between energy
	int np = 4800; // number of energy points in the weight function
	               // points in the weight function w(e)
	               // = 1 if w is constant in e(histogram)
	               // = 2 if w is linear in e(linear - linear)
	               // = 3 if w is linear in ln(e) (linear - log)
	               // = 4 if ln(w) is linear in e(log - linear)
	               // = 5 if ln(w) is linear in ln(e) (log - log)
	               //(default = 2)
	template <class Archive>
	void serialize(Archive& ar) {
		ar(matna, rnam, za, mfd, mtd, temp, sigz, iusegp, igrtop, igrid, iwt, interp, np);
	}
};

//class GanrunConfiguration : public RunConfiguration
class GanrunConfiguration
{
public:
    GanrunConfiguration();
    virtual ~GanrunConfiguration();
    void InitExforCovData(int no_exp);
    void GenerateGridDensity(std::ostringstream & stream);

    void GenrateCovfInput(std::ostringstream & stream);
    void genLabel();

    void GenerateReactions(std::ostringstream & stream);
    void GenerateExforCovData(std::ostringstream & stream);
    void generate_input();

    std::string label = "Garnun";
    // Remove the literal later, it is here for faster debugging
    std::string pendfFileName = "C:\\Users\\Jan Malec\\Documents\\Projects nucl\\Gandrgui\\test_files\\92235.pendf";
    std::string exforFileName;
    std::string covFileName;
    std::string zottDir;
	std::string diceFIleName = "dice.dat";
	std::string comment;

    //CARD 1 imode 10 11 12 13 14
    imodes imode = CLASSIC;
    int inopt = 0;
    int ipick = 1; //1 from local set, -1 working set full copy of working, 0 re-use,
    idefs idef = FROM_EXFOR;
    int iprint = 1; // we don't need this
    int iredo = 0;
    int iloop = 0;
    double xneg = -std::pow(10, 6);
    //CARD 2 imode 13
    int nmat = 0;
    int nlib = 0;
    int izott = 1; // 0 for testing, 1 to execute zottvl
    //CARD 3 imode 13
    int nampr = 0;
    //CARD 4 imode 14
    int mt=1;
    int mf=0;
    int iza=0;
    int proj=0;
    int tmeta=0;
    int pmeta=0;
    //CARD 5 imode 11 12 - plotting, not interested
    //CARD 6 imode 13, ignore
    int matnam=1; //5-character material name
    int mtplot=0; // Identifier for plotting
    //CARD 7 imode 12
    int igrid = 0; // 0 is re-use, negative to define, 1-4 grids
    int imoc = 0; //default, built in, covariance. 1 for modcov
    int ipend = 0; //0 read from pendf folder, 1 use custom in working dir
    int inmt5 = 0;//defaultnoption for inelastic particle emmission
    int iltest = 1; // nonlinear test option, 1 for linear n for n-th pow

    // extra for card 7
    int thin = 0; // thining value to make input nicer

    std::array<int, 74> iflex; //GRID holder
    //CARD 8 imode 12
    std::vector<exfor_cov_data> ecov_data;
    //int *grid; // User supplied grid, allocated if necessary
    //CARD 9 imode 11 12 - if ipick > 0 and ic5 = 0
    int mte = 0;
    int mfe = 3; //Default, cs
    int idx1 = 0; //Default, ignore in search
    int idx2 = 0;
    //CARD 9 imode 11 12 - if ipick > 0 and ic5 = 1
    int target_iza;
    int tproj;
    //tmeta, pmeta ruse from card 4
    //CARD 10 imode 14
    //CARD 11 imode 14
    //CARD 12
    std::vector<int> xexpra; //which reactions contribute to measured one (or total?)
    //CARD 13 imode 11 12 13
    std::vector<int> listex; //when ipick is positive, this is used to select experiments
    //CARD 14 mode 10 11
    std::string expdat; // experimental data file name in ENDF 6 format
    //ipend-reused from card 7
    //CARD 15 imode 10 11
    int nexp;
    bool zott;
    //CARD 16 - 21 imode 10 11
    std::vector<standards_experiment> exp_data;

    //reaction reactions[NRMAX];
    std::array<reaction, NRMAX> reactions;

    // exfor reaction definition
    std::array<bool, NRMAX> xexfor;

    //CARD 21 imode 12 - plotting, not needed
    //CARD 37
    std::vector<additional_cov_data> covf_data; // Card 25
    std::vector<int> thinning; // data thinning to be merged with listex

    std::string input_string;
    exfor_cov_data GetCovData(int i);
    bool custom_reactions; // not in output
    std::string threshName = "92235"; // not in output

	//CARD 38, imode 15
	int nreac = 0;
	int itype = 1;


	std::vector<intReac> intReacs; // Cards 39-42
	sensData sData; // Actual data
	
	//CARD 43, imode 15
	double bref = 1.0;

	std::vector<int> checkedInts;

	//CARD 43, imode 15
	double bmeas = 0.01; // measured integral datum, provided as the
						// difference between the measurementand the
						// reference value, bref, given on card 38.

	//CARD 44, imode 15
	double std = 0.002; //Standard deviation from bmeas 


    template <class Archive>
    void serialize(Archive &ar) {
        ar(label, pendfFileName, exforFileName, covFileName, zottDir,
            imode, inopt, ipick, idef, iprint, iredo, iloop, xneg,
            nmat, nlib, izott,
            nampr,
            mt, mf, iza, proj, tmeta, pmeta,
            matnam, mtplot,
            igrid, imoc, ipend, inmt5, iltest,
            thin, iflex,
            ecov_data,
            mte, mfe, idx1, idx2,
            target_iza, tproj,
            xexpra,
            listex,
            expdat,
            nexp, zott,
            exp_data,
            reactions, custom_reactions,
            xexfor,
            covf_data,
            threshName,
			nreac, itype,
			intReacs, sData,
			bref, bmeas, std,
			checkedInts, comment
            );
    }

private:
    int a; // What is this
    
};

#endif // GANRUNCONFIGURATION_H
