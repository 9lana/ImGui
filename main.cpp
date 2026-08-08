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


bool clearMousePos = true, setup = false;
struct UnityEngine_Vector2_Fields {
    float x;
    float y;
};

struct UnityEngine_Vector2_o {
    UnityEngine_Vector2_Fields fields;
};

enum TouchPhase {
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4
};




struct UnityEngine_Touch_Fields {
    int32_t m_FingerId;
    struct UnityEngine_Vector2_o m_Position;
    struct UnityEngine_Vector2_o m_RawPosition;
    struct UnityEngine_Vector2_o m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_fRadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};


void touch() {
    ImGuiIO& io = ImGui::GetIO();
    int (*TouchCount)(void*) = (int (*)(void*)) (Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "get_touchCount", 0));
    int touchCount = TouchCount(nullptr);
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(int)) (Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "GetTouch", 1))) (0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;

        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                io.MouseDown[0] = true;
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                should_clear_mouse_pos = true;
                break;
            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;
            default:
                break;
        }
    } else {
        io.MouseDown[0] = false;
    }
}
EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSawpBuffer(EGLDisplay dpy, EGLSurface surface) {
    static bool g_Initialized = false;
    static bool should_clear_mouse_pos = false;
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
    touch();
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
    if (should_clear_mouse_pos) {
        io.MousePos = ImVec2(-1, -1);
        should_clear_mouse_pos = false;
    }
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
    
}
