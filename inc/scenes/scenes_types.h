#pragma once

typedef enum {
    SCENE_INVALID = -1,
    SCENE00_AMC_BANNER,
    SCENE01_DOMAIN_LOGO,        // Domain Logo animation
    SCENE02_EARTH_AND_SPACE,    // Earth and Space 
    SCENE03_LUCY,               // Lucy
    SCENE04_INTRO,              // Intro 
    SCENE05_KARUN_RAS,          // Karun Ras 
    SCENE06_BHAYANK_RAS,        // Bhayanak Ras
    SCENE07_RAUDRA_RAS,         // Raudra Ras
    SCENE08_BIBHATSA_RAS,       // Bibhatsa Ras
    SCENE09_VEER_RAS,           // Veer Ras 
    SCENE10_ADBHUT_RAS,         // Adbhut Ras
    SCENE11_SHRINGAR_RAS,       // Shringar Ras
    SCENE12_HASYA_RAS,          // Hasya Ras
    SCENE13_SHANT_RAS,          // Shant Ras
    SCENE14_PARTICLE,           // Particle Simulation
    SCENE15_END_CREDITS,         // End Credits
    SCENE_PLACEHOLDER_INDOOR,
    MAX_SCENES
} scene_types_t;

