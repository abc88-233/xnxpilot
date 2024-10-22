/**
 * @copyright (c) 2021, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * @brief Declaration of class to support access to time functions.
 */
#pragma once

/* *** System include files *** */
#include <cstdint>
#include <string>

/* *** Local include files *** */

namespace etas {
namespace getk {
namespace api {

/**
 * @class IClockAccess
 */
class IClockAccess
{
public:
    virtual ~IClockAccess() {}

    /**
     * @param[out]   timestamp  Current time, in the same time domain and units as provided to the IDataSender Interface.
     */
    virtual void getCurrentTime(uint64_t& timestamp) = 0;
};

} // namespace api
} // namespace getk
} // namespace etas
