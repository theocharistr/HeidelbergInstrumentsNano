%module pyHIN

%pythonbegin %{
import os
os.add_dll_directory(r'C:\vc\opencv\install\x64\vc16\bin')
%}

%{
#define SWIG_FILE_WITH_INIT
%}

%{
#include "src/IContext.h"
#include "src/IImporter.h"
#include "Status.h"
%}

%include "std_vector.i"

namespace std
{
    %template(UCharVector) vector<unsigned char>;
    %template(RectVector) vector<Rect>;
}

%include "src/IContext.h"
%include "src/IImporter.h"
%include "Status.h"

%exception;
