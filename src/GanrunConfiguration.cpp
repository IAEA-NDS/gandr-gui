/***************************************************************
 * Name:      GanrunConfiguration.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for the GunRunConfiguration class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "GanrunConfiguration.h"
#include "GlobalConfiguration.h"
#include <iomanip>
#include <regex>
#include <assert.h>
#include "gen_utils.h"
#include "wx/wx.h" //remove this


GanrunConfiguration::GanrunConfiguration() {
    iflex.fill(1);
}

GanrunConfiguration::~GanrunConfiguration() {
    //dtor
}

/*
Init vectors needed to store configuration for experiments
*/
void GanrunConfiguration::InitExforCovData(int no_exp) {
    ecov_data.clear();
    thinning.clear();
    exfor_cov_data empty_data;

    for (int i = 0; i < no_exp; i++) {
        empty_data.expno = i;
        ecov_data.push_back(empty_data);
        thinning.push_back(1);
    }
}

void GanrunConfiguration::GenerateGridDensity(std::ostringstream &stream) {
	if (thin == 0) {
		for (int i = 0; i < iflex.size(); i++) {
			stream << iflex.at(i) << " ";
			if ((i+1) % 10 == 0 && i > 2) {
				stream << std::endl;
			}
		}
	}
	else {
		stream << thin;
	}
    stream << "/ grid density, card 4" << std::endl;
}

void GanrunConfiguration::genLabel() {
    std::string runDesc = "";
    switch (imode) {
    case STANDARDS:
        runDesc = "Standards, ";
        if (exp_data.size() > 1) {
            runDesc += std::to_string(exp_data.size());
            runDesc += " experiments.";
        }
        else if (exp_data.size()){
            runDesc += " mat = " + std::to_string(exp_data.back().mats);
        }
        break;
    case MIXTURE :
        runDesc = "Mixture, ";
        runDesc += "reaction = " + std::to_string(mte);
        if (exp_data.size() > 1) {
            runDesc += std::to_string(exp_data.size());
            runDesc += " experiments.";
        }
        else if (exp_data.size()) {
            runDesc += " mat = " + std::to_string(exp_data.back().mats);
        }
        break;
    case CLASSIC:
        runDesc = "Classic, ";
        runDesc += " mat = " + std::to_string(matnam);
        runDesc += ", reaction = " + std::to_string(std::abs(mte));
    
	break;
	case INTEGRAL:
		runDesc = "Integral, ";
		runDesc += " " + std::to_string(nreac) + " reactions";
	}
    label = runDesc;
}

void GanrunConfiguration::GenerateReactions(std::ostringstream &stream) {
    for (auto r : reactions) {
        stream << std::right << std::setw(5) << r.gan_no;
        stream << std::right << std::setw(5) << r.mte;
        stream << "  " << r.label << std::endl;
        if (r.composition.size() > 1) {
            for (int i = 0; i < r.composition.size(); i++) {
				stream << std::setw(4) << r.composition.at(i);
                if (i % 19 == 0 && i > 0 && r.composition.size() % 19 != 0) {
                    stream << std::endl;
                }
            }
            stream << std::endl;
        }
    }
}

void GanrunConfiguration::GenerateExforCovData(std::ostringstream & stream) {
    int edited_no = 0;
    for (int i = 0; i < ecov_data.size(); i++) {
		if (ecov_data.at(i).edited)
			edited_no = i;
    }

    if (edited_no >= ecov_data.size())
        return;
    ecov_data.at(edited_no).iglobl = 1;
    for (int i = 0; i <= edited_no; i++) {
        auto d = ecov_data.at(i);
        stream << d.inca << " " << d.covmin << " " << d.irelco <<
            " " << d.isys << " " << d.iglobl;
        if (d.isys == 1) {
			stream << std::endl;
            //grs << vector_to_short_string(d.xsys);
            for (auto x : d.xsys)
                stream << x << " ";
        }
		stream << "/ card 8 and 9, inca covmin irelco isys iglobl, xsys" << std::endl; //card 8, exfor cov data TODO
    }
}

// This will be deprecated, as I only want to refer to exfor cov data by expno/inex
exfor_cov_data GanrunConfiguration::GetCovData(int i) {
    std::vector<exfor_cov_data>::iterator it = std::find_if(ecov_data.begin(), ecov_data.end(),
        [&i](const exfor_cov_data& obj) {return obj.expno == i; });
    if (it != ecov_data.end()) {
        return *it;
    }
    else {
        exfor_cov_data data;
        data.expno = 0;
        return data;
    }
}

void GanrunConfiguration::generate_input() {
    std::ostringstream grs;
	Singleton* s = Singleton::getInstance();

    if (comment != ""){
        grs << "# " << std::regex_replace(comment, std::regex("\n"), "\n# ") << std::endl;
    }

    if (iloop)
        grs << "# Loop over the materials with iloop=1" << std::endl;

    if (imoc && label == "Setup run") {
        //Re-use custom COV
        grs << "cp " << covFileName << " " << s->zottDir << "/modcov/" << matnam << std::endl;
    }
    if (ipend && label == "Setup run") {
        //Re-use custom pendf
        grs << "cp " << pendfFileName << " ./" << matnam << ".pendf" << std::endl;
		grs << "cp " << matnam << ".pendf " << s->zottDir << "/pendf/" << matnam << std::endl;
    }

    if (mte < 0 && (imode != STANDARDS)) {
        //Copy exfor file
        grs << "cp " << exforFileName << " ./" << matnam << ".exfor" << std::endl;
    }

	if (imode == INTEGRAL) {
		grs << "cp " << "dicer_" + std::to_string(inopt) + ".dat " << " " << "dicer.dat" << std::endl;
	}

    //grs << "ganrun" << std::endl;
    if (inopt == 0)
        grs << "echo yes > permit" << std::endl;
    grs << "gaprep.x << EOF" << std::endl;
	if (mtplot == 0){
		mtplot = mt;
	}
    switch (imode)
    {
    case STANDARDS:
        grs << imode << " " << inopt << " " << ipick << " " << idef << " ";
        grs << iprint << " " << iredo << " " << iloop << " " << xneg;
        grs << "/ Card 1, imode inopt ipick idef iprint iredo iloop xneg" << std::endl; // end card 1

		grs << expdat << std::endl; // << "/ Card 14, expdat" << std::endl; // end card 14

        nexp = exp_data.size();
        grs << nexp << " " << izott;
		grs << std::endl;
		//grs << " / Card 15, nexp izott" << std::endl; // end card 15

        for (auto s : exp_data) {
            s.nlib = s.sens_data.size();
            s.nexc = s.cov_data.size();
            grs << " " << s.mats << " " << s.mts << " ";
            grs << s.nps << " " << s.nexc << " ";
            grs << " " << s.nlib << " ";
            grs << s.ismg;
            grs << "/ Card 16, mats mts nps nexc nlib ismg" << std::endl; // end card 16

            grs << "  " << s.nampl << " " << s.mtpl;
			grs <<  std::endl;
			//grs << "/ Card 17, nampl matpl" << std::endl; // end card 17

            for (auto c : s.cov_data) {
                grs << "  " << c.range.first << " " << c.range.second << " ";
                grs << "  " << c.covfc << " " << c.irelfc << " ";
                grs << "/ Card 18, range1 range2 covfc irelfc" << std::endl; // end card 18
            }
            
            s.nps = s.mgrid.size();

            if (s.ismg) {
                for (auto m : s.mgrid) {
                    grs << m << " ";
                }
                grs << "/ Card 19, grs" << std::endl; // end card 19
            }
            
            for (auto sens : s.sens_data) {
                grs << sens.nam << "\n";
                grs << vector_to_short_string(sens.xexary);
                grs << "/ Card 20 mat num and Card 21, xexary" << std::endl; // end card 21
            }
            }

        if (inopt == 0) {
            //grs << igrid << " " << imoc << " " << inmt5 << " ";
            //grs << iltest;
            //grs << "/ Card 22, igrid imoc inmt5 iltest" << std::endl; // end card 22
			grs << "/\n /\n";
        }

		grs << std::endl;
        break;
    case MIXTURE:
        grs << imode << " " << inopt << " " << ipick << " " << idef << " ";
        grs << iprint << " " << iredo << " " << iloop << " " << xneg;
        grs << "/ imode inopt ipick idef iprint iredo iloop xnerg, card 1" << std::endl; // end card 1
		
		grs << matnam << " ";
		if (!iloop)
			grs << mtplot;
        grs << "/ matnam mtplot, card 2" << std::endl; // end card 2

        if (ipick != 0) {
            grs << mte << " " << mfe << " " << idx1 << " " << idx2;
            grs << "/ mte mfe idx1 idx2, card 5" << std::endl;
        } // end card 5

        if (ipick > 0) {
            grs << vector_to_short_string(listex);
            grs << "/ card 6, listex" << std::endl;
        }
        
        grs << ipend << " / card 7, ipend" << std::endl;
        // end card 7

        if (ecov_data.size()) {
            GenerateExforCovData(grs);
            
        }

        grs << expdat << "/ Card 14, expdat" << std::endl; // end card 14

        nexp = exp_data.size();
        grs << nexp << " " << izott;
		grs << std::endl;
        //grs << "/ Card 15, nexp izott" << std::endl; // end card 15

        for (auto s : exp_data) {
            s.nlib = s.sens_data.size();
            s.nexc = s.cov_data.size();
            grs << s.mats << " " << s.mts << " ";
            grs << s.nps << " " << s.nexc << " ";
            grs << " " << s.nlib << " ";
            grs << s.ismg;
            grs << "/ Card 16, mats mts nps nexc nlib ismg" << std::endl; // end card 16

            grs << s.nampl << " " << s.mtpl;
			grs << std::endl;
			//grs << "/ Card 17, nampl matpl" << std::endl; // end card 17

            for (auto c : s.cov_data) {
                grs << c.range.first << " " << c.range.second << " ";
                grs << c.covfc << " " << c.irelfc << " ";
                grs << "/ Card 18, range1 range2 covfc irelfc" << std::endl; // end card 18
            }

            s.nps = s.mgrid.size();

            if (s.ismg) {
                for (auto m : s.mgrid) {
                    grs << m << " ";
                }
                grs << "/ Card 19, grs" << std::endl; // end card 19
            }

            for (auto sens : s.sens_data) {
                grs << sens.nam << "\n";
                grs << vector_to_short_string(sens.xexary);
                grs << "/ Card 20 mat num and Card 21, xexary" << std::endl; // end card 21
            }
        }

        if (inopt == 0) {
            grs << igrid << " " << imoc << " " << inmt5 << " ";
            grs << iltest;
            grs << "/ Card 22, igrid imoc inmt5 iltest" << std::endl; // end card 22
        }

        break;
    case CLASSIC:
        grs << imode << " " << inopt << " " << ipick << " " << idef << " ";
        grs << iprint << " " << iredo << " " << iloop << " " << xneg;
        grs << "/ imode inopt ipick idef iprint iredo iloop xnerg, card 1" << std::endl; // end card 1

		grs << matnam << " ";
		if (!iloop)
			grs << mtplot;
        grs << "/ matnam mtplot, card 2" << std::endl; // end card 2

        if (inopt == 0 || iloop == 1) {
            grs << igrid << " " << imoc << " " << ipend << " " << inmt5 << " ";
            grs << iltest;
            grs << "/ igrid imoc ipend inmt5 iltest, card 3" << std::endl; // end card 3 
        }

        if (igrid == 4 || igrid == -4) {
            GenerateGridDensity(grs);
        } //end card 4

        if (ipick != 0) {
            grs << mte << " " << mfe << " " << idx1 << " " << idx2;
            grs << "/ mte mfe idx1 idx2, card 5" << std::endl;
        } // end card 5

        if (ipick > 0) {
            grs << vector_to_short_string(listex);
            grs << "/ card 6, listex" << std::endl;
        }

        if (ecov_data.size()) {
            GenerateExforCovData(grs);
            grs << "/ card 8 and 9, inca covmin irelco isys iglobl, xsys" << std::endl; //card 8, exfor cov data TODO
        }
        //grs << "/" << std::endl; //card 9, xsys for card 8 exfor cov

        if (igrid < 0) {
            GenerateReactions(grs);
        } //cards 10 and 11

        if (idef == 1) {
            for (auto e : xexfor) {
                grs << e << " ";
            }
            grs << "/ xexfor" << std::endl; //end card 12
        }

        if (iprint == 2) {
            grs << "/ iprint, not used" << std::endl; // 13 Skip, we have our own print
        }

        for (auto c : covf_data) {
            grs << c.range.first << " " << c.range.second << " "
                << c.covfc << " " << c.irelfc << "/" << std::endl;
        }
        // Card 25, nb1, nb2, covfc, irelfc
        //GenrateCovfInput(grs);
        break;
	case INTEGRAL:
		grs << imode << " " << inopt << " " << ipick << " " << idef << " ";
		grs << iprint << " " << iredo << " " << iloop << " " << xneg;
		grs << "/ imode inpot ipick idef iprint iredo iloop xnerg, card 1" << std::endl; // end card 1

		if (inopt == 0) {
			grs << matnam << " " << matnam << "/ matnam mtplot \n";
			grs << igrid << " " << imoc << " " << ipend << "/ igrid imoc ipend \n";
		}

		// CARD 38 only itype=1 sensitivities supported at this point
		nreac = 0;
		for (size_t i = 0; i < checkedInts.size(); i++) {
			if (checkedInts.at(i) == 1)
				nreac++;
		}
		grs << nreac << " " << itype << "/ nreac itype, card 38" << std::endl;

		// CARDs 39-42
		size_t ind = 0;
		for (size_t i = 0; i < checkedInts.size(); i++) {
			if (checkedInts.at(i)) {
			auto s = intReacs.at(i);
			grs  << s.za << " " << s.rnam << std::endl;
			grs << "  " << s.matna << " " << s.mfd << " " << s.mtd << " "
				<< s.temp << " " << s.sigz << "/ matd, mfd, mtd, temp, sigz\n";
			grs << "  " << s.iusegp << " " << s.igrtop << " " <<
				s.igrid << " / iusegp igrtop igrid" << std::endl;
			grs << "  " << s.iwt << " " << s.np << "/ iwt np" << std::endl;
			}
		}
		
		// CARD 43, imode 15
		grs << bmeas << " / bmeas, card 43" << std::endl;

		//CARD 44, imode 15
		grs << std << " / std, card 44" << std::endl;
		
    }

    grs << "EOF\n" << std::endl;

	if (imode == STANDARDS) {
		grs << "rm sepost.in && touch sepost.in" << std::endl;
		grs << "touch mgbse.dat" << std::endl;
	}

	if (iloop == 0) {
		grs << "chmod u+x ./ganrun" << std::endl;
		grs << "./ganrun" << std::endl << std::endl;
	}

    input_string = grs.str();
}
