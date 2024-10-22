/*
* @copyright (c) 2018, ETAS GmbH, Stuttgart. All rights reserved.
* Unauthorized reproduction of this file, in whole or in part, is prohibited.
*/
/**
* @file
* The GETK Adapter provides interfaces for the middleware adaption.
*/
#pragma once

/* *** System include files *** */
#include <cstdint>
#include <memory>

/* *** Local include files *** */
#include "etas/getk-getkadapters/api/sotpasyncreqrep.hpp"
#include "etas/getk-getkadapters/api/datasender.hpp"
#include "etas/getk-getkadapters/api/adapter.hpp"
#include "etas/getk-getkadapters/api/chunklayout.hpp"
#include "etas/getk-getkadapters/api/ecubuffer.hpp"
#include "etas/getk-getkadapters/api/clockaccess.hpp"
#include "etas/base/err.h"

namespace etas {
namespace getk {

/* *** Forward declarations *** */
class VGetkAdapterImpl;

/**
* @class GetkAdapter
* The GETK adapter realizes the interface between the AOS MTA Gateway and the GETK.
*/
class VGetkAdapter : public api::IGetkAdapter
{
private:
    std::unique_ptr<VGetkAdapterImpl> m_impl;

    VGetkAdapter(const VGetkAdapter&) = delete;
    VGetkAdapter& operator=(const VGetkAdapter&) = delete;

public:
    VGetkAdapter();
    virtual ~VGetkAdapter();

    /**
     * Initializes the GetkAdapter.
     *
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT init() override;

    /**
     * Releases the GetkAdapter.
     *
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT release() override;

    /**
     * Opens the GetkAdapter, and checks for a valid configuration.
     * The method open() shall only be called when the GetkAdapter is initialized.
     * With the V-GETK, all arguments will be ignored.
     *
     * @return  Zero in case of the presence of a valid configuration, an error number otherwise
     */
    ErrorCodeT open(const types::MemSegmentListT& segments, 
                    const types::ClientIdT& client = etas::getk::types::ClientIdT("Unspecified")) override;

    /**
     * Closes the GetkAdapter.
     * The method close() shall only be called when the GetkAdapter is opened.
     *
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT close() override;

    /**
     * Method that shall be called cyclically on the GetkAdapter.
     * A call to cyclic() will trigger the configuration requests to be executed. It helps to
     * synchronize the request and therefore supports thread-safe implementation.
     *
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT cyclic() override;

    /**
     * Registers a callback for the interface ISoTpAsyncRequest.
     * The callback has to be registered before initialization or after release of the GetkAdapter.
     *
     * @param[in]   callback    Pointer to the interface ISoTpAsyncRequest.
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT setCallback(api::ISoTpAsyncRequest* callback) override;

    /**
     * Registers a callback for the interface IDataSenderCallback.
     * The callback has to be registered before initialization or after release of the GetkAdapter.
     *
     * @param[in]   callback    Pointer to the interface IDataSenderCallback.
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT setCallback(api::IDataSenderCallback* callback) override;

    /**
     * @brief Registers a callback for the interface IClockAccess.
     * If no callback is registered, no optional time related information will be included in the
     * recorded measurement file. This is useful if the ECU and GETK are in the same time domain and
     * the measurement payload already stores time information or in case of a VGETK.
     * The callback has to be registered before initialization or after release of the GetkAdapter.
     *
     * @param[in]   callback    Pointer to the interface IClockAccess.
     * @return      Zero in case of successful completion of the operation, an error number otherwise.
     */
    ErrorCodeT setCallback(api::IClockAccess* callback) override;

    /**
     * @brief Overrides the default URI for the transport layer connection
     * Not required in normal use-cases
     * Only relevant for GETK enabled ECUs, will return EC_ERR_NOT_IMPLEMENTED for VGETK ECUs
     *
     * @param uri the URI to use
     * @return ErrorCodeT EC_OK (0) on success, error code otherwise.
     */
    ErrorCodeT setTransportURI(std::string &uri) override;

    // Inherited from ISoTpAsyncResponse
    ErrorCodeT offer(const types::ServiceDescListT& srvDescList) override;
    ErrorCodeT offer(const types::ServiceDescT& srvDesc) override;
    ErrorCodeT stopOffer(const types::ServiceDescListT& srvDescList) override;
    ErrorCodeT stopOffer(const types::ServiceDescT& srvDesc) override;
    ErrorCodeT updateServiceState(const types::ServiceDescExtListT& srvDescExtList) override;
    ErrorCodeT updateServiceState(const types::ServiceDescExtT& srvDescExt) override;
    ErrorCodeT requestContentCompleted(const types::ContextIdT contextId) override;
    ErrorCodeT reportError(types::ErrIdT errId, types::ErrStrT errStr) override;
   
    // Inherited from IDataSender
    ErrorCodeT sendDataBuffer(const api::DataInfoT &data) override;
    ErrorCodeT notifyOnDataLoss(etas::getk::types::ServiceIntT, etas::getk::types::ContextIdT, uint64_t, uint64_t) override;

    ErrorCodeT flush() override;

};

} // getk
} // etas
