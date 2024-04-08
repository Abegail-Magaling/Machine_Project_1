#include "Models/Model3D.h"
#include "Models/GameObjects/Player.h"
#include "Models/GameObjects/Enemy.h"
#include "Models/Camera/PerspectiveCamera.h"
#include "Models/Camera/OrthoCamera.h"
#include "Models/Light/Directional.h"
#include "Models/Light/Point.h"
#include "Models/Shader.h"

bool is1stPerson = false;
bool is3rdPerson = false;
bool isPerspective = true;
bool isPlayer = true;
double tLastUpdate = 0.0;
float tTimePerFrame = 1.0 / 60.0;
const double keyPressDelay = 0.2; // Adjust this value as needed
double lastKeyPressTime = 0.0;
PerspectiveCamera* perspectiveCamera = new PerspectiveCamera();
OrthoCamera* orthoCamera = new OrthoCamera();

static void initWindow(GLFWwindow*& window, float window_width, float window_height)
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(window_width, window_height, "Abegail Laureen Magaling | Vincent Ralph Chang", NULL, NULL);
    if (!window)
    {
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	gladLoadGL();
}


int main(void)
{
    GLFWwindow* window;

    float window_width = 600;
    float window_height = 600;

    initWindow(window, window_width, window_height);

    Player* pPlayer = new Player("3D/source/SubLow0Smooth.obj", "Shaders/sample.vert", "Shaders/samplePlayer.frag", "3D/textures/material.jpg", glm::vec3(0.f, 0.f, 0.f), 0.08f, false);

    Enemy* pEnemy = new Enemy("3D/submarine_2.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/submarine_1_tex.jpg", glm::vec3(10.f, 10.f, -20.f), 1.0f, false);
    Enemy* pEnemy2 = new Enemy("3D/tuna.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/tuna_base.png", glm::vec3(-30.f, 10.f, -30.f),4.0f, false);
    Enemy* pEnemy3 = new Enemy("3D/shark_mesh.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/shark_tex.jpg", glm::vec3(0.f, 30.f, -40.f), 10.0f, false);
    Enemy* pEnemy4 = new Enemy("3D/sunken_ship_mesh.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/sunken_ship_tex.jpg", glm::vec3(0.f, -30.f, -50.f), 2.0f, false);
    Enemy* pEnemy5 = new Enemy("3D/YellowSubmarine.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/Submarine_colorID.png", glm::vec3(40.f, -30.f, -60.f), 0.05f, true);
    Enemy* pEnemy6 = new Enemy("3D/bathyscaphe1_01.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/bathyscaphe_tex.png", glm::vec3(-40.f, -30.f, -70.f), 5.0f, false);

    Point* pointLightSrc = new Point("3D/octopus.obj", "Shaders/lightShader.vert", "Shaders/lightShader.frag", "3D/Mario_Albedo.png", 
        glm::vec3(0.f, 7.f, -4.f), 10.f,
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        10.f, 0.5f, 16.0f);

    Directional* directionalLightSrc = new Directional("3d/octopus.obj", "shaders/lightshader.vert", "shaders/lightshader.frag", "3d/octopus_toy_texture.jpg",
        glm::vec3(0.f, 0.f, 0.f), 10.f,
        glm::vec3(0.36f, 0.89f, 0.96f),
        glm::vec3(0.36f, 0.89f, 0.96f),
        1.f, 0.5f, 16.0f);

    directionalLightSrc->SetDirection(glm::vec3(0.0f, -1.0f, 0.f));

    //Model3D* moon = new Model3D("3D/moon.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/Image_0.png", glm::vec3(0.f, 0.f, 0.f), 0.09f);
    
    //skybox start
    Shader* skyBoxShader = new Shader("Shaders/skyboxvert.vert", "Shaders/skyboxfrag.frag");

    GLuint skyboxShaderProg = skyBoxShader->getShaderProg();
    glEnable(GL_DEPTH_TEST);

    float skyboxVertices[]{
    -1.f, -1.f, 1.f, //0
    1.f, -1.f, 1.f,  //1
    1.f, -1.f, -1.f, //2
    -1.f, -1.f, -1.f,//3
    -1.f, 1.f, 1.f,  //4
    1.f, 1.f, 1.f,   //5
    1.f, 1.f, -1.f,  //6
    -1.f, 1.f, -1.f  //7
    };

    unsigned int skyboxIndices[]{
    1,2,6,
    6,5,1,

    0,4,7,
    7,3,0,

    4,5,6,
    6,7,4,

    0,3,2,
    2,1,0,

    0,1,5,
    5,4,0,

    3,7,6,
    6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string facesSkybox[]
    {
        "underwater skybox/right.png",//right
        "underwater skybox/left.png",//left
        "underwater skybox/up.png",//up
        "underwater skybox/down.png",//down
        "underwater skybox/forward.png",//front
        "underwater skybox/back.png"//back

        //This should always be the order
    };

    GLuint skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, skyColorChannel;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyColorChannel, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }

    stbi_set_flip_vertically_on_load(true);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //initiate matrices
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4();
        glm::mat4 viewMatrix = glm::mat4();
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);

        //Start with the translation matrix_mod
        double currentTime = glfwGetTime();
        lastKeyPressTime += currentTime;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && (lastKeyPressTime >= keyPressDelay))
        {
            lastKeyPressTime = currentTime;
            //isPerspective = !isPerspective;
            std::cout << "perspective" << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && (lastKeyPressTime >= keyPressDelay))
        {
			lastKeyPressTime = currentTime;
            isPlayer = !isPlayer;
            std::cout << "perspective" << std::endl;
		}
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && (lastKeyPressTime >= keyPressDelay))
        {
			lastKeyPressTime = currentTime;
            pointLightSrc->cycleLightIntensity();
        }
        if (isPerspective) {
            projectionMatrix = perspectiveCamera->getProjectionMatrix(window_width / window_height);
            viewMatrix = perspectiveCamera->getViewMatrix();
            cameraPos = perspectiveCamera->getCameraPosition();
            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && (lastKeyPressTime >= keyPressDelay)) 
            {
                lastKeyPressTime = currentTime;
                is3rdPerson = true;
                std::cout << "Switched to third-person perspective" << std::endl;
            }
            else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && (lastKeyPressTime >= keyPressDelay)) 
            {
                lastKeyPressTime = currentTime;
                is1stPerson = true;
                std::cout << "Switched to first-person perspective" << std::endl;
            }
        }
        else {
            projectionMatrix = orthoCamera->getProjectionMatrix(window_width / window_height);
            viewMatrix = orthoCamera->getViewMatrix();
            cameraPos = orthoCamera->getCameraPosition();
        }
        
        if (is1stPerson)
        {
            perspectiveCamera->FOV(is1stPerson, is3rdPerson, pPlayer->GetPosition());
            is1stPerson = false;
        }
        if (is3rdPerson)
        {
            perspectiveCamera->FOV(is1stPerson, is3rdPerson, pPlayer->GetPosition());
            is3rdPerson = false;
        }
        tLastUpdate += glfwGetTime();
        glfwSetTime(0.0f);

        while (tLastUpdate > tTimePerFrame)
        {
           tLastUpdate -= tTimePerFrame;
           perspectiveCamera->updateCamera(window, tTimePerFrame);
           pPlayer->Update(window, tTimePerFrame);
           directionalLightSrc->Update(window, tTimePerFrame);
           perspectiveCamera->Update(window, tTimePerFrame, pPlayer);
           glm::vec3 playerPosition = pPlayer->GetPosition();
           glm::vec3 frontPosition = playerPosition + glm::vec3(0.0f, 7, -4);
           pointLightSrc->SetPosition(frontPosition);
        }
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProg);
        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));
        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");
        glUniformMatrix4fv(skyboxViewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(sky_view));

        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProg, "projection");
        glUniformMatrix4fv(skyboxProjLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projectionMatrix));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        pPlayer->DrawPlayer(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

       
        pEnemy->Draw(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

        pEnemy2->Draw(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

        pEnemy3->Draw(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

        pEnemy4->Draw(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

        pEnemy5->Draw(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

        pEnemy6->Draw(viewMatrix, projectionMatrix, pointLightSrc->GetPosition(), pointLightSrc->GetLightColor(isPlayer),
            pointLightSrc->GetAmbientStr(), pointLightSrc->GetAmbientColor(isPlayer), pointLightSrc->GetSpecStr(), pointLightSrc->GetSpecPhong(),
            pointLightSrc->GetIntensity(), directionalLightSrc->GetPosition(), directionalLightSrc->GetLightColor(isPlayer), directionalLightSrc->GetAmbientStr(),
            directionalLightSrc->GetAmbientColor(isPlayer), directionalLightSrc->GetSpecStr(), directionalLightSrc->GetSpecPhong(), directionalLightSrc->GetIntensity(),
            cameraPos);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}