#pragma once

typedef enum {
    SCENE_INVALID = -1,
    SCENE0_AMC_BANNER,          // Loading AMC banner 
    SCENE1_DOMAIN_LOGO,         // Domain Logo animation
    SCENE2_EARTH_AND_SPACE,     // Earth and Space 
    SCENE3_LUCY,                // Lucy
    SCENE4_INTRO,               // Intro 
    SCENE5_KARUN_RAS,           // Karun Ras 
    SCENE6_BHAYANK_RAS,         // Bhayanak Ras
    SCENE7_RAUDRA_RAS,          // Raudra Ras
    SCENE8_BIBHATSA_RAS,        // Bibhatsa Ras
    SCENE9_VEER_RAS,            // Veer Ras 
    SCENE10_ADBHUT_RAS,         // Adbhut Ras
    SCENE11_SHRINGAR_RAS,       // Shringar Ras
    SCENE12_HASYA_RAS,          // Hasya Ras
    SCENE13_SHANT_RAS,          // Shant Ras
    SCENE14_PARTICLE,           // Particle Simulation
    SCENE15_END_CREDIS,         // End Credits
    SCENE_PLACEHOLDER_INDOOR,
    MAX_SCENES
} scene_types_t;

