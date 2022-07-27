set(pyHIN_TARGET "pyHIN")

set(PYTHON_VERSION_MAJOR 3)
set(PYTHON_VERSION_MINOR 8)

set(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION_MAJOR}. ${PYTHON_VERSION_MINOR})
find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}
             REQUIRED)

message(STATUS "Detected %PYTHON_ROOT%: " $ENV{PYTHON_ROOT})
message(STATUS "Detected %VIRTUAL_ENV%: " $ENV{VIRTUAL_ENV})
message(STATUS "NUMPY_INCLUDE_DIR: " ${NUMPY_INCLUDE_DIR})
message(STATUS "PYTHON_LIBRARIES: " ${PYTHON_LIBRARY_RELEASE})

set(CMAKE_SWIG_FLAGS "")

set(CMAKE_SWIG_OUTDIR ${PROJECT_SOURCE_DIR}/win64/dll)

set_source_files_properties(../${HIN_TARGET}.i PROPERTIES CPLUSPLUS ON)

# Add swig module
swig_add_library(${pyHIN_TARGET} LANGUAGE python SOURCES ../${HIN_TARGET}.i)

message(STATUS "HIN_TARGET: " ${HIN_TARGET})

target_link_libraries(${pyHIN_TARGET}
                      ${HIN_TARGET}
                      ${PYTHON_LIBRARY_RELEASE})

set(pyHIN_INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/src
    ${PYTHON_INCLUDE_PATH}
    ${NUMPY_INCLUDE_DIR})

message(STATUS "PYTHON_INCLUDE: " ${PYTHON_INCLUDE_PATH})

set_property(TARGET ${pyHIN_TARGET}
             PROPERTY SWIG_INCLUDE_DIRECTORIES ${pyHIN_INCLUDE_DIR})

target_include_directories(${pyHIN_TARGET} PUBLIC ${pyHIN_INCLUDE_DIR})

# Files to install with Python
set(PYTHON_INSTALL_FILES ${PROJECT_SOURCE_DIR}/win64/dll/${pyHIN_TARGET}.py
    ${PROJECT_SOURCE_DIR}/win64/dll/pyHIN.pyd)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)

set(SETUP_PY_OUT ${PROJECT_SOURCE_DIR}/win64/dll/setup.py)

configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})
configure_file(project.vcxproj.user.in
               ${PROJECT_BINARY_DIR}/swig/python/pyHIN.vcxproj.user @ONLY)

# Copy the pyd file outside of the build folder

add_custom_command(
  TARGET ${pyHIN_TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE_DIR:${pyHIN_TARGET}>/_${pyHIN_TARGET}.pyd
          ${PROJECT_SOURCE_DIR}/win64/dll)

# Copy debug symbols outside of the build folder

set_property(TARGET ${pyHIN_TARGET}
             PROPERTY PYHIN_DEBUG_SYMBOLS
             $<IF:$<CONFIG:Release>,
             echo "No debug symbols in RELEASE configuration." ,
             copy $<TARGET_FILE_DIR:${pyHIN_TARGET}>/_${pyHIN_TARGET}.pdb
                  ${PROJECT_SOURCE_DIR}/win64/dll/.>)

add_custom_command(
  TARGET ${pyHIN_TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E
          $<TARGET_GENEX_EVAL:${pyHIN_TARGET},$<TARGET_PROPERTY:${pyHIN_TARGET},PYHIN_DEBUG_SYMBOLS>>
          COMMAND_EXPAND_LISTS)

# Install target to call setup.py
add_custom_target(install-python
                  DEPENDS pyHIN
                  COMMAND python ${SETUP_PY_OUT} install)
