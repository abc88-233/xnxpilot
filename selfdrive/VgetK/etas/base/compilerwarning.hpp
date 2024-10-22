/*
 * @copyright (c) 2022, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
#pragma once

#if defined(_MSC_VER)
#define DISABLE_MSVC_WARNING_ONCE(number) __pragma(warning(suppress: number))
#else
#define DISABLE_MSVC_WARNING_ONCE(number)
#endif
