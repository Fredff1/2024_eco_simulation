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
include src/Util/CMakeFiles/Util.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/Util/CMakeFiles/Util.dir/compiler_depend.make

# Include the progress variables for this target.
include src/Util/CMakeFiles/Util.dir/progress.make

# Include the compile flags for this target's objects.
include src/Util/CMakeFiles/Util.dir/flags.make

src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj: src/Util/CMakeFiles/Util.dir/flags.make
src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj: src/Util/CMakeFiles/Util.dir/includes_CXX.rsp
src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj: D:/Cpp/Eco_simulation/src/Util/GlobalFunc.cpp
src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj: src/Util/CMakeFiles/Util.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj -MF CMakeFiles\Util.dir\GlobalFunc.cpp.obj.d -o CMakeFiles\Util.dir\GlobalFunc.cpp.obj -c D:\Cpp\Eco_simulation\src\Util\GlobalFunc.cpp

src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Util.dir/GlobalFunc.cpp.i"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Cpp\Eco_simulation\src\Util\GlobalFunc.cpp > CMakeFiles\Util.dir\GlobalFunc.cpp.i

src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Util.dir/GlobalFunc.cpp.s"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Cpp\Eco_simulation\src\Util\GlobalFunc.cpp -o CMakeFiles\Util.dir\GlobalFunc.cpp.s

src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj: src/Util/CMakeFiles/Util.dir/flags.make
src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj: src/Util/CMakeFiles/Util.dir/includes_CXX.rsp
src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj: D:/Cpp/Eco_simulation/src/Util/RandomUtil.cpp
src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj: src/Util/CMakeFiles/Util.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj -MF CMakeFiles\Util.dir\RandomUtil.cpp.obj.d -o CMakeFiles\Util.dir\RandomUtil.cpp.obj -c D:\Cpp\Eco_simulation\src\Util\RandomUtil.cpp

src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Util.dir/RandomUtil.cpp.i"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Cpp\Eco_simulation\src\Util\RandomUtil.cpp > CMakeFiles\Util.dir\RandomUtil.cpp.i

src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Util.dir/RandomUtil.cpp.s"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Cpp\Eco_simulation\src\Util\RandomUtil.cpp -o CMakeFiles\Util.dir\RandomUtil.cpp.s

src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj: src/Util/CMakeFiles/Util.dir/flags.make
src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj: src/Util/CMakeFiles/Util.dir/includes_CXX.rsp
src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj: D:/Cpp/Eco_simulation/src/Util/ThreadSafeQueue.cpp
src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj: src/Util/CMakeFiles/Util.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj -MF CMakeFiles\Util.dir\ThreadSafeQueue.cpp.obj.d -o CMakeFiles\Util.dir\ThreadSafeQueue.cpp.obj -c D:\Cpp\Eco_simulation\src\Util\ThreadSafeQueue.cpp

src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Util.dir/ThreadSafeQueue.cpp.i"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Cpp\Eco_simulation\src\Util\ThreadSafeQueue.cpp > CMakeFiles\Util.dir\ThreadSafeQueue.cpp.i

src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Util.dir/ThreadSafeQueue.cpp.s"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Cpp\Eco_simulation\src\Util\ThreadSafeQueue.cpp -o CMakeFiles\Util.dir\ThreadSafeQueue.cpp.s

# Object files for target Util
Util_OBJECTS = \
"CMakeFiles/Util.dir/GlobalFunc.cpp.obj" \
"CMakeFiles/Util.dir/RandomUtil.cpp.obj" \
"CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj"

# External object files for target Util
Util_EXTERNAL_OBJECTS =

src/Util/libUtil.a: src/Util/CMakeFiles/Util.dir/GlobalFunc.cpp.obj
src/Util/libUtil.a: src/Util/CMakeFiles/Util.dir/RandomUtil.cpp.obj
src/Util/libUtil.a: src/Util/CMakeFiles/Util.dir/ThreadSafeQueue.cpp.obj
src/Util/libUtil.a: src/Util/CMakeFiles/Util.dir/build.make
src/Util/libUtil.a: src/Util/CMakeFiles/Util.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libUtil.a"
	cd /d D:\Cpp\Eco_simulation\build\src\Util && $(CMAKE_COMMAND) -P CMakeFiles\Util.dir\cmake_clean_target.cmake
	cd /d D:\Cpp\Eco_simulation\build\src\Util && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Util.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Util/CMakeFiles/Util.dir/build: src/Util/libUtil.a
.PHONY : src/Util/CMakeFiles/Util.dir/build

src/Util/CMakeFiles/Util.dir/clean:
	cd /d D:\Cpp\Eco_simulation\build\src\Util && $(CMAKE_COMMAND) -P CMakeFiles\Util.dir\cmake_clean.cmake
.PHONY : src/Util/CMakeFiles/Util.dir/clean

src/Util/CMakeFiles/Util.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Cpp\Eco_simulation D:\Cpp\Eco_simulation\src\Util D:\Cpp\Eco_simulation\build D:\Cpp\Eco_simulation\build\src\Util D:\Cpp\Eco_simulation\build\src\Util\CMakeFiles\Util.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/Util/CMakeFiles/Util.dir/depend

