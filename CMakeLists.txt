cmake_minimum_required(VERSION 3.22)

project("ImGui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(test SHARED
    main.cpp
    ImGui/imgui.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp
    # imgui/imgui_demo.cpp

    ImGui/backends/imgui_impl_android.cpp
    ImGui/backends/imgui_impl_opengl3.cpp
    ByNameModding/fake_dlfcn.cpp
    ByNameModding/Il2Cpp.cpp

)
target_include_directories(test PRIVATE
    .
    ImGui
    ImGui/backends
    Dobby
    ByNameModding
    
)
target_link_libraries(test
    ${CMAKE_CURRENT_SOURCE_DIR}/Dobby/libdobby.a 
    log
    android 
    EGL
    GLESv3
)
