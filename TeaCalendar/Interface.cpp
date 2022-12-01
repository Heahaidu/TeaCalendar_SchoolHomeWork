#pragma once

#include "Interface.h"
#include "PersonalPanel.h"
#include "Configuration.h"
#include "GlobalPanel.h"
#include "Date_Time.h"
#include "User.h"


using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]

int main(array<String^>^ args) {

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(true);
	TeaCalendar::Interface client;

	User^ user = client.user;
	PersonalPanel^ personalPanel = client.personalPanel;
	Config^ config = client.config;
	DateTimeReserve^ dateReserve = client.dateReserve;
	GlobalPanel^ gloPanel = client.gloPanel;

	Application::Run(% client);
	return 0;
}

