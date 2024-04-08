#pragma once

#include "../GameObject.h"

class Player :
    public GameObject
{
private:
    static const bool isPlayer;

public:
    Player(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path,
        glm::vec3 position, float scale, bool isPNG);

    ~Player();
    void Update(GLFWwindow* window, float tDeltaTime);
};

