#ifndef _MCIERROR_H
#define _MCIERROR_H
////////////////////////////////////////////////////////////////////////
#include <MmSystem.h>
#include "Log.h"
////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#ifdef	_LOG_DISABLED
static inline LPCTSTR _GetMciErr (MMRESULT pError) {return _T("");}
static inline MMRESULT LogMciErr (unsigned int pLogLevel, MMRESULT pError, LPCTSTR pFormat = NULL, ...) {return pError;}
#define LogMciErrAnon LogMciErr
#else
extern LPCTSTR _GetMciErr (MMRESULT pError);
#ifdef	_LOG_NOFILELINE
extern MMRESULT LogMciErr (unsigned int pLogLevel, MMRESULT pError, LPCTSTR pFormat = NULL, ...);
#define LogMciErrAnon LogMciErr
#else
DEFINE_LogErrFL(Mci,MMRESULT)
#define	LogMciErr _LogMciErrFL(__FILE__,__LINE__).LogErr
#define	LogMciErrAnon _LogMciErrFL(NULL,0).LogErr
#endif
#endif
#else	// _DEBUG
#define	LogMciErr LogWinErr
#define LogMciErrAnon LogWinErrAnon
#endif
////////////////////////////////////////////////////////////////////////
#endif	// _MCIERROR_H
