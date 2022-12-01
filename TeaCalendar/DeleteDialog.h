#pragma once

#include "PersonalPanel.h"

namespace TeaCalendar {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for DeleteDialog
	/// </summary>
	public ref class DeleteDialog : public System::Windows::Forms::Form {
	private: System::Windows::Forms::Label^ label_time;


	private: System::Windows::Forms::Label^ label_date;


	public:
		bool b = false;
		String^ time;
		String^ path;
		String^ userID;
		System::Windows::Forms::Panel^ personal_classroom_panel;
		//PersonalPanel^ perPanel;

		DeleteDialog(String^ time, String^ path, System::Windows::Forms::Panel^ personal_classroom_panel, String^ userID) {
			this->time = time;
			this->path = path;
			this->userID = userID;
			//perPanel = gcnew PersonalPanel(this->personal_classroom_panel);
			this->personal_classroom_panel = personal_classroom_panel;
			InitializeComponent();

			label_date->Text = path;
			label_time->Text = time;
		}

	public: bool getPerm() {
		return this->b;
	}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DeleteDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;
	protected:
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Panel^ panel1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label_time = (gcnew System::Windows::Forms::Label());
			this->label_date = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(100, 85);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Accept";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &DeleteDialog::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(190, 85);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Deny";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &DeleteDialog::button2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->label1->Location = System::Drawing::Point(29, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(226, 16);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Are you sure want to delete calendar:";
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->panel1->Controls->Add(this->label_time);
			this->panel1->Controls->Add(this->label_date);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Location = System::Drawing::Point(5, 5);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(290, 120);
			this->panel1->TabIndex = 3;
			// 
			// label_time
			// 
			this->label_time->AutoSize = true;
			this->label_time->Location = System::Drawing::Point(137, 49);
			this->label_time->Name = L"label_time";
			this->label_time->Size = System::Drawing::Size(32, 16);
			this->label_time->TabIndex = 4;
			this->label_time->Text = L"time";
			// 
			// label_date
			// 
			this->label_date->AutoSize = true;
			this->label_date->Location = System::Drawing::Point(48, 49);
			this->label_date->Name = L"label_date";
			this->label_date->Size = System::Drawing::Size(34, 16);
			this->label_date->TabIndex = 3;
			this->label_date->Text = L"date";
			// 
			// DeleteDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(300, 130);
			this->Controls->Add(this->panel1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"DeleteDialog";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"DeleteDialog";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		Close();
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		User^ user = gcnew User;
		user->userID = this->userID;
		user->deleteCalendar(this->time, this->path);
		personal_classroom_panel->Controls->Clear();
		
		//perPanel->PersonalGet(this->path);
		b = true;

		Close();
	}
};
}
