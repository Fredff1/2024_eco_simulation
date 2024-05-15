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
CMAKE_COMMAND = D:\mingw64\bin\cmake.exe

# The command to remove a file.
RM = D:\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Code\Cpp\Eco_simulation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Code\Cpp\Eco_simulation\build

# Utility rule file for copy_assets.

# Include any custom commands dependencies for this target.
include CMakeFiles/copy_assets.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/copy_assets.dir/progress.make

CMakeFiles/copy_assets: src/assets/Consumer_Normal.png
CMakeFiles/copy_assets: src/assets/Producer_Normal.png

src/assets/Consumer_Normal.png: D:/Code/Cpp/Eco_simulation/assets/Consumer_Normal.png
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=D:\Code\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Copying Consumer_Normal.png to assets directory"
	D:\mingw64\bin\cmake.exe -E copy_if_different D:/Code/Cpp/Eco_simulation/Assets/Consumer_Normal.png D:/Code/Cpp/Eco_simulation/build/src/Assets/Consumer_Normal.png

src/assets/Producer_Normal.png: D:/Code/Cpp/Eco_simulation/assets/Producer_Normal.png
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=D:\Code\Cpp\Eco_simulation\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Copying Producer_Normal.png to assets directory"
	D:\mingw64\bin\cmake.exe -E copy_if_different D:/Code/Cpp/Eco_simulation/Assets/Producer_Normal.png D:/Code/Cpp/Eco_simulation/build/src/Assets/Producer_Normal.png

copy_assets: CMakeFiles/copy_assets
copy_assets: src/assets/Consumer_Normal.png
copy_assets: src/assets/Producer_Normal.png
copy_assets: CMakeFiles/copy_assets.dir/build.make
.PHONY : copy_assets

# Rule to build all files generated by this target.
CMakeFiles/copy_assets.dir/build: copy_assets
.PHONY : CMakeFiles/copy_assets.dir/build

CMakeFiles/copy_assets.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\copy_assets.dir\cmake_clean.cmake
.PHONY : CMakeFiles/copy_assets.dir/clean

CMakeFiles/copy_assets.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Code\Cpp\Eco_simulation D:\Code\Cpp\Eco_simulation D:\Code\Cpp\Eco_simulation\build D:\Code\Cpp\Eco_simulation\build D:\Code\Cpp\Eco_simulation\build\CMakeFiles\copy_assets.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/copy_assets.dir/depend
