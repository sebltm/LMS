# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/IO.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/IO.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IO.dir/flags.make

CMakeFiles/IO.dir/library.c.obj: CMakeFiles/IO.dir/flags.make
CMakeFiles/IO.dir/library.c.obj: ../library.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/IO.dir/library.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\IO.dir\library.c.obj   -c "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\library.c"

CMakeFiles/IO.dir/library.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IO.dir/library.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\library.c" > CMakeFiles\IO.dir\library.c.i

CMakeFiles/IO.dir/library.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IO.dir/library.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\library.c" -o CMakeFiles\IO.dir\library.c.s

# Object files for target IO
IO_OBJECTS = \
"CMakeFiles/IO.dir/library.c.obj"

# External object files for target IO
IO_EXTERNAL_OBJECTS =

libIO.a: CMakeFiles/IO.dir/library.c.obj
libIO.a: CMakeFiles/IO.dir/build.make
libIO.a: CMakeFiles/IO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libIO.a"
	$(CMAKE_COMMAND) -P CMakeFiles\IO.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\IO.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IO.dir/build: libIO.a

.PHONY : CMakeFiles/IO.dir/build

CMakeFiles/IO.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\IO.dir\cmake_clean.cmake
.PHONY : CMakeFiles/IO.dir/clean

CMakeFiles/IO.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO" "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO" "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\cmake-build-debug" "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\cmake-build-debug" "C:\Users\Sebastien Michel\OneDrive\Documents\Exeter\Individual Literature Review and Project\IO\cmake-build-debug\CMakeFiles\IO.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/IO.dir/depend
