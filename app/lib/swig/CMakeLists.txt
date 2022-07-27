set(SWIG_EXECUTABLE "C:\\swig\\swigwin-4.0.2\\swig.exe")
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

add_subdirectory(python)
