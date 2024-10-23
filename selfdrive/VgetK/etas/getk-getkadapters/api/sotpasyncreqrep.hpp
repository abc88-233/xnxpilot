/**
 * @copyright (c) 2018, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
* @file
* @brief Declaration of the related classes ISoTpAsyncRequest and ISoTpAsyncResponse.
*/
#pragma once

/* *** System include files *** */
#include <cstdint>

/* *** Local include files *** */
#include "etas/upinterface-stubs/sotpasynctypes.hpp"
#include "etas/base/err.h"
#include "cereal/services.h"
#include"cereal/messaging/messaging.h"
namespace etas {
namespace getk {
namespace api {
/**
 * @class ISoTpAsyncRequest
 * Service-oriented transport asynchronous request interface for asynchonous communication between the
 * middleware adaption code and the GETK transport.
 */
class ISoTpAsyncRequest
{
public:


    virtual ~ISoTpAsyncRequest() {}

    /**
     * Find specific services based on their temporary integer type representation.
     *
     * @param[in]   srvStrList  List of services to find by name.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT find(const types::ServiceStrListT& srvStrList) = 0;

    /**
     * Find all running services.
     *
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT findAll() = 0;

    /**
     * Subscribe to specific services.
     *
     * @param[in]   mpdSrvList  List of services that shall be considered.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT subscribe(const types::MappedServiceListT& mpdSrvList) = 0;

    /**
     * Unsubscribe specific services.
     *
     * @param[in]   mpdSrvList  List of services that shall be considered.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT unsubscribe(const types::MappedServiceListT& mpdSrvList) = 0;

    /**
     * Unsubscribe all services.
     *
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT unsubscribeAll() = 0;

    /**
     * Request sending of specific content (e.g. field data, metadata)
     *
     * @param[in]   contextId   Context used for metadata transfer
     * @param[in]   srvIntList  A list (may be empty) of services to request content for
     * @param[in]   type        Type of content requested
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT requestContent(const types::ContextIdT contextId, const types::ServiceIntListT& srvIntList, const types::ContentTypeT type) = 0;
};

/**
 * @class ISoTpAsyncResponse
 * Service-oriented transport asynchronous response interface for asynchonous communication between the
 * middleware adaption code and the GETK transport.
 */
class ISoTpAsyncResponse
{
public:
    Poller * poller = Poller::create();
    std::map<std::string,SubSocket*> registeredSockets;
    
    virtual ~ISoTpAsyncResponse() {}

    /**
     * Provide information about services that are (currently) running.
     *
     * @param[in]   srvDescList List holding offered services.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT offer(const types::ServiceDescT& srvDesc) = 0;

    /**
     * Provide information about services that are (currently) running.
     *
     * @param[in]   srvDescList List holding offered services.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT offer(const types::ServiceDescListT& srvDescList) = 0;

    /**
     * Provide information about (recently) stopped services.
     *
     * @param[in]   srvDescList List holding stopped services.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT stopOffer(const types::ServiceDescT& srvDesc) = 0;

    /**
     * Provide information about (recently) stopped services.
     *
     * @param[in]   srvDescList List holding stopped services.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT stopOffer(const types::ServiceDescListT& srvDescList) = 0;

    /**
     * Provide updated information, if the status of a service has changed.
     *
     * @param[in]   srvDescExt  Updated service information, e.g. a successful subscription.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT updateServiceState(const types::ServiceDescExtT& srvDescExt) = 0;   
    /**
     * Provide updated information, if the status of a service has changed.
     *
     * @param[in]   srvDescExtList  List holding updated information, e.g. a successful subscription.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT updateServiceState(const types::ServiceDescExtListT& srvDescExtList) = 0;

    /**
     * The sending of metadata is completed by the ECU and the GETK can invalidate the contextId.
     *
     * param[in] contextId The contextId that shall be invalidated
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT requestContentCompleted(const types::ContextIdT contextId) = 0;

    /**
     * Report an error.
     *
     * @param[in]   errId   An Id describing the error.
     * @param[in]   errStr  A string with detailed information regarding the error.
     * @return Zero in case of successful completion of the operation, an error number to indicate a failure otherwise.
     */
    virtual ErrorCodeT reportError(types::ErrIdT errId, types::ErrStrT errStr) = 0;
};

} // namespace api
} // namespace getk
} // namespace etas
