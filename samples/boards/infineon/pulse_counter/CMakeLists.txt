# Copyright (c) 2023 Cypress Semiconductor Corporation (an Infineon company) or
# an affiliate of Cypress Semiconductor Corporation
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pulse_counter)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
