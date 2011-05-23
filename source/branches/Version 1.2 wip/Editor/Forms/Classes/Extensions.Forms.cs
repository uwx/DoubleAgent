/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
using System;
using System.ComponentModel;
using System.Drawing;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Global
{
	public static partial class StringExtensions
	{
		/// <summary>
		/// Updates the mnemonic prefix indicator ('&amp;') in a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix updated for the current application build.</returns>
		static public String FixMenuPrefix (this String pString)
		{
			return pString;
		}

		/// <summary>
		/// Strips the mnemonic prefix indicator ('&amp;') from a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix removed.</returns>
		static public String NoMenuPrefix (this String pString)
		{
			return pString.Replace ("&", "");
		}
	}
}
