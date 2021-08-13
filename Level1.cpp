#include "Level1.h"

#define L1ENEMY_COUNT 4
#define L1Object_COUNT 466


void Level1::Initialize() {



    state.nextScene = -1;
    //state.enemyCount = L1ENEMY_COUNT; 


    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 1.0f, 0);
    state.player->rotation = glm::vec3(0, 0, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 0.3;

    GLuint enemyTextureID = Util::LoadTexture("mushroom.png");
    state.enemies = new Entity[L1ENEMY_COUNT];
    for (int i = 0; i < L1ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].position = glm::vec3(-4+rand()%8, 0.5, -0-rand()%50);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
    }

    GLuint endTextureID = Util::LoadTexture("finishLine.jpg");
    state.enemies[3].billboard = true;
    state.enemies[3].entityType = FinishLine;
    state.enemies[3].textureID = endTextureID;
    state.enemies[3].position = glm::vec3(0, 0.05, 0);
    state.enemies[3].scale = glm::vec3(5, 1, 1);
    state.enemies[3].rotation = glm::vec3(90, 180, 0);
    state.enemies[3].acceleration = glm::vec3(0, 0, 0);


    state.objects = new Entity[L1Object_COUNT];

    GLuint floorTexutreID = Util::LoadTexture("floor.jpg");
    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 200);

    state.objects[0].textureID = floorTexutreID;
    state.objects[0].position = glm::vec3(0, -0.25, 0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(200, 0.5f, 200);


    GLuint carTexutreID = Util::LoadTexture("crate1_bump.png");
    Mesh* CarMesh = new Mesh();
    CarMesh->LoadOBJ("raceCar.obj", 1);

    state.objects[1].textureID = carTexutreID;
    state.objects[1].position = state.player->position + glm::vec3(0.0f, -0.5f, -1.5f); 
    state.objects[1].entityType = Car;
    state.objects[1].mesh = CarMesh;
    state.objects[1].scale = glm::vec3(0.015,0.02,0.015);
    state.objects[1].rotation = glm::vec3(0,180,0);
    GLuint crateTexutreID = Util::LoadTexture("crate1_diffuse.png");
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);

    for (int i = 2; i < 52; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(4, 0.5, -(i-2)*1.0f);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;
    
    }
    for (int i = 52; i < 102; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(5+ (i - 52)*1.0f, 0.5, -49);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }
    for (int i = 102; i < 152; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(53, 0.5, -48+(i-102)*1.0f);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }
    for (int i = 152; i < 202; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(52-(i-152)*1.0f, 0.5, 1);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }
    for (int i = 202; i < 268; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(-4, 0.5, 9-(i - 202) * 1.0f);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }
    for (int i = 268; i < 334; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(-3 + (i - 268) * 1.0f, 0.5, -57);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }
    for (int i = 334; i < 400; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(62, 0.5, -56 + (i - 334) * 1.0f);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }
    for (int i = 400; i < 466; i++) {
        state.objects[i].textureID = crateTexutreID;
        state.objects[i].position = glm::vec3(61 - (i - 400) * 1.0f, 0.5, 9);
        state.objects[i].entityType = CUBE;
        state.objects[i].mesh = crateMesh;

    }


    

}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, state.enemies, L1Object_COUNT, L1ENEMY_COUNT);


    for (int i = 0; i < L1Object_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, state.enemies, L1Object_COUNT, L1ENEMY_COUNT);
    }
    state.objects[1].position = state.player->position;
    state.objects[1].rotation = glm::vec3(0,180,0)+state.player->rotation;
    state.objects[1].position.z -= cos(glm::radians(state.player->rotation.y)) * 1.5f;
    state.objects[1].position.x -= sin(glm::radians(state.player->rotation.y)) * 1.5f;
    state.objects[1].position.y -= 0.5f;



    for (int i = 0; i < L1ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.objects, state.enemies, L1Object_COUNT, L1ENEMY_COUNT);
    }

}

void Level1::Render(ShaderProgram* program) {

    for (int i = 0; i < L1Object_COUNT; i++) {
        state.objects[i].Render(program);
    };

    for (int i = 0; i < L1ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    };
}