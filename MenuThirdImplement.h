#ifndef MENUTHIRDIMPLEMENT_H
#define MENUTHIRDIMPLEMENT_H

#include "MenuDataType.h"
#include "models.h"
#include "MenuEnums.h"
#include "profile_include.h"

extern Models *models;
extern Profile *profile;

class MenuThirdColorBackColor1Hue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::colorBackHue),COLOR_BACK_COLOR1,value.toInt());
        profile->colorBacks()->colorBack1()->setHue(value.toInt());
    }
};

class MenuThirdColorBackColor1Saturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::colorBackSaturation),COLOR_BACK_COLOR1,value.toInt());
        profile->colorBacks()->colorBack1()->setSaturation(value.toInt());
    }
};

class MenuThirdColorBackColor1Brightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::colorBackBrightness),COLOR_BACK_COLOR1,value.toInt());
        profile->colorBacks()->colorBack1()->setBrightness(value.toInt());
    }
};

class MenuThirdColorBackColor2Hue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::colorBackHue),COLOR_BACK_COLOR2,value.toInt());
        profile->colorBacks()->colorBack2()->setHue(value.toInt());
    }
};

class MenuThirdColorBackColor2Saturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::colorBackSaturation),COLOR_BACK_COLOR2,value.toInt());
        profile->colorBacks()->colorBack2()->setSaturation(value.toInt());
    }
};

class MenuThirdColorBackColor2Brightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::colorBackBrightness),COLOR_BACK_COLOR2,value.toInt());
        profile->colorBacks()->colorBack2()->setBrightness(value.toInt());
    }
};

class MenuThirdSuperSourceStyle:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceControlStyle),value.toInt());
    }
};

class MenuThirdSuperSourceEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceEnable),value.toInt());
    }
};

class MenuThirdSuperSourceMask1Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskEnable),SUPER_SOURCE_MASK1,value.toInt());
    }
};


class MenuThirdSuperSourceMask2Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskEnable),SUPER_SOURCE_MASK2,value.toInt());
    }
};

class MenuThirdSuperSourceSource1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceSource1),value.toInt());
    }
};

class MenuThirdSuperSourceSource2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceSource2),value.toInt());
    }
};

class MenuThirdSuperSourceBackground:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBackground),value.toInt());
    }
};

class MenuThirdSuperSourceYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceControlYPosition),value.toInt());
    }
};

class MenuThirdSuperSourceMask1HStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHStart),SUPER_SOURCE_MASK1,value.toInt());
    }
};


class MenuThirdSuperSourceMask1VStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVStart),SUPER_SOURCE_MASK1,value.toInt());
    }
};


class MenuThirdSuperSourceMask1HEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHEnd),SUPER_SOURCE_MASK1,value.toInt());
    }
};


class MenuThirdSuperSourceMask1VEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVEnd),SUPER_SOURCE_MASK1,value.toInt());
    }
};


class MenuThirdSuperSourceMask2HStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHStart),SUPER_SOURCE_MASK2,value.toInt());
    }
};

class MenuThirdSuperSourceMask2VStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVStart),SUPER_SOURCE_MASK2,value.toInt());
    }
};

class MenuThirdSuperSourceMask2HEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHEnd),SUPER_SOURCE_MASK2,value.toInt());
    }
};

class MenuThirdSuperSourceMask2VEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVEnd),SUPER_SOURCE_MASK2,value.toInt());
    }
};

class MenuThirdSuperSourceBoard1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderWidth),SUPER_SOURCE_BORDER1,value.toInt());
    }
};

class MenuThirdSuperSourceBoard2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderWidth),SUPER_SOURCE_BORDER2,value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor1Hue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorHue),SUPER_SOURCE_BORDER1,value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor1Saturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorSaturation),SUPER_SOURCE_BORDER1,value.toInt());
    }
};


class MenuThirdSuperSourceBoardColor1Brightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorBrightness),SUPER_SOURCE_BORDER1,value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor2Hue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorHue),SUPER_SOURCE_BORDER2,value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor2Saturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorSaturation),SUPER_SOURCE_BORDER2,value.toInt());
    }
};


class MenuThirdSuperSourceBoardColor2Brightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorBrightness),SUPER_SOURCE_BORDER2,value.toInt());
    }
};


class MenuThirdKeyType:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyType),value.toInt());
    }
};

class MenuThirdLumaKeySourceFill:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeySourceKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceKey),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};


class MenuThirdLumaKeyMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};


class MenuThirdLumaKeyMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};


class MenuThirdLumaKeyMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyShapedKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyShapedKey),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};


class MenuThirdLumaKeyClip:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyClip),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyGain:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyGain),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyInvert:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyInvert),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyResize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyResize),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};


class MenuThirdLumaKeySize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};

class MenuThirdLumaKeyYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_LUMA_KEY,value.toInt());
    }
};





class MenuThirdChromaKeySource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};


class MenuThirdChromaKeyMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};

class MenuThirdChromaKeyMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};


class MenuThirdChromaKeyMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};


class MenuThirdChromaKeyMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};


class MenuThirdChromaKeyMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};




class MenuThirdChromaKeyResize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyResize),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};


class MenuThirdChromaKeySize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};

class MenuThirdChromaKeyXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};

class MenuThirdChromaKeyYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_CHROMA_KEY,value.toInt());
    }
};


class MenuThirdChromaKeySMPXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdChromaKeySMPYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdChromaKeySample:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdChromaKeyForeground:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdChromaKeyBackground:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdChromaKeyKeyEdge:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};










class MenuThirdKeyPatternSourceFill:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};



class MenuThirdKeyPatternWipePattern:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipePattern),value.toInt());
    }
};


class MenuThirdKeyPatternWipeSize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeSize),value.toInt());
    }
};

class MenuThirdKeyPatternWipeXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeXPosition),value.toInt());
    }
};

class MenuThirdKeyPatternWipeYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeYPosition),value.toInt());
    }
};

class MenuThirdKeyPatternWipeSymmetry:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeSymmetry),value.toInt());
    }
};

class MenuThirdKeyPatternWipeSoftness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeSoftness),value.toInt());
    }
};



class MenuThirdKeyPatternMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};

class MenuThirdKeyPatternMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};


class MenuThirdKeyPatternMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};


class MenuThirdKeyPatternMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};


class MenuThirdKeyPatternMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};




class MenuThirdKeyPatternResize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyResize),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};


class MenuThirdKeyPatternSize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};

class MenuThirdKeyPatternXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};

class MenuThirdKeyPatternYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_KEY_PATTERN,value.toInt());
    }
};









class MenuThirdPipSource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_PIP,value.toInt());
    }
};

class MenuThirdPipSize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_PIP,value.toInt());
    }
};

class MenuThirdPipXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_PIP,value.toInt());
    }
};

class MenuThirdPipYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_PIP,value.toInt());
    }
};



class MenuThirdPipMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_PIP,value.toInt());
    }
};

class MenuThirdPipMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_PIP,value.toInt());
    }
};


class MenuThirdPipMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_PIP,value.toInt());
    }
};


class MenuThirdPipMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_PIP,value.toInt());
    }
};


class MenuThirdPipMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_PIP,value.toInt());
    }
};




class MenuThirdPipBorderEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderEnable),value.toInt());
    }
};

class MenuThirdPipBorderWidth:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderWidth),value.toInt());
    }
};

class MenuThirdPipBorderColorHue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderColorHue),value.toInt());
    }
};

class MenuThirdPipBorderColorSaturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderColorSaturation),value.toInt());
    }
};

class MenuThirdPipBorderColorBrightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderColorBrightness),value.toInt());
    }
};



class MenuThirdTransitionMixRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionRate),TRANSITION_MIX,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->mixParameters()->setRate(value.toDouble());
    }
};

class MenuThirdTransitionDipSource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionDipSource),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setInput(value.toInt());
    }
};

class MenuThirdTransitionDipRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionRate),TRANSITION_DIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setRate(value.toDouble());
    }
};

class MenuThirdTransitionWipePattern:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipePattern),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setPattern(value.toInt());
    }
};

class MenuThirdTransitionWipeRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionRate),TRANSITION_WIPE,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setRate(value.toDouble());
    }
};

class MenuThirdTransitionWipeXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeXPosition),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setXPosition(value.toDouble());
    }
};

class MenuThirdTransitionWipeYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeYPosition),value.toInt());
        qDebug() << value;
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setYPosition(value.toDouble());
    }
};

class MenuThirdTransitionWipeDirection:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeDirection),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setReverseDirection(value > 0);
    }
};

class MenuThirdTransitionWipeSymmetry:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeSymmetry),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setSymmetry(value.toInt());
    }
};

class MenuThirdTransitionWipeSoftness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeSoftness),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setSoftness(value.toInt());
    }
};

class MenuThirdTransitionWipeBorder:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeBorder),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setBorder(value.toInt());
    }
};

class MenuThirdTransitionWipeFillSource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionWipeFillSource),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setFillSource(value.toInt());
    }
};


class MenuThirdDSKSourceFill:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskSourceFill),value.toInt());
    }
};

class MenuThirdDSKSourceKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskSourceKey),value.toInt());
    }
};

class MenuThirdDSKMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskEnable),value.toInt());
    }
};

class MenuThirdDSKMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskHStart),value.toInt());
    }
};

class MenuThirdDSKMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskVStart),value.toInt());
    }
};

class MenuThirdDSKMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskHEnd),value.toInt());
    }
};

class MenuThirdDSKMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskVEnd),value.toInt());
    }
};

class MenuThirdDSKShapedKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskShapedKey),value.toInt());
    }
};

class MenuThirdDSKClip:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskClip),value.toInt());
    }
};

class MenuThirdDSKMaskGain:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskGain),value.toInt());
    }
};

class MenuThirdDSKInvert:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskInvert),value.toInt());
    }
};

class MenuThirdDSKRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskRate),value.toInt());
    }
};


class MenuThirdFTBRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::ftbRate),value.toInt());
    }
};


class MenuThirdFTBAfv:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::ftbAfv),value.toInt());
    }
};


class MenuThirdAudioMixerMic1Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_MIC1,value.toInt());
    }
};

class MenuThirdAudioMixerMic1Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_MIC1,value.toInt());
    }
};

class MenuThirdAudioMixerMic1Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_MIC1,value.toInt());
    }
};

class MenuThirdAudioMixerMic1Delay:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioDelay),AUDIO_MIXER_MIC1,value.toInt());
    }
};

class MenuThirdAudioMixerMic1Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_MIC1,value.toInt());
    }
};



class MenuThirdAudioMixerMic2Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_MIC2,value.toInt());
    }
};

class MenuThirdAudioMixerMic2Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_MIC2,value.toInt());
    }
};

class MenuThirdAudioMixerMic2Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_MIC2,value.toInt());
    }
};

class MenuThirdAudioMixerMic2Delay:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioDelay),AUDIO_MIXER_MIC2,value.toInt());
    }
};

class MenuThirdAudioMixerMic2Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_MIC2,value.toInt());
    }
};




class MenuThirdAudioMixerIn1Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN1,value.toInt());
    }
};

class MenuThirdAudioMixerIn1Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN1,value.toInt());
    }
};

class MenuThirdAudioMixerIn1Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN1,value.toInt());
    }
};

class MenuThirdAudioMixerIn1Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN1,value.toInt());
    }
};



class MenuThirdAudioMixerIn2Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN2,value.toInt());
    }
};

class MenuThirdAudioMixerIn2Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN2,value.toInt());
    }
};

class MenuThirdAudioMixerIn2Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN2,value.toInt());
    }
};

class MenuThirdAudioMixerIn2Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN2,value.toInt());
    }
};



class MenuThirdAudioMixerIn3Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN3,value.toInt());
    }
};

class MenuThirdAudioMixerIn3Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN3,value.toInt());
    }
};

class MenuThirdAudioMixerIn3Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN3,value.toInt());
    }
};

class MenuThirdAudioMixerIn3Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN3,value.toInt());
    }
};




class MenuThirdAudioMixerIn4Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN4,value.toInt());
    }
};

class MenuThirdAudioMixerIn4Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN4,value.toInt());
    }
};

class MenuThirdAudioMixerIn4Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN4,value.toInt());
    }
};

class MenuThirdAudioMixerIn4Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN4,value.toInt());
    }
};



class MenuThirdAudioMixerAuxFader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_AUX,value.toInt());
    }
};

class MenuThirdAudioMixerAuxBalance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_AUX,value.toInt());
    }
};

class MenuThirdAudioMixerAuxInput:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_AUX,value.toInt());
    }
};

class MenuThirdAudioMixerAuxEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_AUX,value.toInt());
    }
};




class MenuThirdAudioMixerPGMFader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_PGM,value.toInt());
    }
};

class MenuThirdAudioMixerMonitorLevel:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::monitorLevel),value.toInt());
    }
};

class MenuThirdAudioMixerMonitorSource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::monitorSource),value.toInt());
    }
};


class MenuThirdStillSelectionStill1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::stillSelection),STILL_SELECTION_STILL1,value.toInt());
    }
};

class MenuThirdStillSelectionStill2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::stillSelection),STILL_SELECTION_STILL2,value.toInt());
    }
};

class MenuThirdStillLocation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::stillLocation),value.toInt());
    }
};

class MenuThirdStillLoadPicture:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};



class MenuThirdMacroSleep:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdMacroImport:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdMacroExport:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};



class MenuThirdStream1Platform:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::platform),STREAM_STREAM1,value.toInt());
    }
};

class MenuThirdStream1Server:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamServer),STREAM_STREAM1,value.toInt());
    }
};


class MenuThirdStream1Key:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
////        models->macroInvoke((&Models::streamKey),MACRO_STREAM1,value.toInt());
    }
};


class MenuThirdStream1UploadKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdStream1Output:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamOutput),STREAM_STREAM1,value.toInt());
    }
};







class MenuThirdStream2Platform:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::platform),STREAM_STREAM2,value.toInt());
    }
};

class MenuThirdStream2Server:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamServer),STREAM_STREAM2,value.toInt());
    }
};


class MenuThirdStream2Key:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
////        models->macroInvoke((&Models::streamKey),MACRO_STREAM2,value.toInt());
    }
};


class MenuThirdStream2UploadKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdStream2Output:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamOutput),STREAM_STREAM2,value.toInt());
    }
};






class MenuThirdStream3Platform:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::platform),STREAM_STREAM3,value.toInt());
    }
};

class MenuThirdStream3Server:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamServer),STREAM_STREAM3,value.toInt());
    }
};


class MenuThirdStream3Key:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
////        models->macroInvoke((&Models::streamKey),MACRO_STREAM3,value.toInt());
    }
};


class MenuThirdStream3UploadKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdStream3Output:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamOutput),STREAM_STREAM3,value.toInt());
    }
};


class MenuThirdPlaybackMode:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::playbackMode),value.toInt());
    }
};


class MenuThirdSettingVersion:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingBuildInfo:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingDeviceId:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingSrcNamePGM:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNamePVW:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameIn1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameIn2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameIn3:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameIn4:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameAux:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameStill1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcNameStill2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMvMeterPGM:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMvMeterIn1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMvMeterIn2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMvMeterIn3:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMvMeterIn4:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMvMeterAux:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingMvLayout:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMarkerPVW:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMicInputMic1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingMicInputMic2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingRecordFileName:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcSelectionIn1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcSelectionIn2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingSrcSelectionIn3:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSrcSelectionIn4:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingSrcSelectionAux:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingOutFormat:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingOutFormatOut1ColorSpace:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingOutFormatOut2ColorSpace:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};


class MenuThirdSettingOutSourceHDMI1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingOutSourceHDMI2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingOutSourceUVC:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingQualityRecording:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingQualityStreaming:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingNetworkProtocol:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingNetworkIPAddress:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingNetworkSubnetMask:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingNetworkGateway:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingNetworkPrimaryDNS:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingNetworkSecondaryDNS:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingPanelButtonBrightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingLanguage:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->changeLanguage();
    }
};

class MenuThirdSettingFactoryReset:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};

class MenuThirdSettingSDFormat:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
};








#endif // MENUTHIRDIMPLEMENT_H
