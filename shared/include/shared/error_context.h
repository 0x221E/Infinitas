#pragma once

#include "shared/exception.h"

#include <type_traits>
#include <iostream>

namespace shared
{
  template<typename ExceptionType, typename CodeType = ExceptionType::ExceptionCodeType>
  struct ErrorContext
  {
      bool m_ErrorThrown = false;
      
      void LogError(CodeType code, std::string message)
      {
          Log(shared::ExceptionSeverity::EX_SEVERITY_ERROR, code, message);
          m_ErrorThrown = true;
      }
    
      void LogCritical(CodeType code, std::string message)
      {
          Log(shared::ExceptionSeverity::EX_SEVERITY_EXEC_STOP, code, message);
          m_ErrorThrown = true;
      }
      
      void ResetErrors() noexcept
      {
          m_ErrorThrown = false;
      }

  private:
      template<typename...ARGS>
      void Log(ARGS...args)
      {
          static_assert(std::is_constructible_v<ExceptionType, ARGS...>, "Exception cannot be constructed with the supplied arguments");
          ExceptionType ex(std::forward<ARGS>(args)...);
          std::cerr << ex.ToString() << std::endl;
          if (ex.GetSeverity() == shared::ExceptionSeverity::EX_SEVERITY_EXEC_STOP) throw ex;
      }
  };
}
