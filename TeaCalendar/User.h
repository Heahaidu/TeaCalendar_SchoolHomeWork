#pragma once

#include <yaml-cpp/yaml.h>
#include <cliext/vector>
#include <msclr\marshal_cppstd.h>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace msclr::interop;

public ref class User {
public: 

	String^ userID = "";
	String^ path = "";

	bool userCheck(String^ userID, String^ Password) {
		try {
			YAML::Node account = YAML::LoadFile("config/account.yml");
			if (marshal_as<std::string>(Password) == account[marshal_as<std::string>(userID)]["password"].as<std::string>()) {
				this->userID = userID;
				return true;
			} else return false;
		}
		catch (const std::exception&) {
			return false;
		}
	}

	bool keepLogin() {
		struct stat load;
		if (!stat("config/save.txt", &load)) {
			std::string a;
			std::ifstream loadsave("config/save.txt");
			std::getline(loadsave, a);

			String^ userID = marshal_as<String^>(a.substr(0, a.find(":")));
			String^ Password = marshal_as<String^>(a.substr(a.find(":") + 1));

			return userCheck(userID, Password);

		} else return false;
	}

	void save(String^ userID, String^ Password) {
		
		std::fstream savef; savef.open("config/save.txt", std::ios::out);
		savef << marshal_as<std::string>(userID + ":" + Password);
		savef.close();
	}

	void del() {
		remove("config/save.txt");
	}

	cliext::vector<String^> getUser(String^data) {
		String^ userID = this->userID;
		Console::WriteLine("User -> getUser(): " + this->userID);
		struct stat des;		
		cliext::vector<String^> list_class; 

		if (!stat(("config/calendar/" + marshal_as<std::string>(data) + "/" + marshal_as<std::string>(userID) + ".yml").c_str(), &des)) {
			String^ loc = this->path = "config/calendar/" + data + "/" + userID + ".yml";
			try {
				YAML::Node config = YAML::LoadFile(marshal_as<std::string>(loc));
				for (auto i : config["Class"]) {
					String^ lis = marshal_as<String^>(i.as<std::string>());
					list_class.push_back(lis);
				}
			}
			catch (const std::exception&) {
			}
		} 
		return list_class;
	}

	cliext::vector<String^> getTime(String^ s) {
		cliext::vector<String^> time;
		String^ loc = path;
		try {
			YAML::Node config = YAML::LoadFile(marshal_as<std::string>(loc));
			std::string a = marshal_as<std::string>(s);
			for (auto i : config["Calendar"][a]) {
				String^ lis = marshal_as<String^>(i.as<std::string>());
				time.push_back(lis);
			}
		}
		catch (const std::exception&) {
		}
		return time;
	}

	cliext::vector<String^> duplicateTime(String^ data, String^ path) {
		cliext::vector<String^> listDuplicate;
		std::string time = msclr::interop::marshal_as<std::string>(data);

		struct stat des;
		if (!stat(msclr::interop::marshal_as<std::string>("config/calendar/" + path + "/" + this->userID + ".yml").c_str(), &des)) {
			try {
				YAML::Node personTime = YAML::LoadFile(msclr::interop::marshal_as<std::string>("config/calendar/" + path + "/" + this->userID + ".yml"));

				for (auto i : personTime["Class"]) {
					for (auto ii : personTime["Calendar"][i.as<std::string>()]) {
						if (checkTime(time, ii.as<std::string>())) {
							listDuplicate.push_back(msclr::interop::marshal_as<String^>(i.as<std::string>() + ":" + ii.as<std::string>()));
						}
					}
				}
			}
			catch (const std::exception&) {
			}
		}
		listDuplicate.push_back("0");
		return listDuplicate;
	}

	bool checkTime(std::string time, std::string times) {
		if (hour(time, false) < hour(times, true) || (hour(time, false) == hour(times, true) && minute(time, false) <= minute(times, true))) {
			return false;
		}
		else if (hour(time, true) > hour(times, false) || (hour(time, true) == hour(times, false) && minute(time, true) >= minute(times, false))) {
			return false;
		}
		return true;
	}

	void saveCalendar(String^ time, String^ classRoom, String^ date) {
		String^ userID = this->userID;
		struct stat des;
		if (stat(("config/calendar/" + marshal_as<std::string>(date)).c_str(), &des)) {
			_mkdir(("config/calendar/" + marshal_as<std::string>(date)).c_str());
		}
		YAML::Node userConfig, globalConfig;
		if (!stat(("config/calendar/" + marshal_as<std::string>(date) + "/" + marshal_as<std::string>(userID) + ".yml").c_str(), &des)) {
			userConfig = YAML::LoadFile("config/calendar/" + marshal_as<std::string>(date) + "/" + marshal_as<std::string>(userID) + ".yml");
			bool b = true;
			for (auto i : userConfig["Class"]) {
				if (marshal_as<std::string>(classRoom) == i.as<std::string>()) {
					b = false;
					break;
				}
			}
			if (b) userConfig["Class"].push_back(marshal_as<std::string>(classRoom));
		} else {
			userConfig["Class"].push_back(marshal_as<std::string>(classRoom));
		}
		userConfig["Calendar"][marshal_as<std::string>(classRoom)].push_back(marshal_as<std::string>(time));
		std::ofstream fout("config/calendar/" + marshal_as<std::string>(date) + "/" + marshal_as<std::string>(userID) + ".yml");
		fout << userConfig;

		if (!stat(("config/calendar/" + marshal_as<std::string>(date) + "/global.yml").c_str(), &des)) {
			globalConfig = YAML::LoadFile("config/calendar/" + marshal_as<std::string>(date) + "/global.yml");
		}
		globalConfig[marshal_as<std::string>(classRoom)].push_back(marshal_as<std::string>(time) + ":" + marshal_as<std::string>(userID) + ":");
		std::ofstream g_fout("config/calendar/" + marshal_as<std::string>(date) + "/global.yml");
		g_fout << globalConfig;
	}

	void deleteCalendar(String^ time, String^ path) {
		YAML::Node config = YAML::LoadFile(marshal_as<std::string>("config/calendar/" + path + "/" + userID + ".yml"));
		cliext::vector<String^> list = getUser(path);
		bool b = false;
		for (int i = 0; i < list.size(); i++) {
			std::vector<std::string> save;
			for (auto a: config["Calendar"][marshal_as<std::string>(list[i])]) {
				if (marshal_as<String^>(a.as<std::string>()) != time) {
					save.push_back(a.as<std::string>());
				} else {
					b = true;
				}
			}
			
			if (b) {
				std::ofstream fout(marshal_as<std::string>("config/calendar/" + path + "/" + userID + ".yml"));
				config["Calendar"][marshal_as<std::string>(list[i])] = save;

				if (save.size() == 0) {
					for (int ii = 0; ii < list.size(); ii++) {
						if (list[i] != list[ii]) {
							Console::WriteLine(list[ii]);
							save.push_back(marshal_as<std::string>(list[ii]));
						}
					}
					config["Class"] = save;
				}
				fout << config;

				YAML::Node global_config = YAML::LoadFile(marshal_as<std::string>("config/calendar/" + path + "/global.yml"));
				YAML::Node global_save;
				for (auto a : global_config[marshal_as<std::string>(list[i])]) {
					if (marshal_as<String^>(a.as<std::string>().substr(0 , 11)) != time) {
						global_save.push_back(a.as<std::string>());
					}
				}
				
				global_config[marshal_as<std::string>(list[i])] = global_save;
				std::ofstream glo_fout(marshal_as<std::string>("config/calendar/" + path + "/global.yml"));
				glo_fout << global_config;

				//PersonalPanel^ person = gcnew PersonalPanel;
				//personalPanel->PersonalGet(path);

				break;
			} 
		}
	}

	cliext::vector<String^> freeTime(String^ path) {
		cliext::vector<String^> re;
		try {
			YAML::Node config = YAML::LoadFile(marshal_as<std::string>("config/calendar/" + path + "/" + userID + ".yml"));
			for (auto i : config["Class"]) {
				for (auto ii : config["Calendar"][i.as<std::string>()]) {
					re.push_back(marshal_as<String^>(ii.as<std::string>()));
				}
			}
		}
		catch (const std::exception&) {

		}
		String^ time_;
		for (int i = 0; i < re.size(); i++) {
			for (int ii = i + 1; ii < re.size(); ii++) {
				if (hour(convert(re[i]), true) > hour(convert(re[ii]), true)
					|| (hour(convert(re[i]), true) == hour(convert(re[ii]), true) && minute(convert(re[i]), true) > minute(convert(re[ii]), true))) {
					time_ = re[i];
					re[i] = re[ii];
					re[ii] = time_;
				}
			}
		}
		re.push_back("24h00-24h00");
		String^ start_ = "-07h30";
		cliext::vector<String^> result;
		for (int i = 0; i < re.size(); i++) {
			if (hour(convert(start_), false) < hour(convert(re[i]), true)
				|| (hour(convert(start_), false) == hour(convert(re[i]), true) && minute(convert(start_), false) < minute(convert(re[i]), true))) {
				std::string before = convert(start_).substr(convert(start_).find("-") + 1, 5);
				std::string after = convert(re[i]).substr(0, 5);
				result.push_back((marshal_as<String^>(before + "-" + after + ": " + freetimeisMinute(before, after) + "m")));
				start_ = re[i];
			}
			else start_ = re[i];
		}
		return result;
	}

	std::string convert(String^ data) {
		return marshal_as<std::string>(data);
	}
	std::string freetimeisMinute(std::string before, std::string after) {
		int minute_ = 0;
		int b = hour(before + "-", true), a = hour(after + "-", true);
		if (b < a) {
			minute_ += (a - b) * 60 + (60 - minute(before + "-", true)) + (minute(after + "-", true) - 60);
		}
		else {
			return std::to_string(minute(after + "-", true) - minute(before + "-", true));
		}
		return std::to_string(minute_);
	}

	int hour(std::string s, bool b) {
		int re = 0;
		if (b) {
			for (int i = 0; i < s.find("h"); i++) {
				re = re * 10 + (s.at(i) - '0');
			}
			return re;
		}
		else {
			for (int i = s.find("-") + 1; i < s.find("h", s.find("-") + 1); i++) {
				re = re * 10 + (s.at(i) - '0');
			}
			return re;
		}
	}
	int minute(std::string s, bool b) {
		int re = 0;
		if (b) {
			for (int i = s.find("h") + 1; i < s.find("-"); i++) {
				re = re * 10 + (s.at(i) - '0');
			}
			return re;
		}
		else {
			for (int i = s.find("h", s.find("-") + 1) + 1; i < s.find("h", s.find("-") + 1) + 3; i++) {
				re = re * 10 + (s.at(i) - '0');
			}
			return re;
		}
	}

};