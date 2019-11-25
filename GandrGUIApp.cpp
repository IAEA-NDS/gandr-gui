/*************************************************************
 * Name:      GandrguiApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "wx/wxprec.h"
/*
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif
*/
#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WXUSINGDLL
#define WXUSINGDLL
#endif

#include "GandrguiApp.h"
#include "GandrguiMain.h"

IMPLEMENT_APP(GandrguiApp);

bool GandrguiApp::OnInit() {

    auto frame = new GandrguiFrame(0L, _("GANDR Graphical User Interface"));
    frame->Show();

    return true;
}


void GandrguiApp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars("-");
}

bool GandrguiApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    silent_mode = parser.Found("s");

    // to get at your unnamed parameters use
    wxArrayString files;
    for (int i = 0; i < parser.GetParamCount(); i++)
    {
        files.Add(parser.GetParam(i));
    }

    return true;
}
