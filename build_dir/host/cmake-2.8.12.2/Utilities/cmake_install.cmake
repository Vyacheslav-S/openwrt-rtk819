# Install script for directory: /home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Utilities

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/svm/openwrt_rtk/rtk_openwrt_sdk/staging_dir/host")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmakecommands.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmakecompat.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmakeprops.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmakepolicies.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmakevars.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmakemodules.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ctest.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cpack.1"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ccmake.1"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/doc/cmake-2.8" TYPE FILE FILES
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-policies.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-properties.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-variables.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-modules.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-commands.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-compatcommands.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ctest.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cpack.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ccmake.txt"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-policies.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-properties.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-variables.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-modules.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-commands.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake-compatcommands.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ctest.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cpack.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ccmake.html"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cmake.docbook"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ctest.docbook"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/cpack.docbook"
    "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Docs/ccmake.docbook"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/aclocal" TYPE FILE FILES "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Utilities/cmake.m4")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Utilities/Doxygen/cmake_install.cmake")
  INCLUDE("/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Utilities/KWStyle/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

