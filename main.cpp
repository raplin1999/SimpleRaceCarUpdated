#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 2

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "SDL_mixer.h"
#include <cmath>

#include "Scene.h"
#include "Util.h"
#include "Entity.h"
#include "Menu.h"
#include "Level1.h" 
#include <string>


SDL_Window* displayWindow;
bool gameIsRunning = true;
GLuint fontTextureID1;


Scene *CurrentScene;
Scene *SceneList[2];
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

glm::mat4 uiViewMatrix, uiProjectionMatrix;

bool won = false;
bool lose = false;

bool engine = false;
float timeLeft = 120.0f;
int numLapLeft = 3;

bool start =true;
bool c1 = false;
bool c2 = false;
bool c3 = false;

Mix_Music* music;
Mix_Chunk* SpeedUp;
Mix_Chunk* Brake;

void SwitchToScene(Scene* scene) {
    CurrentScene = scene;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1280, 720);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(30.0f), 1.777f, 0.1f, 100.0f);

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    fontTextureID1 = Util::LoadTexture("font1.png");

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);   
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Arc.mp3");
    SpeedUp = Mix_LoadWAV("engine.wav");
    Brake = Mix_LoadWAV("brake.wav");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_VolumeChunk(SpeedUp, MIX_MAX_VOLUME*0.5);

    SceneList[0] = new Menu();
    SceneList[1] = new Level1();

    SceneList[0]->Initialize();
    SceneList[1]->Initialize();
    SwitchToScene(SceneList[0]);
}

void ProcessInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {

            case SDLK_RETURN:
                if (CurrentScene == SceneList[0]) {
                    CurrentScene->state.nextScene = 1;
                }
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) {
        CurrentScene->state.player->rotation.y += 0.5f;
       
    }
    else if (keys[SDL_SCANCODE_D]) {
        CurrentScene->state.player->rotation.y -= 0.5f;
        
    }

    if (keys[SDL_SCANCODE_W]) {
        if (CurrentScene->state.player->acceleration1<=4)
        {
            CurrentScene->state.player->acceleration1 += CurrentScene->state.player->speed;
            
        }
        else {
            CurrentScene->state.player->acceleration1 = 4;
        }

        if (!engine) {
            Mix_PlayChannel(1, SpeedUp, 0);
            engine = true;
        }
        if (Mix_Paused(1)) {
            Mix_Resume(1);
        }
        
    }
    else if (keys[SDL_SCANCODE_S]) {
        CurrentScene->state.player->velocity.z = cos(glm::radians(CurrentScene->state.player->rotation.y)) * CurrentScene->state.player->speed;
        CurrentScene->state.player->velocity.x = sin(glm::radians(CurrentScene->state.player->rotation.y)) * CurrentScene->state.player->speed;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        CurrentScene->state.player->acceleration1 = -3-CurrentScene->state.player->velocity1*0.8;

        Mix_PlayChannel(-1, Brake, 0);
        Mix_Pause(1);
    
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;


void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;


    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        CurrentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
        if (timeLeft >= 0 && CurrentScene==SceneList[1]) {
            timeLeft -= FIXED_TIMESTEP;
        }
    }
    

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,
        glm::radians(CurrentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -CurrentScene->state.player->position);



    if (start) {
        if (CurrentScene->state.player->position.x > 4) {
            c1 = true;
            start = false;
        }
    }
    if (c1) {
        if (CurrentScene->state.player->position.z < -49) {
            c2 = true;
            c1 = false;
        }
    }
    if (c2) {
        if (CurrentScene->state.player->position.x < 53) {
            c3 = true;
            c2 = false;
        }
    }
    if (c3) {
        if (CurrentScene->state.player->position.z > 0 && CurrentScene->state.player->position.x<4) {
            start = true;
            c3 = false;
            numLapLeft -= 1;
        }
    }


    if (numLapLeft==0 &&timeLeft>0) {
        won = true;
    }

    if (timeLeft <= 0 && won == false) {
        lose = true;
    }
}


void Render() {


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    if (CurrentScene != SceneList[0]) {
        CurrentScene->Render(&program);
    }
   
    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);

    if (CurrentScene == SceneList[0]) {
        CurrentScene->Render(&program);
    }
    else {
        int timeLeftint = int(timeLeft);
        Util::DrawText(&program, fontTextureID1, "Time Left:"+std::to_string(timeLeftint), 0.5, -0.3f,
            glm::vec3(-6, 3.2, 0));
    }

    if (won){
        Util::DrawText(&program, fontTextureID1, "You won", 0.5, -0.3f,
            glm::vec3(0, 0, 0));
    }
    if (lose) {
        Util::DrawText(&program, fontTextureID1, "You lose", 0.5, -0.3f,
            glm::vec3(0,0,0));
    }
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        if (CurrentScene->state.nextScene >= 0) SwitchToScene(SceneList[CurrentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}
