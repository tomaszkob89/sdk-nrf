#
# Copyright (c) 2024 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(protocols_serialization_client)



set(TCA_TRANSPORT "RTT" CACHE STRING "TCA transport type")
execute_process(COMMAND ctf_tca -r lib OUTPUT_VARIABLE TCA_TARGET_LIB_DIR)
string(STRIP ${TCA_TARGET_LIB_DIR} TCA_TARGET_LIB_DIR) # strip new line character
include(${TCA_TARGET_LIB_DIR}/CMakeLists.txt)
target_link_libraries(app PRIVATE ctfTargetComm)


# NORDIC SDK APP START
target_sources(app PRIVATE
    src/main.c
)

zephyr_library_sources_ifdef(CONFIG_OPENTHREAD_RPC src/ot_shell.c)
zephyr_library_sources_ifdef(CONFIG_MPSL_CX_SOFTWARE src/coex_shell.c)
zephyr_library_sources_ifdef(CONFIG_LOG_FORWARDER_RPC src/log_rpc_shell.c)
# NORDIC SDK APP START
