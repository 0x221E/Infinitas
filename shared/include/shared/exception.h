#pragma once

#include <cstdint>
#include <string>

namespace shared
{

   enum class ExceptionLocation : std::uint8_t
    {
      EX_TYPE_UNKNOWN = 0,
      EX_TYPE_LEXER = 1,
      EX_TYPE_PARSER = 2,
      EX_TYPE_COMPILER = 3,
      EX_TYPE_OPTIMIZER = 4,
      EX_TYPE_RUNTIME = 5
    };

    enum class ExceptionSeverity : std::uint8_t
    {
      EX_SEVERITY_UNKNOWN = 0,
      EX_SEVERITY_INFORMATIONAL = 1,
      EX_SEVERITY_WARNING = 2,
      EX_SEVERITY_ERROR = 3,
      EX_SEVERITY_EXEC_STOP = 4
    };

    struct ExceptionData
    {
      ExceptionLocation m_Location;
      ExceptionSeverity m_Severity;
      std::uint64_t m_ErrorCode;
      std::string m_Message;
    };

    class Exception 
    { 
    public:
      Exception() = delete;
      Exception(ExceptionLocation location, ExceptionSeverity severity, std::uint64_t code, std::string message);
      virtual ~Exception() = default;
  
    public:
      virtual std::string ToString() const = 0;

    public:
      ExceptionLocation GetLocation() const;
      ExceptionSeverity GetSeverity() const;
      std::uint64_t GetErrorCode() const;
      std::string GetMessage() const; 

    private:
      ExceptionData m_Data; 
    }; 
    
}
