#pragma once
#include <yaml-cpp/yaml.h>
#include <msclr/marshal.h>
#include <fstream>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace msclr::interop;

public ref class UserAccount {
public:
	bool userCheck(String^ userID, String^ Password) {
		try {
			YAML::Node account = YAML::LoadFile("config/account.yml");
			if (marshal_as<std::string>(Password) == account[marshal_as<std::string>(userID)]["password"].as<std::string>()) return true; else return false;
		} catch (const std::exception&) {
			return false;
		}
	}

	void save(String^ userID, String^ Password) {

		std::fstream savef; savef.open("config/save.txt", std::ios::out);
		savef << marshal_as<std::string>(userID + ":" + Password);
		savef.close();
	}

	void del() {
		remove("config/save.txt");
	}
};