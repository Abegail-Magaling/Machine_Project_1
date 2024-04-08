#include "Enemy.h"


Enemy::Enemy(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, bool isPNG)
	: GameObject(Mesh_Path, Vert_Path, Frag_Path, Tex_Path, position, scale, isPNG)
{

}

Enemy::~Enemy()
{
}

void Enemy::Update(GLFWwindow* window, float tDeltaTime)
{

}
