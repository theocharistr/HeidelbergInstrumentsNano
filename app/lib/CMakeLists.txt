cmake_minimum_required(VERSION 3.22.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(HIN)

set(HIN_SOURCE_FILES
    ./Context.cpp
    ./GlobalData.cpp
    ./src/Importer.cpp
    ./src/Splitter.cpp)

set(HIN_HEADER_FILES
    ./Context.h
    ./GlobalData.h
    ./Status.h
    ./src/Importer.h
    ./src/Splitter.h
    ./src/IImporter.h
    ./src/IContext.h
)

set_source_files_properties(HIN_HEADER_FILES PROPERTIES HEADER_FILE_ONLY TRUE)

source_group_by_folder("./")

set(HIN_WIN_RESOURCES_FILES ./HINResource.rc ./resource.h)

set (HIN_OBJ "HIN_object")

add_library(${HIN_OBJ}
            OBJECT
            ${HIN_HEADER_FILES}
            ${HIN_SOURCE_FILES}
            ${HIN_WIN_RESOURCES_FILES})

set_property(TARGET ${HIN_OBJ} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(${HIN_TARGET}
            SHARED
            $<TARGET_OBJECTS:${HIN_OBJ}>)

add_library(${HIN_STATIC_TARGET}
            STATIC
            $<TARGET_OBJECTS:${HIN_OBJ}>)

set(ADDITIONAL_HIN_DEFINITIONS
    "-DWIN64;-D_WIN64;-D_LIB;-D_WINDOWS;-D_USRDLL;-D_CRT_SECURE_NO_WARNINGS;-D_HIN_DLLEXPORT")

set(ADDITIONAL_HIN_CXX_FLAGS_DEBUG "/MTd;${COMMON_HIN_CXX_FLAGS}")

set(ADDITIONAL_HIN_CXX_FLAGS_RELEASE
    "/Ob2;/GS;/Gd;/Gm-;/W3;/EHsc;/WX-;/GF-;/fp:precise;/MT;/D_ITERATOR_DEBUG_LEVEL=0;${COMMON_HIN_CXX_FLAGS}")

target_compile_definitions(${HIN_OBJ} PRIVATE
                           ${ADDITIONAL_HIN_DEFINITIONS})

target_compile_options(
    ${HIN_OBJ}
    PUBLIC "$<$<CONFIG:DEBUG>:${ADDITIONAL_HIN_CXX_FLAGS_DEBUG}>")

target_compile_options(
    ${HIN_OBJ}
    PUBLIC "$<$<CONFIG:RELEASE>:${ADDITIONAL_HIN_CXX_FLAGS_RELEASE}>")

set(HIN_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}
    C:/vc/opencv/install/include
    )

target_include_directories(${HIN_OBJ} PUBLIC ${HIN_INCLUDE_DIRECTORIES})

set(HIN_LINK_LIBRARIES
    debug
    C:/vc/opencv/install/x64/vc16/lib/opencv_core430d.lib
    optimized
    C:/vc/opencv/install/x64/vc16/lib/opencv_core430.lib
    debug
    C:/vc/opencv/install/x64/vc16/lib/opencv_imgproc430d.lib
    optimized
    C:/vc/opencv/install/x64/vc16/lib/opencv_imgproc430.lib
    debug
    C:/vc/opencv/install/x64/vc16/lib/opencv_imgcodecs430d.lib
    optimized
    C:/vc/opencv/install/x64/vc16/lib/opencv_imgcodecs430.lib
    debug
    C:/vc/opencv/install/x64/vc16/lib/opencv_highgui430d.lib
    optimized
    C:/vc/opencv/install/x64/vc16/lib/opencv_highgui430.lib
    )

target_link_libraries(${HIN_OBJ} ${HIN_LINK_LIBRARIES})

target_link_libraries(${HIN_TARGET} ${HIN_OBJ})

target_link_libraries(${HIN_STATIC_TARGET} ${HIN_OBJ})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/win64/dll)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/win64/dll)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/win64/dll)

set_target_properties(${HIN_TARGET} PROPERTIES SUFFIX ".dll")

add_custom_command(
  TARGET ${HIN_TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/win64/dll
  COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE_DIR:${HIN_TARGET}>/${HIN_TARGET}.dll
          ${PROJECT_SOURCE_DIR}/win64/dll/.)

set_property(TARGET ${HIN_TARGET}
             PROPERTY HIN_DEBUG_SYMBOLS
             $<IF:$<CONFIG:Release>,
             echo "No debug symbols in RELEASE configuration." ,
             copy $<TARGET_FILE_DIR:${HIN_TARGET}>/${HIN_TARGET}.pdb
                  ${PROJECT_SOURCE_DIR}/win64/dll/.>)

add_custom_command(TARGET ${HIN_TARGET}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E
                   $<TARGET_GENEX_EVAL:${HIN_TARGET},$<TARGET_PROPERTY:${HIN_TARGET},HIN_DEBUG_SYMBOLS>>
                   COMMAND_EXPAND_LISTS)

add_custom_command(
  TARGET ${HIN_STATIC_TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/win64/lib
  COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE_DIR:${HIN_STATIC_TARGET}>/${HIN_STATIC_TARGET}.lib
          ${PROJECT_SOURCE_DIR}/win64/lib)

add_subdirectory(swig)
