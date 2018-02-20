#pragma once
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Game Settings
// DEBUG
#define DEBUG_PLAYER_SPEED 1.0f
#define DEBUG_PLAYER_ACCELERATION  2.25f
#define DEBUG_PLAYER_MAXSPEED  400.f
#define DEBUG_PLAYER_SPEEDREDUCTION  0.99f
// NOT DEBUG
#define PLAYER_SPEED  1.0f
#define PLAYER_ACCELERATION  2.25f
#define PLAYER_MAXSPEED  400.f
#define PLAYER_SPEEDREDUCTION  0.99f

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Entity Builder
#define ENTITY_ARRAYSIZE	10000

// Bullet manager
#define BULLET_ARRAYSIZE  256
#define BULLET_SPEED  100.f
#define BULLET_LIFETIME  20.f

// Component Manager
#define COMPONENT_ARRAYSIZE 10000

// Level Loader
#define LEVELLOADER_JSONFILES_MAXCOUNT 8
#define LEVELLOADER_MODELSINCOCKPIT_MAXCOUNT 16
#define LEVELLOADER_ASTEROIDS_INWORLD_MAXCOUNT 20000
#define LEVELLOADER_ENEMYS_INWORLD_MAXCOUNT 20000
#define LEVELLOADER_STRUCTURES_INWORLD_MAXCOUNT 20000
#define LEVELLOADER_OBJECTS_INWORLD_MAXCOUNT 20000
#define LEVELLOADER_CHECKPOINT_INWORLD_MAXCOUNT 16
#define LEVELLOADER_PICKUP_INWORLD_MAXCOUNT  64
#define LEVELLOADER_TRIGGER_INWORLD_MAXCOUNT 128
#define LEVELLOADER_PARTICLEEMITTORS_INWORLD_MAXCOUNT 512
#define LEVELLOADER_AUDIOSPLINES_INWORLD_MAXCOUNT 4
#define LEVELLOADER_DATAPOINTS_INWORLD_MAXCOUNT 1024
#define LEVELLOADER_COLLIDERS_INWORLD_MAXCOUNT 1024
#define LEVELLOADER_WAVES_INWORLD_MAXCOUNT 64
#define LEVELLOADER_UNITYEXPORTPATH "../Assets/Data/UnityExport/"
#define LEVELLOADER_SKYBOX_FBX_FILEPATH "../Assets/Models/Skybox/skybox.fbx"
#define LEVELLOADER_SKYBOX_DDS_FILEPATH "../Assets/Models/Skybox/skybox_Basecolor.dds"

