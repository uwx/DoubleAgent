/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

public ref class Win32Native
{
public:
	static property System::Boolean IsInputPending {System::Boolean get();}
	static property System::Boolean IsMouseInputPending {System::Boolean get();}
	static property System::Boolean IsMouseWheelPending {System::Boolean get();}

	static void PopThreadToolTips ();
	static void PopThreadToolTips (System::Boolean pNoFade);
	static void PopThreadToolTips (System::IntPtr pWindowHandle);
	static void PopThreadToolTips (System::IntPtr pWindowHandle, System::Boolean pNoFade);
};

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
