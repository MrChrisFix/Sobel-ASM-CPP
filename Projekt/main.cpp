#include "main.h"

using namespace System::Windows::Forms;

[System::STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ProjektJA::MyForm());
	return 0;
}