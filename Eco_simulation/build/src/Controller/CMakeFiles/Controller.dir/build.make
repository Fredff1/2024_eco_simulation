# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_COMMAND = D:\gcc\mingw64\bin\cmake.exe

# The command to remove a file.
RM = D:\gcc\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Cpp\Eco_simulation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Cpp\Eco_simulation\build

# Include any dependencies generated for this target.
include src/Controller/CMakeFiles/Controller.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/Controller/CMakeFiles/Controller.dir/compiler_depend.make

# Include the progress variables for this target.
include src/Controller/CMakeFiles/Controller.dir/progress.make

# Include the compile flags for this target's objects.
include src/Controller/CMakeFiles/Controller.dir/flags.make

src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj: src/Controller/CMakeFiles/Controller.dir/flags.make
src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj: src/Controller/CMakeFiles/Controller.dir/includes_CXX.rsp
src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj: D:/Cpp/Eco_simulation/src/Controller/MainController.cpp
src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj: src/Controller/CMakeFiles/Controller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj"
	cd /d D:\Cpp\Eco_simulation\build\src\Controller && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj -MF CMakeFiles\Controller.dir\MainController.cpp.obj.d -o CMakeFiles\Controller.dir\MainController.cpp.obj -c D:\Cpp\Eco_simulation\src\Controller\MainController.cpp

src/Controller/CMakeFiles/Controller.dir/MainController.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Controller.dir/MainController.cpp.i"
	cd /d D:\Cpp\Eco_simulation\build\src\Controller && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Cpp\Eco_simulation\src\Controller\MainController.cpp > CMakeFiles\Controller.dir\MainController.cpp.i

src/Controller/CMakeFiles/Controller.dir/MainController.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Controller.dir/MainController.cpp.s"
	cd /d D:\Cpp\Eco_simulation\build\src\Controller && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Cpp\Eco_simulation\src\Controller\MainController.cpp -o CMakeFiles\Controller.dir\MainController.cpp.s

# Object files for target Controller
Controller_OBJECTS = \
"CMakeFiles/Controller.dir/MainController.cpp.obj"

# External object files for target Controller
Controller_EXTERNAL_OBJECTS =

src/Controller/libController.a: src/Controller/CMakeFiles/Controller.dir/MainController.cpp.obj
src/Controller/libController.a: src/Controller/CMakeFiles/Controller.dir/build.make
src/Controller/libController.a: src/Controller/CMakeFiles/Controller.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libController.a"
	cd /d D:\Cpp\Eco_simulation\build\src\Controller && $(CMAKE_COMMAND) -P CMakeFiles\Controller.dir\cmake_clean_target.cmake
	cd /d D:\Cpp\Eco_simulation\build\src\Controller && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Controller.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Controller/CMakeFiles/Controller.dir/build: src/Controller/libController.a
.PHONY : src/Controller/CMakeFiles/Controller.dir/build

src/Controller/CMakeFiles/Controller.dir/clean:
	cd /d D:\Cpp\Eco_simulation\build\src\Controller && $(CMAKE_COMMAND) -P CMakeFiles\Controller.dir\cmake_clean.cmake
.PHONY : src/Controller/CMakeFiles/Controller.dir/clean

src/Controller/CMakeFiles/Controller.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Cpp\Eco_simulation D:\Cpp\Eco_simulation\src\Controller D:\Cpp\Eco_simulation\build D:\Cpp\Eco_simulation\build\src\Controller D:\Cpp\Eco_simulation\build\src\Controller\CMakeFiles\Controller.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/Controller/CMakeFiles/Controller.dir/depend

