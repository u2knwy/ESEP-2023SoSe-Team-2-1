# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(Construct)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add the source files to the project
add_executable(Construct main.cpp Watchdog.cpp Watchdog.h Logger.cpp Logger.h eventmanager.cpp msg_enum.h msg_string_table.h msg_estring_def.h)

# Optional: If you have additional source files, you can add them here using the same format:
# add_executable(MyProject main.cpp file1.cpp file2.cpp)

# Optional: If you have header files, you can specify the include directories using the include_directories() function:
# include_directories(include)

# Optional: If you have external libraries that your project depends on, you can specify them using the target_link_libraries() function:
# target_link_libraries(MyProject library1 library2)

# Optional: If you need to set compiler flags, you can do so using the target_compile_options() function:
# target_compile_options(MyProject PRIVATE -Wall -Wextra)
