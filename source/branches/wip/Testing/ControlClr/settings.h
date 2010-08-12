#pragma once

using namespace System;
using namespace System::Configuration;

/////////////////////////////////////////////////////////////////////////////
public ref class settings :  public ApplicationSettingsBase
{
public:
	initonly static settings^ Default = gcnew settings;

	[UserScopedSetting()]
	property System::Drawing::Point Location
	{
		System::Drawing::Point get ()
		{
			try
			{
				return (System::Drawing::Point) this["Location"];
			}
			catch (...)
			{}
			return System::Drawing::Point (100,100);
		}
		Void set (System::Drawing::Point value)
		{
			this["Location"] = value;
		}
	}

	[UserScopedSetting()]
	property System::Boolean RaiseRequestErrors
	{
		System::Boolean get ()
		{
			try
			{
				return (System::Boolean) this["RaiseRequestErrors"];
			}
			catch (...)
			{}
			return true;
		}
		Void set (System::Boolean value)
		{
			this["RaiseRequestErrors"] = value;
		}
	}

	[UserScopedSetting()]
	property System::Int16 AutoConnect
	{
		System::Int16 get ()
		{
			try
			{
				return (System::Int16) this["AutoConnect"];
			}
			catch (...)
			{}
			return true;
		}
		Void set (System::Int16 value)
		{
			this["AutoConnect"] = value;
		}
	}

	[UserScopedSetting()]
	property System::Int16 MainPage
	{
		System::Int16 get ()
		{
			try
			{
				return (System::Int16) this["MainPage"];
			}
			catch (...)
			{}
			return true;
		}
		Void set (System::Int16 value)
		{
			this["MainPage"] = value;
		}
	}

	[UserScopedSetting()]
	property System::Int16 ControlPage
	{
		System::Int16 get ()
		{
			try
			{
				return (System::Int16) this["ControlPage"];
			}
			catch (...)
			{}
			return true;
		}
		Void set (System::Int16 value)
		{
			this["ControlPage"] = value;
		}
	}

	[UserScopedSetting()]
	property System::Int16 CharacterPage
	{
		System::Int16 get ()
		{
			try
			{
				return (System::Int16) this["CharacterPage"];
			}
			catch (...)
			{}
			return true;
		}
		Void set (System::Int16 value)
		{
			this["CharacterPage"] = value;
		}
	}

	[UserScopedSetting()]
	property System::String^ SpeakText
	{
		System::String^ get ()
		{
			try
			{
				return (System::String^) this["SpeakText"];
			}
			catch (...)
			{}
			return System::String::Empty;
		}
		Void set (System::String^ value)
		{
			this["SpeakText"] = value;
		}
	}

	[UserScopedSetting()]
	property System::String^ ThinkText
	{
		System::String^ get ()
		{
			try
			{
				return (System::String^) this["ThinkText"];
			}
			catch (...)
			{}
			return System::String::Empty;
		}
		Void set (System::String^ value)
		{
			this["ThinkText"] = value;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
