#include "fixnum.hpp"

std::ostream &Fixnum::print(std::ostream &out)
{
	if (_neg) {
		out << '-';
	}

	out << _digits;

	return out;
}

Fixnum *Fixnum::add(const Fixnum *rhs) const
{
	int max_len = _digits.size() > rhs->_digits.size() 
				? _digits.size()
				: rhs->_digits.size();
	std::string result(max_len + 1, '0');
	std::string::const_reverse_iterator 
		lhs_it = _digits.rbegin(); 
	  , rhs_it = rhs->_digits.rbegin();
	int sum, carry = 0;

	if ((_neg || rhs->_neg) && !(_neg && rhs->_neg)) {
		return sub(rhs);
	}

	for (std::string::reverse_iterator res_it = result.rbegin()
	   ; res_it != result.rend()
	   ; res_it++) {
		sum = carry;
		if (lhs_it != _digits.rend()) {
			sum += *lhs_it - '0';
			lhs_it++;
		}
		if (rhs_it != rhs->_digits.rend()) {
			sum += *s_it - '0';
			rhs_it++;
		}
		*res_it += (sum % 10);
		carry = sum > 9 ? 1 : 0;
	}

	return new Fixnum(result[0] == '0' ? result.substr(1) : result, _neg);
}

Fixnum *Fixnum::sub(const Fixnum *rhs) const
{
	const std::string *big, *lil;
	int comp;

	if (_digits.size() > rhs->_digits.size()) {
		big = &_digits;
		lil = &rhs->_digits;
	} else if (_digits.size() < rhs->_digits.size()) {
		big = &rhs->_digits;
		lil = &_digits;
	} else if ((comp= _digits.compare(rhs->_digits)) > 0) {
		big = &_digits;
		lil = &rhs->_digits;
	} else if (comp < 0) {
		big = &rhs->_digits;
		lil = &_digits;
	} else {
		return new Fixnum("0");
	}

	int diff, x;
	bool borrow = false, neg = big != &_digits;
	std::string::const_reverse_iterator 
		b_it = big->rbegin()
	  , l_it = lil->rbegin();
	std::string result(big->size(), '0');
	
	for (std::string::reverse_iterator r_it = result.rbegin()
	   ; r_it != result.rend()
	   ; r_it++, b_it++) {
		diff = (*b_it - '0') - borrow;
		if (l_it != lil->rend()) {
			x = *l_it - '0';
			l_it++;
		} else {
			x = 0;
		}	
		borrow = diff < x;
		diff = diff + (borrow ? 10 : 0) - x;
		*r_it += diff;
	}

	for (int i = 0; i < result.size(); i++) {
		if (result[i] != '0') {
			return new Fixnum(result.substr(i), neg);
		}
	}

	return NULL;

}
	

			
		
