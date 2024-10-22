/*
 * @copyright (c) 2023, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */

#pragma once

/* *** System include files *** */
#include <cstdint>

/* *** Local include files *** */

/**
 * @file
 * @brief Type definitions for GetkAdapter clients. 
 */

namespace etas
{
namespace getk
{
namespace types 
{

/**
 * @class ClientIdT.
 * Idenfifier for an GetkAdapter client. 
 */
class ClientIdT
{
public:
    /**
     * @brief Construct the client identifier.
     *
     * @param[in]  type         String specifying the type of the middleware.
     */
    explicit ClientIdT(const std::string &type) : type_(type)
    {
    }

    /**
     * @brief Construct the client identifier.
     *
     * @param[in]  type         String specifying the type of the middleware.
     * @param[in]  instanceId   Integer specifying an instance number of the middleware.
     */
    ClientIdT(const std::string &type, uint32_t instanceId) : type_(type), instanceId_(instanceId)
    {
    }

    inline bool operator==(const ClientIdT &rhs) const
    {
        return (type_ == rhs.type_ && instanceId_ == rhs.instanceId_);
    }

    std::string type_;
    uint32_t instanceId_ = 0;
};

} // namespace types

} // namespace getk

} // namespace etas