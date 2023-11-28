#ifndef OSEE_MATH_H
#define OSEE_MATH_H

#include <math.h>
#include <algorithm>
#include <stdio.h>

struct ColorYCbCr{
    u_int8_t Y;
    u_int8_t Cr;
    u_int8_t Cb;
};

struct SuperSourceSource{
    u_int8_t source1:4;
    u_int8_t source2:4;
    u_int8_t background:4;
};

struct LumaKeySource{
    u_int8_t fill:4;
    u_int8_t key:4;
};

struct LumaKeyCtrl{
    u_int8_t shaped:1;
    u_int8_t invert:1;
    u_int8_t mask:1;
    u_int8_t resize:1;
};


struct WipePosition{
    int h_pos;
    int v_pos;
    int h_param;
    int v_param;
    WipePosition(){
        h_pos = 0;
        v_pos = 0;
        h_param = 0;
        v_param = 0;
    }
};

struct DSKSource{
    u_int8_t fill:4;
    u_int8_t key:4;
};

struct OutSource{
    u_int16_t hdmi1:3;
    u_int16_t hdmi2:3;
    u_int16_t uvc:3;
};

struct ChromaKeyData{
    int ck_ys;
    int k1;
    int k2;
    int k3;
    int k4;
    int k5;
    int k6;
    int k7;
    int k8;
    int k9;
    int k10;
    int k11;
    int k12;
    int gain;
    int clip;

    void print(){
        printf("ck_ys = %d\n",ck_ys);
        printf("k1 = %d\n",k1);
        printf("k2 = %d\n",k2);
        printf("k3 = %d\n",k3);
        printf("k4 = %d\n",k4);
        printf("k5 = %d\n",k5);
        printf("k6 = %d\n",k6);
        printf("k7 = %d\n",k7);
        printf("k8 = %d\n",k8);
        printf("k9 = %d\n",k9);
        printf("k10 = %d\n",k10);
        printf("k11 = %d\n",k11);
        printf("k12 = %d\n",k12);
        printf("gain = %d\n",gain);
        printf("clip = %d\n",clip);
    }
};



int getAudioRegValue(double number);
int getMonitorRegValue(double number);
void ycbcr2Rgb(int Y,int Cb,int Cr,int *R,int *G,int *B);
ColorYCbCr getColorYC(int Hue,int Saturation,int Brightness);
int getMaskPositionV(float value);
int getMaskPositionH(float value);
int getFTBRateValue(float value);
int getFTBRateValue(float value,int outFormat);
int getWipeSoftness(float value);
int getKeyGain(float value);
int getKeyClip(float value);
ChromaKeyData getChromaKeyData(int y,int cb,int cr,float bgclear,float blkgloss,float green_suppression_ratio,float background_clean,int key_edge,float background,float keyedge);


double dround(double x,int bit);

#define h_active 1920.0
#define v_active 1080.0

class TransitionWipePositionClass{
public:
    TransitionWipePositionClass(float hPos,float vPos,float p_symmetry){
        h_pos = hPos;
        v_pos = vPos;
        symmetry = p_symmetry;
        printf("h_pos:%f,v_pos:%f,symmetry:%f\n",v_pos,h_pos,symmetry);
    }
    virtual ~TransitionWipePositionClass(){}

    virtual void checkout() = 0;

    float h_pos;
    float v_pos;
    float symmetry;
    float ratio;
    float h_param;
    float v_param;
    int reg_h_pos;
    int reg_v_pos;
    int reg_h_param;
    int reg_v_param;
};

class TransitionWipePositionLeftToRight:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        h_param = pow(2,24) / h_active;
        v_param = 0;
        reg_h_pos = 0;
        reg_v_pos = 0;
        reg_h_param = (int)h_param;
        reg_v_param = (int)v_param;
    }
};

class TransitionWipePositionTopToBottom:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        h_param = 0;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 0;
        reg_v_pos = 0;
        reg_h_param = (int)h_param;
        reg_v_param = (int)v_param;
    }
};

class TransitionWipePositionVerticalCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        h_param = pow(2,24) / (h_active * std::max(1 - h_pos,h_pos));
        v_param = 0;
        reg_h_pos = (int)(h_active * h_pos / 2 + 0.5);
        reg_v_pos = 0;
        reg_h_param = (int)h_param;
        reg_v_param = (int)v_param;
    }
};

class TransitionWipePositionHorizontalCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        h_param = 0;
        v_param = pow(2,24) / (v_active * std::max(1 - v_pos,v_pos));
        reg_v_pos = (int)(v_active * v_pos / 2 + 0.5);
        reg_h_pos = 0;
        reg_h_param = (int)h_param;
        reg_v_param = (int)v_param;
    }
};

class TransitionWipePositionCrossCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / (h_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        v_param = pow(2,24) / (v_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        reg_h_pos = (int)(h_active * h_pos / 2 + 0.5);
        reg_v_pos = (int)(v_active * v_pos / 2 + 0.5);
        reg_h_param = (int)(symmetry<0.5?h_param/(ratio/(h_active/v_active)):h_param);
        reg_v_param = (int)(symmetry>=0.5?v_param/((h_active/v_active)/ratio):v_param);
    }
};

class TransitionWipePositionSquareCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / (h_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        v_param = pow(2,24) / (v_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        reg_h_pos = (int)(h_active * h_pos / 2 + 0.5);
        reg_v_pos = (int)(v_active * v_pos / 2 + 0.5);
        reg_h_param = (int)(symmetry<0.5?h_param/(ratio/(h_active/v_active)):h_param);
        reg_v_param = (int)(symmetry>=0.5?v_param/((h_active/v_active)/ratio):v_param);
    }
};

class TransitionWipePositionDiamond:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / (h_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        v_param = pow(2,24) / (v_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        reg_h_pos = (int)(h_active * h_pos / 2 + 0.5);
        reg_v_pos = (int)(v_active * v_pos / 2 + 0.5);

        float I23,I24,J23,J24,K23,K24;
        I23 = symmetry<0.5?h_param/(ratio/(h_active/v_active)):h_param;
        I24 = symmetry>=0.5?v_param/((h_active/v_active)/ratio):v_param;
        J23 = std::max(h_active-reg_h_pos*2,(double)reg_h_pos*2)*I23;
        J24 = std::max(v_active-reg_v_pos*2,(double)reg_v_pos*2)*I24;
        K23 = (J23+J24)/2;
        K24 = pow(2,24)/K23;

        reg_h_param = (int)(I23*K24);
        reg_v_param = (int)(I24*K24);
    }
};

class TransitionWipePositionCircle:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / (h_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        v_param = pow(2,24) / (v_active * std::max({1 - h_pos,h_pos,1 - v_pos,v_pos}));
        reg_h_pos = (int)(h_active * h_pos / 2 + 0.5);
        reg_v_pos = (int)(v_active * v_pos / 2 + 0.5);
        float I21,I22,J21,J22,K21,K22;
        I21 = symmetry<0.5?h_param/(ratio/(h_active/v_active)):h_param;
        I22 = symmetry>=0.5?v_param/((h_active/v_active)/ratio):v_param;
        J21 = std::max(h_active-reg_h_pos*2,(double)reg_h_pos*2)*I21;
        J22 = std::max(v_active-reg_v_pos*2,(double)reg_v_pos*2)*I22;
        K21 = pow(pow(J21,2)+pow(J22,2),0.5);
        K22 = pow(2,24)/K21;
        reg_h_param = (int)(I21*K22);
        reg_v_param = (int)(I22*K22);
    }
};

class TransitionWipePositionSquareTopLeft:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 0;
        reg_v_pos = 0;
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareTopRight:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 960;
        reg_v_pos = 0;
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareBottomRight:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 960;
        reg_v_pos = 540;
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareBottomLeft:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 0;
        reg_v_pos = 540;
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareTopCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24)/(h_active*std::max(1-h_pos,h_pos));
        v_param = pow(2,24) / v_active;
        reg_h_pos = (int)(h_active*h_pos/2+0.5);
        reg_v_pos = 0;
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareLeftCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24)/(v_active*std::max(1-v_pos,v_pos));
        reg_h_pos = 0;
        reg_v_pos = int(v_active*v_pos/2+0.5);
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareBottomCenter:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24)/(h_active*std::max(1-h_pos,h_pos));
        v_param = pow(2,24) / v_active;
        reg_h_pos = (int)(h_active*h_pos/2+0.5);
        reg_v_pos = 540;
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionSquareRightCentert:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24)/(v_active*std::max(1-v_pos,v_pos));
        reg_h_pos = 960;
        reg_v_pos = (int)(v_active * v_pos / 2 + 0.5);
        reg_h_param = (int)symmetry<0.5?(h_param/(ratio/(h_active/v_active))):h_param;
        reg_v_param = (int)symmetry>=0.2?v_param/((h_active/v_active)/ratio):v_param;
    }
};

class TransitionWipePositionBottomRightAngle:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 0;
        reg_v_pos = 0;
        float I25,I26,K25,K26,J25,J26;
        I25 = symmetry<0.5?h_param/(ratio/(h_active/v_active)):h_param;
        I26 = symmetry>=0.5?v_param/((h_active/v_active)/ratio):v_param;
        J25 = std::max(h_active-reg_h_pos*2,(double)reg_h_pos*2)*I25;
        J26 = std::max(v_active-reg_v_pos*2,(double)reg_v_pos*2)*I26;
        K25 = (J25 + J26) / 2;
        K26 = pow(2,24) / K25;
        reg_h_param = int(I25*K26);
        reg_v_param = int(I26*K26);
    }
};

class TransitionWipePositionBottomLeftAngle:public TransitionWipePositionClass{
public:
    using TransitionWipePositionClass::TransitionWipePositionClass;
    void checkout(){
        ratio = -symmetry * 100 * 11 / 450 + 3;
        h_param = pow(2,24) / h_active;
        v_param = pow(2,24) / v_active;
        reg_h_pos = 960;
        reg_v_pos = 0;
        float I27,I28,K27,K28,J27,J28;
        I27 = symmetry<0.5?h_param/(ratio/(h_active/v_active)):h_param;
        I28 = symmetry>=0.5?v_param/((h_active/v_active)/ratio):v_param;
        J27 = std::max(h_active-reg_h_pos*2,(double)reg_h_pos*2)*I27;
        J28 = std::max(v_active-reg_v_pos*2,(double)reg_v_pos*2)*I28;
        K27 = (J27 + J28) / 2;
        K28 = pow(2,24) / K27;
        reg_h_param = int(I27*K28);
        reg_v_param = int(I28*K28);
    }
};

#endif // OSEE_MATH_H
