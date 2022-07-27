//! @file Status.h

#ifndef STATUS_H_
#define STATUS_H_

//-----------------------------------------------------------------------------
enum class Status: unsigned int
{
    Success = 0,
    ImporterError,
    Error,
};

#endif
