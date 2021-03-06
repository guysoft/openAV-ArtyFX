
cmake_minimum_required (VERSION 2.6)

project (artyfx)

#set(ARTYFX_VERSION_MAJOR "1")
#set(ARTYFX_VERSION_MINOR "3")
#set(ARTYFX_VERSION_PATCH "0")

#set(ARTYFX_VERSION "${ARTYFX_VERSION_MAJOR}.${ARTYFX_VERSION_MINOR}\
#.${ARTYFX_VERSION_PATCH}")

option(RELEASE_BUILD  "Build for production usage" ON  )
option(BUILD_GUI      "Build GUI (AVTK based)"     ON  )
option(BUILD_SSE      "Build with SSE flags"       ON  )
option(BUILD_BENCH    "Build benchmarks binary"    OFF )

find_package(PkgConfig)

pkg_check_modules(LV2 lv2>=1.8.0 REQUIRED)
include_directories( ${LV2_INCLUDE_DIRS}  )

pkg_check_modules(SND sndfile REQUIRED)
include_directories( ${SND_INCLUDE_DIRS}  )
#target_link_libraries( avtkDemo ${CAIRO_LIBRARIES}  )

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC -shared -L./src/avtk -Wl,--no-undefined")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O1 -Wno-unused-variable")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O1 -Wno-unused-variable -ftree-vectorize")

# DSP sources
FILE(GLOB sources src/dsp/*.cxx src/dsp/rr/*.cxx  src/dsp/eq/*.cc )

IF(BUILD_SSE)
  SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -msse2 -mfpmath=sse")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -mfpmath=sse")
ENDIF()

IF(BUILD_GUI)
  # TODO: when building GUI, add platform checks for X11, Win, OsX
  #pkg_check_modules(X11 x11 REQUIRED)
  #include_directories( ${NTK_INCLUDE_DIRS}  )
  #link_directories   ( ${NTK_LIBRARY_DIRS}  )
  #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPUGL_HAVE_CAIRO")
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPUGL_HAVE_CAIRO")
  FILE(GLOB ui_sources src/ui/*.cxx src/ui/graphs/*.cxx )
  # AVTK .a static library build
  add_subdirectory (src/avtk)
  include_directories("src/avtk/avtk/")
ENDIF()

string( ASCII 27 _esc)
function(COLOR_MESSAGE TEXT)
    if(CMAKE_COLOR_MAKEFILE)
        MESSAGE(${TEXT})
    else()
        string(REGEX REPLACE "${_esc}.[0123456789;]*m" "" __TEXT ${TEXT} )
        MESSAGE(${__TEXT})
    endif()
endfunction()

SET(green     "${_esc}[1;32m")
SET(blue      "${_esc}[1;34m")
SET(reset     "${_esc}[1;0m")

COLOR_MESSAGE( "
  ${blue}ArtyFX Configuration : ${ARTYFX_VERSION}${reset}
-----------------------
* ${green}Install Directory${reset}               : ${CMAKE_INSTALL_PREFIX}
* ${green}AVTK GUI${reset}                        : ${BUILD_GUI}
* ${green}DSP Benchmarks${reset}                  : ${BUILD_BENCH}
")

ADD_LIBRARY(artyfx SHARED ${sources} )

target_link_libraries( artyfx LINK_PUBLIC ${SND_LIBRARIES}   )
target_link_libraries( artyfx LINK_PUBLIC "-lpthread" )

IF(BUILD_GUI)
  ADD_LIBRARY( artyfx_ui SHARED ${ui_sources} )
  add_dependencies( artyfx_ui avtk )
  target_link_libraries( artyfx_ui "-lavtk -lcairo -lX11" )
  set_target_properties( artyfx_ui PROPERTIES PREFIX "")
  install (TARGETS artyfx_ui DESTINATION lib/lv2/artyfx.lv2/)
ENDIF()

IF(BUILD_BENCH)
  add_executable (artyfx_benchmark src/dsp/perf_test.cpp )
  target_link_libraries(artyfx_benchmark "-lbenchmark -lpthread" )
ENDIF()

# Remove "lib" part before name (artyfx.so, not libartyfx.so)
set_target_properties(artyfx PROPERTIES PREFIX "")

# add the install targets
install (TARGETS artyfx    DESTINATION lib/lv2/artyfx.lv2/)

FILE(GLOB modF "artyfx.lv2/modgui/*")
FILE(GLOB ttls "artyfx.lv2/*.ttl"   )

install(FILES     ${modF} DESTINATION lib/lv2/artyfx.lv2/modgui/)
install(FILES     ${ttls} DESTINATION lib/lv2/artyfx.lv2/)
