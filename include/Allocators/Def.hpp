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

#if defined (_WIN32)
#define FUNCTION_NAME __FUNCDNAME__
#else
#define FUNCTION_NAME __PRETTY_FUNCTION__
#endif


#ifdef NDEBUG
#define DEB(x)
#else // NDEBUG
#include <iostream>

#define DEB(x) std::cout << x << ' ' << __LINE__ << '@' << FUNCTION_NAME << std::endl;
#endif // NDEBUG
