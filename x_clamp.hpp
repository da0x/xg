//
//  x_clamp.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#pragma once

namespace x
{
	template<typename _T> const _T& clamp(const _T& value, const _T& min, const _T& max)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}
}

