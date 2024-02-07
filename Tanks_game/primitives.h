#include "components/simple_scene.h"
#include "components/transform.h"
#include <string>


struct Building {
    Mesh *baza;
    Mesh *roof;
    glm::vec3 position;
    int type;
    float safetyRadius;
    float distance_in_collision;
};

struct Tank {
    Mesh *baza;
    Mesh *sine;
    Mesh *turela;
    Mesh *teava;
    float max_point_sine_offset = 1.169 * 0.5;
    float max_point_offset = 1.7418 * 0.5;
    float length = 3.483 * 0.5;
    float width = 1.443 * 0.5;
    float distance_to_building;
    bool is_shooting;
    glm::vec3 direction_max_point;
    glm::vec3 position_turela = glm::vec3( -0.000, 0.9541 * 0.5, -0.3513 * 0.5);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::mat4 modelMatrixTurela;
    glm::mat4 modelMatrix;
    glm::mat4 rotationMatrix;
    glm::vec3 updated_direction_max_point;
    glm::vector<struct Projectile> ammunition;
    float projectiles_to_load;
    float angleTurela;
    float anglestep;
    //for enemies
    glm::vector<glm::string> choices;
    glm::vector<float> timestamps;
    bool shoot_projectile;
    int choiceIndex;
    int rec_in_vector;
    float life;
    float freeze;
    float hp;
    float time_to_shoot;
};

struct Projectile {
    Mesh *projectile;
    glm::vec3 position;
    glm::vec3 direction;
    float time_to_live = 0;
    float shoot;
    float radius = 0.065214 * 0.5;
};

struct Fire {
    Mesh *fire;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    float time_to_live;
};

struct Life {
    Mesh *key;
    Mesh *screwdriver;
    glm::vec3 position;
    float time_to_live;
    bool is_collected;
    float safetyRadius = 0.5;
};