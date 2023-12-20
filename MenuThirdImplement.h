#ifndef MENUTHIRDIMPLEMENT_H
#define MENUTHIRDIMPLEMENT_H

#include "MenuDataType.h"
#include "models.h"
#include "MenuEnums.h"
#include "profile_include.h"
#include "rv_switch_api.h"
#include "messagedialogcontrol.h"

extern Models *models;
extern Profile *profile;
extern MessageDialogControl* messageDialogControl;

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

class MenuThirdSuperSourceEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceEnable),value.toInt());
        profile->superSources()->superSource()->setEnable(value > 0);
    }
};

class MenuThirdSuperSourceMask1Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskEnable),SUPER_SOURCE_MASK1,value.toInt());
        profile->superSources()->superSource()->mask1()->setEnable(value > 0);
    }
};


class MenuThirdSuperSourceMask2Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskEnable),SUPER_SOURCE_MASK2,value.toInt());
        profile->superSources()->superSource()->mask2()->setEnable(value > 0);
    }
};

class MenuThirdSuperSourceSource1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceSource1),value.toInt());
        profile->superSources()->superSource()->setSource1(value.toInt());
    }
};

class MenuThirdSuperSourceSource2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceSource2),value.toInt());
        profile->superSources()->superSource()->setSource2(value.toInt());
    }
};

class MenuThirdSuperSourceBackground:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBackground),value.toInt());
        profile->superSources()->superSource()->setBackground(value.toInt());
    }
};

class MenuThirdSuperSourceStyle:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceControlStyle),value.toInt());
        profile->superSources()->superSource()->setStyle(SuperSource::styleIndexToString(value.toInt()));
    }
};

class MenuThirdSuperSourceYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceControlYPosition),value.toInt());
        profile->superSources()->superSource()->setYPosition(value.toInt());
    }
};

class MenuThirdSuperSourceMask1HStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHStart),SUPER_SOURCE_MASK1,value.toInt());
        profile->superSources()->superSource()->mask1()->setHStart(value.toInt());
    }
};


class MenuThirdSuperSourceMask1VStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVStart),SUPER_SOURCE_MASK1,value.toInt());
        profile->superSources()->superSource()->mask1()->setVStart(value.toInt());
    }
};


class MenuThirdSuperSourceMask1HEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHEnd),SUPER_SOURCE_MASK1,value.toInt());
        profile->superSources()->superSource()->mask1()->setHEnd(value.toInt());
    }
};


class MenuThirdSuperSourceMask1VEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVEnd),SUPER_SOURCE_MASK1,value.toInt());
        profile->superSources()->superSource()->mask1()->setVEnd(value.toInt());
    }
};


class MenuThirdSuperSourceMask2HStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHStart),SUPER_SOURCE_MASK2,value.toInt());
        profile->superSources()->superSource()->mask2()->setHStart(value.toInt());
    }
};

class MenuThirdSuperSourceMask2VStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVStart),SUPER_SOURCE_MASK2,value.toInt());
        profile->superSources()->superSource()->mask2()->setVStart(value.toInt());
    }
};

class MenuThirdSuperSourceMask2HEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskHEnd),SUPER_SOURCE_MASK2,value.toInt());
        profile->superSources()->superSource()->mask2()->setHEnd(value.toInt());
    }
};

class MenuThirdSuperSourceMask2VEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceMaskVEnd),SUPER_SOURCE_MASK2,value.toInt());
        profile->superSources()->superSource()->mask2()->setVEnd(value.toInt());
    }
};

class MenuThirdSuperSourceBoard1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderWidth),SUPER_SOURCE_BORDER1,value.toInt());
        profile->superSources()->superSource()->border1()->setWidth(value.toInt());
    }
};

class MenuThirdSuperSourceBoard2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderWidth),SUPER_SOURCE_BORDER2,value.toInt());
        profile->superSources()->superSource()->border2()->setWidth(value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor1Hue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorHue),SUPER_SOURCE_BORDER1,value.toInt());
        profile->superSources()->superSource()->border1()->setColorHue(value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor1Saturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorSaturation),SUPER_SOURCE_BORDER1,value.toInt());
        profile->superSources()->superSource()->border1()->setColorSaturation(value.toInt());
    }
};


class MenuThirdSuperSourceBoardColor1Brightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorBrightness),SUPER_SOURCE_BORDER1,value.toInt());
        profile->superSources()->superSource()->border1()->setColorBrightness(value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor2Hue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorHue),SUPER_SOURCE_BORDER2,value.toInt());
        profile->superSources()->superSource()->border2()->setColorHue(value.toInt());
    }
};

class MenuThirdSuperSourceBoardColor2Saturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorSaturation),SUPER_SOURCE_BORDER2,value.toInt());
        profile->superSources()->superSource()->border2()->setColorSaturation(value.toInt());
    }
};


class MenuThirdSuperSourceBoardColor2Brightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::superSourceBorderColorBrightness),SUPER_SOURCE_BORDER2,value.toInt());
        profile->superSources()->superSource()->border2()->setColorBrightness(value.toInt());
    }
};


class MenuThirdKeyType:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyType),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->setType(Key::typeIndexToString(value.toInt()));
    }
};

class MenuThirdLumaKeySourceFill:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setFillSource(value.toInt());
    }
};

class MenuThirdLumaKeySourceKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceKey),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setKeySource(value.toInt());
    }
};

class MenuThirdLumaKeyMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskEnable(value > 0);
    }
};

class MenuThirdLumaKeyMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskHStart(value.toInt());
    }
};


class MenuThirdLumaKeyMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskVStart(value.toInt());
    }
};


class MenuThirdLumaKeyMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskHEnd(value.toInt());
    }
};


class MenuThirdLumaKeyMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskVEnd(value.toInt());
    }
};

class MenuThirdLumaKeyShapedKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyShapedKey),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setShapedKey(value > 0);
    }
};


class MenuThirdLumaKeyClip:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyClip),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setClip(value.toInt());
    }
};

class MenuThirdLumaKeyGain:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyGain),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setGain(value.toInt());
    }
};

class MenuThirdLumaKeyInvert:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyInvert),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setInvert(value > 0);
    }
};

class MenuThirdLumaKeyResize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyResize),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setResize(value > 0);
    }
};


class MenuThirdLumaKeySize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setSize(Keys::sizeIndexToString(value.toInt()));
    }
};

class MenuThirdLumaKeyXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setXPosition(value.toDouble());
    }
};

class MenuThirdLumaKeyYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_LUMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setYPosition(value.toDouble());
    }
};



class MenuThirdChromaKeySource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setFillSource(value.toInt());
    }
};


class MenuThirdChromaKeyMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskEnable(value > 0);
    }
};

class MenuThirdChromaKeyMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskHStart(value.toInt());
    }
};


class MenuThirdChromaKeyMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskVStart(value.toInt());
    }
};


class MenuThirdChromaKeyMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskHEnd(value.toInt());
    }
};


class MenuThirdChromaKeyMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskVEnd(value.toInt());
    }
};




class MenuThirdChromaKeyResize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyResize),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setResize(value > 0);
    }
};


class MenuThirdChromaKeySize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSize(Keys::sizeIndexToString(value.toInt()));
    }
};

class MenuThirdChromaKeyXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setXPosition(value.toDouble());
    }
};

class MenuThirdChromaKeyYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_CHROMA_KEY,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setYPosition(value.toDouble());
    }
};


class MenuThirdChromaKeySMPXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSmpXPosition(value.toDouble());
    }
};

class MenuThirdChromaKeySMPYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSmpYPosition(value.toDouble());
    }
};

class MenuThirdChromaKeySample:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSample(value > 0);
    }
};

class MenuThirdChromaKeyForeground:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setForeground(value.toInt());
    }
};

class MenuThirdChromaKeyBackground:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setBackground(value.toInt());
    }
};

class MenuThirdChromaKeyKeyEdge:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setKeyEdge(value.toInt());
    }
};




class MenuThirdKeyPatternSourceFill:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setFillSource(value.toInt());
    }
};



class MenuThirdKeyPatternWipePattern:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipePattern),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setPattern(value.toInt());
    }
};


class MenuThirdKeyPatternWipeSize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeSize),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeSize(value.toInt());
    }
};

class MenuThirdKeyPatternWipeXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeXPosition),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeXPosition(value.toDouble());
    }
};

class MenuThirdKeyPatternWipeYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeYPosition),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeYPosition(value.toDouble());
    }
};

class MenuThirdKeyPatternWipeSymmetry:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeSymmetry),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeSymmetry(value.toInt());
    }
};

class MenuThirdKeyPatternWipeSoftness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyPatternWipeSoftness),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeSoftness(value.toInt());
    }
};



class MenuThirdKeyPatternMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskEnable(value > 0);
    }
};

class MenuThirdKeyPatternMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskHStart(value.toInt());
    }
};


class MenuThirdKeyPatternMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskVStart(value.toInt());
    }
};


class MenuThirdKeyPatternMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskHEnd(value.toInt());
    }
};


class MenuThirdKeyPatternMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskVEnd(value.toInt());
    }
};




class MenuThirdKeyPatternResize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyResize),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setResize(value > 0);
    }
};


class MenuThirdKeyPatternSize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setSize(Keys::sizeIndexToString(value.toInt()));
    }
};

class MenuThirdKeyPatternXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setXPosition(value.toDouble());
    }
};

class MenuThirdKeyPatternYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_KEY_PATTERN,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setYPosition(value.toDouble());
    }
};



class MenuThirdPipSource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySourceFill),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setFillSource(value.toInt());
    }
};

class MenuThirdPipSize:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keySize),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setSize(Keys::sizeIndexToString(value.toInt()));
    }
};

class MenuThirdPipXPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyXPosition),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setXPosition(value.toDouble());
    }
};

class MenuThirdPipYPosition:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyYPosition),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setYPosition(value.toDouble());
    }
};



class MenuThirdPipMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskEnable),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskEnable(value > 0);
    }
};

class MenuThirdPipMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHStart),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskHStart(value.toInt());
    }
};


class MenuThirdPipMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVStart),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskVStart(value.toInt());
    }
};


class MenuThirdPipMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskHEnd),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskHEnd(value.toInt());
    }
};


class MenuThirdPipMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::keyMaskVEnd),MENU_FIRST_PIP,value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskVEnd(value.toInt());
    }
};




class MenuThirdPipBorderEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderEnable),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderEnable(value > 0);
    }
};

class MenuThirdPipBorderWidth:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderWidth),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderWidth(value.toInt());
    }
};

class MenuThirdPipBorderColorHue:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderColorHue),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderColorHue(value.toInt());
    }
};

class MenuThirdPipBorderColorSaturation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderColorSaturation),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderColorSaturation(value.toInt());
    }
};

class MenuThirdPipBorderColorBrightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::pipBorderColorBrightness),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderColorBrightness(value.toInt());
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

class MenuThirdTransitionDipStinger:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::transitionRate),TRANSITION_DIP,value.toInt());
//        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setRate(value.toDouble());
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setStinger(value > 0);
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
        profile->downstreamKeys()->downstreamKey()->setFillSource(value.toInt());
    }
};

class MenuThirdDSKSourceKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskSourceKey),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setKeySource(value.toInt());
    }
};

class MenuThirdDSKMaskEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskEnable),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setMaskEnable(value > 0);
    }
};

class MenuThirdDSKMaskHStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskHStart),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setMaskHStart(value.toInt());
    }
};

class MenuThirdDSKMaskVStart:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskVStart),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setMaskVStart(value.toInt());
    }
};

class MenuThirdDSKMaskHEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskHEnd),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setMaskHEnd(value.toInt());
    }
};

class MenuThirdDSKMaskVEnd:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskMaskVEnd),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setMaskVEnd(value.toInt());
    }
};

class MenuThirdDSKShapedKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskShapedKey),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setShapedKey(value > 0);
    }
};

class MenuThirdDSKClip:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskClip),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setClip(value.toInt());
    }
};

class MenuThirdDSKMaskGain:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskGain),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setGain(value.toInt());
    }
};

class MenuThirdDSKInvert:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskInvert),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setInvert(value > 0);
    }
};

class MenuThirdDSKRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::dskRate),value.toInt());
        profile->downstreamKeys()->downstreamKey()->setRate(value.toDouble());
    }
};


class MenuThirdFTBRate:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::ftbRate),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->ftb()->setRate(value.toDouble());
    }
};


class MenuThirdFTBAfv:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::ftbAfv),value.toInt());
        profile->mixEffectBlocks()->mixEffectBlock()->ftb()->setAfv(value > 0);
    }
};


class MenuThirdAudioMixerMic1Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_MIC1,value.toInt());
        profile->audioMixer()->audioInput()->mic1()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerMic1Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_MIC1,value.toInt());
        profile->audioMixer()->audioInput()->mic1()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerMic1Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_MIC1,value.toInt());
        profile->audioMixer()->audioInput()->mic1()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerMic1Delay:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioDelay),AUDIO_MIXER_MIC1,value.toInt());
        profile->audioMixer()->audioInput()->mic1()->setDelay(value.toInt());
    }
};

class MenuThirdAudioMixerMic1Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_MIC1,value.toInt());
        profile->audioMixer()->audioInput()->mic1()->setEnable(value.toInt());
    }
};



class MenuThirdAudioMixerMic2Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_MIC2,value.toInt());
        profile->audioMixer()->audioInput()->mic2()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerMic2Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_MIC2,value.toInt());
        profile->audioMixer()->audioInput()->mic2()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerMic2Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_MIC2,value.toInt());
        profile->audioMixer()->audioInput()->mic2()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerMic2Delay:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioDelay),AUDIO_MIXER_MIC2,value.toInt());
        profile->audioMixer()->audioInput()->mic2()->setDelay(value.toInt());
    }
};

class MenuThirdAudioMixerMic2Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_MIC2,value.toInt());
        profile->audioMixer()->audioInput()->mic2()->setEnable(value.toInt());
    }
};




class MenuThirdAudioMixerIn1Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN1,value.toInt());
        profile->audioMixer()->audioInput()->in1()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerIn1Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN1,value.toInt());
        profile->audioMixer()->audioInput()->in1()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerIn1Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN1,value.toInt());
        profile->audioMixer()->audioInput()->in1()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerIn1Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN1,value.toInt());
        profile->audioMixer()->audioInput()->in1()->setEnable(value.toInt());
    }
};



class MenuThirdAudioMixerIn2Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN2,value.toInt());
        profile->audioMixer()->audioInput()->in2()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerIn2Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN2,value.toInt());
        profile->audioMixer()->audioInput()->in2()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerIn2Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN2,value.toInt());
        profile->audioMixer()->audioInput()->in2()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerIn2Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN2,value.toInt());
        profile->audioMixer()->audioInput()->in2()->setEnable(value.toInt());
    }
};



class MenuThirdAudioMixerIn3Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN3,value.toInt());
        profile->audioMixer()->audioInput()->in3()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerIn3Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN3,value.toInt());
        profile->audioMixer()->audioInput()->in3()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerIn3Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN3,value.toInt());
        profile->audioMixer()->audioInput()->in3()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerIn3Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN3,value.toInt());
        profile->audioMixer()->audioInput()->in3()->setEnable(value.toInt());
    }
};




class MenuThirdAudioMixerIn4Fader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_IN4,value.toInt());
        profile->audioMixer()->audioInput()->in4()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerIn4Balance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_IN4,value.toInt());
        profile->audioMixer()->audioInput()->in4()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerIn4Input:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_IN4,value.toInt());
        profile->audioMixer()->audioInput()->in4()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerIn4Enable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_IN4,value.toInt());
        profile->audioMixer()->audioInput()->in4()->setEnable(value.toInt());
    }
};



class MenuThirdAudioMixerAuxFader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_AUX,value.toInt());
        profile->audioMixer()->audioInput()->aux()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerAuxBalance:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioBalance),AUDIO_MIXER_AUX,value.toInt());
        profile->audioMixer()->audioInput()->aux()->setBalance(value.toDouble());
    }
};

class MenuThirdAudioMixerAuxInput:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioInput),AUDIO_MIXER_AUX,value.toInt());
        profile->audioMixer()->audioInput()->aux()->setInput(value.toDouble());
    }
};

class MenuThirdAudioMixerAuxEnable:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioEnable),AUDIO_MIXER_AUX,value.toInt());
        profile->audioMixer()->audioInput()->aux()->setEnable(value.toInt());
    }
};




class MenuThirdAudioMixerPGMFader:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::audioFader),AUDIO_MIXER_PGM,value.toInt());
        profile->audioMixer()->audioOutput()->pgm()->setFader(value.toDouble());
    }
};

class MenuThirdAudioMixerMonitorLevel:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::monitorLevel),value.toInt());
        profile->audioMixer()->audioOutput()->monitor()->setLevel(value.toInt());
    }
};

class MenuThirdAudioMixerMonitorSource:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::monitorSource),value.toInt());
        profile->audioMixer()->audioOutput()->monitor()->setSource(value.toInt());
    }
};


class MenuThirdStillSelectionStill1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::stillSelection),STILL_SELECTION_STILL1,value.toInt());
        profile->stillGenerator()->stillSelection()->setStill1(value.toInt());
    }
};

class MenuThirdStillSelectionStill2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::stillSelection),STILL_SELECTION_STILL2,value.toInt());
        profile->stillGenerator()->stillSelection()->setStill2(value.toInt());
    }
};

class MenuThirdStillLocation:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::stillLocation),value.toInt());
        profile->stillGenerator()->stillSelection()->setLocation(value.toInt());
    }
};

class MenuThirdStillLoadPicture:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        models->stillLoadPictureIndexChanged(value.toInt());
    }
    void doEvent() override{
        models->loadStill1();
    }
};



class MenuThirdMacroSleep:public MenuThird{
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
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        profile->streams()->stream1()->setPlatfrom(settings->platformChangedText(Streams::STREAM1,value.toInt()));
    }
};

class MenuThirdStream1Server:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamServer),STREAM_STREAM1,value.toInt());
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        profile->streams()->stream1()->setServer(settings->serverChangedText(Streams::STREAM1,value.toInt()));
    }
};


class MenuThirdStream1Key:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
////        models->macroInvoke((&Models::streamKey),MACRO_STREAM1,value.toInt());
        profile->streams()->stream1()->setUrl(value.toString());
    }
    void doEvent() override{
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_KEY];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};


class MenuThirdStream1UploadKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        models->streamUploadKeyIndexChanged(STREAM_STREAM1,value.toInt());
    }
    void doEvent() override{
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        models->setLoadStreamKey(STREAM_STREAM1);
        profile->streams()->stream1()->setUrl(settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_KEY]->current.toString());
    }
};


class MenuThirdStream1Output:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamOutput),STREAM_STREAM1,value.toInt());
        profile->streams()->stream1()->setOutput(value > 0);
    }
};







class MenuThirdStream2Platform:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::platform),STREAM_STREAM2,value.toInt());
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        profile->streams()->stream2()->setPlatfrom(settings->platformChangedText(Streams::STREAM2,value.toInt()));
    }
};

class MenuThirdStream2Server:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamServer),STREAM_STREAM2,value.toInt());
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        profile->streams()->stream2()->setServer(settings->serverChangedText(Streams::STREAM2,value.toInt()));
    }
};


class MenuThirdStream2Key:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
////        models->macroInvoke((&Models::streamKey),MACRO_STREAM2,value.toInt());
        profile->streams()->stream2()->setUrl(value.toString());
    }
    void doEvent() override{
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_KEY];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};


class MenuThirdStream2UploadKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        models->streamUploadKeyIndexChanged(STREAM_STREAM2,value.toInt());
    }
    void doEvent() override{
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        models->setLoadStreamKey(STREAM_STREAM2);
        profile->streams()->stream2()->setUrl(settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_KEY]->current.toString());
    }
};


class MenuThirdStream2Output:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamOutput),STREAM_STREAM2,value.toInt());
        profile->streams()->stream2()->setOutput(value > 0);
    }
};






class MenuThirdStream3Platform:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::platform),STREAM_STREAM3,value.toInt());
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        profile->streams()->stream3()->setPlatfrom(settings->platformChangedText(Streams::STREAM3,value.toInt()));
    }
};

class MenuThirdStream3Server:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamServer),STREAM_STREAM3,value.toInt());
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        profile->streams()->stream3()->setServer(settings->serverChangedText(Streams::STREAM3,value.toInt()));
    }
};


class MenuThirdStream3Key:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
////        models->macroInvoke((&Models::streamKey),MACRO_STREAM3,value.toInt());
        profile->streams()->stream3()->setUrl(value.toString());
    }
    void doEvent() override{
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_KEY];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};


class MenuThirdStream3UploadKey:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        models->streamUploadKeyIndexChanged(STREAM_STREAM3,value.toInt());
    }
    void doEvent() override{
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        models->setLoadStreamKey(STREAM_STREAM3);
        profile->streams()->stream3()->setUrl(settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_KEY]->current.toString());
    }
};


class MenuThirdStream3Output:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::streamOutput),STREAM_STREAM3,value.toInt());
        profile->streams()->stream3()->setOutput(value > 0);
    }
};


class MenuThirdPlaybackList:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::playbackMode),value.toInt());
        profile->playback()->setPlaybackList(value.toInt());
    }
};

class MenuThirdPlaybackSequential:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::playbackMode),value.toInt());
        profile->playback()->setSequential(value > 0);
    }
};

class MenuThirdPlaybackProgressBar:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
//        models->macroInvoke((&Models::playbackMode),value.toInt());
        profile->playback()->setProgressBar(value > 0);
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
        profile->setting()->srcNames()->pgm()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_PGM];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNamePVW:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->pvw()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_PVW];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameIn1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->in1()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN1];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameIn2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->in2()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN2];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameIn3:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->in3()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN3];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameIn4:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->in4()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN4];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameAux:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->aux()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_AUX];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameStill1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->still1()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_STILL1];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcNameStill2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->srcNames()->still2()->setName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_SRC_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_STILL2];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingMvMeterPGM:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvMeters()->pgm()->setEnable(value > 0);
    }
};

class MenuThirdSettingMvMeterIn1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvMeters()->in1()->setEnable(value > 0);
    }
};

class MenuThirdSettingMvMeterIn2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvMeters()->in2()->setEnable(value > 0);
    }
};

class MenuThirdSettingMvMeterIn3:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvMeters()->in3()->setEnable(value > 0);
    }
};

class MenuThirdSettingMvMeterIn4:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvMeters()->in4()->setEnable(value > 0);
    }
};

class MenuThirdSettingMvMeterAux:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvMeters()->aux()->setEnable(value > 0);
    }
};


class MenuThirdSettingMvLayout:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->mvLayout()->setLayout(value.toInt());
    }
};

class MenuThirdSettingMarkerPVW:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->marker()->setPvwMarker(value > 0);
    }
};

class MenuThirdSettingMicInputMic1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->micInputs()->mic1()->setInput((value.toInt()));
    }
};

class MenuThirdSettingMicInputMic2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->micInputs()->mic2()->setInput((value.toInt()));
    }
};


class MenuThirdSettingRecordFileName:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->record()->setFileName(value.toString());
    }
    void doEvent() override{
        settings->setKeyboardReg(REG_FILE_NAME);
        settings->setKeyboardVisible(1);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RECORD_FILE]->third[SETTING_RECORD_FILE_NAME];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingSrcSelectionIn1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::INPUT_MAX)
            value = SrcSelections::INPUT_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN1,value.toInt());
    }
    void doEvent() override{
        profile->setting()->srcSelections()->in1()->setSelection(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN1]->current.toInt());
    }
};

class MenuThirdSettingSrcSelectionIn2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::INPUT_MAX)
            value = SrcSelections::INPUT_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN2,value.toInt());
    }
    void doEvent() override{
        profile->setting()->srcSelections()->in2()->setSelection(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN2]->current.toInt());
    }
};


class MenuThirdSettingSrcSelectionIn3:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::INPUT_MAX)
            value = SrcSelections::INPUT_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN3,value.toInt());
    }
    void doEvent() override{
        profile->setting()->srcSelections()->in3()->setSelection(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN3]->current.toInt());
    }
};

class MenuThirdSettingSrcSelectionIn4:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::INPUT_MAX)
            value = SrcSelections::INPUT_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN4,value.toInt());
    }
    void doEvent() override{
        profile->setting()->srcSelections()->in4()->setSelection(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN4]->current.toInt());
    }
};


class MenuThirdSettingSrcSelectionAux:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::AUX_MAX)
            value = SrcSelections::AUX_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_SOURCE,value.toInt());
    }
    void doEvent() override{
        profile->setting()->srcSelections()->aux()->setSelection(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_SOURCE]->current.toInt());
    }
};

class MenuThirdSettingOutFormat:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= OutFormat::OUT_1080pMAX)
            value = OutFormat::OUT_1080pMAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,value.toInt());
    }
    void doEvent() override{
        messageDialogControl->dialogShow(QObject::tr("This will restart the device."),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_OUT_FORMAT);
    }
};

class MenuThirdSettingOutFormatOut1ColorSpace:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::INPUT_MAX)
            value = SrcSelections::INPUT_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE,value.toInt());
    }
    void doEvent() override{
        profile->setting()->outFormat()->out1ColorSpace()->setColorSpace(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE]->current.toInt());
    }
};

class MenuThirdSettingOutFormatOut2ColorSpace:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= SrcSelections::INPUT_MAX)
            value = SrcSelections::INPUT_MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE,value.toInt());
    }
    void doEvent() override{
        profile->setting()->outFormat()->out2ColorSpace()->setColorSpace(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE]->current.toInt());
    }
};


class MenuThirdSettingOutSourceHDMI1:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= OutSources::MAX)
            value = OutSources::MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_HDMI1,value.toInt());
    }
    void doEvent() override{
        profile->setting()->outSources()->hdmi1()->setSource(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_HDMI1]->current.toInt());
    }
};

class MenuThirdSettingOutSourceHDMI2:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
    void doEvent() override{

    }
};

class MenuThirdSettingOutSourceUVC:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        if(value >= OutSources::MAX)
            value = OutSources::MAX - 1;
        if(value <= 0)
            value = 0;
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_AUX,value.toInt());
    }
    void doEvent() override{
        profile->setting()->outSources()->uvc()->setSource(
            settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_AUX]->current.toInt());
    }
};

class MenuThirdSettingQualityRecording:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->record()->setQuality(value.toInt());
    }
};

class MenuThirdSettingQualityStreaming:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->streams()->setQuality(value.toInt());
    }
};

class MenuThirdSettingNetworkProtocol:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->network()->setProtocol(value > 0);
    }
};

class MenuThirdSettingNetworkIPAddress:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->network()->iPAddress()->setValue(value.toString());
    }
    void doEvent() override{
        //dhcp
        if(profile->setting()->network()->protocol())
            return ;
        settings->setKeyboardVisible(1,KeyboardEvent::KEYBOARD_IPADDR);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_IP_ADDRESS];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingNetworkSubnetMask:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->network()->subnetMask()->setValue(value.toString());
    }
    void doEvent() override{
        //dhcp
        if(profile->setting()->network()->protocol())
            return ;
        settings->setKeyboardVisible(1,KeyboardEvent::KEYBOARD_IPADDR);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SUBNET_MASK];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingNetworkGateway:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->network()->gateway()->setValue(value.toString());
    }
    void doEvent() override{
        //dhcp
        if(profile->setting()->network()->protocol())
            return ;
        settings->setKeyboardVisible(1,KeyboardEvent::KEYBOARD_IPADDR);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_GATEWAY];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingNetworkPrimaryDNS:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->network()->primaryDNS()->setValue(value.toString());
    }
    void doEvent() override{
        //dhcp
        if(profile->setting()->network()->protocol())
            return ;
        settings->setKeyboardVisible(1,KeyboardEvent::KEYBOARD_IPADDR);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PRIMARY_DNS];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingNetworkSecondaryDNS:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->network()->secondaryDNS()->setValue(value.toString());
    }
    void doEvent() override{
        //dhcp
        if(profile->setting()->network()->protocol())
            return ;
        settings->setKeyboardVisible(1,KeyboardEvent::KEYBOARD_IPADDR);
        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SECONDAY_DNS];
        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
    }
};

class MenuThirdSettingPanelButtonBrightness:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->panel()->setButtonBrightness(value.toInt());
    }
};

class MenuThirdSettingLanguage:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){
        profile->setting()->language()->setLanguage(value.toInt());
    }
};

class MenuThirdSettingFactoryReset:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
    void doEvent() override{
        messageDialogControl->dialogShow(QObject::tr("This will restart the device."),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_FACTORY_RESET);
    }
};

class MenuThirdSettingSDFormat:public MenuThird{
public:
    using MenuThird::MenuThird;
    void doWork(QVariant value){

    }
    void doEvent() override{
        messageDialogControl->dialogShow(QObject::tr("Your SD card will be formatted."),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_SD_FORMAT);
    }
};








#endif // MENUTHIRDIMPLEMENT_H
