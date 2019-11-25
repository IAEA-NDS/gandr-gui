/***************************************************************
 * Name:      GandrSettingsFrame.h
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Define GandrSettingsFrame class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#ifndef GANDRSETTINGSFRAME_H
#define GANDRSETTINGSFRAME_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class GandrSettingsFrame : public wxFrame
{
public:
    GandrSettingsFrame();
    virtual ~GandrSettingsFrame();

    wxPanel *parentPanel;
    void SetMainFrame(wxPanel *parentPanel) { parentPanel = parentPanel; }
    std::string zottDir = "./";

protected:

private:
};

#endif // GANDRSETTINGSFRAME_H
