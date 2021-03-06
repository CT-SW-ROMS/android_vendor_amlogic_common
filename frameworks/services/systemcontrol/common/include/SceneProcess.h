/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *  @author   kaifu.hu
 *  @version  1.0
 *  @date     2020/11/06
 *  @par function description:
 *  scene logic process
 */

#ifndef ANDROID_SCENEPROCESS_H
#define ANDROID_SCENEPROCESS_H

#include "common.h"
#include "SysWrite.h"

typedef enum {
    SCENE_STATE_INIT               = 0,//boot
    SCENE_STATE_POWER              = 1,//hot plug or suspend/resume
    SCENE_STATE_SWITCH             = 2,//user switch the mode
    SCENE_STATE_SWITCH_ADAPTER     = 3,//video auto switch the mode
    SCENE_STATE_RESERVE            = 4,
    SCENE_STATE_ADAPTER_END        = 5 //end hint video auto switch the mode
}scene_state;

typedef enum {
    SINK_TYPE_NONE                 = 0, //hdmi plug out,use cvbs
    SINK_TYPE_SINK                 = 1,
    SINK_TYPE_REPEATER             = 2,
    SINK_TYPE_RESERVE              = 3
}sink_type;

typedef struct dv_input_info {
    char ubootenv_dv_type[MODE_LEN];  //the env of dolby vision type
    char dv_cap[MAX_STR_LEN];         //tv dolby vision cap
    char dv_displaymode[MODE_LEN];    //tv dolby vision max resolution
    char dv_deepcolor[MODE_LEN];      //tv dolby vision type
}dv_input_info_t;

typedef struct hdmi_input_info {
    bool isSupport4K30Hz; // soc is support 4k30 or not, false:not support true:support
    bool isSupport4K;     // soc is support 4k or not, false:not support true:support
    bool isDeepColor;     // deepcolor feature enable or not, false:disable true:enable
    bool isframeratepriority;//frame priority feature enable or not, false:disable true:enable
    bool isLowPowerMode;//low power feature enable or not, false:disable true:enable
    int  sinkType; //sink type,0:not hdmi sink 1:hdmi sink,2:repeater sink
    char edidParsing[MODE_LEN];//edid parse ok or not, ok:parse ok,ng:parse ng
    char dc_cap[MAX_STR_LEN];  //device colorspace cap
    char disp_cap[MAX_STR_LEN]; //device resolution cap
    char ubootenv_cvbsmode[MODE_LEN]; //the env of cvbsmode
    char ubootenv_colorattribute[MODE_LEN];//the env of colospace for UI change colorspace or boot
}hdmi_input_info_t;

typedef struct dv_output_info {
    char dv_final_displaymode[MODE_LEN];
    char dv_final_deepcolor[MODE_LEN];
}dv_output_info_t;

typedef struct scene_input_info {
    scene_state state; //scene state
    bool isbestpolicy; //hdmi best policy,false:disable true:enable
    bool isDvEnable;   //dolby vision enable or not,false:disable true:enable
    bool isTvSupportHDR;//tv is support HDR or not, false:not support true:support
    bool isTvSupportDv;//tv is support dolby vision or not, false:not support true:support
    char cur_displaymode[MODE_LEN]; // hdmi current output mode
    dv_input_info_t dv_input_info;
    hdmi_input_info_t hdmi_input_info;
}scene_input_info_t;

typedef struct scene_output_info {
    char final_displaymode[MODE_LEN];  //final resolution
    char final_deepcolor[MODE_LEN];    //final colorspace
    int  dv_type;                      //final dolby vision type
}scene_output_info_t;

class SceneProcess
{
public:
    SceneProcess();
    ~SceneProcess();

    void UpdateSceneInputInfo(scene_input_info_t * input_info);
    void DolbyVisionSceneProcess(scene_output_info_t* output_info);
    void SDRSceneProcess(scene_output_info_t* output_info);
    void Process(scene_input_info_t* input_info, scene_output_info_t* output_info);

private:
    int updateDolbyVisionType(void);
    void updateDolbyVisionAttr(int dolbyvision_type, char * dv_attr);
    void updateDolbyVisionDisplayMode(char * cur_outputmode, int dv_type, char * final_displaymode);
    bool IsBestPolicy();
    bool IsFrameratePriority();
    bool IsSupport4K();
    bool IsSupport4K30Hz();
    bool IsSupportDeepColor();
    bool isLowPowerMode();
    int64_t resolveResolutionValue(const char *mode);
    bool isModeSupportDeepColorAttr(const char *mode, const char * color);
    bool initColorAttribute(char* supportedColorList, int len);
    void getBestHdmiDeepColorAttr(const char *outputmode, char* colorAttribute);
    void getHdmiColorAttribute(const char* outputmode, char* colorAttribute, int state);
    void updateHdmiDeepColor(scene_state state, const char* outputmode, char* colorAttribute);
    void getHighestHdmiMode(char* mode);
    void filterHdmiMode(char* mode);
    void getHdmiOutputMode(char* mode);

    scene_input_info_t    mScene_Input_Info;
    scene_output_info_t   mScene_output_info;

    SysWrite*             mpSysWrite  = NULL;

    pthread_mutex_t       mSceneLock;
};
#endif
