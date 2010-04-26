#include "stdafx.h"
#include "ControlTestForm.h"

using namespace DoubleAgent;
using namespace DoubleAgent::Control;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 
	Application::Run(gcnew ControlTestForm());
	return 0;
}
