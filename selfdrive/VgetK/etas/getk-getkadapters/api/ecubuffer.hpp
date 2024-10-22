/**
 * @copyright (c) 2020, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * @brief Memory in middleware which is used as a buffer by the GETK.
 */
#pragma once

/* *** System include files *** */
#include <cstdint>

namespace etas {
namespace getk {
namespace types {

/**
* @struct EcuBufferT
* Buffer in ECU used by GETK.
*/
struct EcuBufferT{
    EcuBufferT()
        : physAddr(0)
        , virtAddr(nullptr)
        , size(0)
    {
    }
    EcuBufferT(uint64_t physAddr_, void* virtAddr_, uint32_t size_)
        : physAddr(physAddr_)
        , virtAddr(virtAddr_)
        , size(size_)
    {
    }

    bool operator==(const EcuBufferT& rhs) const
    {
        return ((rhs.physAddr == physAddr) && (rhs.virtAddr == virtAddr) && (rhs.size == size));
    }

    uint64_t physAddr; ///< Physical start address
    void* virtAddr; ///< Virtual start address
    uint32_t size; ///< Size of buffer
};

} // namespace types
} // namespace getk
} // namespace etas
