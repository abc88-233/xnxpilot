/*
 * @copyright (c) 2019, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * Event unsubscription interface declaration.
 */
#pragma once

/* *** System include files *** */
#include <stdint.h>

/* *** Local include files *** */
#include "etas/base/err.h"
#include "etas/soa/soa_export.hpp"

namespace etas {

/// Event subscription handle.
struct SubscriptionHandleT
{
    void *handle;
};

/**
 * @class IEventUnsubscription
 * Interface declaration for event unsubscription.
 */
class SOA_API IEventUnsubscription
{
public:
    /**
     * Unsubscribe for event notification.
     *
     * @param[in] handle The handle to unsubscribe from.
     * @return  Zero in case of successful completion of the operation, an error number otherwise.
     */
    virtual ErrorCodeT unsubscribe(SubscriptionHandleT handle) = 0;
    virtual ~IEventUnsubscription() = default;
};

}