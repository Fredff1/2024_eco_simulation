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
include src/CMakeFiles/EcoSystem.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/EcoSystem.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/EcoSystem.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/EcoSystem.dir/flags.make

src/CMakeFiles/EcoSystem.dir/main.cpp.obj: src/CMakeFiles/EcoSystem.dir/flags.make
src/CMakeFiles/EcoSystem.dir/main.cpp.obj: src/CMakeFiles/EcoSystem.dir/includes_CXX.rsp
src/CMakeFiles/EcoSystem.dir/main.cpp.obj: D:/Cpp/Eco_simulation/src/main.cpp
src/CMakeFiles/EcoSystem.dir/main.cpp.obj: src/CMakeFiles/EcoSystem.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/EcoSystem.dir/main.cpp.obj"
	cd /d D:\Cpp\Eco_simulation\build\src && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/EcoSystem.dir/main.cpp.obj -MF CMakeFiles\EcoSystem.dir\main.cpp.obj.d -o CMakeFiles\EcoSystem.dir\main.cpp.obj -c D:\Cpp\Eco_simulation\src\main.cpp

src/CMakeFiles/EcoSystem.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/EcoSystem.dir/main.cpp.i"
	cd /d D:\Cpp\Eco_simulation\build\src && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Cpp\Eco_simulation\src\main.cpp > CMakeFiles\EcoSystem.dir\main.cpp.i

src/CMakeFiles/EcoSystem.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/EcoSystem.dir/main.cpp.s"
	cd /d D:\Cpp\Eco_simulation\build\src && D:\gcc\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Cpp\Eco_simulation\src\main.cpp -o CMakeFiles\EcoSystem.dir\main.cpp.s

# Object files for target EcoSystem
EcoSystem_OBJECTS = \
"CMakeFiles/EcoSystem.dir/main.cpp.obj"

# External object files for target EcoSystem
EcoSystem_EXTERNAL_OBJECTS =

src/EcoSystem.exe: src/CMakeFiles/EcoSystem.dir/main.cpp.obj
src/EcoSystem.exe: src/CMakeFiles/EcoSystem.dir/build.make
src/EcoSystem.exe: src/Util/libUtil.a
src/EcoSystem.exe: src/Model/libModel.a
src/EcoSystem.exe: src/View/libView.a
src/EcoSystem.exe: src/Controller/libController.a
src/EcoSystem.exe: D:/Cpp/Eco_simulation/SDL2-2.28.4/x86_64-w64-mingw32/lib/libSDL2main.a
src/EcoSystem.exe: D:/Cpp/Eco_simulation/SDL2-2.28.4/x86_64-w64-mingw32/lib/libSDL2.dll.a
src/EcoSystem.exe: D:/Cpp/Eco_simulation/SDL2-2.28.4/x86_64-w64-mingw32/lib/libSDL2_ttf.dll.a
src/EcoSystem.exe: D:/Cpp/Eco_simulation/SDL2-2.28.4/x86_64-w64-mingw32/lib/libSDL2_image.dll.a
src/EcoSystem.exe: src/CMakeFiles/EcoSystem.dir/linkLibs.rsp
src/EcoSystem.exe: src/CMakeFiles/EcoSystem.dir/objects1.rsp
src/EcoSystem.exe: src/CMakeFiles/EcoSystem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable EcoSystem.exe"
	cd /d D:\Cpp\Eco_simulation\build\src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\EcoSystem.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/EcoSystem.dir/build: src/EcoSystem.exe
.PHONY : src/CMakeFiles/EcoSystem.dir/build

src/CMakeFiles/EcoSystem.dir/clean:
	cd /d D:\Cpp\Eco_simulation\build\src && $(CMAKE_COMMAND) -P CMakeFiles\EcoSystem.dir\cmake_clean.cmake
.PHONY : src/CMakeFiles/EcoSystem.dir/clean

src/CMakeFiles/EcoSystem.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Cpp\Eco_simulation D:\Cpp\Eco_simulation\src D:\Cpp\Eco_simulation\build D:\Cpp\Eco_simulation\build\src D:\Cpp\Eco_simulation\build\src\CMakeFiles\EcoSystem.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/EcoSystem.dir/depend

