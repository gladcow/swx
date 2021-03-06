###
# Copyright 2014-2016 Matthew Harvey
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###


# Preliminaries

cmake_minimum_required(VERSION 2.8.10)


# Project and version

set(application_name "swx")
set(executable_name "swx")
set(file_extension ".swx")
project(${application_name})
set(version_major 1)
set(version_minor 0)
set(version_patch 2)


# Definitions and flags to be passed to the compiler

add_definitions(
    -DSWX_APPLICATION_NAME="${application_name}"
    -DSWX_FILE_EXTENSION="${file_extension}"
    -DSWX_VERSION_MAJOR=${version_major}
    -DSWX_VERSION_MINOR=${version_minor}
    -DSWX_VERSION_PATCH=${version_patch}
    # -DSWX_RELEASE  # uncomment for release version
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Dependencies

find_package(Boost 1.53.0 COMPONENTS unit_test_framework)
set(min_gcc "4.9")
if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${min_gcc})
    # versions less than 4.9 do not support regex
    message(FATAL_ERROR "GCC version >= ${gcc_version_required} required")
endif()
include_directories(include)
set(libraries pthread dl)


# Building the swx_common library, which contains code used both by the main
# executable, and the test suite.

set(
    common_sources
    src/activity_filter.cpp
    src/activity_node.cpp
    src/activity_stats.cpp
    src/activity_tree.cpp
    src/application.cpp
    src/arithmetic.cpp
    src/atomic_writer.cpp
    src/command.cpp
    src/config.cpp
    src/config_command.cpp
    src/csv_list_report_writer.cpp
    src/csv_row.cpp
    src/csv_summary_report_writer.cpp
    src/current_command.cpp
    src/edit_command.cpp
    src/exact_activity_filter.cpp
    src/file_utilities.cpp
    src/help_command.cpp
    src/help_line.cpp
    src/human_list_report_writer.cpp
    src/human_summary_report_writer.cpp
    src/info.cpp
    src/interval.cpp
    src/list_report_writer.cpp
    src/ordinary_activity_filter.cpp
    src/placeholder.cpp
    src/print_command.cpp
    src/recording_command.cpp
    src/rename_command.cpp
    src/regex_activity_filter.cpp
    src/report_writer.cpp
    src/reporting_command.cpp
    src/resume_command.cpp
    src/stint.cpp
    src/stream_flag_guard.cpp
    src/string_utilities.cpp
    src/summary_report_writer.cpp
    src/switch_command.cpp
    src/day_command.cpp
    src/time_point.cpp
    src/time_log.cpp
    src/true_activity_filter.cpp
    src/version_command.cpp
)
add_library(swx_common ${common_sources})
target_link_libraries (${executable_stem} swx_common ${libraries})


# Build the tests

set(
    test_sources
    test/arithmetic.cpp
    test/csv_row.cpp
    test/exact_activity_filter.cpp
    test/ordinary_activity_filter.cpp
    test/regex_activity_filter.cpp
    test/string_utilities.cpp
    test/test.cpp
    test/true_activity_filter.cpp
)
add_executable(
    test_driver EXCLUDE_FROM_ALL
    ${test_sources}
)
target_link_libraries(
    test_driver
    swx_common
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${libraries}
)
set(test_execution_command "${PROJECT_SOURCE_DIR}/test_driver")
add_custom_target(
    run_tests
    COMMAND "${test_execution_command}"
    DEPENDS test_driver
)


# Build the main executable

add_executable(${executable_name} src/main.cpp)
target_link_libraries(${executable_name} swx_common ${libraries})


# Installation

install(
    TARGETS
        ${executable_name}
    RUNTIME DESTINATION
        "${CMAKE_INSTALL_PREFIX}/bin"
)
