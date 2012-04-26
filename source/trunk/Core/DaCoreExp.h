#ifndef	_DACOREEXP
#define	_DACOREEXP
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DACORE_EXPORT
#define	_DACORE_IMPEXP	__declspec(dllexport)
#else
#ifdef	_DACORE_LOCAL
#define	_DACORE_IMPEXP
#else
#ifndef	_DACORE_IMPEXP
#define	_DACORE_IMPEXP	__declspec(dllimport)
#endif
#endif
#endif
/////////////////////////////////////////////////////////////////////////////
#endif	// _DACOREEXP