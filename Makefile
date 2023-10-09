# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Dawid\Desktop\GIT\OpenGL\Landscape

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Dawid\Desktop\GIT\OpenGL\Landscape

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"C:\Program Files\CMake\bin\cmake-gui.exe" -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"C:\Program Files\CMake\bin\cmake.exe" --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	"C:\Program Files\CMake\bin\cmake.exe" -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	"C:\Program Files\CMake\bin\cmake.exe" -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	"C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	"C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	"C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	"C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start C:\Users\Dawid\Desktop\GIT\OpenGL\Landscape\CMakeFiles C:\Users\Dawid\Desktop\GIT\OpenGL\Landscape\\CMakeFiles\progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start C:\Users\Dawid\Desktop\GIT\OpenGL\Landscape\CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles\Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Main

# Build rule for target.
Main: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 Main
.PHONY : Main

# fast build rule for target.
Main/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Main.dir\build.make CMakeFiles/Main.dir/build
.PHONY : Main/fast

#=============================================================================
# Target rules for targets named lib_all

# Build rule for target.
lib_all: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 lib_all
.PHONY : lib_all

# fast build rule for target.
lib_all/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\lib_all.dir\build.make CMakeFiles/lib_all.dir/build
.PHONY : lib_all/fast

#=============================================================================
# Target rules for targets named MyTests

# Build rule for target.
MyTests: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 MyTests
.PHONY : MyTests

# fast build rule for target.
MyTests/fast:
	$(MAKE) $(MAKESILENT) -f tests\CMakeFiles\MyTests.dir\build.make tests/CMakeFiles/MyTests.dir/build
.PHONY : MyTests/fast

#=============================================================================
# Target rules for targets named gmock

# Build rule for target.
gmock: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 gmock
.PHONY : gmock

# fast build rule for target.
gmock/fast:
	$(MAKE) $(MAKESILENT) -f _deps\googletest-build\googlemock\CMakeFiles\gmock.dir\build.make _deps/googletest-build/googlemock/CMakeFiles/gmock.dir/build
.PHONY : gmock/fast

#=============================================================================
# Target rules for targets named gmock_main

# Build rule for target.
gmock_main: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 gmock_main
.PHONY : gmock_main

# fast build rule for target.
gmock_main/fast:
	$(MAKE) $(MAKESILENT) -f _deps\googletest-build\googlemock\CMakeFiles\gmock_main.dir\build.make _deps/googletest-build/googlemock/CMakeFiles/gmock_main.dir/build
.PHONY : gmock_main/fast

#=============================================================================
# Target rules for targets named gtest

# Build rule for target.
gtest: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 gtest
.PHONY : gtest

# fast build rule for target.
gtest/fast:
	$(MAKE) $(MAKESILENT) -f _deps\googletest-build\googletest\CMakeFiles\gtest.dir\build.make _deps/googletest-build/googletest/CMakeFiles/gtest.dir/build
.PHONY : gtest/fast

#=============================================================================
# Target rules for targets named gtest_main

# Build rule for target.
gtest_main: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 gtest_main
.PHONY : gtest_main

# fast build rule for target.
gtest_main/fast:
	$(MAKE) $(MAKESILENT) -f _deps\googletest-build\googletest\CMakeFiles\gtest_main.dir\build.make _deps/googletest-build/googletest/CMakeFiles/gtest_main.dir/build
.PHONY : gtest_main/fast

Main.obj: Main.cpp.obj
.PHONY : Main.obj

# target to build an object file
Main.cpp.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Main.dir\build.make CMakeFiles/Main.dir/Main.cpp.obj
.PHONY : Main.cpp.obj

Main.i: Main.cpp.i
.PHONY : Main.i

# target to preprocess a source file
Main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Main.dir\build.make CMakeFiles/Main.dir/Main.cpp.i
.PHONY : Main.cpp.i

Main.s: Main.cpp.s
.PHONY : Main.s

# target to generate assembly for a file
Main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Main.dir\build.make CMakeFiles/Main.dir/Main.cpp.s
.PHONY : Main.cpp.s

src/classes/helper.obj: src/classes/helper.cpp.obj
.PHONY : src/classes/helper.obj

# target to build an object file
src/classes/helper.cpp.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\lib_all.dir\build.make CMakeFiles/lib_all.dir/src/classes/helper.cpp.obj
.PHONY : src/classes/helper.cpp.obj

src/classes/helper.i: src/classes/helper.cpp.i
.PHONY : src/classes/helper.i

# target to preprocess a source file
src/classes/helper.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\lib_all.dir\build.make CMakeFiles/lib_all.dir/src/classes/helper.cpp.i
.PHONY : src/classes/helper.cpp.i

src/classes/helper.s: src/classes/helper.cpp.s
.PHONY : src/classes/helper.s

# target to generate assembly for a file
src/classes/helper.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\lib_all.dir\build.make CMakeFiles/lib_all.dir/src/classes/helper.cpp.s
.PHONY : src/classes/helper.cpp.s

# Help Target
help:
	@echo The following are some of the valid targets for this Makefile:
	@echo ... all (the default if no target is provided)
	@echo ... clean
	@echo ... depend
	@echo ... edit_cache
	@echo ... install
	@echo ... install/local
	@echo ... install/strip
	@echo ... list_install_components
	@echo ... rebuild_cache
	@echo ... Main
	@echo ... MyTests
	@echo ... gmock
	@echo ... gmock_main
	@echo ... gtest
	@echo ... gtest_main
	@echo ... lib_all
	@echo ... Main.obj
	@echo ... Main.i
	@echo ... Main.s
	@echo ... src/classes/helper.obj
	@echo ... src/classes/helper.i
	@echo ... src/classes/helper.s
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles\Makefile.cmake 0
.PHONY : cmake_check_build_system

