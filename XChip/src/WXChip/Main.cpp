/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Jared Bruni.


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <XChip/Utility/Memory.h>

#include <WXChip/Main.h>
#include <WXChip/MainWindow.h>




wxIMPLEMENT_APP(WXChip);


bool WXChip::OnInit()
{
	using xchip::utility::make_unique;

	try {
		auto mainwin = make_unique<MainWindow>("WXChip", wxPoint(50,50), wxSize(800, 600));
		mainwin->Show(true);
		mainwin->SetFocus();
		_mainwin = mainwin.release();

	}
	catch(std::exception& err) {
		std::cout << err.what() << std::endl;
		return false;
	}


	return true;
}



int WXChip::OnExit()
{
	return 0;
}
