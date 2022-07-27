//! @file Context.cpp

#include "Context.h"
#include "src/Importer.h"

#include "GlobalData.h"

//-----------------------------------------------------------------------------
Status init()
{
    Importer *importer = HIN::importer;
    if (importer != nullptr)
        return Status::Error;

    importer = new Importer;

    HIN::importer = importer;

    return Status::Success;
}

//-----------------------------------------------------------------------------
Status quit()
{
    Importer *importer = HIN::importer;
    if (importer != nullptr)
        delete importer;

    HIN::importer = nullptr;

    return Status::Success;
}

//-----------------------------------------------------------------------------
IImporter * getImporter()
{
    Importer *importer = HIN::importer;

    return (IImporter *)importer;
}
