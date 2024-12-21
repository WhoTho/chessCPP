cmake_minimum_required(VERSION 3.10)
# set(CMAKE_PREFIX_PATH "c:\\Program Files (x86)\\Catch2")
set(Catch2_DIR "C:\\Program Files (x86)\\Catch2\\lib\\cmake\\Catch2")
find_package(Catch2 3 REQUIRED)

# option(BUILD_SHARED_LIBS "Build shared libraries" ON)
# set(SFML_STATIC_LIBRARIES TRUE)

# set(SFML_DIR "C:\\Users\\alesa\\Downloads\\SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit\\SFML-2.6.2\\lib\\cmake\\SFML")

# find_package(SFML 2.6 COMPONENTS graphics REQUIRED) # window system REQUIRED)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Project name
project(ChessCPP)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the main executable
add_executable(main
Board.cpp
Minimax.cpp
Piece.cpp
main.cpp
)

add_executable(main-gui
Board.cpp
Minimax.cpp
Piece.cpp
mainGUI.cpp
)

# Add the test executable
add_executable(tests
Board.cpp
Minimax.cpp
Piece.cpp
testing/perfts/perftTester.cpp
testing/MinimaxTests.cpp
)



# Include directories for both executables
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(main-gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(main-gui PRIVATE SFML::Graphics SFML::Window SFML::System)
# target_link_libraries(main-gui PRIVATE sfml-graphics) # sfml-window sfml-system opengl32 ws2_32 winmm gdi32)

