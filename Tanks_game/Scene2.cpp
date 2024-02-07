#include "Scene2.h"
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <random>
#include <cmath>


using namespace std;
using namespace m1;


void Scene2::Init()
{ 
    struct Building building1;
    struct Building building2;
    struct Building building3;

    //MESHES FOR BUILDINGS-----------------------------------------------------------------------------------------------
    building1.baza = new Mesh("building_1_base");
    building1.baza->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "building_1_base.obj");
    meshes[ building1.baza->GetMeshID()] =  building1.baza;

    building1.roof = new Mesh("building_1_roof");
    building1.roof->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "building_1_roof.obj");
    meshes[building1.roof->GetMeshID()] = building1.roof;

    building1.safetyRadius = 1 * 0.7;
    building1.type = 1;

    building2.baza = new Mesh("building_2_base");
    building2.baza->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "building_2_base.obj");
    meshes[building2.baza->GetMeshID()] = building2.baza;

    building2.roof = new Mesh("building_2_roof");
    building2.roof->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "building_2_roof.obj");
    meshes[building2.roof->GetMeshID()] = building2.roof;

    building2.safetyRadius = 0.859 * 0.7;
    building2.type = 2;

    type_of_buildings.push_back(building1);
    type_of_buildings.push_back(building2);
    type_of_buildings.push_back(building3);

    //Ground------------------------------------------------------------------------------------------------------------
    Mesh *mesh1 = new Mesh("ground");
    mesh1->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "ground.obj");
    meshes[mesh1->GetMeshID()] = mesh1;

    //MESHES FOR ENEMIES------------------------------------------------------------------------------------------------
   
    Mesh *mesh = new Mesh("baza1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "baza_enemy.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("sine1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "sine.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("teava1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "teava.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("turela1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "turela_enemy.obj");
    meshes[mesh->GetMeshID()] = mesh;

    //MESHES FOR EXPLOSION------------------------------------------------------------------------------------------------
    mesh = new Mesh("explosion");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "fire.obj");
    meshes[mesh->GetMeshID()] = mesh;

    //MESHES FOR LIFE------------------------------------------------------------------------------------------------
    mesh = new Mesh("key");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "key.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("screwdriver");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "key2.obj");
    meshes[mesh->GetMeshID()] = mesh;

    int cnt = 30, posX, posZ;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Definirea intervalului pentru x și z la cladiri
    std::uniform_real_distribution<double> xDistribution(-25.0, 25.0);
    std::uniform_real_distribution<double> zDistribution(-25.0, 25.0);

    do {
        int index = rand() % 2;
        posX = xDistribution(gen);
        posZ = zDistribution(gen);
        // pentru a nu genera in centru
        if (posZ < 0 && posZ > -2.741) {
            posZ -= 2;
        } else if (posZ > 0 && posZ < 2.741) {
            posZ += 2;
        }
        if (posX < 0 && posX > -1.721) {
            posX -= 1;
        } else if (posX > 0 && posX < 1.721) {
            posX += 1;
        }
        struct Building building;
        building.baza = type_of_buildings[index].baza;
        building.roof = type_of_buildings[index].roof;
        building.safetyRadius =  type_of_buildings[index].safetyRadius;
        building.distance_in_collision = 0;
        building.position = glm::vec3(posX, 0, posZ);
        buildings.push_back(building);
        cnt--;
    } while (cnt >= 0);

    // Definirea intervalului pentru x și z la tancuri
    cnt = 8;
    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_real_distribution<double> xDistributionTank(-22.26, 22.26);
    std::uniform_real_distribution<double> zDistributionTank(-22.26, 22.26);
    do {
        struct Tank enemy;
        enemy.baza = meshes["baza1"];
        enemy.sine = meshes["sine1"];
        enemy.teava = meshes["teava1"];
        enemy.turela = meshes["turela1"];
        posX = xDistributionTank(gen2);
        posZ = zDistributionTank(gen2);
        // pentru a nu genera in centru
        if (posZ < 0 && posZ > -2.741) {
            posZ -= 2;
        } else if (posZ > 0 && posZ < 2.741) {
            posZ += 2;
        }
        if (posX < 0 && posX > -1.721) {
            posX -= 1;
        } else if (posX > 0 && posX < 1.721) {
            posX += 1;
        }
        enemy.position = glm::vec3(posX, 0, posZ);
        enemy.choiceIndex = 0;
        enemy.angleTurela = 0;
        enemy.anglestep = 0;
        enemy.freeze = false;
        enemy.life = 100;
        enemy.time_to_shoot = 1.0;
        enemy.hp = 0;
        enemy.shoot_projectile = false;
        enemies.push_back(enemy);
        cnt--;
      } while (cnt >= 0);

    //Definire intervalului x si z pentru viata
    cnt = 2;

    std::mt19937 gen_life(rd_life());
    std::uniform_real_distribution<double> xDistributionLife(-22.26, 22.26);
    std::uniform_real_distribution<double> zDistributionLife(-22.26, 22.26);

    do {
        struct Life life;
        life.key = meshes["key"];
        life.screwdriver = meshes["screwdriver"];
        posX = xDistributionLife(gen_life);
        posZ = zDistributionLife(gen_life);
        // pentru a nu genera in centru
        if (posZ < 0 && posZ > -2.741) {
            posZ -= 2;
        } else if (posZ > 0 && posZ < 2.741) {
            posZ += 2;
        }
        if (posX < 0 && posX > -1.721) {
            posX -= 1;
        } else if (posX > 0 && posX < 1.721) {
            posX += 1;
        }
        life.position = glm::vec3(posX, 0, posZ);
        life.time_to_live = 3.0f;
        life.is_collected = false;
        lives.push_back(life);
        cnt--;
      } while (cnt >= 0);
    load_lives = 5.0f;

    Shader *shader = new Shader("EnemyShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tanks_game", "shaders", "VertexShader_enemy.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tanks_game", "shaders", "FragmentShader_enemy.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    Shader *shader1 = new Shader("MyShader");
    shader1->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tanks_game", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader1->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tanks_game", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader1->CreateAndLink();
    shaders[shader1->GetName()] = shader1;

}

void Scene2::move_enemies(struct Tank *tank, string choice, float deltaTime) {
    float moveSpeed = 2.0f; // Viteza de deplasare a tancului
    float rotationSpeed = 0.5f; // Viteza de rotație a tancului

    // Calculam vectorul de deplasare relativ la orientarea tancului
    glm::vec3 forwardVector = glm::vec3(glm::sin(tank->anglestep), 0.0f, glm::cos(tank->anglestep));

    // Set rotation for the tank
    tank->rotationMatrix = glm::rotate(glm::mat4(1.0f), tank->anglestep, glm::vec3(0, 1, 0));
    // Calculăm coordonatele maxime
    glm::vec3 maxPointOffset = glm::vec3(0, 0, tank->max_point_offset);
    tank->direction_max_point = tank->position + glm::vec3(tank->rotationMatrix * glm::vec4(maxPointOffset, 1.0));
    glm::vec3 tankMaxPoint = tank->direction_max_point + glm::vec3(tank->max_point_offset, tank->max_point_offset, tank->max_point_offset);

    Scene2::is_collision_to_building(tank);
    Scene2::is_colision_to_tank(tank);

    if (choice == "W"
        && (glm::abs(tank->direction_max_point.z) < 25.0f && glm::abs(tank->direction_max_point.x) < 25.0f)) {
            tank->position += forwardVector * deltaTime * moveSpeed;
    }
    if (choice == "S") {
        if ((glm::abs(tank->position.z) > glm::abs(tank->direction_max_point.z) && glm::abs(tank->position.z) + tank->max_point_sine_offset >= 25.0f)
            || (glm::abs(tank->position.x) > glm::abs(tank->direction_max_point.x) && glm::abs(tank->position.x) + tank->max_point_sine_offset >= 25.0f)) {
                //do nothing
        } else {
                tank->position -= forwardVector * deltaTime * moveSpeed;
        }
    }
    if (choice == "A") {
        tank->anglestep += deltaTime * rotationSpeed;
    }
    if (choice == "D") {
        tank->anglestep -= deltaTime * rotationSpeed;
    }

    
}

void Scene2::is_colision_to_tank(struct Tank *tank) {
     for (int i = 0; i < enemies.size(); i++) {
        if (tank->rec_in_vector != i) {
            glm::vec3 position = glm::vec3(tank->position.x, 0, tank->position.z);
            float distanceTankToTank = glm::distance(position, enemies[i].position);
            if (distanceTankToTank < enemies[i].max_point_offset + tank->max_point_offset) {
                float P = glm::abs(enemies[i].max_point_offset + tank->max_point_offset - distanceTankToTank);
                glm::vec3 diff = position - enemies[i].position;
                glm::vec3 correction = P * glm::normalize(diff);
                tank->position += correction * 0.5f;
                
                // Verificare pentru axa x
                if (tank->position.x - correction.x * 0.5f < -25.0f) {
                    tank->position.x = -25.0f + tank->max_point_offset;
                } else if (tank->position.x + correction.x * 0.5f > 25.0f) {
                    tank->position.x = 25.0f - tank->max_point_offset;
                }

                // Verificare pentru axa z
                if (tank->position.z - correction.z * 0.5f < -25.0f) {
                    tank->position.z = -25.0f + tank->max_point_offset;
                } else if (tank->position.z + correction.z * 0.5f > 25.0f) {
                    tank->position.z = 25.0f - tank->max_point_offset;
                }
                
                enemies[i].position -= correction * 0.5f;
                // Verificare pentru inamic (axa x)
                if (enemies[i].position.x - correction.x * 0.5f < -25.0f) {
                    enemies[i].position.x = -25.0f + enemies[i].max_point_offset;
                } else if (enemies[i].position.x + correction.x * 0.5f > 25.0f) {
                    enemies[i].position.x = 25.0f - enemies[i].max_point_offset;
                }

                // Verificare pentru inamic (axa z)
                if (enemies[i].position.z - correction.z * 0.5f < -25.0f) {
                    enemies[i].position.z = -25.0f + enemies[i].max_point_offset; 
                } else if (enemies[i].position.z + correction.z * 0.5f > 25.0f) {
                    enemies[i].position.z = 25.0f - enemies[i].max_point_offset;
                }
            }
        }
    }
}

struct Tank* Scene2::get_tank_by_position(struct  Projectile projectile) {
    for (int i = enemies.size() - 1; i >= 0; i--) {
        glm::vec3 tankPos = glm::vec3(enemies[i].position.x, 0, enemies[i].position.z);
        float distanceprojectileTank = glm::distance(projectile.position, tankPos);
        if (distanceprojectileTank < enemies[i].max_point_offset + projectile.radius) {
            return &enemies[i];
        }
    }
    return NULL;
}

void Scene2::is_collision_to_building(struct Tank *tank) {
    for (int i = 0; i < buildings.size(); i++) {
        glm::vec3 position = glm::vec3(tank->position.x, 0, tank->position.z);
        float distanceTankBuilding = glm::distance(position, buildings[i].position);
        if (distanceTankBuilding < buildings[i].safetyRadius + tank->max_point_offset) {
            float P = glm::abs(buildings[i].safetyRadius + tank->max_point_offset - distanceTankBuilding);
            glm::vec3 diff = position - buildings[i].position;
            glm::vec3 correction = P * glm::normalize(diff);
            tank->position += correction * 0.5f;
        }
    }
}

bool Scene2::projectile_is_collision_with_building(struct Projectile projectile) {
    for (int i = 0; i < buildings.size(); i++) {
        if (projectile.position.x + projectile.radius >= buildings[i].position.x - buildings[i].safetyRadius &&
            projectile.position.x + projectile.radius <= buildings[i].position.x + buildings[i].safetyRadius &&
            projectile.position.z + projectile.radius >= buildings[i].position.z - buildings[i].safetyRadius &&
            projectile.position.z + projectile.radius <= buildings[i].position.z + buildings[i].safetyRadius) {
            return true;
        }
    }
    return false;
}

bool Scene2::projectile_is_collision_with_tank(struct Projectile projectile) {
    for (int i = 0; i < enemies.size(); i++) {
        if (projectile.position.x + projectile.radius >= enemies[i].position.x - enemies[i].max_point_sine_offset &&
            projectile.position.x + projectile.radius <= enemies[i].position.x + enemies[i].max_point_sine_offset &&
            projectile.position.z + projectile.radius >= enemies[i].position.z - enemies[i].max_point_sine_offset &&
            projectile.position.z + projectile.radius <= enemies[i].position.z + enemies[i].max_point_sine_offset) {
            return true;
        }
    }
    return false;
}


bool Scene2::projectile_is_collision_with_player(struct Projectile projectile) {
    if (projectile.position.x + projectile.radius >= tank.position.x - tank.max_point_sine_offset &&
        projectile.position.x + projectile.radius <= tank.position.x + tank.max_point_sine_offset &&
        projectile.position.z + projectile.radius >= tank.position.z - tank.max_point_sine_offset &&
        projectile.position.z + projectile.radius <= tank.position.z + tank.max_point_sine_offset) {
        return true;
    }
    return false;
}

bool Scene2::is_collision_to_life(struct Tank tank) {
    for (int i = 0; i < lives.size(); i++) {
        glm::vec3 position = glm::vec3(tank.position.x, 0, tank.position.z);
        float distanceTankLife = glm::distance(position, lives[i].position);
        if (distanceTankLife < lives[i].safetyRadius + tank.max_point_offset) {
            lives[i].is_collected = true;
            return true;
        }
    }
    return false;
}

void Scene2::pos_random_life(struct Life *life) {
    int cnt = 4;
    float posX, posZ;
    std::mt19937 gen_life(rd_life());
    std::uniform_real_distribution<double> xDistributionLife(-22.26, 22.26);
    std::uniform_real_distribution<double> zDistributionLife(-22.26, 22.26);
    
    posX = xDistributionLife(gen_life);
    posZ = zDistributionLife(gen_life);
    // pentru a nu genera in centru
    if (posZ < 0 && posZ > -2.741) {
        posZ -= 2;
    } else if (posZ > 0 && posZ < 2.741) {
        posZ += 2;
    }
    if (posX < 0 && posX > -1.721) {
        posX -= 1;
    } else if (posX > 0 && posX < 1.721) {
        posX += 1;
    }
    life->position = glm::vec3(posX, 0, posZ);
}


void Scene2::Update(float deltaTimeSeconds)
{   
    //BUILDINGS-----------------------------------------------------------
    for (int i = 0; i < buildings.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, buildings[i].position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7, 0.7, 0.7));
        RenderMesh(buildings[i].baza, shaders["MyShader"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, buildings[i].position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7, 0.7, 0.7));
        RenderMesh(buildings[i].roof, shaders["MyShader"], modelMatrix);
    }
    //GROUND--------------------------------------------------------------
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.02f, 0));
    RenderMesh(meshes["ground"], shaders["MyShader"], modelMatrix);

    //LIVES----------------------------------------------------------------
    load_lives -= deltaTimeSeconds;
    for (int i = 0; i <= lives.size(); i++) {
        if (!lives[i].is_collected) {
            if (lives[i].position.x == 0 && lives[i].position.z == 0) {
                pos_random_life(&lives[i]);
            }
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, lives[i].position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
            RenderMesh(lives[i].key, shaders["MyShader"], modelMatrix);
            RenderMesh(lives[i].screwdriver, shaders["MyShader"], modelMatrix);
        }
    }
    if (load_lives <= 0) {
        load_lives = 30;
        for (int i = 0; i < lives.size(); i++) {
            lives[i].is_collected = false;
        }
    }

    //TANKS---------------------------------------------------------------
    //choose random the movement of the enemy tanks
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].rec_in_vector = i;
        if (enemies[i].choices.size() == 0 || enemies[i].choiceIndex >= 4 || enemies[i].timestamps[enemies[i].choiceIndex] <= 0){
            int cnt = 5;
            enemies[i].choices.clear();
            enemies[i].timestamps.clear();
            for (int j = 0; j < cnt; j++) {
                int random = rand() % 4;
                if (random == 0) {
                    enemies[i].choices.push_back("W");
                }
                if (random == 1) {
                    enemies[i].choices.push_back("S");
                }
                if (random == 2) {
                    enemies[i].choices.push_back("A");
                }
                if (random == 3) {
                    enemies[i].choices.push_back("D");
                }
                int random2 = rand() % 4 + 1;
                if (random >= 2) {
                    random2 = rand() % 2 + 1;
                }
                enemies[i].timestamps.push_back(random2);
            }
            enemies[i].choiceIndex = 0;
        }
        if (enemies[i].timestamps[enemies[i].choiceIndex] > 0) {
            enemies[i].timestamps[enemies[i].choiceIndex] -= deltaTimeSeconds;
        } else {
            if (enemies[i].choiceIndex <= 3) {
                enemies[i].choiceIndex++;
            }
        }
        //move the enemies tanks
        if (!enemies[i].freeze && !gameOver) {
            move_enemies(&enemies[i], enemies[i].choices[enemies[i].choiceIndex], deltaTimeSeconds);
        }
        enemies[i].modelMatrix = glm::mat4(1);
        enemies[i].modelMatrix *= transform3D::Translate(enemies[i].position.x, enemies[i].position.y, enemies[i].position.z);
        enemies[i].modelMatrix *= transform3D::RotateOY(enemies[i].anglestep);
        enemies[i].modelMatrixTurela = glm::mat4(1);
        enemies[i].modelMatrixTurela *= transform3D::Translate(enemies[i].position.x, enemies[i].position.y, enemies[i].position.z);

        // START SHOOTING------------------------------------------------------------------------------
            if (!enemies[i].freeze && !gameOver) {
                if (detect_player(enemies[i])) {
                    start_shooting(&enemies[i], deltaTimeSeconds);
                } else {
                    enemies[i].modelMatrixTurela *= transform3D::RotateOY(enemies[i].anglestep);
                }
            }
        //MOVE AMMUNITION------------------------------------------------------------------------------
        for (int k = enemies[i].ammunition.size() - 1; k >= 0; k--) {
            if (enemies[i].ammunition[k].time_to_live >= 0 && !Scene2::projectile_is_collision_with_building(enemies[i].ammunition[k])
                && !projectile_is_collision_with_player(enemies[i].ammunition[k]) && !gameOver) {
                enemies[i].ammunition[k].time_to_live -= deltaTimeSeconds;
                glm::mat4 modelMatrixAmmunition = glm::mat4(1);
                enemies[i].ammunition[k].position += enemies[i].ammunition[k].direction * deltaTimeSeconds * 20.0f;
                modelMatrixAmmunition *= transform3D::Translate(enemies[i].ammunition[k].position.x, enemies[i].ammunition[k].position.y, enemies[i].ammunition[k].position.z);
                modelMatrixAmmunition *= transform3D::Scale(0.5f, 0.5f, 0.5f);
                RenderMesh(enemies[i].ammunition[k].projectile, shaders["MyShader"], modelMatrixAmmunition);
            } else {
                if (projectile_is_collision_with_player(enemies[i].ammunition[k])) {
                    //create explosion---------------------------------------------------------------
                    struct Fire fire;
                    fire.fire = meshes["explosion"];
                    float offsetx = (tank.position.x - enemies[i].ammunition[k].position.x) / 2;
                    float offsetz = (tank.position.z - enemies[i].ammunition[k].position.z) / 2;
                    fire.position.x = enemies[i].ammunition[k].position.x + offsetx;
                    fire.position.z = enemies[i].ammunition[k].position.z + offsetz;
                    fire.position.y = enemies[i].ammunition[k].position.y;
                    fire.time_to_live = 0.25f;
                    fires.push_back(fire);

                    tank.life -= 10;
                    if (tank.life <= 0) {
                        cout<<"GAME OVER"<<endl;
                        gameOver = true;
                    }
                }
                enemies[i].ammunition.erase(enemies[i].ammunition.begin() + k);
            }
        }
        enemies[i].modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
        enemies[i].modelMatrixTurela *= transform3D::Scale(0.5f, 0.5f, 0.5f);
        RenderSimpleMesh(enemies[i].baza,shaders["EnemyShader"], enemies[i].modelMatrix, enemies[i].hp);
        RenderSimpleMesh(enemies[i].sine,shaders["EnemyShader"], enemies[i].modelMatrix, enemies[i].hp);
        RenderSimpleMesh(enemies[i].turela,shaders["EnemyShader"], enemies[i].modelMatrixTurela, enemies[i].hp);
        RenderSimpleMesh(enemies[i].teava,shaders["EnemyShader"], enemies[i].modelMatrixTurela, enemies[i].hp);
    }
}

bool Scene2::detect_player(struct Tank enemy) {
    glm::vec3 playerPosition = tank.position;

    // Calculează distanța dintre tancul inamic și tancul utilizatorului
    float distanceToPlayer = glm::distance(playerPosition, enemy.position);

    // Specificația distanței mici (poate fi modificată în funcție de necesități)
    float smallDistanceThreshold = 10.0f;

    // Verifică dacă inamicul este la o distanță mică de tancul utilizatorului
    if (distanceToPlayer < smallDistanceThreshold) {
        return true;
    } else {
        return false;
    }
}

void Scene2::start_shooting(struct Tank *enemy, float deltaTimeSeconds) {
    //move enemy turela towards the player    
    glm::vec3 direction = glm::normalize(tank.position - enemy->position);
    float angle = atan2(direction.x, direction.z);
    enemy->modelMatrixTurela *= transform3D::Translate(enemy->position_turela.x, 0, enemy->position_turela.z);
    enemy->modelMatrixTurela *= transform3D::RotateOY(angle);
    enemy->modelMatrixTurela *= transform3D::Translate(-enemy->position_turela.x, 0, -enemy->position_turela.z);
    enemy->angleTurela = angle;
    enemy->time_to_shoot -= deltaTimeSeconds * 0.8f;
    if (enemy->time_to_shoot <= 0) {
        load_ammunition(enemy);
        enemy->time_to_shoot = 1.0f;
    }
}

void Scene2::load_ammunition(struct Tank *enemy) {
    enemy->projectiles_to_load++;
    struct Projectile projectile;
    projectile.projectile = new Mesh("proiectil");
    projectile.projectile->LoadMesh(PATH_JOIN(window->props.selfDir,  SOURCE_PATH::M1, "Tanks_game", "objects"), "proiectil.obj");
    meshes[projectile.projectile->GetMeshID()] = projectile.projectile;

    glm::mat3 rotationMatrixdir = glm::mat3(enemy->modelMatrixTurela);
    glm::vec3 tankDirectionMaxPoint = glm::vec3(enemy->modelMatrixTurela * glm::vec4(enemy->position_turela, 1.0f));
    projectile.direction = glm::vec3(rotationMatrixdir * glm::vec4(glm::vec3(0, 0, 1), 1.0));
    projectile.position = glm::vec3(tankDirectionMaxPoint.x, 0.7901 * 0.5, tankDirectionMaxPoint.z) - glm::normalize(projectile.direction) * (-0.8f);
    projectile.time_to_live = 3.0f;
    projectile.shoot = false;
    enemy->ammunition.push_back(projectile);
}

void Scene2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    mesh->Render();
}


void Scene2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, float hp)
{
    
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    GLint hpUniformLocation = glGetUniformLocation(shader->GetProgramID(), "hp");
    glUniform1f(hpUniformLocation, hp);

    mesh->Render();
}

