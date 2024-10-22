/**
 * @copyright (c) 2018, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * @brief Declaration of the corresponding classes IDataSender and IDataSenderCallback.
 */
#pragma once

/* *** System include files *** */
#include <cstdint>
#include "etas/upinterface-stubs/sotpasynctypes.hpp"
#include "etas/base/err.h"
namespace etas {
namespace getk {
namespace api {

/// Handle to identify a DataInfoT object.
using DataInfoHandleT = uint64_t;

using EventPriorityT = uint8_t;


enum class SendDataBufferFlags : uint16_t
{
    none = 0x0000,
    sync = 0x0001  /**< If the sync flag is not set, the buffer will be sent asynchronously. The ECU software can reuse the buffer as soon
                        as IDataSenderCallback::releaseDataBuffer is called for the referencing dataInfo object.

                        If the flag sendBufferFlags::sync is set the buffer will be sent synchronously. The GETK system may create an
                        internal copy of the buffer. The ECU software may reuse the buffer once the context of IDataSender::SendDataBuffer
                        is left or might free the buffer if mandatory.
                    */
};



/**
 * @brief Structure containing the virtual and physical address (if known) of a buffer
 */
struct BufferAddressT
{
public:
    BufferAddressT(const void* _virtAddr, uint64_t _physAddr) : virtAddr(_virtAddr), physAddr(_physAddr)
    {}
    const void *virtAddr;
    uint64_t physAddr;
};

/**
 * @brief Description of a single buffer to be sent
 *
 */
struct BufferInfoT
{
public:
    BufferInfoT(const BufferAddressT &_address, uint32_t _size, SendDataBufferFlags _flags) : flags(_flags), address(_address), size(_size)
    {
    }
    SendDataBufferFlags flags;
    BufferAddressT address;
    uint32_t size;
};

/**
 * @brief Description of a data sample to be sent
 */
struct DataInfoT
{
    DataInfoT(types::ContentTypeT _type, types::ContextIdT _contextId,
                types::ServiceIntT _ecuEventId,
                types::TimestampT _timestamp,
                DataInfoHandleT _handle, EventPriorityT _priority = 0) :
                type(_type),
                contextId(_contextId),
                ecuEventId(_ecuEventId),
                priority(_priority),
                timestamp(_timestamp),
                dataInfoHandle(_handle)
    {}
    std::vector<BufferInfoT> buffers; // The list of buffers which comprise this data sample
    types::ContentTypeT type;                // The type of data sample
    types::ContextIdT contextId;             // The context ID used when subscribing / requesting the data channel
    types::ServiceIntT ecuEventId;           // The ECU's own ID for this data channel
    EventPriorityT priority;          // 0 = Default priority, other values are reserved
    types::TimestampT timestamp;             // ECU timestamp for this sample
    DataInfoHandleT dataInfoHandle;   // Handle to identify this sample, returned by releaseDataSample() on release.
};

/**
 * @class IDataSender
 * Interface to transmit data buffers.
 */
class IDataSender
{
public:

    static const types::ContextIdT CONTEXTID_UNUSED = 0xFFFFFFFF; /// Context ID that is used if no context is currently active.
    static const types::ServiceIntT ECUEVENTID_UNUSED = 0xFFFFFFFF; /// EcuEventID that is used as placeholder if no associated ID exists in the current context (e.g. Discovery Messages).

    virtual ~IDataSender() {}

    /**
     * @brief Pass a data buffer to the GETK system.
     *
     * The data buffer descriptors will be accumulated in the data sender. The actual transmission
     * will be initiated either according to the data senders internal algorithm (e.g. in case a
     * total data size threshold has been reached) or by calling the method flush().
     *
     * @param[in] data  DataInfo object defining the data to be sent
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT sendDataBuffer(const DataInfoT &data) = 0;

    /**
     * Initiate the transmission of the accumulated data buffers. Please note that both, synchronous and asynchronous transfers
     * are affected.
     *
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT flush() = 0;

    /**
     * @brief Notify GETK system on a loss of ECU data buffers
     *
     * If the ECU software is not able to send a data buffer, it can call the method to update the information
     * in the GETK status monitoring.
     *
     * @param[in] ecuEventId    ECU event identifier integer representation.
     * @param[in] contextId     Context identifier given for the respective buffer.
     * @param[in] numberOfLostDataBuffers Number of lost data buffers
     * @param[in] numberOfLostBytes Number of lost bytes
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT notifyOnDataLoss(types::ServiceIntT ecuEventId, types::ContextIdT contextId, uint64_t numberOfLostDataBuffers, uint64_t numberOfLostBytes) = 0;
};

/**
 * @brief Description of a DataBufferT object released by the GETK system
 */
struct ReleasedDataInfoT
{
    types::ServiceIntT ecuEventId;
    ErrorCodeT errorCode;  /// A potential error code
    DataInfoHandleT dataInfoHandle;
};

/**
 * @class IDataSenderCallback
 * Interface to free data buffers transmitted through the interface IDataSender.
 */
class IDataSenderCallback
{
public:
    virtual ~IDataSenderCallback() {}

    /**
     * @brief Release all data buffers referenced by a dataInfo handle
     *
     * Once the GETK system does not access the buffers referenced in a dataInfo object anymore it will call the method
     * to notify the ECU software to release all of these buffers. The ECU software is responsible to freeing
     * those memory buffers.
     *
     * @param[in] releasedDataInfo Information that addresses the relevant memory buffers
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT releaseDataBuffers(const ReleasedDataInfoT& releasedDataInfo) = 0;
};

} // namespace api
} // namespace getk
} // namespace etas
