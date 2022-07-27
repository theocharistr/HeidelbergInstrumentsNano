//! @file IContext.h

#ifndef API_H_
#define API_H_

#include "Status.h"

#ifdef _HIN_DLLEXPORT
#define API __declspec(dllexport)
#else
#define API
#endif

//-----------------------------------------------------------------------------
struct IImporter;

//-----------------------------------------------------------------------------

API Status init(void);
API Status quit(void);

API IImporter * getImporter(void);

#endif
