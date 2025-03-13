cmake_minimum_required(VERSION 3.14)
project(Autocomplete_using_Trie_Datastructures)

# Set Qt path (replace with your Qt installation path)
set(CMAKE_PREFIX_PATH "/Users/roshank/Qt/6.8.2/macos/lib/cmake")

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add your source files
set(SOURCES
        main.cpp
        mainwindow.cpp
        trie.cpp
)

# Add your header files
set(HEADERS
        mainwindow.h
        trie.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)