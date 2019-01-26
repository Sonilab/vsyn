//
//  VSyn.cpp
//  vSyn
//
//  Created by sonir on 9/8/17.
//
//

#include "VSyn.hpp"

using namespace std;

void VSyn::setup(){
    
    ofBackground(0, 0, 0);
    ofSetCircleResolution(50);
    
    //CAM SETUP
    cam_flg = false;
    pov.set(POV_INIT_X*ofGetWidth()*(-1), POV_INIT_Y*ofGetHeight(), POV_INIT_Z*ofGetWidth());
    look.set(LOOK_INIT_X*ofGetWidth()*(-1), LOOK_INIT_Y*ofGetHeight(), LOOK_INIT_Z*ofGetWidth());
    //Setup Particles
    particle.setup(ofGetWidth(),ofGetHeight());
    
    
    cout << "VSyn SETUP" << endl;
    receiver.setup(PORT);
    current_msg_string = 0;
    
    //Init Buffers for Graphics
    initColors(CONTAINER_MAX);
    initShapes(CONTAINER_MAX);
    
    //init font
    font.setGlobalDpi(FONT_DPI);
//    font.load("Helvetica.ttf", 18);
    font.load(OF_TTF_SANS, DEFALUT_FONT_SIZE, true, true, true);
//    bool ofTrueTypeFont::load(const std::string& _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi) {

//    font.setLineHeight(24);
    font.setLetterSpacing(1.0);
    
}


void VSyn::update(){
    
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for mouse moved message
        if(m.getAddress() == "/circle"){
            
            int uid =  m.getArgAsInt32(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float size = m.getArgAsFloat(3);
            int fill = m.getArgAsInt32(4);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toCircle(&shapes[uid] ,uid, x, y, size, fill);
            }
        } else if(m.getAddress() == "/triangle"){
            
            int uid =  m.getArgAsInt32(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float size = m.getArgAsFloat(3);
            float angle = m.getArgAsFloat(4);
            int fill = m.getArgAsInt32(5);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toTriangle(&shapes[uid] ,uid, x, y, size, angle, fill);
            }
        } else if(m.getAddress() == "/square"){
            
            int uid =  m.getArgAsInt32(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float size = m.getArgAsFloat(3);
            float angle = m.getArgAsFloat(4);
            int fill = m.getArgAsInt32(5);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toSquare(&shapes[uid] ,uid, x, y, size, angle, fill);
            }
        } else if(m.getAddress() == "/rect"){
            
            int uid =  m.getArgAsInt32(0);
            float x1 = m.getArgAsFloat(1);
            float y1 = m.getArgAsFloat(2);
            float x2 = m.getArgAsFloat(3);
            float y2 = m.getArgAsFloat(4);
            float angle = m.getArgAsFloat(5);
            int fill = m.getArgAsInt32(6);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toRect(&shapes[uid] ,uid, x1, y1, x2, y2, angle, fill);
            }
        } else if(m.getAddress() == "/line"){
            
            int uid =  m.getArgAsInt32(0);
            float x1 = m.getArgAsFloat(1);
            float y1 = m.getArgAsFloat(2);
            float x2 = m.getArgAsFloat(3);
            float y2 = m.getArgAsFloat(4);
            float thick = m.getArgAsFloat(5);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toLine(&shapes[uid] ,uid, x1, y1, x2, y2, thick);
            }
        } else if(m.getAddress() == "/arc"){
            
            int uid =  m.getArgAsInt32(0);
            float x1 = m.getArgAsFloat(1);
            float y1 = m.getArgAsFloat(2);
            float x2 = m.getArgAsFloat(3);
            float y2 = m.getArgAsFloat(4);
            float height = m.getArgAsFloat(5);
            float expose = m.getArgAsFloat(6);
            float thick = m.getArgAsFloat(7);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toArc(&shapes[uid] ,uid, x1, y1, x2, y2, height, expose, thick);
            }
        } else if(m.getAddress() == "/wave"){
            
            int uid =  m.getArgAsInt32(0);
            
            float x1 = m.getArgAsFloat(1);
            float y1 = m.getArgAsFloat(2);
            float x2 = m.getArgAsFloat(3);
            float y2 = m.getArgAsFloat(4);
            float freq = m.getArgAsFloat(5);
            float height = m.getArgAsFloat(6);
            float phase = m.getArgAsFloat(7);
            float thick = m.getArgAsFloat(8);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toWave(&shapes[uid] ,uid, x1, y1, x2, y2, freq, height, phase, thick);
            }
            
        } else if(m.getAddress() == "/color"){
            
            int uid = m.getArgAsInt32(0);
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                colors[uid].r = (char) ( 255.0f * m.getArgAsFloat(1) );
                colors[uid].g = (char) ( 255.0f * m.getArgAsFloat(2) );
                colors[uid].b = (char) ( 255.0f * m.getArgAsFloat(3) );
                colors[uid].a = (char) ( 255.0f * m.getArgAsFloat(4) );
                
            }
            
        } else if(m.getAddress() == "/mute"){
            
            int uid =  m.getArgAsInt32(0);
            bool flg = (bool)m.getArgAsInt32(1);
            
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toMute(&shapes[uid], flg);
            }
            
        } else if(m.getAddress() == "/cam/pov"){
            
            pov.x =  ( m.getArgAsFloat(0) * ofGetWidth() * (-1)); //Invert x posi for GL coordinate sys
            pov.y =  ( m.getArgAsFloat(1) * ofGetWidth() );
            pov.z =  ( m.getArgAsFloat(2) * ofGetWidth() );
            
        } else if(m.getAddress() == "/cam/look"){
            
            look.x =  ( m.getArgAsFloat(0) * ofGetWidth()  * (-1) ); //Invert x posi for GL coordinate sys
            look.y =  ( m.getArgAsFloat(1) * ofGetWidth() );
            look.z =  ( m.getArgAsFloat(2) * ofGetWidth() );
            
        } else if(m.getAddress() == "/cam/on"){
            
            if ( m.getArgAsInt(0) == 1 )
                cam_flg = true;
            else if(m.getArgAsInt(0) == 0)
                cam_flg = false;
            
        }else if(m.getAddress() == "/particle/attractor/x"){
            
            particle.attractor_x = m.getArgAsFloat(0) * ofGetWidth();
            
        }else if(m.getAddress() == "/particle/attractor/y"){
            
            particle.attractor_y = m.getArgAsFloat(0) * ofGetHeight();
            
        }else if(m.getAddress() == "/particle/lfo"){
            
            particle.lfo = m.getArgAsFloat(0);
            
        }else if(m.getAddress() == "/particle/size"){
            
            particle.size = m.getArgAsFloat(0) * 2.5;
            
        }else if(m.getAddress() == "/particle/num"){
            
            particle.particleNum = m.getArgAsInt(0);
            
        }else if(m.getAddress() == "/particle/mode"){
            
            //particle.mode = m.getArgAsInt(0);
            int flg =  m.getArgAsInt(0);
            switch (flg){
                    
                case 0:
                    particle.mode = BROWN;
                    break;
                case 1:
                    particle.mode = BUBBLE;
                    particle.bubbleInit();
                    break;
                case 2:
                    particle.mode = GRAVITY;
                    break;
                    
                    
            }
            
        }else if(m.getAddress() == "/particle/speed"){
            
            particle.speed = m.getArgAsFloat(0);
            
        }else if(m.getAddress() == "/particle/range"){
            
            particle.range_x = m.getArgAsFloat(0);
            particle.range_y = m.getArgAsFloat(1);
            
        }else if(m.getAddress() == "/particle/mute"){
            particle.mute = bool(m.getArgAsInt(0));

            
        }else if(m.getAddress() == "/string"){
            
            int uid = m.getArgAsInt32(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float size = m.getArgAsFloat(3);
            string str = m.getArgAsString(4);
            
            if(uid < CONTAINER_MAX) // check the index is enable
            {
                toString(&shapes[uid] ,uid, x, y, size, str);
            }
            
            
        }else {
            
            cout << "OSC :: unknown ADR :: " << m.getAddress() << endl;
            
        }
        
        
        
    }// end of while
    
    
    particle.update();
    
}


void VSyn::initColors(int max_num){
    
    for(int i=0; i<max_num; i++){
        
        colors[i].r = 255;
        colors[i].g = 255;
        colors[i].b = 255;
        colors[i].a = 255;
        
        
    }
    
}


void VSyn::initShapes(int max_num){
    
    for(int i=0; i<max_num; i++){
        
        shapeContainer *pC = &shapes[i];
        pC->type = VOID;
        pC->active = true;
        pC->uid = -1;
        
        pC->x1 = 0.5f;
        pC->y1 = 0.5f;
        pC->x2 = 0.5f;
        pC->y2 = 0.5f;
        
        pC->size = 0.0f;
        pC->height = 0.0f;
        pC->angle = 0.0f;
        pC->freq = 0.0f;
        pC->phase = 0.0f;
        pC->thick = 0.0f;
        pC->expose = 1.0f;
        
        pC->fill = false;
        
        
    }
    
}



void VSyn::draw(){
    
    
    //CAM CONTROL
    if(cam_flg){
        
        cam.begin();
        cam.setPosition(pov);
        cam.lookAt(look);
        
        //Invert Y axis for GL
        ofPushMatrix();
        ofScale(1, -1); //Invert Y only
        
    }
    
    
    
    //drawing particle
    particle.draw();
    
    
    
    for(int i=0; i<CONTAINER_MAX; i++){
        
        shapeContainer *elm = &shapes[i];
        //Set Color
        ofSetColor(colors[i]);
        
        if ( !elm->active || elm->type == VOID){
            
            continue;
            
        }
        
        switch(elm->type){
                
            case CIRCLE:
                vbo.circle(elm->x1, elm->y1, elm->size, elm->fill);
                break;
                
            case TRIANGLE:
                vbo.triangle(elm->x1, elm->y1, elm->size, elm->angle, elm->fill);
                break;
                
            case SQUARE:
                vbo.square(elm->x1, elm->y1, elm->size, elm->angle, elm->fill);
                break;
                
            case RECT:
                vbo.rect(elm->x1, elm->y1, elm->x2, elm->y2, elm->angle, elm->fill);
                break;
                
            case LINE:
                vbo.line(elm->x1, elm->y1, elm->x2, elm->y2, elm->thick);
                break;
                
            case ARC:
                vbo.arc(elm->x1, elm->y1, elm->x2, elm->y2, elm->height, elm->expose, elm->thick);
                break;
                
            case WAVE:
                vbo.wave(elm->x1, elm->y1, elm->x2, elm->y2, elm->freq, elm->height, elm->phase, elm->thick);
                break;
                
            case STR:
                drawStrings(elm->x1, elm->y1, elm->size, elm->str);
                break;
                
            default:
                break;
                
        }
        
        
    }
    
    //Draw primitives
    vbo.draw();
    
    if(cam_flg){
        ofPopMatrix();
        cam.end();
    }
    
}


void VSyn::initWindowSize(){
    
    int tmp_w = ofGetWidth();
    int tmp_h = ofGetHeight();
    
    vbo.setup(tmp_w, tmp_h);
    particle.screen_width = tmp_w;
    particle.screen_height = tmp_h;
    
    
}

void VSyn::drawStrings(float x1, float y1, float size, string str){
    int tmp_w = ofGetWidth();
    int tmp_h = ofGetHeight();
    ofPushMatrix();
    ofTranslate(tmp_w * x1, tmp_h * y1);
    ofScale((0.05 + size * size * size * DEFAULT_FONT_SIZE_PER_Y) * tmp_h, (0.05 + size * size * size * DEFAULT_FONT_SIZE_PER_Y)* tmp_h);
    ofScale(1.0 / float(FONT_DPI), 1.0 / float(FONT_DPI));
    ofTranslate(-font.stringWidth(str) * 0.5, font.stringHeight(str) * 0.5);
    font.drawStringAsShapes(str, 0, 0);
    ofPopMatrix();
}

