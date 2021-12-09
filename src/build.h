/**
 * @file build.h.h
 * @brief common defines for the build
 *
 * Copyright (c) 2015 - 2021 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifdef _MSC_VER
#  ifndef STATIC
#    define LIBYANG_API_IMPL __declspec(dllexport)
#    ifdef LIBYANG_BUILD
#      define LIBYANG_API __declspec(dllexport)
#    else
#      define LIBYANG_API __declspec(dllimport)
#    endif
#  endif
#else
/*
 * If the compiler supports attribute to mark objects as hidden, mark all
 * objects as hidden and export only objects explicitly marked to be part of
 * the public API.
 */
#  define LIBYANG_API_IMPL __attribute__((visibility("default")))
#  define LIBYANG_API
#endif

