#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_


#include <wx/frame.h>




class MainWindow final : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainWindow();

	enum { ID_LOADROM };

private:
	void OnExit(wxCommandEvent& ev);
	void OnLoadRom(wxCommandEvent& ev);

	wxDECLARE_EVENT_TABLE();
};





#endif