/*
 * @copyright (c) 2019, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * @brief Node type definition.
 */
#pragma once

/* *** System include files *** */
#include <stdint.h>
#include <string>
#include <tuple>
#include <vector>

namespace etas {
namespace soa {
namespace api {

/// Numerical identifier for a node.
typedef uint64_t NodeIdT;

/// Name of a node.
typedef std::string NodeNameT;

/// Contains information about a node.
struct NodeT
{
    NodeIdT id;
    NodeNameT name;
};

/// Node list type.
typedef std::vector<NodeT> NodeListT;

inline bool operator==(const NodeT& lhs, const NodeT& rhs)
{
    return std::tie(lhs.id, lhs.name) == std::tie(rhs.id, rhs.name);
}

inline bool operator!=(const NodeT& lhs, const NodeT& rhs)
{
    return std::tie(lhs.id, lhs.name) != std::tie(rhs.id, rhs.name);
}

inline bool operator<(const NodeT& lhs, const NodeT& rhs)
{
    return std::tie(lhs.id, lhs.name) < std::tie(rhs.id, rhs.name);
}

} // namespace api
} // namespace soa
} // namespace etas
