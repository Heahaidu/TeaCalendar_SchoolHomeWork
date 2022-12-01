#pragma once

#include <cliext/vector>
#include <msclr\marshal_cppstd.h>
#include "PersonalPanel.h"
#include "User.h"
#include "Configuration.h"
#include "Date_Time.h"
#include "GlobalPanel.h"

namespace TeaCalendar {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Collections::Generic;

	public ref class Interface : public System::Windows::Forms::Form {
	public:

		bool loading = false, calendarloading_b = false, click = false;
		bool swtich = true;
		bool runnable = false;
		int _day, _month, _year;

		/**/
		GlobalPanel^ gloPanel;
		User^ user = gcnew User;
		Config^ config = gcnew Config;
		PersonalPanel^ personalPanel;
		DateTimeReserve^ dateReserve = gcnew DateTimeReserve;

	public:

		cliext::vector<String^> monTh = dateReserve->getDate();

		int  minute, hour;

		Interface(void) {
			InitializeComponent();

			Login_loadingPanel->Location = System::Drawing::Point(0, 20);
			Login_loadingPanel->Hide();
			Calendar_loadingPanel->Hide();

			comboBox_recommend->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;

			loginPanel->Location = System::Drawing::Point(0, 20);
			if (user->keepLogin()) {
				onLogin();
			}
		}

		void runnableDateTime() {
			while (this->runnable) {
				DateTime now = DateTime::Now;
				_month = now.Month;
				_day = now.Day;
				_year = now.Year;
				minute = now.Minute;
				hour = now.Hour;

				personalPanel->runnableDateTime(_month, _day, _year, minute, hour);
				Thread::Sleep(30000);
			}
		}

		void onEnable() {

			Console::WriteLine("onEnable() 1:" + user->userID);

			_month = dateTimePicker->Value.Date.Month;
			_day = dateTimePicker->Value.Date.Day;
			_year = dateTimePicker->Value.Date.Year;

			runnable = true;

			personalPanel = gcnew PersonalPanel(personal_classroom_panel, user->userID);

			Thread^ datetimeThread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::runnableDateTime));
			datetimeThread->Start();

			String^ path = System::Convert::ToString(_month + "-" + _day + "-" + _year);



			//




			gloPanel = gcnew GlobalPanel(global_classroom_panel);

			add_calendar_panel->Hide();
			freetime_panel->Location = System::Drawing::Point(7, 300);
			global_classroom_panel->Hide();



			personalPanel->PersonalGet(path, _month, _day, _year);
			Console::WriteLine("onEnable() 2:" + user->userID);
			config->getListClass(path);
			gloPanel->GlobalGet(path);

			DatePanel();
			freeTimeBox(path);

			date_to_add_calendar->MinDate = System::DateTime(_year, _month, _day, 0, 0, 0, 0);

			comboBox_recommend->CheckForIllegalCrossThreadCalls = false;
			book_button->CheckForIllegalCrossThreadCalls = false;
			warning_time->CheckForIllegalCrossThreadCalls = false;
			followTime->CheckForIllegalCrossThreadCalls = false;
			dateTimePicker->CheckForIllegalCrossThreadCalls = false;
			dayOfWeek->CheckForIllegalCrossThreadCalls = false;
			dayInMonth->CheckForIllegalCrossThreadCalls = false;
			Month_->CheckForIllegalCrossThreadCalls = false;
			Year_->CheckForIllegalCrossThreadCalls = false;
			Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::getOffer));
			thread->Start();
			loading = true;
		}

		void onLogin() {
			teaName->Text = "Name: " + config->getUserName(user->userID);
			position->Text = "Position: " + config->getPosition(user->userID);
			Console::WriteLine("Success");
			invalid1->Visible = false;
			invalid2->Visible = false;
			useridText->Text = "";
			passwordText->Text = "";
			loginPanel->Hide();
			onEnable();
		}

		void freeTimeBox(String^ path) {
			freetimeList->Items->Clear();
			cliext::vector<String^> freeTime = user->freeTime(path);
			for (int i = 0; i < freeTime.size(); i++) {
				freetimeList->Items->Add(freeTime[i]);
			}
		}

		void DatePanel() {
			dayOfWeek->Text = System::Convert::ToString(dateTimePicker->Value.Date.DayOfWeek);

			if (dateTimePicker->Value.Date.Day < 10) 
				dayInMonth->Text = " " + System::Convert::ToString(dateTimePicker->Value.Date.Day);
			else dayInMonth->Text = System::Convert::ToString(dateTimePicker->Value.Date.Day);

			Month_->Text = ", " + monTh[System::Convert::ToInt32(dateTimePicker->Value.Date.Month) - 1];
			Year_->Text = System::Convert::ToString(dateTimePicker->Value.Date.Year);
		}

		void refreshPanel() {
			String^ path = System::Convert::ToString(dateTimePicker->Value.Date.Month + "-" + dateTimePicker->Value.Date.Day + "-" + dateTimePicker->Value.Date.Year);
			if (this->swtich) {
				personal_classroom_panel->Controls->Clear();
				personalPanel->PersonalGet(path, dateTimePicker->Value.Date.Month, dateTimePicker->Value.Date.Day, dateTimePicker->Value.Date.Year);
			}
			else {
				global_classroom_panel->Controls->Clear();
				gloPanel->GlobalGet(path);
			}
			freeTimeBox(path);
			calendarloading_b = true;
		}

		void getOffer() {
			String^ timeStart = "";
			String^ timeEnd = "";
			String^ date = "";
			while (this->runnable) {
				String^ main_Date = System::Convert::ToString(date_to_add_calendar->Value.Date.Month + "-" + date_to_add_calendar->Value.Date.Day + "-" + date_to_add_calendar->Value.Date.Year);
				if (this->timeStart->Text != timeStart || this->timeEnd->Text != timeEnd || date != main_Date) {
					timeStart = this->timeStart->Text; timeEnd = this->timeEnd->Text; date = main_Date;
					int hourStart = this->timeStart->Value.Hour,
						hourEnd = this->timeEnd->Value.Hour,
						minuteStart = this->timeStart->Value.Minute,
						minuteEnd = this->timeEnd->Value.Minute;
					if ((hourStart == 7 && minuteStart >= 30) || hourStart > 7) {
						if ((hourEnd == hourStart && minuteEnd > minuteStart) || hourEnd > hourStart) {
							if ((date_to_add_calendar->Value.Date.Month >= this->_month && date_to_add_calendar->Value.Date.Year >= this->_year && date_to_add_calendar->Value.Date.Day > this->_day ) || (date_to_add_calendar->Value.Date.Month == this->_month && date_to_add_calendar->Value.Date.Year == this->_year && date_to_add_calendar->Value.Date.Day == this->_day && (hourStart > this->hour) || (hourStart == this->hour && minuteStart > this->minute))) {
								cliext::vector<String^> dup = user->duplicateTime(this->timeStart->Text->Replace(":", "h") + "-" + this->timeEnd->Text->Replace(":", "h"), date);
								if (dup.size() == 1) {
									warning_time->Hide();
									comboBox_recommend->Items->Clear();
									comboBox_recommend->Text = "";
									cliext::vector<String^> recommend = config->recommendClass(this->timeStart->Text->Replace(":", "h") + "-" + this->timeEnd->Text->Replace(":", "h"), date);
									for (int i = 0; i < recommend.size(); i++) {
										comboBox_recommend->Items->Add(recommend[i]);
									}
								}
								else {
									book_button->Hide();
									warning_time->Show();
									comboBox_recommend->Items->Clear();
									comboBox_recommend->Text = "";
									String^ s = "";
									int j = 0;
									for (int i = 0; i < dup.size() - 1; i++) {
										j++;
										s += "    " + dup[i];
										if (j == 2) {
											j = 0;
											s += "\n";
										}
									}
									warning_time->Text = s;
								}

							} else {
								book_button->Hide();
								warning_time->Show();
								warning_time->Text = "This time has passed.";
								comboBox_recommend->Items->Clear();
								comboBox_recommend->Text = "";
							}
						}
						else {
							book_button->Hide();
							warning_time->Show();
							warning_time->Text = "End time must be more than start time or start \ntime must be less than end time.";
							comboBox_recommend->Items->Clear();
							comboBox_recommend->Text = "";
						}
					}
					else {
						book_button->Hide();
						warning_time->Show();
						warning_time->Text = "Start time must be more than or equal to 7h30.";
						comboBox_recommend->Items->Clear();
						comboBox_recommend->Text = "";
					}
				}
				Thread::Sleep(100);
			}
		}

		void loginLoading() {
			while (!loading) {
				Console::WriteLine("Login...");
				Thread::Sleep(1000);
			}
			Login_loadingPanel->Hide();
		}

		void calendarLoading() {
			while (!calendarloading_b) {
				Console::WriteLine("Calendar...");
				Thread::Sleep(500);
			}
			Calendar_loadingPanel->Hide();
			calendarloading_b = false;
		}

	protected:
		~Interface() {
			if (components) {
				delete components;
			}
		}
private: System::Windows::Forms::PictureBox^ Calendar_loadingPanel;
private: System::Windows::Forms::PictureBox^ Login_loadingPanel;
protected:
	protected:

	protected:




	private: System::Windows::Forms::Panel^ loginPanel;
	private: System::Windows::Forms::Panel^ panel5;
	private: System::Windows::Forms::CheckBox^ keeploginBox;
	private: System::Windows::Forms::Label^ label25;
	private: System::Windows::Forms::TextBox^ useridText;
	private: System::Windows::Forms::Label^ label23;
	private: System::Windows::Forms::Label^ label21;
	private: System::Windows::Forms::Button^ loginButton;
	private: System::Windows::Forms::Panel^ panel7;
	private: System::Windows::Forms::TextBox^ passwordText;
	private: System::Windows::Forms::Panel^ panel6;
	private: System::Windows::Forms::Label^ invalid2;
	private: System::Windows::Forms::Label^ invalid1;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ teaName;
	private: System::Windows::Forms::Label^ position;
	private: System::Windows::Forms::Panel^ panel4;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label24;
	private: System::Windows::Forms::Button^ X;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Panel^ freetime_panel;
	private: System::Windows::Forms::Label^ warning_time;
	private: System::Windows::Forms::CheckBox^ followTime;
	private: System::Windows::Forms::Panel^ panel3;
	private: System::Windows::Forms::Panel^ Control_panel;
	private: System::Windows::Forms::Button^ thedayBefore;
	private: System::Windows::Forms::Button^ thedayAfter;
	private: System::Windows::Forms::DateTimePicker^ dateTimePicker;
	private: System::Windows::Forms::Label^ dayInMonth;
	private: System::Windows::Forms::Label^ Month_;
	private: System::Windows::Forms::Label^ Year_;
	private: System::Windows::Forms::Label^ dayOfWeek;
	private: System::Windows::Forms::Panel^ add_calendar_panel;
	private: System::Windows::Forms::Label^ label18;
	private: System::Windows::Forms::Label^ label20;
	private: System::Windows::Forms::DateTimePicker^ date_to_add_calendar;
	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::Button^ close_calendar_panel;

	private: System::Windows::Forms::ComboBox^ comboBox_recommend;

	private: System::Windows::Forms::DateTimePicker^ timeEnd;
	private: System::Windows::Forms::DateTimePicker^ timeStart;

	private: System::Windows::Forms::Button^ book_button;
	private: System::Windows::Forms::Label^ label22;
	private: System::Windows::Forms::ListBox^ freetimeList;
	private: System::Windows::Forms::Button^ add_calendar;
	private: System::Windows::Forms::Panel^ global_classroom_panel;
	private: System::Windows::Forms::Panel^ string_decorate_control_panel;
	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Button^ global_button;
	private: System::Windows::Forms::Button^ personal_button;

	private: System::Windows::Forms::Panel^ main_personal_panel;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Panel^ personal_classroom_panel;
	private: System::Windows::Forms::Panel^ bot_line;


	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void) {
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Interface::typeid));
			this->Control_panel = (gcnew System::Windows::Forms::Panel());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->teaName = (gcnew System::Windows::Forms::Label());
			this->position = (gcnew System::Windows::Forms::Label());
			this->freetime_panel = (gcnew System::Windows::Forms::Panel());
			this->freetimeList = (gcnew System::Windows::Forms::ListBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->add_calendar_panel = (gcnew System::Windows::Forms::Panel());
			this->followTime = (gcnew System::Windows::Forms::CheckBox());
			this->warning_time = (gcnew System::Windows::Forms::Label());
			this->book_button = (gcnew System::Windows::Forms::Button());
			this->timeEnd = (gcnew System::Windows::Forms::DateTimePicker());
			this->timeStart = (gcnew System::Windows::Forms::DateTimePicker());
			this->comboBox_recommend = (gcnew System::Windows::Forms::ComboBox());
			this->close_calendar_panel = (gcnew System::Windows::Forms::Button());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->date_to_add_calendar = (gcnew System::Windows::Forms::DateTimePicker());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->global_button = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->personal_button = (gcnew System::Windows::Forms::Button());
			this->string_decorate_control_panel = (gcnew System::Windows::Forms::Panel());
			this->loginPanel = (gcnew System::Windows::Forms::Panel());
			this->invalid2 = (gcnew System::Windows::Forms::Label());
			this->invalid1 = (gcnew System::Windows::Forms::Label());
			this->panel7 = (gcnew System::Windows::Forms::Panel());
			this->passwordText = (gcnew System::Windows::Forms::TextBox());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->useridText = (gcnew System::Windows::Forms::TextBox());
			this->loginButton = (gcnew System::Windows::Forms::Button());
			this->keeploginBox = (gcnew System::Windows::Forms::CheckBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->Login_loadingPanel = (gcnew System::Windows::Forms::PictureBox());
			this->main_personal_panel = (gcnew System::Windows::Forms::Panel());
			this->Calendar_loadingPanel = (gcnew System::Windows::Forms::PictureBox());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->thedayBefore = (gcnew System::Windows::Forms::Button());
			this->dayInMonth = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->Year_ = (gcnew System::Windows::Forms::Label());
			this->thedayAfter = (gcnew System::Windows::Forms::Button());
			this->Month_ = (gcnew System::Windows::Forms::Label());
			this->dayOfWeek = (gcnew System::Windows::Forms::Label());
			this->add_calendar = (gcnew System::Windows::Forms::Button());
			this->personal_classroom_panel = (gcnew System::Windows::Forms::Panel());
			this->bot_line = (gcnew System::Windows::Forms::Panel());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->global_classroom_panel = (gcnew System::Windows::Forms::Panel());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->X = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->Control_panel->SuspendLayout();
			this->panel4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->freetime_panel->SuspendLayout();
			this->add_calendar_panel->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel1->SuspendLayout();
			this->loginPanel->SuspendLayout();
			this->panel7->SuspendLayout();
			this->panel6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Login_loadingPanel))->BeginInit();
			this->main_personal_panel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Calendar_loadingPanel))->BeginInit();
			this->panel3->SuspendLayout();
			this->SuspendLayout();
			// 
			// Control_panel
			// 
			this->Control_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)), static_cast<System::Int32>(static_cast<System::Byte>(246)),
				static_cast<System::Int32>(static_cast<System::Byte>(251)));
			this->Control_panel->Controls->Add(this->panel4);
			this->Control_panel->Controls->Add(this->pictureBox1);
			this->Control_panel->Controls->Add(this->teaName);
			this->Control_panel->Controls->Add(this->position);
			this->Control_panel->Controls->Add(this->freetime_panel);
			this->Control_panel->Controls->Add(this->add_calendar_panel);
			this->Control_panel->Controls->Add(this->panel2);
			this->Control_panel->Controls->Add(this->panel1);
			this->Control_panel->Controls->Add(this->string_decorate_control_panel);
			this->Control_panel->Location = System::Drawing::Point(0, 20);
			this->Control_panel->Name = L"Control_panel";
			this->Control_panel->Size = System::Drawing::Size(291, 720);
			this->Control_panel->TabIndex = 0;
			// 
			// panel4
			// 
			this->panel4->Controls->Add(this->button2);
			this->panel4->Location = System::Drawing::Point(0, 673);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(291, 48);
			this->panel4->TabIndex = 1;
			// 
			// button2
			// 
			this->button2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button2.BackgroundImage")));
			this->button2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->button2->FlatAppearance->BorderSize = 0;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Location = System::Drawing::Point(247, 7);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(37, 35);
			this->button2->TabIndex = 0;
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Interface::button2_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->ErrorImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.ErrorImage")));
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.InitialImage")));
			this->pictureBox1->Location = System::Drawing::Point(15, 17);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(61, 57);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 23;
			this->pictureBox1->TabStop = false;
			// 
			// teaName
			// 
			this->teaName->AutoSize = true;
			this->teaName->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->teaName->Location = System::Drawing::Point(82, 20);
			this->teaName->Name = L"teaName";
			this->teaName->Size = System::Drawing::Size(47, 17);
			this->teaName->TabIndex = 0;
			this->teaName->Text = L"Name:";
			// 
			// position
			// 
			this->position->AutoSize = true;
			this->position->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->position->Location = System::Drawing::Point(82, 39);
			this->position->Name = L"position";
			this->position->Size = System::Drawing::Size(63, 17);
			this->position->TabIndex = 1;
			this->position->Text = L"Position: ";
			// 
			// freetime_panel
			// 
			this->freetime_panel->BackColor = System::Drawing::Color::White;
			this->freetime_panel->Controls->Add(this->freetimeList);
			this->freetime_panel->Controls->Add(this->label22);
			this->freetime_panel->Location = System::Drawing::Point(7, 470);
			this->freetime_panel->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->freetime_panel->Name = L"freetime_panel";
			this->freetime_panel->Size = System::Drawing::Size(277, 130);
			this->freetime_panel->TabIndex = 22;
			// 
			// freetimeList
			// 
			this->freetimeList->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->freetimeList->FormattingEnabled = true;
			this->freetimeList->ItemHeight = 16;
			this->freetimeList->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"All day" });
			this->freetimeList->Location = System::Drawing::Point(0, 21);
			this->freetimeList->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->freetimeList->Name = L"freetimeList";
			this->freetimeList->Size = System::Drawing::Size(277, 84);
			this->freetimeList->TabIndex = 5;
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label22->Location = System::Drawing::Point(1, 5);
			this->label22->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(111, 15);
			this->label22->TabIndex = 6;
			this->label22->Text = L"Free time in the day:";
			// 
			// add_calendar_panel
			// 
			this->add_calendar_panel->BackColor = System::Drawing::Color::White;
			this->add_calendar_panel->Controls->Add(this->followTime);
			this->add_calendar_panel->Controls->Add(this->warning_time);
			this->add_calendar_panel->Controls->Add(this->book_button);
			this->add_calendar_panel->Controls->Add(this->timeEnd);
			this->add_calendar_panel->Controls->Add(this->timeStart);
			this->add_calendar_panel->Controls->Add(this->comboBox_recommend);
			this->add_calendar_panel->Controls->Add(this->close_calendar_panel);
			this->add_calendar_panel->Controls->Add(this->label20);
			this->add_calendar_panel->Controls->Add(this->date_to_add_calendar);
			this->add_calendar_panel->Controls->Add(this->label19);
			this->add_calendar_panel->Controls->Add(this->label18);
			this->add_calendar_panel->Location = System::Drawing::Point(7, 300);
			this->add_calendar_panel->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->add_calendar_panel->Name = L"add_calendar_panel";
			this->add_calendar_panel->Size = System::Drawing::Size(277, 161);
			this->add_calendar_panel->TabIndex = 3;
			// 
			// followTime
			// 
			this->followTime->AutoSize = true;
			this->followTime->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->followTime->Location = System::Drawing::Point(214, 30);
			this->followTime->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->followTime->Name = L"followTime";
			this->followTime->Size = System::Drawing::Size(57, 19);
			this->followTime->TabIndex = 23;
			this->followTime->Text = L"Follow";
			this->followTime->UseVisualStyleBackColor = true;
			this->followTime->CheckedChanged += gcnew System::EventHandler(this, &Interface::followTime_CheckedChanged);
			// 
			// warning_time
			// 
			this->warning_time->AutoSize = true;
			this->warning_time->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->warning_time->ForeColor = System::Drawing::Color::Red;
			this->warning_time->Location = System::Drawing::Point(19, 110);
			this->warning_time->Name = L"warning_time";
			this->warning_time->Size = System::Drawing::Size(25, 15);
			this->warning_time->TabIndex = 22;
			this->warning_time->Text = L"asd\r\n";
			// 
			// book_button
			// 
			this->book_button->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->book_button->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->book_button->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->book_button->FlatAppearance->MouseDownBackColor = System::Drawing::Color::DimGray;
			this->book_button->FlatAppearance->MouseOverBackColor = System::Drawing::Color::White;
			this->book_button->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->book_button->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->book_button->Location = System::Drawing::Point(99, 115);
			this->book_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->book_button->Name = L"book_button";
			this->book_button->Size = System::Drawing::Size(86, 23);
			this->book_button->TabIndex = 21;
			this->book_button->Text = L"Book";
			this->book_button->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
			this->book_button->UseVisualStyleBackColor = true;
			this->book_button->Click += gcnew System::EventHandler(this, &Interface::book_button_Click);
			// 
			// timeEnd
			// 
			this->timeEnd->CustomFormat = L"HH:mm";
			this->timeEnd->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->timeEnd->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->timeEnd->Location = System::Drawing::Point(14, 76);
			this->timeEnd->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->timeEnd->Name = L"timeEnd";
			this->timeEnd->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->timeEnd->ShowUpDown = true;
			this->timeEnd->Size = System::Drawing::Size(52, 22);
			this->timeEnd->TabIndex = 11;
			this->timeEnd->Value = System::DateTime(2022, 11, 24, 0, 0, 0, 0);
			// 
			// timeStart
			// 
			this->timeStart->CustomFormat = L"HH:mm";
			this->timeStart->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->timeStart->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->timeStart->Location = System::Drawing::Point(14, 29);
			this->timeStart->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->timeStart->Name = L"timeStart";
			this->timeStart->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->timeStart->ShowUpDown = true;
			this->timeStart->Size = System::Drawing::Size(52, 22);
			this->timeStart->TabIndex = 10;
			this->timeStart->Value = System::DateTime(2022, 11, 15, 0, 0, 0, 0);
			// 
			// comboBox_recommend
			// 
			this->comboBox_recommend->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->comboBox_recommend->FormattingEnabled = true;
			this->comboBox_recommend->Location = System::Drawing::Point(99, 76);
			this->comboBox_recommend->MaxDropDownItems = 16;
			this->comboBox_recommend->Name = L"comboBox_recommend";
			this->comboBox_recommend->Size = System::Drawing::Size(112, 23);
			this->comboBox_recommend->TabIndex = 9;
			this->comboBox_recommend->SelectedIndexChanged += gcnew System::EventHandler(this, &Interface::comboBox_recommend_SelectedIndexChanged);
			// 
			// close_calendar_panel
			// 
			this->close_calendar_panel->FlatAppearance->BorderSize = 0;
			this->close_calendar_panel->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->close_calendar_panel->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI Light", 11.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->close_calendar_panel->ForeColor = System::Drawing::SystemColors::AppWorkspace;
			this->close_calendar_panel->Location = System::Drawing::Point(253, 2);
			this->close_calendar_panel->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->close_calendar_panel->Name = L"close_calendar_panel";
			this->close_calendar_panel->Size = System::Drawing::Size(22, 22);
			this->close_calendar_panel->TabIndex = 7;
			this->close_calendar_panel->Text = L"✕";
			this->close_calendar_panel->UseVisualStyleBackColor = true;
			this->close_calendar_panel->Click += gcnew System::EventHandler(this, &Interface::close_calendar_panel_Click);
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(97, 56);
			this->label20->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(38, 13);
			this->label20->TabIndex = 6;
			this->label20->Text = L"Room:";
			// 
			// date_to_add_calendar
			// 
			this->date_to_add_calendar->CustomFormat = L"dd-MM-yyyy";
			this->date_to_add_calendar->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->date_to_add_calendar->Location = System::Drawing::Point(99, 29);
			this->date_to_add_calendar->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->date_to_add_calendar->MinDate = System::DateTime(2022, 11, 20, 0, 0, 0, 0);
			this->date_to_add_calendar->Name = L"date_to_add_calendar";
			this->date_to_add_calendar->Size = System::Drawing::Size(112, 20);
			this->date_to_add_calendar->TabIndex = 4;
			this->date_to_add_calendar->ValueChanged += gcnew System::EventHandler(this, &Interface::date_to_add_calendar_ValueChanged);
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label19->Location = System::Drawing::Point(11, 58);
			this->label19->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(60, 16);
			this->label19->TabIndex = 2;
			this->label19->Text = L"End Time:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label18->Location = System::Drawing::Point(11, 11);
			this->label18->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(65, 16);
			this->label18->TabIndex = 1;
			this->label18->Text = L"Start Time:";
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->global_button);
			this->panel2->Location = System::Drawing::Point(4, 254);
			this->panel2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(282, 41);
			this->panel2->TabIndex = 2;
			// 
			// global_button
			// 
			this->global_button->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display Semib", 9.75F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->global_button->Location = System::Drawing::Point(-2, -10);
			this->global_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->global_button->Name = L"global_button";
			this->global_button->Size = System::Drawing::Size(286, 56);
			this->global_button->TabIndex = 3;
			this->global_button->Text = L"School";
			this->global_button->UseVisualStyleBackColor = true;
			this->global_button->Click += gcnew System::EventHandler(this, &Interface::global_button_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->personal_button);
			this->panel1->Location = System::Drawing::Point(4, 200);
			this->panel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(282, 41);
			this->panel1->TabIndex = 1;
			// 
			// personal_button
			// 
			this->personal_button->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display Semib", 9.75F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->personal_button->Location = System::Drawing::Point(-2, -6);
			this->personal_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->personal_button->Name = L"personal_button";
			this->personal_button->Size = System::Drawing::Size(286, 56);
			this->personal_button->TabIndex = 0;
			this->personal_button->Text = L"Personal";
			this->personal_button->UseVisualStyleBackColor = true;
			this->personal_button->Click += gcnew System::EventHandler(this, &Interface::personal_button_Click);
			// 
			// string_decorate_control_panel
			// 
			this->string_decorate_control_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)),
				static_cast<System::Int32>(static_cast<System::Byte>(236)), static_cast<System::Int32>(static_cast<System::Byte>(240)));
			this->string_decorate_control_panel->Location = System::Drawing::Point(28, 193);
			this->string_decorate_control_panel->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->string_decorate_control_panel->Name = L"string_decorate_control_panel";
			this->string_decorate_control_panel->Size = System::Drawing::Size(225, 2);
			this->string_decorate_control_panel->TabIndex = 0;
			// 
			// loginPanel
			// 
			this->loginPanel->BackColor = System::Drawing::Color::White;
			this->loginPanel->Controls->Add(this->invalid2);
			this->loginPanel->Controls->Add(this->invalid1);
			this->loginPanel->Controls->Add(this->panel7);
			this->loginPanel->Controls->Add(this->panel6);
			this->loginPanel->Controls->Add(this->loginButton);
			this->loginPanel->Controls->Add(this->keeploginBox);
			this->loginPanel->Controls->Add(this->label25);
			this->loginPanel->Controls->Add(this->label23);
			this->loginPanel->Controls->Add(this->label21);
			this->loginPanel->Controls->Add(this->panel5);
			this->loginPanel->Location = System::Drawing::Point(1269, 4);
			this->loginPanel->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->loginPanel->Name = L"loginPanel";
			this->loginPanel->Size = System::Drawing::Size(1365, 720);
			this->loginPanel->TabIndex = 0;
			// 
			// invalid2
			// 
			this->invalid2->AutoSize = true;
			this->invalid2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 8.25F, System::Drawing::FontStyle::Italic));
			this->invalid2->ForeColor = System::Drawing::Color::Red;
			this->invalid2->Location = System::Drawing::Point(179, 347);
			this->invalid2->Name = L"invalid2";
			this->invalid2->Size = System::Drawing::Size(160, 16);
			this->invalid2->TabIndex = 13;
			this->invalid2->Text = L"UserID or Password is invalid.";
			this->invalid2->Visible = false;
			// 
			// invalid1
			// 
			this->invalid1->AutoSize = true;
			this->invalid1->BackColor = System::Drawing::Color::White;
			this->invalid1->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->invalid1->ForeColor = System::Drawing::Color::Red;
			this->invalid1->Location = System::Drawing::Point(176, 267);
			this->invalid1->Name = L"invalid1";
			this->invalid1->Size = System::Drawing::Size(160, 16);
			this->invalid1->TabIndex = 12;
			this->invalid1->Text = L"UserID or Password is invalid.";
			this->invalid1->Visible = false;
			// 
			// panel7
			// 
			this->panel7->BackColor = System::Drawing::Color::WhiteSmoke;
			this->panel7->Controls->Add(this->passwordText);
			this->panel7->Location = System::Drawing::Point(86, 370);
			this->panel7->Name = L"panel7";
			this->panel7->Size = System::Drawing::Size(250, 40);
			this->panel7->TabIndex = 11;
			// 
			// passwordText
			// 
			this->passwordText->BackColor = System::Drawing::Color::WhiteSmoke;
			this->passwordText->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->passwordText->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text Semiligh", 9));
			this->passwordText->Location = System::Drawing::Point(7, 12);
			this->passwordText->Name = L"passwordText";
			this->passwordText->Size = System::Drawing::Size(235, 16);
			this->passwordText->TabIndex = 4;
			this->passwordText->UseSystemPasswordChar = true;
			// 
			// panel6
			// 
			this->panel6->BackColor = System::Drawing::Color::WhiteSmoke;
			this->panel6->Controls->Add(this->useridText);
			this->panel6->Location = System::Drawing::Point(86, 292);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(250, 40);
			this->panel6->TabIndex = 10;
			// 
			// useridText
			// 
			this->useridText->BackColor = System::Drawing::Color::WhiteSmoke;
			this->useridText->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->useridText->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->useridText->Location = System::Drawing::Point(7, 12);
			this->useridText->Name = L"useridText";
			this->useridText->Size = System::Drawing::Size(235, 16);
			this->useridText->TabIndex = 4;
			// 
			// loginButton
			// 
			this->loginButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->loginButton->FlatAppearance->BorderColor = System::Drawing::Color::White;
			this->loginButton->FlatAppearance->BorderSize = 0;
			this->loginButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->loginButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Blue;
			this->loginButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->loginButton->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->loginButton->ForeColor = System::Drawing::Color::White;
			this->loginButton->Location = System::Drawing::Point(86, 496);
			this->loginButton->Name = L"loginButton";
			this->loginButton->Size = System::Drawing::Size(250, 50);
			this->loginButton->TabIndex = 9;
			this->loginButton->Text = L"Login";
			this->loginButton->UseVisualStyleBackColor = false;
			this->loginButton->Click += gcnew System::EventHandler(this, &Interface::loginButton_Click);
			// 
			// keeploginBox
			// 
			this->keeploginBox->AutoSize = true;
			this->keeploginBox->FlatAppearance->BorderSize = 0;
			this->keeploginBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->keeploginBox->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->keeploginBox->Location = System::Drawing::Point(86, 429);
			this->keeploginBox->Name = L"keeploginBox";
			this->keeploginBox->Size = System::Drawing::Size(107, 19);
			this->keeploginBox->TabIndex = 8;
			this->keeploginBox->Text = L"Keep me sign in.";
			this->keeploginBox->UseVisualStyleBackColor = true;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label25->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label25->Location = System::Drawing::Point(85, 345);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(65, 17);
			this->label25->TabIndex = 6;
			this->label25->Text = L"Password";
			// 
			// label23
			// 
			this->label23->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->label23->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label23->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label23->Location = System::Drawing::Point(85, 265);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(48, 17);
			this->label23->TabIndex = 2;
			this->label23->Text = L"UserID";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Segoe UI", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label21->Location = System::Drawing::Point(78, 124);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(121, 47);
			this->label21->TabIndex = 1;
			this->label21->Text = L"Login.";
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::Cyan;
			this->panel5->Location = System::Drawing::Point(430, 0);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(935, 720);
			this->panel5->TabIndex = 0;
			// 
			// Login_loadingPanel
			// 
			this->Login_loadingPanel->BackColor = System::Drawing::Color::White;
			this->Login_loadingPanel->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Login_loadingPanel.Image")));
			this->Login_loadingPanel->Location = System::Drawing::Point(1230, 8);
			this->Login_loadingPanel->Name = L"Login_loadingPanel";
			this->Login_loadingPanel->Size = System::Drawing::Size(1365, 720);
			this->Login_loadingPanel->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->Login_loadingPanel->TabIndex = 0;
			this->Login_loadingPanel->TabStop = false;
			// 
			// main_personal_panel
			// 
			this->main_personal_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(246)), static_cast<System::Int32>(static_cast<System::Byte>(251)));
			this->main_personal_panel->Controls->Add(this->Calendar_loadingPanel);
			this->main_personal_panel->Controls->Add(this->panel3);
			this->main_personal_panel->Controls->Add(this->dayOfWeek);
			this->main_personal_panel->Controls->Add(this->add_calendar);
			this->main_personal_panel->Controls->Add(this->personal_classroom_panel);
			this->main_personal_panel->Controls->Add(this->bot_line);
			this->main_personal_panel->Controls->Add(this->label17);
			this->main_personal_panel->Controls->Add(this->label16);
			this->main_personal_panel->Controls->Add(this->label15);
			this->main_personal_panel->Controls->Add(this->label14);
			this->main_personal_panel->Controls->Add(this->label13);
			this->main_personal_panel->Controls->Add(this->label12);
			this->main_personal_panel->Controls->Add(this->label11);
			this->main_personal_panel->Controls->Add(this->label10);
			this->main_personal_panel->Controls->Add(this->label9);
			this->main_personal_panel->Controls->Add(this->label8);
			this->main_personal_panel->Controls->Add(this->label7);
			this->main_personal_panel->Controls->Add(this->label6);
			this->main_personal_panel->Controls->Add(this->label5);
			this->main_personal_panel->Controls->Add(this->label4);
			this->main_personal_panel->Controls->Add(this->label3);
			this->main_personal_panel->Controls->Add(this->label2);
			this->main_personal_panel->Controls->Add(this->label1);
			this->main_personal_panel->Controls->Add(this->global_classroom_panel);
			this->main_personal_panel->Location = System::Drawing::Point(293, 20);
			this->main_personal_panel->Name = L"main_personal_panel";
			this->main_personal_panel->Size = System::Drawing::Size(1074, 720);
			this->main_personal_panel->TabIndex = 1;
			// 
			// Calendar_loadingPanel
			// 
			this->Calendar_loadingPanel->BackColor = System::Drawing::Color::White;
			this->Calendar_loadingPanel->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Calendar_loadingPanel.Image")));
			this->Calendar_loadingPanel->Location = System::Drawing::Point(60, 62);
			this->Calendar_loadingPanel->Name = L"Calendar_loadingPanel";
			this->Calendar_loadingPanel->Size = System::Drawing::Size(1000, 660);
			this->Calendar_loadingPanel->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->Calendar_loadingPanel->TabIndex = 1;
			this->Calendar_loadingPanel->TabStop = false;
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->thedayBefore);
			this->panel3->Controls->Add(this->dayInMonth);
			this->panel3->Controls->Add(this->dateTimePicker);
			this->panel3->Controls->Add(this->Year_);
			this->panel3->Controls->Add(this->thedayAfter);
			this->panel3->Controls->Add(this->Month_);
			this->panel3->Location = System::Drawing::Point(800, 16);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(251, 36);
			this->panel3->TabIndex = 0;
			// 
			// thedayBefore
			// 
			this->thedayBefore->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"thedayBefore.BackgroundImage")));
			this->thedayBefore->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->thedayBefore->FlatAppearance->BorderSize = 0;
			this->thedayBefore->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->thedayBefore->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei Light", 12));
			this->thedayBefore->Location = System::Drawing::Point(0, 6);
			this->thedayBefore->Name = L"thedayBefore";
			this->thedayBefore->Size = System::Drawing::Size(25, 25);
			this->thedayBefore->TabIndex = 24;
			this->thedayBefore->UseVisualStyleBackColor = true;
			this->thedayBefore->Click += gcnew System::EventHandler(this, &Interface::thedayBefore_Click);
			// 
			// dayInMonth
			// 
			this->dayInMonth->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->dayInMonth->AllowDrop = true;
			this->dayInMonth->AutoSize = true;
			this->dayInMonth->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 17.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dayInMonth->Location = System::Drawing::Point(31, 1);
			this->dayInMonth->Name = L"dayInMonth";
			this->dayInMonth->Size = System::Drawing::Size(24, 31);
			this->dayInMonth->TabIndex = 21;
			this->dayInMonth->Text = L"..";
			this->dayInMonth->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// dateTimePicker
			// 
			this->dateTimePicker->CustomFormat = L"MM-dd-yyyy";
			this->dateTimePicker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker->Location = System::Drawing::Point(225, 8);
			this->dateTimePicker->Name = L"dateTimePicker";
			this->dateTimePicker->Size = System::Drawing::Size(16, 20);
			this->dateTimePicker->TabIndex = 22;
			this->dateTimePicker->ValueChanged += gcnew System::EventHandler(this, &Interface::dateTimePicker_ValueChanged);
			// 
			// Year_
			// 
			this->Year_->AutoSize = true;
			this->Year_->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 17.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Year_->Location = System::Drawing::Point(156, 3);
			this->Year_->Name = L"Year_";
			this->Year_->Size = System::Drawing::Size(34, 31);
			this->Year_->TabIndex = 26;
			this->Year_->Text = L"....";
			// 
			// thedayAfter
			// 
			this->thedayAfter->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"thedayAfter.BackgroundImage")));
			this->thedayAfter->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->thedayAfter->FlatAppearance->BorderSize = 0;
			this->thedayAfter->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->thedayAfter->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei Light", 12));
			this->thedayAfter->Location = System::Drawing::Point(65, 6);
			this->thedayAfter->Name = L"thedayAfter";
			this->thedayAfter->Size = System::Drawing::Size(25, 25);
			this->thedayAfter->TabIndex = 23;
			this->thedayAfter->UseVisualStyleBackColor = true;
			this->thedayAfter->Click += gcnew System::EventHandler(this, &Interface::thedayAfter_Click);
			// 
			// Month_
			// 
			this->Month_->AutoSize = true;
			this->Month_->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 17.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Month_->Location = System::Drawing::Point(91, 3);
			this->Month_->Name = L"Month_";
			this->Month_->Size = System::Drawing::Size(40, 31);
			this->Month_->TabIndex = 25;
			this->Month_->Text = L". ...";
			// 
			// dayOfWeek
			// 
			this->dayOfWeek->AutoSize = true;
			this->dayOfWeek->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display Semib", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dayOfWeek->Location = System::Drawing::Point(55, 16);
			this->dayOfWeek->Name = L"dayOfWeek";
			this->dayOfWeek->Size = System::Drawing::Size(68, 32);
			this->dayOfWeek->TabIndex = 27;
			this->dayOfWeek->Text = L".........";
			// 
			// add_calendar
			// 
			this->add_calendar->BackColor = System::Drawing::Color::White;
			this->add_calendar->Font = (gcnew System::Drawing::Font(L"Candara Light", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->add_calendar->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(171)), static_cast<System::Int32>(static_cast<System::Byte>(173)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)));
			this->add_calendar->Location = System::Drawing::Point(14, 18);
			this->add_calendar->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->add_calendar->Name = L"add_calendar";
			this->add_calendar->Size = System::Drawing::Size(26, 26);
			this->add_calendar->TabIndex = 20;
			this->add_calendar->Text = L"+";
			this->add_calendar->UseVisualStyleBackColor = false;
			this->add_calendar->Click += gcnew System::EventHandler(this, &Interface::add_calendar_Click);
			// 
			// personal_classroom_panel
			// 
			this->personal_classroom_panel->AutoScroll = true;
			this->personal_classroom_panel->Location = System::Drawing::Point(60, 62);
			this->personal_classroom_panel->Name = L"personal_classroom_panel";
			this->personal_classroom_panel->Size = System::Drawing::Size(1000, 660);
			this->personal_classroom_panel->TabIndex = 18;
			// 
			// bot_line
			// 
			this->bot_line->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)), static_cast<System::Int32>(static_cast<System::Byte>(236)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)));
			this->bot_line->Location = System::Drawing::Point(18, 702);
			this->bot_line->Name = L"bot_line";
			this->bot_line->Size = System::Drawing::Size(43, 2);
			this->bot_line->TabIndex = 19;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label17->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label17->Location = System::Drawing::Point(13, 660);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(44, 17);
			this->label17->TabIndex = 17;
			this->label17->Text = L"22h00";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label16->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label16->Location = System::Drawing::Point(13, 625);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(40, 17);
			this->label16->TabIndex = 16;
			this->label16->Text = L"21h10";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label15->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label15->Location = System::Drawing::Point(13, 590);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(42, 17);
			this->label15->TabIndex = 15;
			this->label15->Text = L"20h15";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label14->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label14->Location = System::Drawing::Point(13, 555);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(42, 17);
			this->label14->TabIndex = 14;
			this->label14->Text = L"19h25";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label13->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label13->Location = System::Drawing::Point(13, 520);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(42, 17);
			this->label13->TabIndex = 13;
			this->label13->Text = L"18h35";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label12->Location = System::Drawing::Point(13, 485);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(42, 17);
			this->label12->TabIndex = 12;
			this->label12->Text = L"17h45";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label11->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label11->Location = System::Drawing::Point(13, 450);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(42, 17);
			this->label11->TabIndex = 11;
			this->label11->Text = L"16h30";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label10->Location = System::Drawing::Point(13, 415);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(42, 17);
			this->label10->TabIndex = 10;
			this->label10->Text = L"15h40";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label9->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label9->Location = System::Drawing::Point(13, 380);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(42, 17);
			this->label9->TabIndex = 9;
			this->label9->Text = L"14h50";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label8->Location = System::Drawing::Point(13, 345);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(42, 17);
			this->label8->TabIndex = 8;
			this->label8->Text = L"13h50";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label7->Location = System::Drawing::Point(13, 310);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(42, 17);
			this->label7->TabIndex = 7;
			this->label7->Text = L"13h00";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label6->Location = System::Drawing::Point(15, 275);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(42, 17);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Noon";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label5->Location = System::Drawing::Point(13, 240);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(40, 17);
			this->label5->TabIndex = 5;
			this->label5->Text = L"11h00";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label4->Location = System::Drawing::Point(13, 205);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(40, 17);
			this->label4->TabIndex = 4;
			this->label4->Text = L"10h10";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label3->Location = System::Drawing::Point(20, 170);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(37, 17);
			this->label3->TabIndex = 3;
			this->label3->Text = L"9h20";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label2->Location = System::Drawing::Point(20, 135);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(37, 17);
			this->label2->TabIndex = 2;
			this->label2->Text = L"8h20";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Small", 9.75F));
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(177)),
				static_cast<System::Int32>(static_cast<System::Byte>(182)));
			this->label1->Location = System::Drawing::Point(20, 100);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(37, 17);
			this->label1->TabIndex = 1;
			this->label1->Text = L"7h30";
			// 
			// global_classroom_panel
			// 
			this->global_classroom_panel->AutoScroll = true;
			this->global_classroom_panel->Location = System::Drawing::Point(60, 62);
			this->global_classroom_panel->Name = L"global_classroom_panel";
			this->global_classroom_panel->Size = System::Drawing::Size(1000, 660);
			this->global_classroom_panel->TabIndex = 19;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label24->ForeColor = System::Drawing::SystemColors::ActiveBorder;
			this->label24->Location = System::Drawing::Point(2, 2);
			this->label24->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(84, 17);
			this->label24->TabIndex = 2;
			this->label24->Text = L"TeaCalendar";
			// 
			// X
			// 
			this->X->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->X->FlatAppearance->BorderSize = 0;
			this->X->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Red;
			this->X->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->X->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->X->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI Light", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->X->ForeColor = System::Drawing::SystemColors::AppWorkspace;
			this->X->Location = System::Drawing::Point(1343, -2);
			this->X->Name = L"X";
			this->X->Size = System::Drawing::Size(22, 22);
			this->X->TabIndex = 3;
			this->X->Text = L"✕";
			this->X->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI Light", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::SystemColors::AppWorkspace;
			this->button1->Location = System::Drawing::Point(1320, -2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(22, 22);
			this->button1->TabIndex = 4;
			this->button1->Text = L"–";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// Interface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)), static_cast<System::Int32>(static_cast<System::Byte>(236)),
				static_cast<System::Int32>(static_cast<System::Byte>(240)));
			this->ClientSize = System::Drawing::Size(1365, 740);
			this->Controls->Add(this->Login_loadingPanel);
			this->Controls->Add(this->loginPanel);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->X);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->main_personal_panel);
			this->Controls->Add(this->Control_panel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"Interface";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TeaCalendar";
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Interface::Interface_MouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Interface::Interface_MouseMove);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Interface::Interface_MouseUp);
			this->Control_panel->ResumeLayout(false);
			this->Control_panel->PerformLayout();
			this->panel4->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->freetime_panel->ResumeLayout(false);
			this->freetime_panel->PerformLayout();
			this->add_calendar_panel->ResumeLayout(false);
			this->add_calendar_panel->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->loginPanel->ResumeLayout(false);
			this->loginPanel->PerformLayout();
			this->panel7->ResumeLayout(false);
			this->panel7->PerformLayout();
			this->panel6->ResumeLayout(false);
			this->panel6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Login_loadingPanel))->EndInit();
			this->main_personal_panel->ResumeLayout(false);
			this->main_personal_panel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Calendar_loadingPanel))->EndInit();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		//
		bool dragging;
		Point offset;
	private: System::Void Interface_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		dragging = true;
		offset.X = e->X;
		offset.Y = e->Y;
	}
	private: System::Void Interface_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (dragging) {
			Point curren = PointToScreen(Point(e->X, e->Y));
			Location = Point(curren.X - offset.X, curren.Y - offset.Y);
		}
	}
	private: System::Void Interface_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		dragging = false;
	}
		   //

	private: System::Void personal_button_Click(System::Object^ sender, System::EventArgs^ e) {
		swtich = true;
		refreshPanel();
		global_classroom_panel->Hide();
		personal_classroom_panel->Show();
	}
	private: System::Void global_button_Click(System::Object^ sender, System::EventArgs^ e) {
		swtich = false;
		refreshPanel();
		personal_classroom_panel->Hide();
		global_classroom_panel->Show();
	}
	private: System::Void add_calendar_Click(System::Object^ sender, System::EventArgs^ e) {
		freetime_panel->Location = System::Drawing::Point(7, 470);
		timeStart->Value = System::DateTime(_year, _month, _day, 0, 0, 0, 0);
		timeEnd->MinDate = System::DateTime(_year, _month, _day, 0, 0, 0, 0);
		add_calendar_panel->Show();
		add_calendar->Visible = false;
	}
	private: System::Void close_calendar_panel_Click(System::Object^ sender, System::EventArgs^ e) {
		add_calendar_panel->Hide();
		add_calendar->Visible = true;
		freetime_panel->Location = System::Drawing::Point(7, 300);
	}
	private: System::Void search_room_Click(System::Object^ sender, System::EventArgs^ e) {
		getOffer();
	}
	private: System::Void book_button_Click(System::Object^ sender, System::EventArgs^ e) {
		Calendar_loadingPanel->Show();
		Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::calendarLoading));
		thread->Start();
		user->saveCalendar(this->timeStart->Text->Replace(":", "h") + "-" + this->timeEnd->Text->Replace(":", "h"),
			comboBox_recommend->Text,
			System::Convert::ToString(date_to_add_calendar->Value.Date.Month + "-" +
				date_to_add_calendar->Value.Date.Day + "-" +
				date_to_add_calendar->Value.Date.Year));
		refreshPanel();

		timeStart->Value = System::DateTime(_year, _month, _day, 0, 0, 0, 0);
		timeEnd->MinDate = System::DateTime(_year, _month, _day, 0, 0, 0, 0);
	}
	private: System::Void thedayAfter_Click(System::Object^ sender, System::EventArgs^ e) {
		dateTimePicker->Value = dateTimePicker->Value.Date.AddDays(1);

		//Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::DatePanel));
		//thread->Start();
		//thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::calendarLoading));
		//thread->Start();

		if (followTime->Checked && ((_year == dateTimePicker->Value.Date.Year && _month == dateTimePicker->Value.Date.Month && _day <= dateTimePicker->Value.Date.Day) || (_year < dateTimePicker->Value.Date.Year && _month < dateTimePicker->Value.Date.Month))) {
			date_to_add_calendar->Value = System::DateTime(dateTimePicker->Value.Date.Year, dateTimePicker->Value.Date.Month, dateTimePicker->Value.Date.Day, 0, 0, 0, 0);
		}
		refreshPanel();
	}
	private: System::Void thedayBefore_Click(System::Object^ sender, System::EventArgs^ e) {
		dateTimePicker->Value = dateTimePicker->Value.Date.AddDays(-1);
		Calendar_loadingPanel->Show();

		//Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::DatePanel));
		//thread->Start();
		//thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::calendarLoading));
		//thread->Start();

		if (followTime->Checked && ((_year == dateTimePicker->Value.Date.Year && _month == dateTimePicker->Value.Date.Month && _day <= dateTimePicker->Value.Date.Day) || (_year < dateTimePicker->Value.Date.Year && _month < dateTimePicker->Value.Date.Month))) {
			date_to_add_calendar->Value = System::DateTime(dateTimePicker->Value.Date.Year, dateTimePicker->Value.Date.Month, dateTimePicker->Value.Date.Day, 0, 0, 0, 0);
		}
		refreshPanel();
	}
	private: System::Void dateTimePicker_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		Calendar_loadingPanel->Show();
		Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::DatePanel));
		thread->Start();

		thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::calendarLoading));
		thread->Start();

		refreshPanel();
	}
	private: System::Void comboBox_recommend_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		book_button->Show();
	}
	private: System::Void date_to_add_calendar_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		if (followTime->Checked && (date_to_add_calendar->Value.Date.Year != dateTimePicker->Value.Date.Year || date_to_add_calendar->Value.Date.Month != dateTimePicker->Value.Date.Month || date_to_add_calendar->Value.Date.Day != dateTimePicker->Value.Date.Day)) {
			dateTimePicker->Value = System::DateTime(date_to_add_calendar->Value.Date.Year, date_to_add_calendar->Value.Date.Month, date_to_add_calendar->Value.Date.Day, 0, 0, 0, 0);
			//Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::DatePanel));
			//thread->Start();
			Calendar_loadingPanel->Show();
			//thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::calendarLoading));
			//thread->Start();

			refreshPanel();
			freeTimeBox(date_to_add_calendar->Value.Date.Month + "-" +
				date_to_add_calendar->Value.Date.Day + "-" +
				date_to_add_calendar->Value.Date.Year);
		}
	}
	private: System::Void followTime_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		if (followTime->Checked && (date_to_add_calendar->Value.Date.Year != dateTimePicker->Value.Date.Year || date_to_add_calendar->Value.Date.Month != dateTimePicker->Value.Date.Month || date_to_add_calendar->Value.Date.Day != dateTimePicker->Value.Date.Day)) {
			dateTimePicker->Value = System::DateTime(date_to_add_calendar->Value.Date.Year, date_to_add_calendar->Value.Date.Month, date_to_add_calendar->Value.Date.Day, 0, 0, 0, 0);
			//Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::DatePanel));
			//thread->Start();
			Calendar_loadingPanel->Show();
			//thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::calendarLoading));
			//thread->Start();

			refreshPanel();
			freeTimeBox(date_to_add_calendar->Value.Date.Month + "-" +
				date_to_add_calendar->Value.Date.Day + "-" +
				date_to_add_calendar->Value.Date.Year);
		}
	}
	private: System::Void loginButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (user->userCheck(this->useridText->Text, this->passwordText->Text)) {
			add_calendar->Show();
			Login_loadingPanel->Show();
			Thread^ thread = gcnew Thread(gcnew ThreadStart(this, &TeaCalendar::Interface::loginLoading));
			thread->Start();
			if (keeploginBox->Checked) {
				user->save(this->useridText->Text, this->passwordText->Text);
			}
			onLogin();
		}
		else {
			Console::WriteLine("NULL");
			invalid1->Visible = true;
			invalid2->Visible = true;
		}
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		loading = false;
		this->runnable = false;
		personal_classroom_panel->Controls->Clear();
		global_classroom_panel->Controls->Clear();
		loginPanel->Show();
		user->del();
	}
};
}

