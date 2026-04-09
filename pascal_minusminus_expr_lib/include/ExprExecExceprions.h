#pragma once
#include <string>
using namespace std;

class ExprExecException {
protected:
	string _message;
public:
	virtual inline string message() { return _message; }
};


class EEE_invalid_argument : public ExprExecException {
public:
	EEE_invalid_argument(string mess) { 
		this->_message = "<EEE_Invalid_argument> error: " + mess; 
	}
};