//
// Created by Kirill Delimbetov on 17.06.2018.
// GitHub: https://github.com/dKirill
// 
// Copyright © 2018 Kirill Delimbetov.
// 
// This source code is licensed under the license found in the LICENSE file
// in the root directory of this source tree.
//

#pragma once

#ifdef NDEBUG
#define DEB(x)
#else // NDEBUG
#include <iostream>

using cstr = const char * const;

static constexpr cstr past_last_slash(cstr str, cstr last_slash)
{
	return
	*str == '\0' ? last_slash :
	*str == '/'  ? past_last_slash(str + 1, str + 1) :
	past_last_slash(str + 1, last_slash);
}

static constexpr cstr past_last_slash(cstr str)
{
	return past_last_slash(str, str);
}
#define __SHORT_FILE__ ({constexpr cstr sf__ {past_last_slash(__FILE__)}; sf__;})

#define DEB(x) std::cout << x << ' ' << __LINE__ << '@' << __SHORT_FILE__ << std::endl;
#endif // NDEBUG
