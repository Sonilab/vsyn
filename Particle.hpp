//
//  particle.hpp
//
//  Created by momoha/rin on 2017/10/13.
//
//
#ifndef particle_hpp
#define particle_hpp

#include "ofMain.h"


typedef enum mode_e {BROWN, BUBBLE, GRAVITY} mode_e;

class Particle {
    
public:
    
    int screen_width;
    int screen_height;
    
    void setup(int w, int h);
    
    void update();
    void draw();
    void drawLog();
    
    void brownMotion();
    void bubbleMotion();
    void bubbleInit();
    void gravityMotion();
    
    static constexpr float PARTICLE_ALPHA_DEFAULT = 0.8;
    static constexpr float BROWN_COEF = 0.001;
    static constexpr float MOD_COEF = 0.0003;
    static constexpr float SPEED_COEF = 0.01;
    static constexpr float BUBBLE_SPD_ACC = -0.000005;
    static constexpr float BUBBLE_SPD_LIMIT = -0.008;
    static constexpr float GRAVITY_SPD_LIMIT = 0.002;
    static constexpr float ATTRACTOR_A = -0.00003;
    static const int MAX = 15000;   //particle max num
    
    //particles
    ofVec2f pos[MAX], vel[MAX], drawPos[MAX];
    int particleNum;
    
    //general
    float lfo;
    float size;
    float speed;
    float range_x;
    float range_y;
    bool mute;
    
    //bubble
    float init_x[MAX];
    float bubble_range[MAX];
    float theta;
    
    //gravity
    float attractor_x;
    float attractor_y;
    
    //vbo
    ofVbo vbo;
    
    mode_e mode;
    bool state_log;
    
};

#endif /* particle_hpp */
