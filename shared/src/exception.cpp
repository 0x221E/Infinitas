#include "shared/exception.h"

namespace shared
{
  Exception::Exception(ExceptionLocation location, ExceptionSeverity severity, std::uint64_t code, std::string message) : m_Data(location, severity, code, message), std::runtime_error(message)
  {
        
  }

  ExceptionLocation Exception::GetLocation() const
  {
    return m_Data.m_Location;
  }

  ExceptionSeverity Exception::GetSeverity() const
  {
    return m_Data.m_Severity;
  }

  std::uint64_t Exception::GetErrorCode() const
  {
    return m_Data.m_ErrorCode;
  }
  
  std::string Exception::GetMessage() const
  {
    return m_Data.m_Message;
  }
}
