/******************************************************************************/
/*!
\file   trace.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
Takes messages and either prints to a console or a file. Used for debugging
and aggregating error messages.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef TRACE_H
#define TRACE_H

#include <string> // std::string
#include <sstream> // std::ostringstream

namespace Trace {

#define Message(x) MFunc(x, __LINE__, __FILE__)
#define Error(x) ReportError(x, __LINE__, __FILE__)

  /**************************************************************************/
  /*!
  \brief opens the trace file and any other forms of output
  */
  /**************************************************************************/
  void Init(void);
  
  /**************************************************************************/
  /*!
  \brief Takes any message (not just an error message) and puts it into a buffer
  that can be dumped into a file on a crash
  \param m
  error message
  */
  /**************************************************************************/
  void MFunc(std::string m, int l, const char * f);

  /**************************************************************************/
  /*!
  \brief Takes any message (not just an error message) and puts it into a buffer
  that can be dumped into a file on a crash
  \param m
  error message
  */
  /**************************************************************************/
  void MFunc(std::stringstream m, int l, const char * f);
  
  /**************************************************************************/
  /*!
  \brief Takes any message (not just an error message) and puts it into a buffer
  that can be dumped into a file on a crash
  \param m
  error message
  */
  /**************************************************************************/
  void MFunc(const char * m, int l, const char * f);
  
  /**************************************************************************/
  /*!
  \brief Reports an error
  \param m
	error message
  */
  /**************************************************************************/
  void ReportError(std::stringstream m, int l, const char * f);

  /**************************************************************************/
  /*!
  \brief Reports an error
  \param m
	error message
  */
  /**************************************************************************/
  void ReportError(std::string m, int l, const char * f);

  /**************************************************************************/
  /*!
  \brief Reports an error
  \param m
	error message
  */
  /**************************************************************************/
  void ReportError(const char * m, int l, const char * f);

  /**************************************************************************/
  /*!
  \brief closes all means of output for error messages
  */
  /**************************************************************************/
  void Shutdown(void);

  /**************************************************************************/
  /*!
  \brief Creates a dump log file, dumping all information needed for debugging errors
  */
  /**************************************************************************/
  std::string CreateCrashDump(void);
	
  std::string GetAppDataLocalLow();
}

#endif
