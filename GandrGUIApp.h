/***************************************************************
 * Name:      GandrguiApp.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define GandrguiApp class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef GANDRGUIAPP_H
#define GANDRGUIAPP_H

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/chartype.h> 

class GandrguiApp : public wxApp
{
public:
    virtual bool OnInit();

    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
private:
    bool silent_mode;
};

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
     { wxCMD_LINE_SWITCH, "h", "help","displays help on the command line parameters",
          wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_SWITCH, "t", "test", "test switch",
          wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_MANDATORY  },
     { wxCMD_LINE_SWITCH, "s", "silent","disables the GUI" },

     { wxCMD_LINE_NONE }
};

#endif // GANDRGUIAPP_H
