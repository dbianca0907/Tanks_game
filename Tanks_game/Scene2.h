#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "primitives.h"
#include "transform3D.h"
#include <random>
#include <string>
#include "camera.h"

namespace m1
{ 
    class Scene2 : public gfxc::SimpleScene {
        public:
        void Init()override;
        void Update(float deltaTimeSeconds)override;
        void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)override;
        void is_collision_to_building(struct Tank *tank);
        int find_building_in_collision(struct Tank tank);
        void check_and_correct_collision(Tank *tank, float deltaTime);
        bool projectile_is_collision_with_building(struct Projectile projectile);
        void move_enemies(struct Tank *enemy, glm::string choice, float deltaTime);
        void is_colision_to_tank(struct Tank *tank);
        bool projectile_is_collision_with_tank(struct Projectile projectile);
        struct Tank* get_tank_by_position(struct  Projectile projectile);
        void load_ammunition(struct Tank *tank);
        bool detect_player(struct Tank enemy);
        void start_shooting(struct Tank *enemy, float deltaTimeSeconds);
        bool projectile_is_collision_with_player(struct Projectile projectile);
        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, float hp);
        void pos_random_life(struct Life *life);
        bool is_collision_to_life(struct Tank tank);

    struct Tank tank;
    glm::vec3 lightPosition;
    unsigned int materialShininess;
    float materialKd;
    float materialKs;
    std::random_device rd_life;
    float load_lives;
    std::vector<struct Life> lives;
    glm::vector<struct Building> type_of_buildings;
    glm::vector<struct Building> buildings;
    glm::vector<struct Tank> enemies;
    glm::vector<struct Fire> fires;
    Camera *camera;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPos, cameraCenter, cameraUp, cameraOffset;
    bool cameraRotation;
    bool gameOver;
    };
}
