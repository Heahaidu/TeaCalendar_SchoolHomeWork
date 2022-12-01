#pragma once
#include <cliext/vector>
#include<msclr/marshal_cppstd.h>
using namespace System::Collections::Generic;
using namespace System;
using namespace msclr::interop;

public ref class DateTimeReserve {
public:
	cliext::vector<String^>  getDate() {
		cliext::vector<String^> monTh;
		monTh.push_back("Jan");
		monTh.push_back("Feb");
		monTh.push_back("Mar");
		monTh.push_back("Apr");
		monTh.push_back("May");
		monTh.push_back("Jun");
		monTh.push_back("Jul");
		monTh.push_back("Aug");
		monTh.push_back("Sep");
		monTh.push_back("Oct");
		monTh.push_back("Nov");
		monTh.push_back("Dec");
		return monTh;
	}
};