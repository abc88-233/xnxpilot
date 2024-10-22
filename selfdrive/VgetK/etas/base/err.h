/*
 * @copyright (c) 2019, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * Common error codes.
 */
#ifndef ERR_H_
#define ERR_H_

/* *** System include files *** */
#include <stdint.h>

/* *** Local include files *** */
#include "etas/base/base_export.hpp"

/* *** Type definitions *** */
typedef uint32_t ErrorCodeT;

enum hadMtErrors
{
    ENUM_EC_OK,
    ENUM_EC_ERR_INVALID_PARAMETER,
    ENUM_EC_ERR_INVALID_HANDLE,
    ENUM_EC_ERR_BUFFER_OVERFLOW,
    ENUM_EC_ERR_UNSUPPORTED,
    ENUM_EC_ERR_INCOMPATIBLE,
    ENUM_EC_ERR_INVALID_STATE,
    ENUM_EC_ERR_INVALID_CONTEXT,
    ENUM_EC_ERR_UNAVAILABLE,
    ENUM_EC_ERR_NO_MEM,
    ENUM_EC_ERR_NO_RESOURCES,
    ENUM_EC_ERR_OBJECT_IN_USE,
    ENUM_EC_ERR_OBJECT_EXISTS,
    ENUM_EC_ERR_NOT_IMPLEMENTED,
    ENUM_EC_ERR_TIMEOUT,
    NOT_DEFINED,
    ENUM_EC_ERR_HW_NOT_FOUND,
    ENUM_EC_ERR_NO_CONNECTION,
    ENUM_EC_ERR_PERMISSION,
    ENUM_EC_ERR_AGAIN,
    ENUM_EC_ERR_INVALID_CONFIG,
    ENUM_EC_ERR_FILE_DOES_NOT_EXIST,
    ENUM_EC_ERR_EMPTY,
    ENUM_EC_ERR_ECU_UNAVAILABLE,
    ENUM_EC_ERR_ECU_HANDSHAKE_FAILURE,
    ENUM_EC_ERR_FPGA_UNAVAILABLE,
    ENUM_EC_ERR_IP_PROTECTION,
    HADMT_NUMBER_OF_ERRORS,
    ENUM_EC_ERR_INTERNAL = 0xFFFF
};

/* *** Preprocessor definitions *** */
// General error code classifications.
#define EC_OK 0x00000000u
#define EC_WARNING 0x40000000u
#define EC_ERROR 0x80000000u

/** Generic failure test for given error code or expression.*/
#define EC_FAILED(code) ((code) & EC_ERROR)

/** Generic success test for given error code or expression.*/
#define EC_SUCCEEDED(code) (((code) & EC_ERROR) == 0)

// Error codes.
/** At least one parameter is invalid. */
#define EC_ERR_INVALID_PARAMETER (EC_ERROR | ENUM_EC_ERR_INVALID_PARAMETER)
/** Provided handle is not valid. */
#define EC_ERR_INVALID_HANDLE (EC_ERROR | ENUM_EC_ERR_INVALID_HANDLE)
/** The data was too large to fit into the specified buffer. */
#define EC_ERR_BUFFER_OVERFLOW (EC_ERROR | ENUM_EC_ERR_BUFFER_OVERFLOW)
/** Parameter or operation is not supported. */
#define EC_ERR_UNSUPPORTED (EC_ERROR | ENUM_EC_ERR_UNSUPPORTED)
/** Incompatible parameter or configuration. */
#define EC_ERR_INCOMPATIBLE (EC_ERROR | ENUM_EC_ERR_INCOMPATIBLE)
/** Internal state is invalid for the requested operation. */
#define EC_ERR_INVALID_STATE (EC_ERROR | ENUM_EC_ERR_INVALID_STATE)
/** Internal context is invalid for the requested operation. */
#define EC_ERR_INVALID_CONTEXT (EC_ERROR | ENUM_EC_ERR_INVALID_CONTEXT)
/** Requested data is not available. */
#define EC_ERR_UNAVAILABLE (EC_ERROR | ENUM_EC_ERR_UNAVAILABLE)
/** Insufficient memory. */
#define EC_ERR_NO_MEM (EC_ERROR | ENUM_EC_ERR_NO_MEM)
/** Insufficient resources. */
#define EC_ERR_NO_RESOURCES (EC_ERROR | ENUM_EC_ERR_NO_RESOURCES)
/** Requested object is already in use. */
#define EC_ERR_OBJECT_IN_USE (EC_ERROR | ENUM_EC_ERR_OBJECT_IN_USE)
/** Caller tried to create object that already exists. */
#define EC_ERR_OBJECT_EXISTS (EC_ERROR | ENUM_EC_ERR_OBJECT_EXISTS)
/** Functionality not implemented. */
#define EC_ERR_NOT_IMPLEMENTED (EC_ERROR | ENUM_EC_ERR_NOT_IMPLEMENTED)
/** A timeout has ocurred. */
#define EC_ERR_TIMEOUT (EC_ERROR | ENUM_EC_ERR_TIMEOUT)
/** Hardware not found. */
#define EC_ERR_HW_NOT_FOUND (EC_ERROR | ENUM_EC_ERR_HW_NOT_FOUND)
/** Connection not available. */
#define EC_ERR_NO_CONNECTION (EC_ERROR | ENUM_EC_ERR_NO_CONNECTION)
/** Neccessary permission not available. */
#define EC_ERR_PERMISSION (EC_ERROR | ENUM_EC_ERR_PERMISSION)
/** Operation could not be completed, try again. */
#define EC_ERR_AGAIN (EC_ERROR | ENUM_EC_ERR_AGAIN)
/** Configuration is invalid */
#define EC_ERR_INVALID_CONFIG (EC_ERROR | ENUM_EC_ERR_INVALID_CONFIG)
/** File does not exist */
#define EC_ERR_FILE_DOES_NOT_EXIST (EC_ERROR | ENUM_EC_ERR_FILE_DOES_NOT_EXIST)
/** File or buffer is empty */
#define EC_ERR_EMPTY (EC_ERROR | ENUM_EC_ERR_EMPTY)
/** ECU unavailable */
#define EC_ERR_ECU_UNAVAILABLE (EC_ERROR | ENUM_EC_ERR_ECU_UNAVAILABLE)
/** ECU handshake failure */
#define EC_ERR_ECU_HANDSHAKE_FAILURE (EC_ERROR | ENUM_EC_ERR_ECU_HANDSHAKE_FAILURE)
/** FPGA unavailable */
#define EC_ERR_FPGA_UNAVAILABLE (EC_ERROR | ENUM_EC_ERR_FPGA_UNAVAILABLE)
/** IP protection failure */
#define EC_ERR_IP_PROTECTION (EC_ERROR | ENUM_EC_ERR_IP_PROTECTION)

/** Internal error. */
#define EC_ERR_INTERNAL (EC_ERROR | ENUM_EC_ERR_INTERNAL)

#ifdef __cplusplus
extern "C" {
#endif
BASE_API const char* strLogError(int err);
#ifdef __cplusplus
}
#endif

#endif
