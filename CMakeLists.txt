cmake_minimum_required(VERSION 3.9)
project(ffmpeg-gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################################################################
####### Misc
##################################################################

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

##################################################################
###### Qt
##################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC TRUE)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

##################################################################
####### ffmpeg-gui
##################################################################

set(FFMPEG_GUI_RESOURCES "src/res.qrc")

IF (WIN32)
  set(FFMPEG_GUI_RESOURCES "src/windows_res.rc" ${FFMPEG_GUI_RESOURCES})
ENDIF()

file(GLOB_RECURSE FFMPEG_GUI_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE FFMPEG_GUI_HDR_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)
add_executable(ffmpeg-gui ${FFMPEG_GUI_HDR_FILES} ${FFMPEG_GUI_SRC_FILES} ${FFMPEG_GUI_RESOURCES})
target_include_directories(ffmpeg-gui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(ffmpeg-gui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(ffmpeg-gui Qt5::Core Qt5::Gui Qt5::Widgets)

foreach(_source IN ITEMS ${FFMPEG_GUI_HDR_FILES} ${FFMPEG_GUI_SRC_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
