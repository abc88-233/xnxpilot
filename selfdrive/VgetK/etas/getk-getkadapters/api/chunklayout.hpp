/**
 * @copyright (c) 2020, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * @brief Type definitions for chunk-layout in middleware (memPools and segements). 
 *        This relevant for the GETK for configuring the PCIe switch.
 */
#pragma once

/* *** System include files *** */
#include <cstdint>
#include <vector>

namespace etas {
namespace getk {
namespace types {

/**
* @struct MemPoolT
* A memory pool in the middleware consists of chunks, which all have the same size.
*/
struct MemPoolT{
    MemPoolT()
        : physAddr(0)
        , poolSize(0)
        , chunkSize(0)
        , count(0)
    {
    }
    MemPoolT(uint64_t physAddr_, uint64_t poolSize_, uint32_t chunkSize_, uint32_t count_)
        : physAddr(physAddr_)
        , poolSize(poolSize_)
        , chunkSize(chunkSize_)
        , count(count_)
    {
    }

    bool operator==(const MemPoolT& rhs) const
    {
        return ((rhs.physAddr == physAddr) && (rhs.poolSize == poolSize) && (rhs.chunkSize == chunkSize) && (rhs.count == count));
    }

    uint64_t physAddr; ///< Physical start address of memPool
    uint64_t poolSize; ///< Size of whole memPool (this includes padding bytes)
    uint32_t chunkSize; ///< Size of chunks inside memPool
    uint32_t count; ///< Number of chunks inside memPool
}; 
// Elements in the vector describe a continuous 
// block of physical memory (no holes). 
typedef std::vector<MemPoolT> MemPoolConfigT;

/**
* @struct MemSegmentT
* A segment consists of a list of memory pools with equal access restrictions.
*/
struct MemSegmentT{ 
    bool operator==(const MemSegmentT& rhs) const
    {
        return (rhs.mempoolConfig == mempoolConfig);
    }

    MemPoolConfigT mempoolConfig; ///< A segement consists of several memPools
}; 
typedef std::vector<MemSegmentT> MemSegmentListT;

} // namespace api
} // namespace getk
} // namespace etas
