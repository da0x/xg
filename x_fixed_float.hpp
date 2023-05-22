//
//  x.cpp
//  x
//
//  Created by Daher Alfawares on 6/2/19.
//  Copyright © 2019 Daher Alfawares. All rights reserved.
//

#pragma once

namespace x
	{
	class fixed_float {
		const float & f;
		const int p;
	public:
		fixed_float(const float& floatValue, const int precision = 2) :
			f(floatValue), p(precision) {}
		const float& value() const { return f; }
		const int& precision() const { return p; }
		};
	}

#include <iomanip>

inline
std::ostream& operator << (std::ostream& stream, const x::fixed_float& f)
	{
	return stream << std::fixed << std::setprecision(f.precision()) << f.value();
	}

