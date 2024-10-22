/*
 * @copyright (c) 2021, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
#pragma once

#ifndef BASE_API
    #if defined(_MSC_VER)
        #ifdef BASE_EXPORT
            #define BASE_API __declspec(dllexport)
        #else
            #define BASE_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__)
        #define BASE_API __attribute__((visibility("default")))
    #elif defined(SWIG)
        #define BASE_API
    #else
        #error Unsupported platform
    #endif
#endif

