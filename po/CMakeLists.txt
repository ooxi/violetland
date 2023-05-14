PROJECT(po)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR) 

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/CMake-Gettext)
set(GettextTranslate_ALL true)
set(GettextTranslate_GMO_BINARY true)

include(GettextTranslate)
GettextTranslate()

