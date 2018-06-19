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
        init_x[i] = pos[i].x;
    }
    
    vbo.setVertexData(drawPos, MAX, GL_DYNAMIC_DRAW);
    
    //general
    particleNum = 0;    //0 - MAX
    lfo = 0.;
    speed = 0.1;
    range_x = 1.0;
    range_y = 1.0;
    size = 0.5;
    mode = BROWN;
    
    //bubble
    for(int i = 0; i < MAX; i++){
        bubble_range[i] = ofRandom(0.01, 0.001);
    }
    theta = 0.0;
    
    //gravity
    attractor_x = 0.5;
    attractor_y = 0.5;
    
}

void Particle :: update(){
    if(particleNum == 0 || mute == true){
        return;
    }
    
    //pos & vel are defined as 0-1
    if(particleNum > 0){
        switch (mode)
        {
            case BROWN:
                brownMotion();
                break;
                
            case BUBBLE:
                bubbleMotion();
                break;
                
            case GRAVITY:
                gravityMotion();
                break;
                
            default:
                break;
        }
        
        for(int i = 0; i < MAX; i++){
            //            pos[i] += vel[i];
            
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
    
    if(particleNum == 0 || mute == true){
        return;
    }
    
    glPointSize(size);
    glEnable(GL_BLEND);
    //enable addition synthesis
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glColor4f(1.0f, 1.0f, 1.0f, PARTICLE_ALPHA_DEFAULT);    //white
    vbo.draw(GL_POINTS, 0, particleNum);
    
}

void Particle :: brownMotion(){
    float coef = lfo * (speed * BROWN_SPEED_COEF + 1.0) * BROWN_COEF;
    
    for(int i = 0; i < MAX; i++){
        vel[i].set(coef * ofRandom(-1.0, 1.0), coef * ofRandom(-1.0, 1.0));
        pos[i] += vel[i];
    }
    
}

void Particle :: bubbleInit(){
    for(int i = 0; i < MAX; i++){
        init_x[i] = pos[i].x;
        vel[i].y = 0.0;
    }
}

void Particle :: bubbleMotion(){
    theta += speed * 0.1;
    if(theta > 360.0){
        theta = 0.0;
    }
    
    for(int i = 0; i < MAX; i++){
        if(vel[i].y <= BUBBLE_SPD_LIMIT * (speed + 1.0)){
            vel[i].y = BUBBLE_SPD_LIMIT * (speed + 1.0);
        }else{
            vel[i].y += BUBBLE_SPD_ACC * (speed + 1.0);
        }
        
        float diff = lfo * bubble_range[i] * sin(theta + i);
        
        pos[i].x = init_x[i] + diff;
        pos[i].y += vel[i].y;
    }
    
}

void Particle :: gravityMotion(){
    
    for(int i = 0; i < MAX; i++){
        float theta = atan2(drawPos[i].y - attractor_y, drawPos[i].x - attractor_x);
        float dist = ofDist(drawPos[i].x, drawPos[i].y, attractor_x, attractor_y);
        if(dist < 100.0 && vel[i].length() > GRAVITY_SPD_LIMIT){
            vel[i] = vel[i].normalize() * GRAVITY_SPD_LIMIT;
        }
        vel[i].x += GRAVITY_ATTRACTOR_A * (speed + 1.0) * cos(theta) + lfo * GRAVITY_MOD_COEF * ofRandom(-1.0, 1.0);
        vel[i].y += GRAVITY_ATTRACTOR_A * (speed + 1.0) * sin(theta) + lfo * GRAVITY_MOD_COEF * ofRandom(-1.0, 1.0);
        
        pos[i] += vel[i];
    }
    
}
