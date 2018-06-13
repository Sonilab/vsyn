//
//  particle.hpp
//
//  Created by momoha/rin on 2017/10/13.
//
//
#ifndef particle_hpp
#define particle_hpp

#include "ofMain.h"


typedef enum mode_e {BROWNIAN, GRAVITY, GATHERING} mode_e;

class Particle {
    
public:
    
    int screen_width;
    int screen_height;
    
    void setup(int w, int h);
    
    void update();
    void draw();
    void drawLog();
    
    void blownianMotion();
    void gravityMotion();
    void gatheringMotion();
    
    static constexpr float PARTICLE_ALPHA_DEFAULT = 0.8;
    static constexpr float MOD_COEF = 0.0003;
    static constexpr float GRAVITY_SPD_ACC = -0.00001;
    static constexpr float GRAVITY_SPD_LIMIT = -0.008;
    static constexpr float GATHERING_SPD_LIMIT = 0.002;
    static constexpr float ATTRACTOR_A = -0.00003;
    static const int MAX = 15000;   //particle max num
    
    //particles
    ofVec2f pos[MAX], vel[MAX], drawPos[MAX];
    int particleNum;
    
    //general
    float modulation;
    float size;
    float range_x;
    float range_y;
    
    //gathering
    float attractor_x;
    float attractor_y;
    float attractor_size;   //0.0 - 1.0
    
    //vbo
    ofVbo vbo;
    
    mode_e mode;
    bool state_log;
    
};

#endif /* particle_hpp */
