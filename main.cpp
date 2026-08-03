#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <bits/pthread_types.h>
#include <dlfcn.h>
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "Dobby/dobby.h"

#include <pthread.h>
#include <jni.h>
#include <sys/cdefs.h>
EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSawpBuffer(EGLDisplay dpy, EGLSurface surface) {
    static bool g_Initialized = false;
    if (!g_Initialized) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGui::StyleColorsDark();
        g_Initialized = true;
    }
    bool idk = false;
    float value = 0.0f;
    EGLint w, h;
    eglQuerySurface(dpy, surface, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Always);
    ImGui::Begin("Dear ImGui");
    ImGui::Text("Android!");
    ImGui::Checkbox("Click", &idk);
    ImGui::SliderFloat("Value",&value,0.0f,100.0f);
    ImGui::Text("Bye!");
    ImGui::End(); 
    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    return orig_eglSwapBuffers(dpy, surface);

}
void *sylphy(void*) {
    void *egl = dlopen("libEGL.so", RTLD_NOW);
    if (!egl) {
        return nullptr;
    }
    void *swap = dlsym(egl, "eglSwapBuffers");
    if (!swap) {
        return nullptr;
    }
    DobbyHook(swap, (void*)hook_eglSawpBuffer, (void**)&orig_eglSwapBuffers);
    return nullptr;
}
__attribute__((constructor))
void lib_main() {
    pthread_t trixie;
    pthread_create(&trixie, NULL, sylphy, NULL);
    pthread_detach(trixie);
    
}
