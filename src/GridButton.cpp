/***************************************************************
 * Name:      GridButton.cpp
 * Desc.:     This code is part of the software GANDR GUI,
 *            developed to provide a user friendly interface to
 *            some of the functions of GANDR software.
 * Purpose:   Code for the GridButton class.
 * Author:    Jan (jan.malec@gmail.com)
 * Created:   2018-09-12
 * License:   GPL-3
 **************************************************************/

#include "GridButton.h"

BEGIN_EVENT_TABLE(gridButton, wxPanel)

EVT_MOTION(gridButton::mouseMoved)
EVT_LEFT_DOWN(gridButton::mouseDown)
EVT_LEFT_UP(gridButton::mouseReleased)
EVT_RIGHT_DOWN(gridButton::rightClick)
EVT_LEAVE_WINDOW(gridButton::mouseLeftWindow)
EVT_KEY_DOWN(gridButton::keyPressed)
EVT_KEY_UP(gridButton::keyReleased)
EVT_MOUSEWHEEL(gridButton::mouseWheelMoved)

// catch paint events
EVT_PAINT(gridButton::paintEvent)

END_EVENT_TABLE()

wxDEFINE_EVENT(GRID_BUTTON_EVENT, wxCommandEvent);
wxDEFINE_EVENT(GRID_BUTTON_RCLICK_EVENT, wxCommandEvent);


gridButton::gridButton(wxWindow* parent, wxString text, wxPoint position, wxWindowID id) :
    wxWindow(parent, id, position, wxSize(buttonWidth * 2.25, buttonHeight * 2.5)) {
    //SetMinSize(wxSize(buttonWidth, buttonHeight));
    this->text = text;
    pressedDown = false;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */


void gridButton::paintEvent(wxPaintEvent & evt) {
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void gridButton::paintNow() {
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void gridButton::render(wxDC&  dc) {
    if (pressedDown)
        dc.SetBrush(*wxRED_BRUSH);
    else
        if (!zero)
            dc.SetBrush(*wxBLACK_BRUSH);
        else
            dc.SetBrush(*wxWHITE_BRUSH);

    dc.DrawCircle(wxPoint(4, 5), 4);
    dc.SetBrush(*wxBLACK_BRUSH);

    // Commented out for now due to some problems with overlapping
    /*
    double linelen = 6;
    if (zero && false) {
        dc.DrawLine(wxPoint(-linelen, -linelen), wxPoint(linelen, linelen));
        dc.DrawLine(wxPoint(linelen, linelen), wxPoint(-linelen, -linelen));
    }
    */
    //dc.DrawText(text, 20, 15);
}

void gridButton::mouseDown(wxMouseEvent& event) {
    pressedDown = true;
    paintNow();
}
void gridButton::mouseReleased(wxMouseEvent& event) {
    pressedDown = false;
    paintNow();
    wxCommandEvent evt(GRID_BUTTON_EVENT, GetId());
    evt.SetEventObject(this);
    ProcessWindowEvent(evt);
    //wxMessageBox(wxT("You pressed a custom button"));
}
void gridButton::mouseLeftWindow(wxMouseEvent& event) {
    if (pressedDown)
    {
        pressedDown = false;
        paintNow();
    }
}

// currently unused events
void gridButton::mouseMoved(wxMouseEvent& event) {}
void gridButton::mouseWheelMoved(wxMouseEvent& event) {}
void gridButton::rightClick(wxMouseEvent& event) {
    wxCommandEvent evt(GRID_BUTTON_RCLICK_EVENT, GetId());
    evt.SetEventObject(this);
    ProcessWindowEvent(evt);
}
void gridButton::keyPressed(wxKeyEvent& event) {}
void gridButton::keyReleased(wxKeyEvent& event) {}

void gridButton::setZero(bool choice) {
    zero = choice;
}
