#pragma once

#include "compiler.hpp"
#include "scheme_value.hpp"

#include <string>
#include <iostream>

class Fixnum : public Atom 
{
private:

	std::string _digits;
	bool _neg;

	#if !TEST
	Fixnum(const std::string &d, bool n=false) : _digits(d), _neg(n) {}
	#endif

public:

	#if TEST
	Fixnum(const std::string &d, bool n=false) : _digits(d), _neg(n) {}
	#endif

	Fixnum *add(const Fixnum *rhs) const;
	Fixnum *sub(const Fixnum *rhs) const;
	std::ostream &print(std::ostream &out);
};
