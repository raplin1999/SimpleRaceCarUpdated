#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    acceleration1 = 0;
    movement= glm::vec3(0);
    velocity = glm::vec3(0);
    velocity1 = 0;
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
    modelMatrix = glm::mat4(1.0f);
    billboard = false;
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
    speed = 0.0f;
    isActive = true;
}

bool Entity::CheckCollision(Entity* other) {
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}


void Entity::Update(float deltaTime, Entity* player, Entity* objects, Entity* Enemies, int objectCount, int enemyCount)
{
    glm::vec3 previousPosition = position;

    if (entityType == PLAYER) {
        velocity1 += acceleration1* deltaTime;
        if (velocity1 < 0) {
            velocity1 = 0;
        }
    }
    
    velocity.x = -velocity1 * sin(glm::radians(rotation.y));
    velocity.z = -velocity1 * cos(glm::radians(rotation.y));
    position += velocity * deltaTime;

    if (billboard) {
        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;


        if (entityType == ENEMY) {
            velocity.x = 0;
            if (position.x<0) {
                velocity.x = -0.1f;
            }
            else {
                velocity.x = 0.1f;
            }
            position += velocity * deltaTime;
        }

    }

    if (entityType == PLAYER) {
        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;

            if (objects[i].entityType == Car) continue;

            if (CheckCollision(&objects[i])) {

                acceleration1 = 0;
                velocity1 = 0;
                position = previousPosition;
                break;
            }

            
        }
        for (int i = 0; i < enemyCount-1; i++) {
            if (CheckCollision(&Enemies[i])) {
                Enemies[i].isActive = false;
                speed += 0.05;
                
            }
        }


    }


    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
    modelMatrix = glm::scale(modelMatrix, scale);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

}

void Entity::Render(ShaderProgram* program) {


    program->SetModelMatrix(modelMatrix);

    glBindTexture(GL_TEXTURE_2D, textureID);
    if (billboard) {
        if (isActive) {
            DrawBillboard(program);
        }
    }
    else {
        mesh->Render(program);
    }
    
}



void Entity::DrawBillboard(ShaderProgram* program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}