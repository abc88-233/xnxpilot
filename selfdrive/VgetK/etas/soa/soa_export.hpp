/*
 * @copyright (c) 2021, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
#pragma once

#ifndef SOA_API
    #if defined(_MSC_VER)
        #ifdef SOA_EXPORT
            #define SOA_API __declspec(dllexport)
        #else
            #define SOA_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__)
        #define SOA_API __attribute__((visibility("default")))
    #elif defined(SWIG)
        #define SOA_API
    #else
        #error Unsupported platform
    #endif
#endif

