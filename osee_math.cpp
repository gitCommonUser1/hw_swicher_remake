#include "osee_math.h"
#include <stdio.h>

int getAudioRegValue(double number)
{
#define Max 10.0
#define Min -75.0
    if(number > Max)
        number = Max;
    if(number < Min)
        number = Min;
    if(number != (int)number){
        if(!(number + 0.5 == (int)number || number - 0.5 == (int)number)){
            return -1;
        }
    }
    double fA = pow(10,number / 20) * 0x00020000;
    return fA < 0x8000?(int)(fA + 0.5):(int)(((fA - 0x8000)/16 + 0.5)+0x8000);
}

ColorYCbCr getColorYC(int Hue, int Saturation, int Brightness)
{
#define A12 0.2126
#define A13 -0.1172
#define A14 0.5114
#define B12 0.7152
#define B13 -0.3942
#define B14 -0.4645
#define C12 0.0722
#define C13 0.5114
#define C14 -0.0469

    float C = (Saturation / 100.0) * (Brightness / 100.0);
    float X = C * (1 - abs(fmod(Hue / 60.0,2) - 1));
    float M = (Brightness / 100.0) - C;
    float R_ = Hue < 60?C:Hue < 120?X:Hue < 240?0:Hue < 300?X:C;
    float G_ = Hue < 60?X:Hue < 180?C:Hue < 240?X:0;
    float B_ = Hue < 120?0:Hue < 180?X:Hue < 300?C:X;
    float R = (M + R_) * 255;
    float G = (M + G_) * 255;
    float B = (M + B_) * 255;
    float RLimited = R * 219 / 255 + 16;
    float GLimited = G * 219 / 255 + 16;
    float BLimited = B * 219 / 255 + 16;
    u_int8_t Y = RLimited * A12 + GLimited * B12 + BLimited * C12 + 0.5;
    u_int8_t Cb = RLimited * A13 + GLimited * B13 + BLimited * C13 + 0.5 + 128;
    u_int8_t Cr = RLimited * A14 + GLimited * B14 + BLimited * C14 + 0.5 + 128;

    ColorYCbCr ycbcr = {Y,Cr,Cb};
    return ycbcr;
}

int getMaskPositionV(float value)
{
    //=INT((value+9)/18*539+0.5)
    return ((value+9)/18*539+0.5);
}

int getMaskPositionH(float value)
{
    //=INT((value+16)/32*959+0.5)
    return ((value + 16) / 32 * 959 + 0.5);
}

int getMonitorRegValue(double number)
{
    return pow(10,number / 20) * pow(2,15) + 0.5;
}

int getFTBRateValue(float value)
{
#define output_frame_rate 60
    return 256 / (value * output_frame_rate) * 256 + 0.5;
}

int getWipeSoftness(float value)
{
    return (int)((128.0/127.0)/(value+1/127.0)*pow(2,8)+0.5);
}

int getKeyGain(float value)
{
#define C3 20.0
#define B3 1.07
    float p1 = value - C3;
    float p2 = pow(B3,p1)*256.0/219.0;
    int ret = (int)(std::min(p2*256.0,pow(2,16)-1)+0.5);
    return ret;
}

int getKeyClip(float value)
{
    return 255 - std::min((int)(value*256.0/100+0.5),255);
}


double dround(double x,int bit)//double round
{
    double y=1;
    for(int i=0;i<bit;i++)
        y *=10;
    if(x >= 0)
        return (int)(x*y+0.5)/y;
    return (int)(x*y-0.5)/y;

}

ChromaKeyData getChromaKeyData(int y, int cb, int cr, float bgclear, float blkgloss, float green_suppression_ratio, float background_clean, int key_edge,float background,float keyedge)
{
    float green_y = y;
    float green_cb = (cb - 128);
    float green_cr = (cr - 128);

    printf("green_y:%f\n",green_y);
    printf("green_cb:%f\n",green_cb);
    printf("green_cr:%f\n",green_cr);

#define y_par1  1.00
#define cb_par1  0.00
#define cr_par1  1.5748
#define y_par2  1.00
#define cb_par2  -0.187324272930649
#define cr_par2  -0.468124272930649
#define y_par3  1.00
#define cb_par3  1.8556
#define cr_par3  0.00

    int r = green_y * y_par1 + green_cb * cb_par1 + green_cr * cr_par1 + 0.5;
    int g = green_y * y_par2 + green_cb * cb_par2 + green_cr * cr_par2 + 0.5;
    int b = green_y * y_par3 + green_cb * cb_par3 + green_cr * cr_par3 + 0.5;

    printf("r:%d\n",r);
    printf("g:%d\n",g);
    printf("b:%d\n",b);

    float angle = atan(green_cr / green_cb) / 2 / M_PI * 360;
    float x = green_cb * cos(atan(green_cr / green_cb)) + green_cr * sin(atan(green_cr / green_cb));

    printf("angle:%f\n",angle);
    printf("x:%f\n",x);

    ChromaKeyData data;
    data.ck_ys = (int)(green_y + 0.5);
    data.k1 = dround(cos(atan(green_cr / green_cb)) * 256 / x * 512,0);
    data.k1 = data.k1 > 4095?4095:data.k1;
    data.k1 = data.k1 < -4095?-4095:data.k1;
    data.k2 = dround(sin(atan(green_cr / green_cb))* 256 / x * 512,0);
    data.k2 = data.k2 > 4095?4095:data.k2;
    data.k2 = data.k2 < -4095?-4095:data.k2;
    data.k3 = dround((1 - bgclear) * 512,0);
    data.k3 = data.k3 > 2047?2047:data.k3;
    data.k3 = data.k3 < -2047?-2047:data.k3;
    data.k4 = dround(bgclear / green_y * 8192,0);
    data.k4 = data.k4 > 511?511:data.k4;
    data.k4 = data.k4 < 0?0:data.k4;
    data.k5 = dround((1 - blkgloss) * 512,0);
    data.k5 = data.k5 > 2047?2047:data.k5;
    data.k5 = data.k5 < -2047?-2047:data.k5;
    data.k6 = dround(blkgloss / green_y * 8192,0);
    data.k6 = data.k6 > 511?511:data.k6;
    data.k6 = data.k6 < 0?0:data.k6;
    data.k7 = dround(1 / tan(key_edge / 2.0 / 360.0 * 2.0 * M_PI) * 256.0,0);
    data.k7 = data.k7 > 1023?1023:data.k7;
    data.k7 = data.k7 < 0?0:data.k7;
    data.k8 = dround(1 / (1 + green_suppression_ratio) * 512,0);
    data.k9 = 512 - data.k8;
    data.k10 = dround(r / 255.0 * background_clean * 256,0);
    data.k10 = data.k10 > 255?255:data.k10;
    data.k10 = data.k10 < 0?0:data.k10;

    float green_suppression = dround((data.k8 * r + data.k9 * b) / 512,0);
    printf("green_suppression:%f\n",green_suppression);

    data.k11 = dround(green_suppression / 255 * background_clean * 256,0);
    data.k11 = data.k11 > 255?255:data.k11;
    data.k11 = data.k11 < 0?0:data.k11;
    data.k12 = dround(b / 255.0 * background_clean * 256,0);
    data.k12 = data.k12 > 255?255:data.k12;
    data.k12 = data.k12 < 0?0:data.k12;

    data.gain = 256 * (background * 1.25 + 1) + 0.5;
    data.gain = data.gain > 1023?1023:data.gain;
    data.gain = data.gain < 0?0:data.gain;
    data.clip = (((keyedge - 0.5) * 0.6 + 1) * 128) * (background * 1.25 + 1) + 128 * ((background * 1.25 + 1) - 1) * 0.7;
    data.clip = data.clip > 1023?1023:data.clip;
    data.clip = data.clip < 0?0:data.clip;

    return data;
}

void ycbcr2Rgb(int Y, int Cb, int Cr, int *R, int *G, int *B)
{
    double r, g, b;

    // Convert YCbCr to RGB
    r = Y + 1.402 * (Cr - 128);
    g = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128);
    b = Y + 1.772 * (Cb - 128);

    // Clamp RGB values to [0, 255]
    *R = (int)round(fmax(0, fmin(255, r)));
    *G = (int)round(fmax(0, fmin(255, g)));
    *B = (int)round(fmax(0, fmin(255, b)));
}

int getFTBRateValue(float value, int outFormat)
{
    return 256 / (value * outFormat) * 256 + 0.5;
}
