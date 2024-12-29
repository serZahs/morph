#include "stdio.h"
#include "windows.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "util.h"
#include "find.h"

int main()
{
    InitPool(&ResultsPool, 1*1024*1024);
    InitPool(&ResultsDataPool, 1*1024*1024);
    find_results result_list;
    result_list.Count = 0;
    result_list.Results = NULL;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    const char* GLSLVersion = "#version 330";
    
    int window_width = 1280;
    int window_height = 960;
    
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont *font1 = io.Fonts->AddFontFromFileTTF("fonts/segoeui.ttf", 24);
    ImFont *font2 = io.Fonts->AddFontFromFileTTF("fonts/consola.ttf", 18);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSLVersion);
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        bool p_open = true;
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        ImGui::Begin("Example fullscreen", &p_open, flags);
        
        static char filename[128] = "";
        ImGui::InputTextWithHint("File path", "C:/dev/my_proj/main.c", filename, IM_ARRAYSIZE(filename));
        static char pattern[128] = "";
        ImGui::InputTextWithHint("Search pattern", "DrawRectangle", pattern, IM_ARRAYSIZE(pattern));
        
        if (ImGui::Button("Find!"))
        {
            result_list.Count = 0;
            result_list.Results = NULL;
            Find(&result_list, filename, pattern);
        }

        if (result_list.Count > 0)
        {
            ImGuiTableFlags flags = ImGuiTableFlags_Resizable;
            if (ImGui::BeginTable("table1", 2, flags))
            {
                ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                ImGui::TableSetupColumn("Line");
                ImGui::TableHeadersRow();
                ImGui::PushFont(font2);
                for (int index = 0; index < result_list.Count; index++)
                {
                    result_instance result = result_list.Results[index];
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(result.Line);
                    ImGui::TableSetColumnIndex(0);
                    char buffer[200];
                    sprintf_s(buffer, 200, "%s:%d", result.Filename, result.LineNumber);
                    ImGui::TextUnformatted(buffer);
                }
                ImGui::PopFont();
                ImGui::EndTable();
            }
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}