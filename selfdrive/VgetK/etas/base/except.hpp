/*
 * @copyright (c) 2019, ETAS GmbH, Stuttgart. All rights reserved.
 * Unauthorized reproduction of this file, in whole or in part, is prohibited.
 */
/**
 * @file
 * @brief ETAS exception classes.
 * The classes defined within this file are derived from the C++ standard exceptions
 * and support the respective error categories and hierarchies.
 */
#pragma once

/* *** System include files *** */
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <system_error>

/* *** Local include files *** */
#include "etas/base/base_export.hpp"
#include "etas/base/compilerwarning.hpp"

namespace etas {

/**
 * @class LogicError
 * @brief Exception base class for a logic error.
 * Own user-defined logic error types can be derived from this class.
 */
class BASE_API LogicError : public std::logic_error
DISABLE_MSVC_WARNING_ONCE(4275) // Suppress the warning since a class from the standard library cannot be exported.
{
public:
    // Introduce members from base class.
    using std::logic_error::logic_error;
};

/**
 * @class InvalidArgument
 * Specifies an invalid argument exception.
 * An invalid argument is part of the logic error category.
 */
class BASE_API InvalidArgument : public LogicError
{
public:
    // Introduce members from base class.
    using LogicError::LogicError;

    /// Enum to define argument number constants.
    enum class ArgNum : uint32_t
    {
        Unspecified = 0
    };

    /**
     * Constructor overload for InvalidArgument taking a message as const char* and an argument number.
     * The number of the invalid argument that causes the exception can be specified.
     *
     * @param[in]   msg         The error message.
     * @param[in]   invArgNum   The number of the invalid argument.
     */
    InvalidArgument(const char* msg, uint32_t invArgNum)
        : LogicError(msg)
        , invArgNum_(invArgNum)
    {
    }

    /**
     * Constructor overload for InvalidArgument taking a message as std::string and an argument number.
     * The number of the invalid argument that causes the exception can be specified.
     *
     * @param[in]   msg         The error message.
     * @param[in]   invArgNum   The number of the invalid argument.
     */
    InvalidArgument(const std::string& msg, uint32_t invArgNum)
        : LogicError(msg)
        , invArgNum_(invArgNum)
    {
    }

    /**
     * Get the number of the invalid argument.
     *
     * @return  Number of the invalid argument.
     */
    uint32_t getInvArgNum() const
    {
        return invArgNum_;
    }

private:
    uint32_t invArgNum_ = static_cast<uint32_t>(ArgNum::Unspecified);
};

/**
 * @class OutOfRange
 * Specifies an out-of-range exception.
 * An out-of-range exception is part of the logic error category.
 * This type can be used to report an argument value is not in its expected range.
 */
class BASE_API OutOfRange : public LogicError
{
public:
    // Introduce members from base class.
    using LogicError::LogicError;
};

/**
 * @class LengthError
 * Specifies an length error.
 * A length error is part of the logic error category.
 */
class BASE_API LengthError : public LogicError
{
public:
    // Introduce members from base class.
    using LogicError::LogicError;
};

/**
 * @class DomainError
 * Specifies a domain error exception.
 * A domain error is part of the logic error category.
 * Functions may define a set of values for their domain (e.g. positive real numbers).
 * This type can be used to report when the arguments are not contained within the required domain.
 */
class BASE_API DomainError : public LogicError
{
public:
    // Introduce members from base class.
    using LogicError::LogicError;
};

/**
 * @class RuntimeError
 * Exception base class for a runtime error.
 * Own user-defined runtime error exceptions can be derived from this class.
 */
class BASE_API RuntimeError : public std::runtime_error
DISABLE_MSVC_WARNING_ONCE(4275) // Suppress the warning since a class from the standard library cannot be exported.
{
public:
    // Introduce members from base class.
    using std::runtime_error::runtime_error;
};

/**
 * @class OutOfResources
 * Specifies an out-of-resources exception.
 * An out-of-resources exception is part of the runtime error category.
 * This type can be used to report when resources (e.g. memory) are exhausted.
 */
class BASE_API OutOfResources : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class OverflowError
 * Specifies an overflow error.
 * An overflow error is part of the runtime error category.
 * This type can be used to report an arithmetic overflow.
 */
class BASE_API OverflowError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class UnderflowError
 * Specifies an underflow error.
 * An underflow error is part of the runtime error category.
 * This type can be used to report an arithmetic underflow.
 */
class BASE_API UnderflowError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class RangeError
 * Specifies a range error.
 * A range error is part of the runtime error category.
 * This type can be used to report range errors in internal computations.
 */
class BASE_API RangeError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class FileNotFoundError
 * This type can used to report missing files.
 */
class BASE_API FileNotFoundError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class InvalidConfigurationError
 * This type can be used to report errors during configuration parsing.
 */
class BASE_API InvalidConfigurationError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class XmlParserError
 * This type can be used to report general errors during xml parsing
 */
class BASE_API XmlParserError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class InvalidXmlTagError
 * This type can be used to report errors during xml parsing (invalid tag encountered)
 */
class BASE_API InvalidXmlTagError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class InvalidXmlValueError
 * This type can be used to report errors during xml parsing (invalid content of tag or attribute)
 */
class BASE_API InvalidXmlValueError : public RuntimeError
{
public:
    // Introduce members from base class.
    using RuntimeError::RuntimeError;
};

/**
 * @class SystemError
 * Specifies a system error.
 * A system error actually is part of the runtime error category.
 * Since new functionalities are introduced - like mapping to system error codes -
 * it becomes a new group of exceptions.
 */
class BASE_API SystemError : public std::system_error
DISABLE_MSVC_WARNING_ONCE(4275) // Suppress the warning since a class from the standard library cannot be exported.
{
public:
    // Introduce members from base class.
    using std::system_error::system_error;

    /**
     * Get the error code associated with the exception.
     *
     * @return  Error code.
     */
    int32_t getErrVal() const
    {
        return code().value();
    }
};

/**
 * @class GenericSystemError
 * Convenience class for creation of generic system errors.
 */
class BASE_API GenericSystemError : public SystemError
{
public:
    // Introduce members from base class.
    using SystemError::SystemError;

    /**
     * Constructor for generic system errors.
     *
     * @param[in]   errval  Error code value.
     * @param[in]   msg     The error message.
     */
    GenericSystemError(int32_t errval, const char* msg)
        : SystemError(errval, std::generic_category(), msg)
    {
    }
};

/**
 * @class OperatingSystemError
 * Convenience class for creation of operating system errors.
 */
class BASE_API OperatingSystemError : public SystemError
{
public:
    // Introduce members from base class.
    using SystemError::SystemError;

    /**
     * Constructor for operating system errors.
     *
     * @param[in]   errval  Error code value.
     * @param[in]   msg     The error message.
     */
    OperatingSystemError(int32_t errval, const char* msg)
        : SystemError(errval, std::system_category(), msg)
    {
    }
};

} // namespace etas
