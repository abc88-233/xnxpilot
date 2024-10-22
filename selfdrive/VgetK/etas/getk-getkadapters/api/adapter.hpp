/*
 * @copyright (c) 2022, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * The GETK Adapter Interface.
 */

#pragma once
/* *** Local include files *** */
#include "etas/base/err.h"
#include "etas/getk-getkadapters/api/datasender.hpp"
#include "etas/getk-getkadapters/api/sotpasyncreqrep.hpp"
#include "etas/getk-getkadapters/api/ecubuffer.hpp"
#include "etas/getk-getkadapters/api/chunklayout.hpp"
#include "etas/getk-getkadapters/api/clockaccess.hpp"
#include "etas/getk-getkadapters/api/client.hpp"

namespace etas {
namespace getk {
namespace api {

class IGetkAdapter : public IDataSender, public ISoTpAsyncResponse {
public:
  /**
   * Initializes the GetkAdapter.
   *
   * @return  Zero in case of successful completion of the operation, an error
   * number otherwise.
   */
  virtual ErrorCodeT init() = 0;

  /**
   * Releases the GetkAdapter.
   *
   * @return  Zero in case of successful completion of the operation, an error
   * number otherwise.
   */
  virtual ErrorCodeT release() = 0;

  /**
   * @brief Opens the GetkAdapter and checks if provided configuration is valid.
   * The method open() shall only be called after the GetkAdapter has been initialized.
   * With the V-GETK, all arguments will be ignored.
   * 
   * @param[in] segments  A list of memory segments with pools of memory chunks to be measured.
   * @param[in] client    Unique identifier of the middleware client.
   *
   * @return  Zero in case of the presence of a valid configuration, an error number otherwise
   */
  virtual ErrorCodeT open(const etas::getk::types::MemSegmentListT &segments,
                          const etas::getk::types::ClientIdT &client = etas::getk::types::ClientIdT("Unspecified")) = 0;

  /**
   * Closes the GetkAdapter.
   * The method close() shall only be called when the GetkAdapter is opened.
   *
   * @return  Zero in case of successful completion of the operation, an error
   * number otherwise.
   */
  virtual ErrorCodeT close() = 0;

  /**
   * Method that shall be called cyclically on the GetkAdapter.
   * A call to cyclic() will trigger the configuration requests to be executed.
   * It helps to synchronize the request and therefore supports thread-safe
   * implementation.
   *
   * @return  Zero in case of successful completion of the operation, an error
   * number otherwise.
   */
  virtual ErrorCodeT cyclic() = 0;

  /**
   * Registers a callback for the interface ISoTpAsyncRequest.
   * The callback has to be registered before initialization or after release of
   * the GetkAdapter.
   *
   * @param[in]   callback    Pointer to the interface ISoTpAsyncRequest.
   * @return  Zero in case of successful completion of the operation, an error
   * number otherwise.
   */
  virtual ErrorCodeT setCallback(api::ISoTpAsyncRequest *callback) = 0;

  /**
   * Registers a callback for the interface IDataSenderCallback.
   * The callback has to be registered before initialization or after release of
   * the GetkAdapter.
   *
   * @param[in]   callback    Pointer to the interface IDataSenderCallback.
   * @return  Zero in case of successful completion of the operation, an error
   * number otherwise.
   */
  virtual ErrorCodeT setCallback(api::IDataSenderCallback *callback) = 0;

  /**
   * @brief Registers a callback for the interface IClockAccess.
   * If no callback is registered, no optional time related information will be
   * included in the recorded measurement file. This is useful if the ECU and
   * GETK are in the same time domain and the measurement payload already stores
   * time information or in case of a VGETK. The callback has to be registered
   * before initialization or after release of the GetkAdapter.
   *
   * @param[in]   callback    Pointer to the interface IClockAccess.
   * @return      Zero in case of successful completion of the operation, an
   * error number otherwise.
   */
  virtual ErrorCodeT setCallback(api::IClockAccess *callback) = 0;

  /**
   * @brief Overrides the default URI for the transport layer connection
   * Not required in normal use-cases
   * Only relevant for GETK enabled ECUs, will return EC_ERR_NOT_IMPLEMENTED for
   * VGETK ECUs
   *
   * @param uri the URI to use
   * @return ErrorCodeT EC_OK (0) on success, error code otherwise.
   */
  virtual ErrorCodeT setTransportURI(std::string &uri) = 0;
};

} // namespace api
} // namespace getk
} // namespace etas
