#include "lab_m1/Tanks_game/Tanks_game.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tanks_game::Tanks_game()
{
}


Tanks_game::~Tanks_game()
{
}


void Tanks_game::Init()
{
    tank.baza = new Mesh("baza");
    tank.baza->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "baza.obj");
    meshes[tank.baza->GetMeshID()] = tank.baza;

    tank.sine = new Mesh("sine");
    tank.sine->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "sine.obj");
    meshes[tank.sine->GetMeshID()] = tank.sine;

    tank.teava = new Mesh("teava");
    tank.teava->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "teava.obj");
    meshes[tank.teava->GetMeshID()] = tank.teava;

    tank.turela = new Mesh("turela");
    tank.turela->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "turela.obj");
    meshes[tank.turela->GetMeshID()] = tank.turela;

    timer_for_shooting = 0.0f;
    tank.position = glm::vec3(0, -0.03, 0); // era 0.07
    tank.rotation = glm::vec3(1, 0, 1);
    tank.direction_max_point = glm::vec3(0, 0, 0);
    tank.distance_to_building = 0;
    tank.is_shooting = false;
    tank.projectiles_to_load = 0;
    tank.rec_in_vector = -1;
    tank.life = 100;
    anglestep = 0;
    cameraRotation = false;
    playingTimer = 0;
    gameOver = false;
    camera = new Camera();
    cameraOffset = glm::vec3(0,  1, -0.5f);
    camera->Set(cameraOffset, tank.position, glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    Scene2::Init();

}


void Tanks_game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tanks_game::Update(float deltaTimeSeconds)
{  
    Scene2::Update(deltaTimeSeconds);
    playingTimer += deltaTimeSeconds;
    timer_for_shooting -= deltaTimeSeconds;
    if (playingTimer > 180) {
        gameOver = true;
    }
    //UPDATE USER TANK---------------------------------------------------------------------------
    tank.modelMatrix = glm::mat4(1);
    tank.modelMatrix *= transform3D::Translate(tank.position.x, tank.position.y, tank.position.z);
    tank.modelMatrix *= transform3D::RotateOY(anglestep);
    tank.modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    // Move turela
    tank.modelMatrixTurela = glm::mat4(1);
    tank.modelMatrixTurela *= transform3D::Translate(tank.position.x, tank.position.y, tank.position.z);
    tank.modelMatrixTurela *= transform3D::RotateOY(anglestep);
    tank.modelMatrixTurela *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    if (!cameraRotation) {
        float sensitivityOY = 0.001f;
        float position = window->GetResolution().x / 2 - window->GetCursorPosition().x;
        tank.modelMatrixTurela *= transform3D::Translate(tank.position_turela.x, 0, tank.position_turela.z);
        tank.modelMatrixTurela *= transform3D::RotateOY(position * sensitivityOY);
        tank.modelMatrixTurela *= transform3D::Translate(-tank.position_turela.x, 0, -tank.position_turela.z);
        tank.angleTurela = position * sensitivityOY;
    }

    //MOVE AMMUNITION------------------------------------------------------------------------------
    for (int i = tank.ammunition.size() - 1; i >= 0; i--) {
        if (tank.ammunition[i].time_to_live >= 0 && !Scene2::projectile_is_collision_with_building(tank.ammunition[i])
            && !Scene2::projectile_is_collision_with_tank(tank.ammunition[i]) && !gameOver) {
            tank.ammunition[i].time_to_live -= deltaTimeSeconds;
            glm::mat4 modelMatrixAmmunition = glm::mat4(1);
            tank.ammunition[i].position += tank.ammunition[i].direction * deltaTimeSeconds * 40.0f;
            modelMatrixAmmunition *= transform3D::Translate(tank.ammunition[i].position.x, tank.ammunition[i].position.y, tank.ammunition[i].position.z);
            modelMatrixAmmunition *= transform3D::Scale(0.5f, 0.5f, 0.5f);
            RenderMesh(tank.ammunition[i].projectile, shaders["MyShader"], modelMatrixAmmunition);
        } else {
            if (Scene2::projectile_is_collision_with_tank(tank.ammunition[i]) && !gameOver) {
                struct Tank *enemy = get_tank_by_position(tank.ammunition[i]);
                    if (!enemy->freeze) {
                    struct Fire fire;
                    fire.fire = meshes["explosion"];
                    float offsetx = (enemy->position.x - tank.ammunition[i].position.x) / 3;
                    float offsetz = (enemy->position.z - tank.ammunition[i].position.z) / 3;
                    fire.position.x = tank.ammunition[i].position.x + offsetx;
                    fire.position.z = tank.ammunition[i].position.z + offsetz;
                    fire.position.y = tank.ammunition[i].position.y;
                    fire.time_to_live = 0.2f;
                    fires.push_back(fire);

                    enemy->life -= 20;
                    enemy->hp++;
                    if (enemy->life == 0) {
                    enemy->freeze = true;
                    }
                }
            }
            tank.ammunition.erase(tank.ammunition.begin() + i);
        }
    }
    //Generate explosion----------------------------------------------------------------------------
    for (int i = fires.size() - 1; i >= 0; i--) {
        if (fires[i].time_to_live >= 0) {
            fires[i].time_to_live -= deltaTimeSeconds;
            glm::mat4 modelMatrixFire = glm::mat4(1);
            modelMatrixFire *= transform3D::Translate(fires[i].position.x, fires[i].position.y, fires[i].position.z);
            modelMatrixFire *= transform3D::Scale(0.7f, 0.4f, 0.7f);
            RenderMesh(fires[i].fire, shaders["MyShader"], modelMatrixFire);
        } else {
            fires.erase(fires.begin() + i);
        }
    }
    RenderMesh(tank.baza,shaders["MyShader"], tank.modelMatrix);
    RenderMesh(tank.sine,shaders["MyShader"], tank.modelMatrix);
    if (!gameOver) {
        RenderMesh(tank.turela,shaders["MyShader"], tank.modelMatrixTurela);
        RenderMesh(tank.teava,shaders["MyShader"], tank.modelMatrixTurela);
    } else {
        RenderMesh(tank.turela, shaders["MyShader"], tank.modelMatrix);
        RenderMesh(tank.teava, shaders["MyShader"], tank.modelMatrix);
    }
}



void Tanks_game::FrameEnd()
{
   DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tanks_game::OnInputUpdate(float deltaTime, int mods)
{   
    if (!cameraRotation && !gameOver) {
        float moveSpeed = 4.0f; // Viteza de deplasare a tancului
        float rotationSpeed = 1.0f; // Viteza de rotație a tancului

        // Calculam vectorul de deplasare relativ la orientarea tancului
        glm::vec3 forwardVector = glm::vec3(glm::sin(anglestep), 0.0f, glm::cos(anglestep));

        // Set rotation for the tank
        tank.rotationMatrix = glm::rotate(glm::mat4(1.0f), anglestep, glm::vec3(0, 1, 0));
        // Calculăm coordonatele maxime
        glm::vec3 maxPointOffset = glm::vec3(0, 0, tank.max_point_offset);
        tank.direction_max_point = tank.position + glm::vec3(tank.rotationMatrix * glm::vec4(maxPointOffset, 1.0));
        glm::vec3 tankMaxPoint = tank.direction_max_point + glm::vec3(tank.max_point_offset, tank.max_point_offset, tank.max_point_offset);

        Scene2::is_collision_to_building(&tank);
        Scene2::is_colision_to_tank(&tank);

        //Daca a colectat viata
        if (Scene2::is_collision_to_life(tank)) {
            tank.life += 10;
        }

        if (window->KeyHold(GLFW_KEY_W)
            && (glm::abs(tank.direction_max_point.z) < 25.0f && glm::abs(tank.direction_max_point.x) < 25.0f)) {
                tank.position += forwardVector * deltaTime * moveSpeed;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            if ((glm::abs(tank.position.z) > glm::abs(tank.direction_max_point.z) && glm::abs(tank.position.z) + tank.max_point_sine_offset >= 25.0f)
                || (glm::abs(tank.position.x) > glm::abs(tank.direction_max_point.x) && glm::abs(tank.position.x) + tank.max_point_sine_offset >= 25.0f)) {
                    //do nothing
            } else {
                    tank.position -= forwardVector * deltaTime * moveSpeed;
            }
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            anglestep += deltaTime * rotationSpeed;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            anglestep -= deltaTime * rotationSpeed;
        }

        // Update camera position to follow the tank
        glm::vec3 rotatedCameraOffset = glm::vec3(tank.rotationMatrix * glm::vec4(cameraOffset, 1.0));
        camera->Set(tank.position + rotatedCameraOffset, tank.position, glm::vec3(0, 1, 0));

        camera->TranslateForward(-1.5f);
        camera->RotateThirdPerson_OX(0.8f);

    }
}


void Tanks_game::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tanks_game::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tanks_game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        camera->RotateThirdPerson_OY(deltaX * sensivityOY);
        camera->RotateThirdPerson_OX(deltaY * sensivityOX);
    }
}


void Tanks_game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        cameraRotation = true;
    }
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        tank.projectiles_to_load++;
        struct Projectile projectile;
        projectile.projectile = new Mesh("proiectil");
        projectile.projectile->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "proiectil.obj");
        meshes[projectile.projectile->GetMeshID()] = projectile.projectile;
        glm::mat3 rotationMatrixdir = glm::mat3(tank.modelMatrixTurela);
        glm::vec3 tankDirectionMaxPoint = glm::vec3(tank.modelMatrixTurela * glm::vec4(tank.position_turela, 1.0f));
        projectile.direction = glm::vec3(rotationMatrixdir * glm::vec4(glm::vec3(0, 0, 1), 1.0));
        projectile.position = glm::vec3(tankDirectionMaxPoint.x, 0.7901 * 0.5, tankDirectionMaxPoint.z) - glm::normalize(projectile.direction) * (-0.8f);
        projectile.time_to_live = 3.0f;
        if (timer_for_shooting <= 0.0f){
            tank.ammunition.push_back(projectile);
            timer_for_shooting = 1.0f;
        }
    }
}


void Tanks_game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        cameraRotation = false;
    }
}


void Tanks_game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tanks_game::OnWindowResize(int width, int height)
{
}
