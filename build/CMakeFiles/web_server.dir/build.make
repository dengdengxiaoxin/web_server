# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/cmake/1088/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1088/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dx/CLionProjects/web_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dx/CLionProjects/web_server/build

# Include any dependencies generated for this target.
include CMakeFiles/web_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/web_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/web_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/web_server.dir/flags.make

CMakeFiles/web_server.dir/main.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/main.cpp.o: ../main.cpp
CMakeFiles/web_server.dir/main.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/web_server.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/main.cpp.o -MF CMakeFiles/web_server.dir/main.cpp.o.d -o CMakeFiles/web_server.dir/main.cpp.o -c /home/dx/CLionProjects/web_server/main.cpp

CMakeFiles/web_server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/main.cpp > CMakeFiles/web_server.dir/main.cpp.i

CMakeFiles/web_server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/main.cpp -o CMakeFiles/web_server.dir/main.cpp.s

CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o: ../http_conn/http_conn.cpp
CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o -MF CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o.d -o CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o -c /home/dx/CLionProjects/web_server/http_conn/http_conn.cpp

CMakeFiles/web_server.dir/http_conn/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/http_conn/http_conn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/http_conn/http_conn.cpp > CMakeFiles/web_server.dir/http_conn/http_conn.cpp.i

CMakeFiles/web_server.dir/http_conn/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/http_conn/http_conn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/http_conn/http_conn.cpp -o CMakeFiles/web_server.dir/http_conn/http_conn.cpp.s

CMakeFiles/web_server.dir/log/log.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/log/log.cpp.o: ../log/log.cpp
CMakeFiles/web_server.dir/log/log.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/web_server.dir/log/log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/log/log.cpp.o -MF CMakeFiles/web_server.dir/log/log.cpp.o.d -o CMakeFiles/web_server.dir/log/log.cpp.o -c /home/dx/CLionProjects/web_server/log/log.cpp

CMakeFiles/web_server.dir/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/log/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/log/log.cpp > CMakeFiles/web_server.dir/log/log.cpp.i

CMakeFiles/web_server.dir/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/log/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/log/log.cpp -o CMakeFiles/web_server.dir/log/log.cpp.s

CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o: ../sql_conn/sql_connection_pool.cpp
CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o -MF CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o.d -o CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o -c /home/dx/CLionProjects/web_server/sql_conn/sql_connection_pool.cpp

CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/sql_conn/sql_connection_pool.cpp > CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.i

CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/sql_conn/sql_connection_pool.cpp -o CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.s

CMakeFiles/web_server.dir/signal/signal_about.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/signal/signal_about.cpp.o: ../signal/signal_about.cpp
CMakeFiles/web_server.dir/signal/signal_about.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/web_server.dir/signal/signal_about.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/signal/signal_about.cpp.o -MF CMakeFiles/web_server.dir/signal/signal_about.cpp.o.d -o CMakeFiles/web_server.dir/signal/signal_about.cpp.o -c /home/dx/CLionProjects/web_server/signal/signal_about.cpp

CMakeFiles/web_server.dir/signal/signal_about.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/signal/signal_about.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/signal/signal_about.cpp > CMakeFiles/web_server.dir/signal/signal_about.cpp.i

CMakeFiles/web_server.dir/signal/signal_about.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/signal/signal_about.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/signal/signal_about.cpp -o CMakeFiles/web_server.dir/signal/signal_about.cpp.s

CMakeFiles/web_server.dir/socket/socket_server.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/socket/socket_server.cpp.o: ../socket/socket_server.cpp
CMakeFiles/web_server.dir/socket/socket_server.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/web_server.dir/socket/socket_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/socket/socket_server.cpp.o -MF CMakeFiles/web_server.dir/socket/socket_server.cpp.o.d -o CMakeFiles/web_server.dir/socket/socket_server.cpp.o -c /home/dx/CLionProjects/web_server/socket/socket_server.cpp

CMakeFiles/web_server.dir/socket/socket_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/socket/socket_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/socket/socket_server.cpp > CMakeFiles/web_server.dir/socket/socket_server.cpp.i

CMakeFiles/web_server.dir/socket/socket_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/socket/socket_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/socket/socket_server.cpp -o CMakeFiles/web_server.dir/socket/socket_server.cpp.s

CMakeFiles/web_server.dir/web/web.cpp.o: CMakeFiles/web_server.dir/flags.make
CMakeFiles/web_server.dir/web/web.cpp.o: ../web/web.cpp
CMakeFiles/web_server.dir/web/web.cpp.o: CMakeFiles/web_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/web_server.dir/web/web.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/web_server.dir/web/web.cpp.o -MF CMakeFiles/web_server.dir/web/web.cpp.o.d -o CMakeFiles/web_server.dir/web/web.cpp.o -c /home/dx/CLionProjects/web_server/web/web.cpp

CMakeFiles/web_server.dir/web/web.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server.dir/web/web.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dx/CLionProjects/web_server/web/web.cpp > CMakeFiles/web_server.dir/web/web.cpp.i

CMakeFiles/web_server.dir/web/web.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server.dir/web/web.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dx/CLionProjects/web_server/web/web.cpp -o CMakeFiles/web_server.dir/web/web.cpp.s

# Object files for target web_server
web_server_OBJECTS = \
"CMakeFiles/web_server.dir/main.cpp.o" \
"CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o" \
"CMakeFiles/web_server.dir/log/log.cpp.o" \
"CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o" \
"CMakeFiles/web_server.dir/signal/signal_about.cpp.o" \
"CMakeFiles/web_server.dir/socket/socket_server.cpp.o" \
"CMakeFiles/web_server.dir/web/web.cpp.o"

# External object files for target web_server
web_server_EXTERNAL_OBJECTS =

web_server: CMakeFiles/web_server.dir/main.cpp.o
web_server: CMakeFiles/web_server.dir/http_conn/http_conn.cpp.o
web_server: CMakeFiles/web_server.dir/log/log.cpp.o
web_server: CMakeFiles/web_server.dir/sql_conn/sql_connection_pool.cpp.o
web_server: CMakeFiles/web_server.dir/signal/signal_about.cpp.o
web_server: CMakeFiles/web_server.dir/socket/socket_server.cpp.o
web_server: CMakeFiles/web_server.dir/web/web.cpp.o
web_server: CMakeFiles/web_server.dir/build.make
web_server: CMakeFiles/web_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dx/CLionProjects/web_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable web_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/web_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/web_server.dir/build: web_server
.PHONY : CMakeFiles/web_server.dir/build

CMakeFiles/web_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/web_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/web_server.dir/clean

CMakeFiles/web_server.dir/depend:
	cd /home/dx/CLionProjects/web_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dx/CLionProjects/web_server /home/dx/CLionProjects/web_server /home/dx/CLionProjects/web_server/build /home/dx/CLionProjects/web_server/build /home/dx/CLionProjects/web_server/build/CMakeFiles/web_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/web_server.dir/depend

