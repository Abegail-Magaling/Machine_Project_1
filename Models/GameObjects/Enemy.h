#pragma once

#include "../GameObject.h"

class Enemy :
    public GameObject
{
private:
public:
    Enemy(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path,
        glm::vec3 position, float scale, bool isPNG);

    ~Enemy();
    void Update(GLFWwindow* window, float tDeltaTime);
};