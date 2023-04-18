cmake_minimum_required(VERSION 2.1)

# Settings:
set(CTEST_DASHBOARD_ROOT                "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Tests/CTestTest")
set(CTEST_SITE                          "ubuntu")
set(CTEST_BUILD_NAME                    "CTestTest-Linux-g++-Upload")

set(CTEST_SOURCE_DIRECTORY              "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Tests/CTestTestUpload")
set(CTEST_BINARY_DIRECTORY              "/home/svm/openwrt_rtk/rtk_openwrt_sdk/build_dir/host/cmake-2.8.12.2/Tests/CTestTestUpload")
set(CTEST_CMAKE_GENERATOR               "Unix Makefiles")
set(CTEST_CMAKE_GENERATOR_TOOLSET       "")
set(CTEST_BUILD_CONFIGURATION           "$ENV{CMAKE_CONFIG_TYPE}")

CTEST_START(Experimental)
CTEST_CONFIGURE(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
CTEST_BUILD(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
CTEST_UPLOAD(FILES "${CTEST_SOURCE_DIRECTORY}/sleep.c" "${CTEST_BINARY_DIRECTORY}/CMakeCache.txt")
CTEST_SUBMIT()
