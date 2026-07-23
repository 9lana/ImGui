cmake_minimum_required(VERSION 3.22)

project("ImGui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(test SHARED
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    # imgui/imgui_demo.cpp

    imgui/backends/imgui_impl_android.cpp
    imgui/backends/imgui_impl_opengl3.cpp

)
target_include_directories(test PRIVATE
    .
    ImGui
    ImGui/backends
    Dobby
)
target_link_libraries(test
    ${CMAKE_CURRENT_SOURCE_DIR}/Dobby/libdobby.a 
    log
    android 
    EGL
    GLESv3
)
