/**
 * @copyright (c) 2018, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * Type definitions for the GETK API.
 */
#pragma once

/* *** System include files *** */
#include <cstdint>
#include <string>
#include <vector>

namespace etas {
namespace getk {
namespace types {

/// Error identifier type.
using ErrIdT = uint32_t;

/// Error string type.
using ErrStrT = std::string;

/// Service description string (set by ECU).
using ServiceStrT = std::string;

/// Service description ID (set by ECU).
using ServiceIntT = uint32_t;

/// ETAS context ID for service mapping.
using ContextIdT = uint32_t;

/// Type of the Id to correlate resent information with a subset of streams.
using EchoIdT = uint32_t;

/// Service description ID vector type.
using ServiceIntListT = std::vector<ServiceIntT>;

/// Service description string vector type. @todo: mandatory?
using ServiceStrListT = std::vector<ServiceStrT>;

/** Structure to contain the timestamp value for type safety.
 *  Preferred format for the timestamp is nanoseconds.
 **/
struct TimestampT
{
    uint64_t value;
};

/// Structure that maps an ETAS context ID to a service description ID
struct MappedServiceT
{
    MappedServiceT()
        : contextId(0)
        , serviceInt(0)
    {
    }
    MappedServiceT(ContextIdT contextId_, ServiceIntT serviceInt_)
        : contextId(contextId_)
        , serviceInt(serviceInt_)
    {
    }

    bool operator==(const MappedServiceT& rhs) const
    {
        return ((rhs.contextId == contextId) && (rhs.serviceInt == serviceInt));
    }

    ContextIdT contextId;
    ServiceIntT serviceInt;
};

/// Mapped service IDs vector type.
using MappedServiceListT = std::vector<MappedServiceT>;

/// Service description type.
struct ServiceDescT
{
    ServiceDescT()
        : serviceInt(0)
    {
    }

    ServiceDescT(ServiceIntT serviceInt_, const ServiceStrT& serviceStr_)
        : serviceInt(serviceInt_)
        , serviceStr(serviceStr_)
    {
    }

    ServiceDescT(const ServiceStrT& serviceStr_)
        : serviceInt(0)
        , serviceStr(serviceStr_)
    {
    }

    bool operator<(const ServiceDescT& rhs) const
    {
        return (serviceInt < rhs.serviceInt);
    }

    bool operator>(const ServiceDescT& rhs) const
    {
        return rhs < *this;
    }

    bool operator<=(const ServiceDescT& rhs) const
    {
        return !(*this > rhs);
    }

    bool operator>=(const ServiceDescT& rhs) const
    {
        return !(*this < rhs);
    }

    bool operator==(const ServiceDescT& rhs) const
    {
        return ((this->serviceInt == rhs.serviceInt) && (this->serviceStr == rhs.serviceStr));
    }

    bool operator!=(const ServiceDescT& rhs) const
    {
        return !(*this == rhs);
    }

    bool operator<(const ServiceIntT& b) const
    {
        return serviceInt < b;
    }

    bool operator<(const ServiceStrT& b) const
    {
        return serviceStr < b;
    }

    ServiceIntT serviceInt; ///< Short identifier for the service description.
    ServiceStrT serviceStr; ///< Service description string representation.
    bool isField = false;
};

/// Service description vector type.
using ServiceDescListT = std::vector<ServiceDescT>;
inline bool operator<(const ServiceIntT& a, const ServiceDescT& b)
{
    return a < b.serviceInt;
}
// inline bool operator< (const ServiceDescT& a, const ServiceIntT& b) { return a.serviceInt < b;}
inline bool operator<(const ServiceStrT& a, const ServiceDescT& b)
{
    return a < b.serviceStr;
}

/// Service state type definition.
enum class ServiceStateT
{
    UNAVAILABLE = 0, ///< Service is unavailable (i.e. not running).
    OFFERED,         ///< Service is offered.
    SUBSCRIBED,      ///< Service is subscribed.
    UNDEFINED
};

/// Extended service description type definition.
struct ServiceDescExtT
{
    ServiceDescExtT()
        : serviceState(ServiceStateT::UNDEFINED)
    {
    }
    ServiceDescExtT(const ServiceDescT& serviceDesc_, ServiceStateT serviceState_)
        : serviceDesc(serviceDesc_)
        , serviceState(serviceState_)
    {
    }

    bool operator==(const ServiceDescExtT& rhs) const
    {
        return ((rhs.serviceDesc == serviceDesc) && (rhs.serviceState == serviceState));
    }
    inline bool operator<(const ServiceDescExtT& b) const
    {
        return serviceDesc < b.serviceDesc;
    }

    ServiceDescT serviceDesc;
    ServiceStateT serviceState;
};

/// Extended service description vector type.
using ServiceDescExtListT = std::vector<ServiceDescExtT>;

/**
 * @brief Type of data content to be requested by the GETK / sent by the ECU
 *
 */
enum class ContentTypeT : uint32_t
{
    GLOBAL_HEADER = 0,        //< Header of the data stream stored in the output stream. Will always be stored before any payload is stored
    GLOBAL_STATIC_INFO,       //< Global (i.e. not event/field specific) data
    GLOBAL_DYNAMIC_INFO,      //< Dynamic global information, e.g. Service Discovery information.
    DATA,                     //< Payload of an event
    METADATA                  //< Meta information related to an event (e.g. encoding information)
};

} // namespace types
} // namespace getk
} // namespace etas
