#pragma once
#include <cliext/vector>
#include "User.h"
#include "Configuration.h"
#include "Date_Time.h"
#include <msclr\marshal_cppstd.h>
#include "DeleteDialog.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace msclr::interop;

public ref class PersonalPanel {
public:

	User^ user = gcnew User();
	String^ path;
	String^ userID;


	System::Windows::Forms::Panel^ personal_classroom_panel;

	PersonalPanel(System::Windows::Forms::Panel^ personal_classroom_panel, String^ userID) {
		this->personal_classroom_panel = personal_classroom_panel;
		user->userID = userID;
		this->userID = userID;
	}

	int _month, _day, _year, minute, hour;
	void runnableDateTime(int _month, int _day, int _year, int minute, int hour) {
		this->_month = _month;
		this->_day = _day;
		this->_year = _year;
		this->minute = minute;
		this->hour = hour;
	}

	int month, day, year, _minute, _hour;

	void PersonalGet(String^ path, int month, int day, int year) {
		this->month = month; this->day = day; this->year = year;
		Console::WriteLine("PersonalGet():" + user->userID);
		this->path = path;
		cliext::vector<String^> personal_listclass = user->getUser(path);

		double jamb_x = -1;

		int k = (personal_listclass.size() == 0) ? 1 : personal_listclass.size();
		int j = (k < 5) ? (1000 / k) : 200;

		if (personal_listclass.size() != 0)
			for (int i = 0; i < k; i++) {
				jamb_x += j;
				/*
				Jamb create
				*/

				System::Windows::Forms::Panel^ jamb = (gcnew System::Windows::Forms::Panel());;
				jamb->CheckForIllegalCrossThreadCalls = false;
				personal_classroom_panel->Controls->Add(jamb);
				jamb->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)), static_cast<System::Int32>(static_cast<System::Byte>(236)),
					static_cast<System::Int32>(static_cast<System::Byte>(240)));
				jamb->Location = System::Drawing::Point(jamb_x, 45);
				jamb->Margin = System::Windows::Forms::Padding(2);
				jamb->Name = L"jamb";
				jamb->Size = System::Drawing::Size(2, 595);
				jamb->TabIndex = 2;
				/*
				Class Name
				*/
				System::Windows::Forms::Label^ class_name = (gcnew System::Windows::Forms::Label());
				personal_classroom_panel->Controls->Add(class_name);
				class_name->CheckForIllegalCrossThreadCalls = false;
				class_name->Font = (gcnew System::Drawing::Font(L"Arial", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				class_name->Location = System::Drawing::Point(jamb_x - j, 15);
				class_name->Name = L"class_name";
				class_name->Size = System::Drawing::Size(j, 30);
				class_name->Text = (System::Convert::ToString(personal_listclass[i]));
				class_name->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

				/*Create buuton*/
				cliext::vector<String^> time = user->getTime(personal_listclass[i]);

				for (int i = 0; i < time.size(); i++) {
					System::Windows::Forms::Button^ test = (gcnew System::Windows::Forms::Button());
					personal_classroom_panel->Controls->Add(test);
					test->CheckForIllegalCrossThreadCalls = false;
					test->BackColor = System::Drawing::Color::White;
					test->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
					test->FlatAppearance->BorderColor = System::Drawing::Color::Gainsboro;
					test->FlatAppearance->MouseDownBackColor = System::Drawing::Color::White;
					test->FlatAppearance->MouseOverBackColor = System::Drawing::Color::WhiteSmoke;
					test->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
					test->Location = System::Drawing::Point(jamb_x - j + 5, formLocation(marshal_as<std::string>(time[i]), true));
					test->Text = (System::Convert::ToString(time[i]));
					test->Size = System::Drawing::Size(j - 10, formLocation(marshal_as<std::string>(time[i]), false) - formLocation(marshal_as<std::string>(time[i]), true));
					test->TabIndex = 0;
					test->TextAlign = System::Drawing::ContentAlignment::TopLeft;
					if ((year >= _year) && (month >= _month)) {

						if ((year == _year && month == _month && day == _day) && 
							(hour_void(marshal_as<std::string>(time[i]), true) > hour || (hour_void(marshal_as<std::string>(time[i]), true)  == hour && minute_void(marshal_as<std::string>(time[i]), true) > minute))) {
							test->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PersonalPanel::Delete_MouseDown);
						}
						if (day > _day) {
							test->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PersonalPanel::Delete_MouseDown);
						}
					} 
				}
			}

		int crossbar_y = 10;
		for (int i = 0; i < 18; i++) {
			crossbar_y += 35;
			System::Windows::Forms::Panel^ crossbar = (gcnew System::Windows::Forms::Panel());;
			personal_classroom_panel->Controls->Add(crossbar);

			crossbar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)), static_cast<System::Int32>(static_cast<System::Byte>(236)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)));
			crossbar->Location = System::Drawing::Point(0, System::Convert::ToInt32(crossbar_y));
			crossbar->Margin = System::Windows::Forms::Padding(2);
			crossbar->Name = L"crossbar";
			crossbar->Size = System::Drawing::Size(j * k, 2);
			crossbar->TabIndex = 2;
		}

	}

	//00h00-00h00
	double formLocation(std::string s, bool b) {
		int h, m;
		if (b) {
			h = (s.at(0) - '0') * 10 + s.at(1) - '0';
			m = (s.at(3) - '0') * 10 + s.at(4) - '0';
		} else {
			h = (s.at(6) - '0') * 10 + s.at(7) - '0';
			m = (s.at(9) - '0') * 10 + s.at(10) - '0';
		}
		switch (h) {
		case 7: 
			if (m == 30) return 45;
			return 45 + (m - 30) * 0.7;
		case 8: 
			if (m == 20) return 80;
			if (m < 20) return 66 + m * 0.7;
			return 80 + (m - 20) * (35.0 / 60.0);
		case 9:
			if (m == 20) return 115;
			if (m < 20) return 115 - (20-m) * (35.0 / 60.0); 
			return 115 + (m - 20) * (0.7);
		case 10:
			if (m == 10) return 150;
			if (m < 10) return 143 + m * 0.7;
			return 150 + (m - 10) * 0.7;
		case 11: 
			if (m == 0) return 185;
			return (185 + m * (35.0 / 60.0));
		case 12: 
			Console::WriteLine(m);
			if (m == 0) return 220;
			return 220 + m * (35.0 / 60.0);
		case 13: 
			if (m == 0) return 255;;
			if (m == 50) return 290;
			if (m < 50) return 225 + m * 0.7;
			return 290 + (m - 50) * (35.0 / 60.0);
		case 14:
			if (m == 50) return 325;
			if (m < 50) return 325 - (50 - m) * (35.0 / 60.0);
			return 325 + (m - 50) * (0.7);
		case 15:
			if (m == 40) return 360;
			if (m < 40) return 360 - (40 - m) * 0.7;
			return 360 + (m - 40) * 0.7;
		case 16:
			if (m == 30) return 395;
			if (m < 30) return 395 - (30 - m) * 0.7;
			return 395 + (m - 30) * (35.0 / 75.0);
		case 17:
			if (m == 45) return 430;
			if (m < 45) return 430 - (45 - m) * (35.0 / 75.0);
			return 430 + (m - 45) * 0.7;
		case 18:
			if (m == 35) return 465;
			if (m < 35) return 465 - (35 - m) * 0.7;
			return 465 + (m - 45) * 0.7;
		case 19:
			if (m == 25) return 500;
			if (m < 20) return 500 - (20 - m) * 0.7;
			return 500 + (m - 50) * 0.7;
		case 20:
			if (m == 15) return 535;
			if (m < 15) return 535 - (15 - m) * 0.7;
			return 535 + (m - 15) * (35.0 / 55.0);
		case 21:
			if (m == 10) return 570;
			if (m < 10) return 570 - (10 - m) * (35.0 / 55.0);
			return 570 + (m - 10) * (35.0 / 50.0);
		case 22:
			if (m == 0) return 605;
			return 605 + m;
		case 23:
			if (m == 0) return 665;
			return 665 + m;
		case 24:
			return 725;
		} 
	}

	void Delete_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Right) {
			if (e->Clicks == 2) {
				Console::WriteLine(sender->ToString()->Substring(35));
				Console::WriteLine(this->path);
				TeaCalendar::DeleteDialog^ log = gcnew TeaCalendar::DeleteDialog(sender->ToString()->Substring(35), this->path, this->personal_classroom_panel, this->userID);
				log->ShowDialog();
				if (log->getPerm()) {
					PersonalGet(this->path, this->month, this->day, this->year);
				}
			}
		}
	}

	int hour_void(std::string s, bool b) {
		if (b) return (s.at(0) - '0') * 10 + s.at(1) - '0';
		return (s.at(6) - '0') * 10 + s.at(7) - '0';
	}

	int minute_void(std::string s, bool b) {
		if (b) return (s.at(3) - '0') * 10 + s.at(4) - '0';
		return (s.at(9) - '0') * 10 + s.at(10) - '0';
	}

};