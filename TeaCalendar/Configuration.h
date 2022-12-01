#pragma once
#include <yaml-cpp/yaml.h>
#include <cliext/vector>
#include <cliext/map>
#include <msclr\marshal_cppstd.h>
#include <string>
#include <fstream>

using namespace System;
using namespace msclr::interop;

public ref class Config {

public:

	cliext::vector<String^> list_class;
	String^ path = "";

	cliext::vector<String^> getListClass(String^path) {
		this->path = path;
		try {
			YAML::Node config = YAML::LoadFile("config/config.yml");
			if (list_class.size() == 0)
				for (auto i : config["Class"]) {
					String^ lis = marshal_as<String^>(i.as<std::string>());
					list_class.push_back(lis);
				}
		}
		catch (const std::exception&) {
		}
		return list_class;
	}



	String^ getUserName(String^ id) {
		YAML::Node account = YAML::LoadFile("config/account.yml");
		return marshal_as<String^>(account[marshal_as<std::string>(id)]["name"].as<std::string>());
	}

	String^ getPosition(String^ id) {
		YAML::Node account = YAML::LoadFile("config/account.yml");
		return marshal_as<String^>(account[marshal_as<std::string>(id)]["position"].as<std::string>());
	}

	cliext::vector<String^> getTime(String^ s) {
		cliext::vector<String^> time;
		String^ loc = path;
		struct stat des;
		if (!stat(("config/calendar/" + msclr::interop::marshal_as<std::string>(loc) + "/global.yml").c_str(), &des))
			try {
				YAML::Node config = YAML::LoadFile("config/calendar/" + msclr::interop::marshal_as<std::string>(loc) + "/global.yml");
				std::string a = marshal_as<std::string>(s);
				for (auto i : config[a]) {
					String^ lis = marshal_as<String^>(i.as<std::string>());
					time.push_back(lis);
				}
			}
			catch (const std::exception&) {
			}
		return time;
	}

	cliext::vector<String^> recommendClass(String^ data, String^ date) {
		cliext::vector<String^> classRecommend;
		std::string time = msclr::interop::marshal_as<std::string>(data);
		struct stat des;
		if (!stat(("config/calendar/" + msclr::interop::marshal_as<std::string>(date) + "/global.yml").c_str(), &des)) {
				try {
				YAML::Node global_Time = YAML::LoadFile("config/calendar/" + msclr::interop::marshal_as<std::string>(date) + "/global.yml");

				for (int i = 0; i < list_class.size(); i++) {
					bool b = true;
					for (auto ii : global_Time[msclr::interop::marshal_as<std::string>(list_class[i])]) {
						if (checkTime(time, ii.as<std::string>())) {
							b = false;
							break;
						}
					}
					if (b) { classRecommend.push_back(list_class[i]); }
				}
			}
			catch (const std::exception&) {
			}
			return classRecommend;
		}
		return list_class;
	}

	bool checkTime(std::string time, std::string times) {
		if (hour(time, false) < hour(times, true) || (hour(time, false) == hour(times, true) && minute(time, false) <= minute(times, true)))  {
			return false;
		} else if (hour(time, true) > hour(times, false) || (hour(time, true) == hour(times, false) && minute(time, true) >= minute(times, false))) {
			return false;
		} 
		return true;
	}


	int hour(std::string s, bool b) {
		if (b) return (s.at(0) - '0') * 10 + s.at(1) - '0';
		return (s.at(6) - '0') * 10 + s.at(7) - '0';
	}

	int minute(std::string s, bool b) {
		if (b) return (s.at(3) - '0') * 10 + s.at(4) - '0';
		return (s.at(9) - '0') * 10 + s.at(10) - '0';
	}

};