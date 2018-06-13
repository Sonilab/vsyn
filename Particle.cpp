//
//  particle.cpp
//
//  Created by momoha/rin on 2017/10/13.
//
//


#include "Particle.hpp"

void Particle :: setup(int w, int h){
    
    //Set the default screen
    screen_width = w;
    screen_height = h;
    
    for(int i = 0; i < MAX; i++){
        pos[i].set(ofRandom(0.0, 1.0), ofRandom(0.0, 1.0));
    }
    
    vbo.setVertexData(drawPos, MAX, GL_DYNAMIC_DRAW);
    
    //general
    particleNum = 0;    //0 - MAX
    modulation = 0.;
    range_x = 1.0;
    range_y = 1.0;
    size = 0.5;
    mode = BROWNIAN;
    //gathering
    attractor_size = 0.5;
    attractor_x = 0.5;
    attractor_y = 0.5;
}

void Particle :: update(){
    
    //pos & vel are defined as 0-1
    if(particleNum > 0){
        switch (mode)
        {
            case BROWNIAN:
                blownianMotion();
                break;
                
            case GRAVITY:
                gravityMotion();
                break;
                
            case GATHERING:
                gatheringMotion();
                break;
                
            default:
                break;
        }
        
        for(int i = 0; i < MAX; i++){
            pos[i] += vel[i];
            
            //boundary condition
            if(pos[i].x < 0.0){
                pos[i].x = 1.0;
            }
            if(pos[i].x > 1.0){
                pos[i].x = 0.0;
            }
            if(pos[i].y < 0.0){
                pos[i].y = 1.0;
            }
            if(pos[i].y > 1.0){
                pos[i].y = 0.0;
            }
            
            drawPos[i].x = (0.5 + (pos[i].x - 0.5) * range_x) * screen_width;
            drawPos[i].y = (0.5 + (pos[i].y - 0.5) * range_y) * screen_height;
            
        }
        
        vbo.updateVertexData(drawPos, MAX);
    }
    
    if(particleNum < 0){
        particleNum = 0;
    }
    if(particleNum > MAX){
        particleNum = MAX;
    }
    
}

void Particle :: draw(){
    
    if(particleNum == 0){
        return;
    }
    
    glPointSize(size);
    glEnable(GL_BLEND);
    //enable addition synthesis
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glColor4f(1.0f, 1.0f, 1.0f, PARTICLE_ALPHA_DEFAULT);    //white
    vbo.draw(GL_POINTS, 0, particleNum);
    
}

void Particle :: blownianMotion(){
    
    for(int i = 0; i < MAX; i++){
        vel[i].set(modulation * MOD_COEF * ofRandom(-1.0, 1.0), modulation * MOD_COEF * ofRandom(-1.0, 1.0));
    }
    
}

void Particle :: gravityMotion(){
    
    for(int i = 0; i < MAX; i++){
        float next_y;
        if(vel[i].y <= GRAVITY_SPD_LIMIT){
            vel[i].y = GRAVITY_SPD_LIMIT;
            next_y = vel[i].y;
        }else{
            
            next_y = vel[i].y + GRAVITY_SPD_ACC;
        }
        
        vel[i].set(modulation * MOD_COEF * ofRandom(-1.0, 1.0), next_y);
    }
    
}

void Particle :: gatheringMotion(){
    
    for(int i = 0; i < MAX; i++){
        float theta = atan2(drawPos[i].y - attractor_y, drawPos[i].x - attractor_x);
        float dist = ofDist(drawPos[i].x, drawPos[i].y, attractor_x, attractor_y);
        if(dist < 100.0 && vel[i].length() > GATHERING_SPD_LIMIT){
            vel[i] = vel[i].normalize() * GATHERING_SPD_LIMIT;
        }
        vel[i].x += ATTRACTOR_A * attractor_size * cos(theta) + modulation * MOD_COEF * ofRandom(-1.0, 1.0);
        vel[i].y += ATTRACTOR_A * attractor_size * sin(theta) + modulation * MOD_COEF * ofRandom(-1.0, 1.0);
    }
    
}
