# Generic c++ project make template.
# Author: Simon Brummer <simon.brummer@posteo.de>

# --- Global Project settings ---
PROJECT_NAME     := host_monitor_cli
PROJECT_TYPE     := binary
PROJECT_CONFIG   := release
VERSION_MAJOR    := 1
VERSION_MINOR    := 1
VERSION_REVISION := 0

include etc/make/header.mk

# ---Install settings ---
INSTALL_INC_DIR := /usr/local/include
INSTALL_BIN_DIR := /usr/local/bin
INSTALL_LIB_DIR := /usr/local/lib

# --- Sources files ---
SRC := \
	Args.cpp \
	Config.cpp \
	GroupElement.cpp \
	ObserverElement.cpp \
	UserInterface.cpp \
	Util.cpp \
	Window.cpp \
	Version.cpp \
	main.cpp

# --- Compiler settings ---
CC := g++

STD := -std=c++17

DEFINES := \
	$(VERSION_DEFINES)

WARNINGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Werror \
	-Wtype-limits \
	-Wstrict-overflow=5 \
	-fstrict-overflow \
	-Wsign-compare \
	-Wconversion

CPPFLAGS := \

CPPFLAGS_DEBUG := \
	-Og \
	-ggdb

CPPFLAGS_RELEASE := \
	-O2 \
	-DNDEBUG

CPPFLAGS_GTEST := \
	-Og \
	-ggdb

# --- Linker settings ---
LDFLAGS := \
	-Wl,-rpath=$(INSTALL_LIB_DIR)

LDFLAGS_DEBUG := \

LDFLAGS_RELEASE := \

# --- Library settings ---
LIBS_DEBUG := \
	-L$(INSTALL_LIB_DIR) \
	-lhost_monitor_dbg \
	-lpthread \
	-lncurses

LIBS_RELEASE := \
	-L$(INSTALL_LIB_DIR) \
	-lhost_monitor \
	-lpthread \
	-lncurses

# --- Execution Arguments ---
RUN_ARGS := \
	-f ./config/host_monitor_cli

# Include actual make targets
include etc/make/targets.mk