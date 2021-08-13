#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "mesh.h"

enum EntityType { PLAYER, FinishLine, ENEMY, CUBE, FLOOR, NONE, Car};
//
class Entity {
public:

    EntityType entityType;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float acceleration1;
    float velocity1;
    glm::vec3 movement;
    glm::vec3 rotation;
    glm::vec3 scale;
    Mesh *mesh;

    float speed;

    GLuint textureID;

    glm::mat4 modelMatrix;

    bool isActive;
    bool billboard;
    float width;
    float height;
    float depth;

    Entity();


    bool CheckCollision(Entity* other);
    void Update(float deltaTime, Entity* player, Entity* objects, Entity* Enemies, int objectCount, int enemyCount);
    void Render(ShaderProgram* program);
    void  DrawBillboard(ShaderProgram* program);
};



