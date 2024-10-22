/*
 * @copyright (c) 2019, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * Service access information.
 */

#pragma once

/* *** System include files *** */
#include <stdint.h>
#include <string>
#include <tuple>
#include <vector>

/* *** Local include files *** */
#include "etas/soa/node.hpp"

namespace etas {
namespace soa {
namespace api {

/// Contains information for service access.
class ServiceAccessInfo
{
public:
    ServiceAccessInfo() = default;
    virtual ~ServiceAccessInfo() = default;

    ServiceAccessInfo(uint16_t pPort, const std::string& pIpAddress, NodeIdT pNodeId)
        : port(pPort)
        , ipAddress(pIpAddress)
        , m_initialized(true)
    {
        node.id = pNodeId;
    }

    ServiceAccessInfo(uint16_t pPort, const std::string& pIpAddress, const NodeT& pNode)
        : port(pPort)
        , ipAddress(pIpAddress)
        , node(pNode)
        , m_initialized(true)
    {
    }

    bool isInitialized() const
    {
        return m_initialized;
    }

    uint16_t port = 0xFFFFU;
    std::string ipAddress;
    NodeT node;

private:
    bool m_initialized = false;
};

class IdentifiedServiceAccessInfo
{
public:
    IdentifiedServiceAccessInfo() = default;
    explicit IdentifiedServiceAccessInfo(ServiceAccessInfo& service, ServiceAccessInfo& identService)
        : m_serviceAccessInfo(service)
        , m_identServiceAccessInfo(identService)
    {
    }

    ServiceAccessInfo getServiceAccessInfo() const
    {
        return m_serviceAccessInfo;
    }

    ServiceAccessInfo getIdentServiceAccessInfo() const
    {
        return m_identServiceAccessInfo;
    }

private:
    ServiceAccessInfo m_serviceAccessInfo;
    ServiceAccessInfo m_identServiceAccessInfo;
};

inline bool operator<(const ServiceAccessInfo& lhs, const ServiceAccessInfo& rhs)
{
    return std::tie(lhs.port, lhs.ipAddress, lhs.node) < std::tie(rhs.port, rhs.ipAddress, rhs.node);
}

inline bool operator==(const ServiceAccessInfo& lhs, const ServiceAccessInfo& rhs)
{
    return std::tie(lhs.ipAddress, lhs.port, lhs.node) == std::tie(rhs.ipAddress, rhs.port, rhs.node);
}

/// A list of ServiceAccessInfos.
typedef std::vector<ServiceAccessInfo> ServiceAccessInfoList;
typedef std::vector<IdentifiedServiceAccessInfo> IdentifiedServiceAccessInfoList;

} // namespace api
} // namespace soa
} // namespace etas
