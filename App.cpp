#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "External/GL/glew.h"
#include "External/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "External/STB/stb_image.h"
#include "External/IMGUI/imgui.h"
#include "External/IMGUI/imgui_impl_glfw.h"
#include "External/IMGUI/imgui_impl_opengl3.h"

#include "Universal/Universal.h"

static int menuHeight = 480;
static int menuWidth = 640;

static ImFont* Fonts[4];
static MASH_uint VAO[2], VBO[2], EBO[2];
static float mmVerts[3];
static MASH_uint mmInds[2];
static bool is_hoveredin, is_hoveredout;
static int button = 1;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ImGuiIO& io = ImGui::GetIO();

    // Set the viewport size based on the new framebuffer size
    glViewport(0, 0, width, height);

    //Resize ImGui menu
    menuHeight = height;
    menuWidth = width;
}
MASH_str readFromFile(MASH_str fileName) {
    Universal u;

    MASH_ifstream shaderFile(fileName);
    MASH_sstream buffer;
    if (!shaderFile.is_open()) {
        u.MASH_log("File error!!");
    }

    buffer << shaderFile.rdbuf();

    return buffer.str();
}
int main(void)
{
    Universal u;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Economy App", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    //ImGui::ShowUserGuide();
    //ImGui::ShowFontSelector("Fonts");
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

   
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle(); 
  
    MASH_str fonts[] = {
        "/Res/Fonts/verdanab.ttf",
        "/Res/Fonts/verdanab.ttf",
        "/Res/Fonts/verdanai.ttf",
        "/Res/Fonts/SuperFunky.ttf"
    };

    float fontSizes[] = {
        43.0f, //Used for the text "Welcome to the"
        36.0f,  //Used for the text "Economy App"
        30.0f,
        40.0f
    };

    for (int i = 0; i < 4; i++) {
        MASH_str fontFilePath = std::filesystem::current_path().string() + fonts[i];
        Fonts[i] = io.Fonts->AddFontFromFileTTF(fontFilePath.c_str(), fontSizes[i]);
    };
 
    io.Fonts->Build();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    glfwGetError(NULL);
    glGetError();
    
    
    
    //VARIABLES


    bool showMM = true;
    bool showCredits = false;
    bool showShop = false;
    bool buySweater = false;
    bool showSText = false;
    bool buyPant = false;
    //ImVec4 menuColor = ImVec4(1.0f, 0.0f, 0.0f, 0.75f);
    //style.Colors[ImGuiCol_MenuBarBg] = menuColor;



    //VERTS & INDINCIES

    //Main Menu
    float mmVerts[] = {

        //Position          //Textures
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, //Bottom left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, //Bottom Right
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, //Top Left
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f  //Top Right
    };

    MASH_uint mmInds[] = {
        0, 2, 1,
        1, 2, 3
    };


    //Play start
    float pVerts[] = {

        //Position          //Textures
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, //Bottom left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, //Bottom Right
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, //Top Left
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f  //Top Right
    };

    MASH_uint pInds[] = {
        0, 2, 1,
        1, 2, 3
    };

    //BUFFER GENERATION & BINDING

    //Main Menu
    MASH_uint mmVAO, mmVBO, mmEBO;

    glGenVertexArrays(1, &mmVAO);
    glGenBuffers(1, &mmVBO);
    glGenBuffers(1, &mmEBO);
    
    glBindVertexArray(mmVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mmVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mmVerts), mmVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mmEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mmInds), mmInds, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //Uses the first 3 floats in the vert array
    glEnableVertexAttribArray(0); //inputs the positions into the vert shader with a location of 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //Uses the first 3 floats in the vert array
    glEnableVertexAttribArray(1); //inputs the positions into the vert shader with a location of 0

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Main Menu
    MASH_uint pVAO, pVBO, pEBO;

    glGenVertexArrays(1, &pVAO);
    glGenBuffers(1, &pVBO);
    glGenBuffers(1, &pEBO);

    glBindVertexArray(pVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pVerts), pVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pInds), pInds, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //Uses the first 3 floats in the vert array
    glEnableVertexAttribArray(0); //inputs the positions into the vert shader with a location of 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //Uses the first 3 floats in the vert array
    glEnableVertexAttribArray(1); //inputs the positions into the vert shader with a location of 0

    glBindBuffer(GL_ARRAY_BUFFER, 0);



    //SHADERS


    //Main Menu
    MASH_uint mmShaderProgram = glCreateProgram();
    MASH_uint mmVertShader = glCreateShader(GL_VERTEX_SHADER);
    MASH_uint mmFragShader = glCreateShader(GL_FRAGMENT_SHADER);

    MASH_str vertShaderstr = readFromFile("./Res/Shaders/vertShader.glsl");
    MASH_str fragShaderstr = readFromFile("./Res/Shaders/fragShader.glsl"); 
    MASH_pcchar vertShadersrc = vertShaderstr.c_str();
    MASH_pcchar fragShadersrc = fragShaderstr.c_str();

    glShaderSource(mmVertShader, 1, &vertShadersrc, MASH_null);
    glCompileShader(mmVertShader);
    glShaderSource(mmFragShader, 1, &fragShadersrc, MASH_null);
    glCompileShader(mmFragShader);
    
    glAttachShader(mmShaderProgram, mmVertShader);
    glAttachShader(mmShaderProgram, mmFragShader);
    glLinkProgram(mmShaderProgram);
    glLinkProgram(0);

    //Play
    MASH_uint pShaderProgram = glCreateProgram();
    MASH_uint pVertShader = glCreateShader(GL_VERTEX_SHADER);
    MASH_uint pFragShader = glCreateShader(GL_FRAGMENT_SHADER);

    MASH_str pvertShaderstr = readFromFile("./Res/Shaders/pVertShader.glsl");
    MASH_str pfragShaderstr = readFromFile("./Res/Shaders/pFragShader.glsl");
    MASH_pcchar pvertShadersrc = pvertShaderstr.c_str();
    MASH_pcchar pfragShadersrc = pfragShaderstr.c_str();

    glShaderSource(pVertShader, 1, &pvertShadersrc, MASH_null);
    glCompileShader(pVertShader);
    glShaderSource(pFragShader, 1, &pfragShadersrc, MASH_null);
    glCompileShader(pFragShader);

    glAttachShader(pShaderProgram, pVertShader);
    glAttachShader(pShaderProgram, pFragShader);
    glLinkProgram(pShaderProgram);



    //TEXTURES


    //Main Menu
    MASH_uint mmTexture;

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &mmTexture);
    glBindTexture(GL_TEXTURE_2D, mmTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    MASH_int width, height, channels;
    MASH_punchar image = stbi_load("./Res/Images/Title_Screen.jpg", &width, &height, &channels, STBI_rgb_alpha);

    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
    }
    else
    {
        u.MASH_log("Image error!!");
    }

    //Main Menu
    MASH_uint pTexture;

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &pTexture);
    glBindTexture(GL_TEXTURE_2D, pTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    MASH_int pwidth, pheight, pchannels;
    MASH_punchar pimage = stbi_load("./Res/Images/Title_Screen.jpg", &pwidth, &pheight, &pchannels, STBI_rgb_alpha);

    if (pimage) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pwidth, pheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pimage);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(pimage);
    }
    else
    {
        u.MASH_log("Image error!!");
    }



    //TIE TEXTURE WITH SHADER

    //Main Menu
    MASH_uint texUni = glGetUniformLocation(mmShaderProgram, "ourTexture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mmTexture);
    glUniform1i(texUni, 0);
    u.MASH_log(mmTexture);

    //Main Menu
    MASH_uint ptexUni = glGetUniformLocation(pShaderProgram, "ourTexture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mmTexture);
    glUniform1i(ptexUni, 0);
    u.MASH_log(pTexture);



    std::string enteredText;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (showMM) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
            ImGui::Begin("Main Menu"
                , MASH_null
                , ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_AlwaysAutoResize
                | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings
            );

            ImGui::PushFont(Fonts[0]);
            float bigtitletextWidth = ImGui::CalcTextSize("Welcome to the").x;
            float bigtitlewidth = (menuWidth - bigtitletextWidth) / 2;
            ImGui::SetCursorPos(ImVec2(bigtitlewidth, 15.0f));
            ImGui::Text("Welcome to the");
            ImGui::PopFont();

            ImGui::PushFont(Fonts[1]);
            float smalltitletextWidth = ImGui::CalcTextSize("Economy App").x;
            float smalltitlewidth = (menuWidth - smalltitletextWidth) / 2;
            ImGui::SetCursorPos(ImVec2(smalltitlewidth, 85.0f));
            ImGui::Text("Economy App");
            ImGui::PopFont();

            ImGui::PushFont(Fonts[2]);
            float playButtonItemWidth = 100.0f;
            float playButtonWidth = (menuWidth - playButtonItemWidth) / 2;
            ImGui::SetCursorPos(ImVec2(playButtonWidth, 300.0f));
            if (ImGui::Button("Play", ImVec2(100.0f, 50.0f))) {
                showMM = false;
                showShop = true;
            }
            ImGui::PopFont();

            ImGui::PushFont(Fonts[2]);
            float settingsButtonItemWidth = 125.0f;
            float settingsButtonWidth = (menuWidth - settingsButtonItemWidth) / 2;
            ImGui::SetCursorPos(ImVec2(settingsButtonWidth, 450.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.5f)); //
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 0.5f));
            if (ImGui::Button("Settings", ImVec2(125.0f, 50.0f))) {
            }
            ImGui::PopStyleColor(3);
            ImGui::PopFont();

            ImGui::PushFont(Fonts[2]);
            float creditsButtonItemWidth = 100.0f;
            float creditsButtonWidth = (menuWidth - creditsButtonItemWidth) / 2;
            ImGui::SetCursorPos(ImVec2(creditsButtonWidth, 600.0f));
            if (ImGui::Button("Credits", ImVec2(100.0f, 50.0f))) {
                showMM = false;

                showCredits = true;
            }
            ImGui::PopFont();

            ImGui::PushFont(Fonts[2]);
            float quitButtonItemWidth = 100.0f;
            float quitButtonWidth = (menuWidth - quitButtonItemWidth) / 2;
            ImGui::SetCursorPos(ImVec2(quitButtonWidth, 750.0f));
            if (ImGui::Button("Quit", ImVec2(100.0f, 50.0f))) {
                glfwWindowShouldClose(window);
                glfwTerminate();
            }
            ImGui::PopFont();
            ImGui::End();
            ImGui::EndFrame();

            glUseProgram(mmShaderProgram);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mmTexture);
            glUniform1i(texUni, 0);
            glBindVertexArray(mmVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glUseProgram(0);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        if (showCredits) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
            ImGui::Begin("Main Menu"
                , MASH_null
                , ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_AlwaysAutoResize
                | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings
            );
           
            ImGui::PushFont(Fonts[1]);
            float bigtitletextWidth = ImGui::CalcTextSize("Fonts").x;
            float bigtitlewidth = (menuWidth - bigtitletextWidth) / 2;
            ImGui::SetCursorPos(ImVec2(bigtitlewidth, 35.0f));
            ImGui::Text("Fonts");
            ImGui::PopFont();

            ImGui::PushFont(Fonts[1]);
            bigtitletextWidth = ImGui::CalcTextSize("SuperFunky").x;
            bigtitlewidth = (menuWidth - bigtitletextWidth) / 2;
            ImGui::SetCursorPos(ImVec2(bigtitlewidth, 200.0f));
            ImGui::Text("SuperFunky");
            ImGui::PopFont();

            ImGui::PushFont(Fonts[1]);
            bigtitletextWidth = ImGui::CalcTextSize("verdanab").x;
            bigtitlewidth = (menuWidth - bigtitletextWidth) / 2;
            ImGui::SetCursorPos(ImVec2(bigtitlewidth, 270.0f));
            ImGui::Text("verdanab");
            ImGui::PopFont();

            ImGui::PushFont(Fonts[1]);
            bigtitletextWidth = ImGui::CalcTextSize("verdanai").x;
            bigtitlewidth = (menuWidth - bigtitletextWidth) / 2;
            ImGui::SetCursorPos(ImVec2(bigtitlewidth, 340.0f));
            ImGui::Text("verdanai");
            ImGui::PopFont();

            ImGui::SetCursorPos(ImVec2(bigtitlewidth, 400.0f));
            if (ImGui::Button("Exit")) {
                showCredits = false;

                showMM = true;
            }

            ImGui::End();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        if (showShop) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
            ImGui::Begin("Shop"
                , MASH_null
                , ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_AlwaysAutoResize
                | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings
            );

            ImGui::PushFont(Fonts[3]);
            float questionWidth = ImGui::CalcTextSize("What do you want to buy?").x;
            float questionboxPlacement = (menuWidth - questionWidth) / 2;
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 box_position = ImVec2(questionboxPlacement, 200);
            ImVec2 box_size = ImVec2(430, 60);
            ImU32 box_color = IM_COL32(0, 0, 0, 255); // RGBA color (255, 0, 0, 255 is red)
            draw_list->AddRectFilled(box_position, ImVec2(box_position.x + box_size.x, box_position.y + box_size.y), box_color);
            ImGui::SetCursorPos(ImVec2(box_position.x + 10, box_position.y + 10)); // Adjust text position
            ImGui::Text("What do you want to buy?");
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::PopFont();

            float ShopAreaPlacement = ((float)menuWidth - 1250) / 2;
            ImVec2 ShopArea_position = ImVec2(ShopAreaPlacement, 350);
            ImVec2 ShopArea_size = ImVec2(1300, 400);
            ImU32 ShopArea_color = IM_COL32(0, 0, 255, 255); // RGBA color (255, 0, 0, 255 is red)
            draw_list->AddRectFilled(ShopArea_position, ImVec2(ShopArea_position.x + ShopArea_size.x, ShopArea_position.y + ShopArea_size.y), ShopArea_color);

            ImGui::PushFont(Fonts[3]);
            float pantsButtonPlacement = ((float)menuWidth - 900) / 2;
            float sum = box_position.y + box_size.y + 90;
            ImVec2 center = ImVec2(pantsButtonPlacement, sum + (ShopArea_size.y / 2));
            float radius = 80.0f;
            ImU32 button_color = IM_COL32(255, 0, 0, 255);
            ImVec2 mouse_pos = ImGui::GetMousePos();
            float distance = sqrt((mouse_pos.x - center.x) * (mouse_pos.x - center.x) + (mouse_pos.y - center.y) * (mouse_pos.y - center.y));
            is_hoveredin = distance <= radius;
            is_hoveredout = distance > radius;
            draw_list->AddCircleFilled(center, radius, button_color, 32);
            ImVec2 text_size = ImGui::CalcTextSize("Pants");
            ImVec2 text_position = ImVec2(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f);
            draw_list->AddText(text_position, IM_COL32(255, 255, 255, 255), "Pants");
            if (is_hoveredin && ImGui::IsMouseClicked(0)) {
                showShop = false;
                buyPant = true;

                button = 0;
            }
            ImGui::PopFont();

            ImGui::PushFont(Fonts[3]);
            float sweaterButtonPlacement = ((float)menuWidth + 900) / 2;
            float sweatersum = box_position.y + box_size.y + 90;
            center = ImVec2(sweaterButtonPlacement, sweatersum + (ShopArea_size.y / 2));
            radius = 80.0f;
            button_color = IM_COL32(255, 0, 0, 255);
            mouse_pos = ImGui::GetMousePos();
            distance = sqrt((mouse_pos.x - center.x) * (mouse_pos.x - center.x) + (mouse_pos.y - center.y) * (mouse_pos.y - center.y));
            is_hoveredin = distance <= radius;
            draw_list->AddCircleFilled(center, radius, button_color, 32);
            text_size = ImGui::CalcTextSize("Sweaters");
            text_position = ImVec2(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f);
            draw_list->AddText(text_position, IM_COL32(255, 255, 255, 255), "Sweaters");
            if (is_hoveredin && ImGui::IsMouseClicked(0)) {
                buySweater = true;
                showShop = false;

                button = 0;
            }
            ImGui::PopFont();

            ImGui::PushFont(Fonts[3]);
            ImGui::SetCursorPos(ImVec2(100.0f, 900.0f));
            if (ImGui::Button("Exit")) {
                showShop = false;

                showMM = true;
            }
            ImGui::PopFont();

            glUseProgram(pShaderProgram);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mmShaderProgram);
            glUniform1i(ptexUni, 0);
            glBindVertexArray(pVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glUseProgram(0);

            ImGui::End();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        if (buySweater) {
            showSText = true;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
            ImGui::Begin("Sweater Menu"
                , MASH_null
                , ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_AlwaysAutoResize
                | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings
            );

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            float SweaterPlacement = ((float)menuWidth - 600) / 2;
            ImVec2 Sweater_position = ImVec2(SweaterPlacement, 350);
            ImVec2 Sweater_size = ImVec2(600, 150);
            ImU32 Sweater_color = IM_COL32(0, 0, 255, 255); // RGBA color (255, 0, 0, 255 is red)
            draw_list->AddRectFilled(Sweater_position, ImVec2(Sweater_position.x + Sweater_size.x, Sweater_position.y + Sweater_size.y), Sweater_color);
            ImGui::SetCursorPos(ImVec2(850.0f, 360.0f));
            char inputBuffer[256] = {};
            ImGui::SetNextItemWidth(400);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

            if (ImGui::InputText("##Label", inputBuffer, IM_ARRAYSIZE(inputBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue) && showSText == true) {

                enteredText = inputBuffer;
            }

            if (!enteredText.empty() && showSText == true) {
                ImGui::SetCursorPos(ImVec2(850.0f, 600.0f));
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "You have %s sweaters", enteredText.c_str());
            }
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            ImGui::PushFont(Fonts[3]);
            ImGui::SetCursorPos(ImVec2(675.0f, 430.0f));
            if (ImGui::Button("Exit")) {
                buySweater = false;

                showShop = true;
                enteredText = "";
            }
            ImGui::PopFont();
            ImGui::End();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        if (buyPant) {
            showSText = true;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
            ImGui::Begin("Pant Menu"
                , MASH_null
                , ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_AlwaysAutoResize
                | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings
            );

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            float SweaterPlacement = ((float)menuWidth - 600) / 2;
            ImVec2 Sweater_position = ImVec2(SweaterPlacement, 350);
            ImVec2 Sweater_size = ImVec2(600, 150);
            ImU32 Sweater_color = IM_COL32(0, 0, 255, 255); // RGBA color (255, 0, 0, 255 is red)
            draw_list->AddRectFilled(Sweater_position, ImVec2(Sweater_position.x + Sweater_size.x, Sweater_position.y + Sweater_size.y), Sweater_color);
            ImGui::SetCursorPos(ImVec2(850.0f, 360.0f));
            char inputBuffer[256] = {};
            ImGui::SetNextItemWidth(400);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

            if (ImGui::InputText("##Label", inputBuffer, IM_ARRAYSIZE(inputBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue) && showSText == true) {

                enteredText = inputBuffer;
            }

            if (!enteredText.empty() && showSText == true) {
                ImGui::SetCursorPos(ImVec2(850.0f, 600.0f));
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "You have %s pants", enteredText.c_str());
            }
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            ImGui::PushFont(Fonts[3]);
            ImGui::SetCursorPos(ImVec2(675.0f, 430.0f));
            if (ImGui::Button("Exit")) {
                buyPant = false;

                showShop = true;
                enteredText = "";
            }
            ImGui::PopFont();
            ImGui::End();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glfwPollEvents(); //Poll for and process events
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteShader(mmVertShader);
    glDeleteShader(mmFragShader);
    glDeleteProgram(mmShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}