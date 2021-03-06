/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: header file
 */


#ifndef _C_CPQCONTROL_H
#define _C_CPQCONTROL_H

#include "SSMAction.h"
#include "CDevicePollCheckThread.h"
#include "CPQdb.h"
#include "PQType.h"
#include "CPQColorData.h"
#include "CPQLog.h"
#include "CDynamicBackLight.h"
#include "CConfigFile.h"
#include "COverScandb.h"

#include "PqNotify.h"
#include "CHlgToneMapping.h"
#include "CDolbyVision.h"
#include "SysFs.h"

#define LDIM_PATH                 "/dev/aml_ldim"
#define VPP_DEV_PATH              "/dev/amvecm"
#define DI_DEV_PATH               "/dev/di0"
#define AFE_DEV_PATH              "/dev/tvafe0"

#define BOOTVIDEO_ENABLE_PROP     "persist.vendor.media.bootvideo"
#define BOOTVIDEO_EXIT_PROP       "service.bootvideo.exit"
#define STATIC_FRAME_ENABLE_PROP  "vendor.media.video.setvideoblackout"

#define FINAL_GAIN_REG_NUM        46

#define TVIN_IOC_MAGIC 'T'
#define TVIN_IOC_LOAD_REG           _IOW(TVIN_IOC_MAGIC, 0x20, struct am_regs_s)

// screem mode index value
#define  SCREEN_MODE_NORMAL           0
#define  SCREEN_MODE_FULL_STRETCH     1
#define  SCREEN_MODE_4_3              2
#define  SCREEN_MODE_16_9             3
#define  SCREEN_MODE_NONLINEAR        4
#define  SCREEN_MODE_NORMAL_NOSCALEUP 5
#define  SCREEN_MODE_4_3_IGNORE       6
#define  SCREEN_MODE_4_3_LETTER_BOX   7
#define  SCREEN_MODE_4_3_PAN_SCAN     8
#define  SCREEN_MODE_4_3_COMBINED     9
#define  SCREEN_MODE_16_9_IGNORE      10
#define  SCREEN_MODE_16_9_LETTER_BOX  11
#define  SCREEN_MODE_16_9_PAN_SCAN    12
#define  SCREEN_MODE__16_9_COMBINED   13
#define  SCREEN_MODE_WIDEOPTION_CUSTOM 14
#define  SCREEN_MODE_WIDEOPTION_AFD   15

//NR Param
#define NR_3D_YGAIN_ADDR         (0X371C)
#define NR_3D_CGAIN_ADDR         (0X2DCE)
#define NR_2D_GAIN_ADDR          (0X2DAE)
#define VPP_BLACKEXT_CTRL        (0x1D80)

//Sharpness CTI
#define VPP_CTI_YC_DELAY         (0X7)
#define VPP_DECODE_CTI           (0XB5)
#define VPP_CTI_SR0_GAIN         (0X322F)
#define VPP_CTI_SR1_GAIN         (0X32AF)

#define YC_DELAY_REG_MASK        (0XF)
#define DECODE_CTI_REG_MASK      (0XFFFF)
#define SR0_GAIN0_REG_MASK       (0XFF000000)
#define SR0_GAIN1_REG_MASK       (0X00FF0000)
#define SR0_GAIN2_REG_MASK       (0X0000FF00)
#define SR0_GAIN3_REG_MASK       (0X000000FF)
#define SR1_GAIN0_REG_MASK       (0XFF000000)
#define SR1_GAIN1_REG_MASK       (0X00FF0000)
#define SR1_GAIN2_REG_MASK       (0X0000FF00)
#define SR1_GAIN3_REG_MASK       (0X000000FF)

//Video Decode Luma
#define DECODE_BRI_ADDR          (0X157)
#define DECODE_CON_ADDR          (0X157)
#define DECODE_SAT_ADDR          (0XA)
#define DECODE_BRI_REG_MASK      (0X3FF01FF)
#define DECODE_CON_REG_MASK      (0X3FF01FF)
#define DECODE_SAT_REG_MASK      (0XFF)

//Sharpness Advanced
#define SHARPNESS_SD_GAIN                (0x3213)
#define SHARPNESS_SD_HP_DIAG_CORE        (0x320f)
#define SHARPNESS_SD_BP_DIAG_CORE        (0x3210)
#define SHARPNESS_SD_PKGAIN_VSLUMA       (0x327e)
#define SHARPNESS_HD_GAIN                (0x3293)
#define SHARPNESS_HD_HP_DIAG_CORE        (0x328f)
#define SHARPNESS_HD_BP_DIAG_CORE        (0x3290)
#define SHARPNESS_HD_PKGAIN_VSLUMA       (0x32fe)

//memc
#define PROP_CPQ_MEMC               "persist.vendor.sys.memc"
#define CPQ_MEMC_SYSFS              "/dev/frc"
#define MEMDEV_CONTRL               _IOW('F', 0x06, unsigned int)
#define FRC_IOC_SET_MEMC_LEVEL      _IOW('F', 0x07, unsigned int)
#define FRC_IOC_SET_MEMC_DEMO       _IOW('F', 0x08, unsigned int)

typedef enum db_name_e {
    DB_NAME_PQ = 0,
    DB_NAME_OVERSCAN,
    DB_NAME_MAX,
} db_name_t;

typedef enum rgb_ogo_type_e{
    TYPE_INVALID = -1,
    R_GAIN = 0,
    G_GAIN,
    B_GAIN,
    R_POST_OFFSET,
    G_POST_OFFSET,
    B_POST_OFFSET,
    RGB_TYPE_MAX,
} rgb_ogo_type_t;

typedef enum video_layer_color_e{
    VIDEO_LAYER_COLOR_BLACK   = 0,
    VIDEO_LAYER_COLOR_BLUE    = 1,
    VIDEO_LAYER_COLOR_MAX,
} video_layer_color_t;

class CPQControl: public CDevicePollCheckThread::IDevicePollCheckObserver,
                         public CDynamicBackLight::IDynamicBackLightObserver,
                         public SSMAction::ISSMActionObserver {
public:
    CPQControl();
    ~CPQControl();
    static CPQControl *GetInstance();
    void CPQControlInit(void);
    void CPQControlUnInit(void);
    virtual void onVframeSizeChange();
    virtual void onTXStatusChange();
    virtual void resetAllUserSettingParam();
    virtual void Set_Backlight(int value);
    virtual void GetDynamicBacklighConfig(int *thtf, int *lut_mode, int *heigh_param, int *low_param);
    virtual void GetDynamicBacklighParam(dynamic_backlight_Param_t *DynamicBacklightParam);
    int LoadPQSettings();
    int LoadCpqLdimRegs(void);
    int Cpq_LoadRegs(am_regs_t regs);
    int Cpq_LoadDisplayModeRegs(ve_pq_load_t regs);
    int DI_LoadRegs(am_pq_param_t di_regs );
    int Cpq_LoadBasicRegs(source_input_param_t source_input_param, vpp_picture_mode_t pqMode);
    int Cpq_SetDIModuleParam(source_input_param_t source_input_param);
    int ResetLastPQSettingsSourceType(void);
    int BacklightInit(void);
    //PQ mode
    int SetPQMode(int pq_mode, int is_save, int is_autoswitch);
    int GetPQMode(void);
    int GetLastPQMode(void);
    int SavePQMode(int pq_mode);
    int SaveLastPQMode(int pq_mode);
    int setPQModeByTvService(pq_status_update_e gameStatus, pq_status_update_e pcStatus, int autoSwitchMonitorModeFlag);
    int Cpq_SetPQMode(vpp_picture_mode_t pq_mode, source_input_param_t source_input_param, pq_mode_switch_type_t switch_type);
    int SetPQParams(source_input_param_t source_input_param, vpp_picture_mode_t pq_mode, vpp_pq_para_t pq_para);
    int GetPQParams(source_input_param_t source_input_param, vpp_picture_mode_t pq_mode, vpp_pq_para_t *pq_para);
    //color Temperature
    int SetColorTemperature(int temp_mode, int is_save, rgb_ogo_type_t rgb_ogo_type = TYPE_INVALID, int value = -1);
    int GetColorTemperature(void);
    int SaveColorTemperature(int temp_mode);
    tcon_rgb_ogo_t GetColorTemperatureUserParam(void);
    int Cpq_SetColorTemperatureWithoutSave(vpp_color_temperature_mode_t Tempmode, tv_source_input_t tv_source_input __unused);
    int Cpq_CheckColorTemperatureParamAlldata(source_input_param_t source_input_param);
    unsigned short Cpq_CalColorTemperatureParamsChecksum(void);
    int Cpq_SetColorTemperatureParamsChecksum(void);
    unsigned short Cpq_GetColorTemperatureParamsChecksum(void);
    int Cpq_SetColorTemperatureUser(tv_source_input_t source_input, rgb_ogo_type_t rgb_ogo_type, int is_save, int value);
    int Cpq_GetColorTemperatureUser(tv_source_input_t source_input, tcon_rgb_ogo_t *p_tcon_rgb_ogo);
    int Cpq_SaveColorTemperatureUser(tv_source_input_t source_input, rgb_ogo_type_t rgb_ogo_type, int value);
    int Cpq_RestoreColorTemperatureParamsFromDB(source_input_param_t source_input_param);
    int Cpq_CheckTemperatureDataLable(void);
    int Cpq_SetTemperatureDataLable(void);
    int SetColorTemperatureParams(vpp_color_temperature_mode_t Tempmode, tcon_rgb_ogo_t params);
    int GetColorTemperatureParams(vpp_color_temperature_mode_t Tempmode, tcon_rgb_ogo_t *params);
    int SaveColorTemperatureParams(vpp_color_temperature_mode_t Tempmode, tcon_rgb_ogo_t params);
    int Cpq_CheckColorTemperatureParams(void);
    //Brightness
    int SetBrightness(int value, int is_save);
    int GetBrightness(void);
    int SaveBrightness(int value);
    int Cpq_SetBrightnessBasicParam(source_input_param_t source_input_param);
    int Cpq_SetBrightness(int value, source_input_param_t source_input_param);
    int Cpq_SetVideoBrightness(int value);
    //Contrast
    int SetContrast(int value, int is_save);
    int GetContrast(void);
    int SaveContrast(int value);
    int Cpq_SetContrastBasicParam(source_input_param_t source_input_param);
    int Cpq_SetContrast(int value, source_input_param_t source_input_param);
    int Cpq_SetVideoContrast(int value);
    //Saturation
    int SetSaturation(int value, int is_save);
    int GetSaturation(void);
    int SaveSaturation(int value);
    int Cpq_SetSaturationBasicParam(source_input_param_t source_input_param);
    int Cpq_SetSaturation(int value, source_input_param_t source_input_param);
    //Hue
    int SetHue(int value, int is_save);
    int GetHue(void);
    int SaveHue(int value);
    int Cpq_SetHueBasicParam(source_input_param_t source_input_param);
    int Cpq_SetHue(int value, source_input_param_t source_input_param);
    int Cpq_SetVideoSaturationHue(int satVal, int hueVal);
    void video_set_saturation_hue(signed char saturation, signed char hue, signed long *mab);
    void video_get_saturation_hue(signed char *sat, signed char *hue, signed long *mab);
    //Sharpness
    int SetSharpness(int value, int is_enable, int is_save);
    int GetSharpness(void);
    int SaveSharpness(int value);
    int Cpq_SetSharpness(int value, source_input_param_t source_input_param);
    int Cpq_SetSharpness0FixedParam(source_input_param_t source_input_param);
    int Cpq_SetSharpness0VariableParam(source_input_param_t source_input_param);
    int Cpq_SetSharpness1FixedParam(source_input_param_t source_input_param);
    int Cpq_SetSharpness1VariableParam(source_input_param_t source_input_param);
    //NoiseReductionMode
    int SetNoiseReductionMode(int nr_mode, int is_save);
    int GetNoiseReductionMode(void);
    int SaveNoiseReductionMode(int nr_mode);
    int Cpq_SetNoiseReductionMode(vpp_noise_reduction_mode_t nr_mode, source_input_param_t source_input_param);
    //GammaValue
    int SetGammaValue(vpp_gamma_curve_t gamma_curve, int is_save);
    int GetGammaValue();
    //Memc
    bool hasMemcFunc();
    int SetMemcMode(int memc_mode, int is_save);
    int GetMemcMode(void);
    int SaveMemcMode(vpp_memc_mode_t memc_mode);
    int Cpq_SetMemcMode(vpp_memc_mode_t memc_mode, source_input_param_t source_input_param);
    int SetMemcDeBlurLevel(int level, int is_save);
    int GetMemcDeBlurLevel(void);
    int SaveMemcDeBlurLevel(int level);
    int Cpq_SetMemcDeBlurLevel(int level, source_input_param_t source_input_param);
    int SetMemcDeJudderLevel(int level, int is_save);
    int GetMemcDeJudderLevel(void);
    int SaveMemcDeJudderLevel(int level);
    int Cpq_SetMemcDeJudderLevel(int level, source_input_param_t source_input_param);

    //Displaymode
    int SetDisplayMode(vpp_display_mode_t display_mode, int is_save);
    int GetDisplayMode(void);
    int SaveDisplayMode(vpp_display_mode_t mode);
    int Cpq_SetDisplayModeAllTiming(tv_source_input_t source_input, vpp_display_mode_t display_mode);
    int Cpq_SetDisplayModeOneTiming(tv_source_input_t source_input, vpp_display_mode_t display_mode);
    int Cpq_SetDisplayModeCrop(tv_source_input_t source_input, vpp_display_mode_t display_mode);
    int Cpq_SetDisplayModeScreenMode(tv_source_input_t source_input, vpp_display_mode_t display_mode);
    int Cpq_SetVideoScreenMode(int value);
    int Cpq_GetScreenModeValue(vpp_display_mode_t display_mode);
    int Cpq_SetVideoCrop(int Voffset0, int Hoffset0, int Voffset1, int Hoffset1);
    int Cpq_SetNonLinearFactor(int value);
    //Backlight
    int SetBacklight(int value, int is_save);
    int GetBacklight(void);
    int SaveBacklight(int value);
    int Cpq_SetBackLight(int value);
    void Cpq_GetBacklight(int *value);
    int SetDynamicBacklight(Dynamic_backlight_status_t mode, int is_save);
    int GetDynamicBacklight(void);
    int GetVideoPlayStatus(void);
    //smooth plus
    int SetSmoothPlusMode(int smoothplus_mode, int is_save);
    int GetSmoothPlusMode(void);
    int SaveSmoothPlusMode(int smoothplus_mode);
    int Cpq_SetSmoothPlusMode(vpp_smooth_plus_mode_t smoothplus_mode, source_input_param_t source_input_param);
    //local contrast
    int SetLocalContrastMode(local_contrast_mode_t mode, int is_save);
    int GetLocalContrastMode(void);
    int SaveLocalContrastMode(local_contrast_mode_t mode);
    //BlackExtension
    int SetBlackExtensionMode(black_extension_mode_t mode, int is_save);
    int GetBlackExtensionMode(void);
    int SaveBlackExtensionMode(black_extension_mode_t mode);
    int SetBlackExtensionParam(source_input_param_t source_input_param);
    //DI deblock
    int SetDeblockMode(di_deblock_mode_t mode, int is_save);
    int GetDeblockMode(void);
    int SaveDeblockMode(di_deblock_mode_t mode);
    int Cpq_SetDeblockMode(di_deblock_mode_t deblock_mode, source_input_param_t source_input_param);
    //DI demosquito
    int SetDemoSquitoMode(di_demosquito_mode_t mode, int is_save);
    int GetDemoSquitoMode(void);
    int SaveDemoSquitoMode(di_demosquito_mode_t mode);
    int Cpq_SetDemoSquitoMode(di_demosquito_mode_t DeMosquito_mode, source_input_param_t source_input_param);
    //DI MCDI
    int SetMcDiMode(vpp_mcdi_mode_e mode, int is_save);
    int GetMcDiMode(void);
    int SaveMcDiMode(vpp_mcdi_mode_e mode);
    int Cpq_SetMcDiMode(vpp_mcdi_mode_e McDi_mode, source_input_param_t source_input_param);
    //static frame
    int SetStaticFrameEnable(int enable, int isSave);
    int GetStaticFrameEnable();
    //screen color
    int SetScreenColorForSignalChange(int screenColor, int isSave);
    int GetScreenColorForSignalChange();
    int setVideoScreenColor (int color);
    //Factory
    int FactoryResetPQMode(void);
    int FactoryResetColorTemp(void);
    int FactorySetPQMode_Brightness(source_input_param_t source_input_param, int pq_mode, int brightness );
    int FactoryGetPQMode_Brightness(source_input_param_t source_input_param, int pq_mode );
    int FactorySetPQMode_Contrast(source_input_param_t source_input_param, int pq_mode, int contrast );
    int FactoryGetPQMode_Contrast(source_input_param_t source_input_param, int pq_mode );
    int FactorySetPQMode_Saturation(source_input_param_t source_input_param, int pq_mode, int saturation );
    int FactoryGetPQMode_Saturation(source_input_param_t source_input_param, int pq_mode );
    int FactorySetPQMode_Hue(source_input_param_t source_input_param, int pq_mode, int hue );
    int FactoryGetPQMode_Hue(source_input_param_t source_input_param, int pq_mode );
    int FactorySetPQMode_Sharpness(source_input_param_t source_input_param, int pq_mode, int sharpness );
    int FactoryGetPQMode_Sharpness(source_input_param_t source_input_param, int pq_mode );
    int FactorySetColorTemp_Rgain ( int source_input, int colortemp_mode, int rgain );
    int FactorySaveColorTemp_Rgain ( int source_input, int colortemp_mode, int rgain );
    int FactoryGetColorTemp_Rgain ( int source_input, int colortemp_mode );
    int FactorySetColorTemp_Ggain ( int source_input, int colortemp_mode, int ggain );
    int FactorySaveColorTemp_Ggain ( int source_input, int colortemp_mode, int ggain );
    int FactoryGetColorTemp_Ggain ( int source_input, int colortemp_mode );
    int FactorySetColorTemp_Bgain ( int source_input, int colortemp_mode, int bgain );
    int FactorySaveColorTemp_Bgain ( int source_input, int colortemp_mode, int bgain );
    int FactoryGetColorTemp_Bgain ( int source_input, int colortemp_mode );
    int FactorySetColorTemp_Roffset ( int source_input, int colortemp_mode, int roffset );
    int FactorySaveColorTemp_Roffset ( int source_input, int colortemp_mode, int roffset );
    int FactoryGetColorTemp_Roffset ( int source_input, int colortemp_mode );
    int FactorySetColorTemp_Goffset ( int source_input, int colortemp_mode, int goffset );
    int FactorySaveColorTemp_Goffset ( int source_input, int colortemp_mode, int goffset );
    int FactoryGetColorTemp_Goffset ( int source_input, int colortemp_mode );
    int FactorySetColorTemp_Boffset ( int source_input, int colortemp_mode, int boffset );
    int FactorySaveColorTemp_Boffset ( int source_input, int colortemp_mode, int boffset );
    int FactoryGetColorTemp_Boffset ( int source_input, int colortemp_mode );
    int FactoryResetNonlinear(void);
    int FactorySetParamsDefault(void);
    int FactorySetNolineParams(source_input_param_t source_input_param, int type, noline_params_t noline_params);
    noline_params_t FactoryGetNolineParams(source_input_param_t source_input_param,          int type);
    int FactorySetHdrMode(int mode);
    int FactoryGetHdrMode(void);
    int FactorySetOverscanParam(source_input_param_t source_input_param, tvin_cutwin_t cutwin_t);
    tvin_cutwin_t FactoryGetOverscanParam(source_input_param_t source_input_param);
    int FactorySetGamma(int gamma_r_value, int gamma_g_value, int gamma_b_value);
    int FcatorySSMRestore(void);

    int SetColorDemoMode(vpp_color_demomode_t demomode);
    int SetColorBaseMode(vpp_color_basemode_t basemode, int isSave);
    vpp_color_basemode_t GetColorBaseMode(void);
    int SaveColorBaseMode(vpp_color_basemode_t basemode);
    int Cpq_SetColorBaseMode(vpp_color_basemode_t basemode, source_input_param_t source_input_param);
    int Cpq_SetRGBOGO(const struct tcon_rgb_ogo_s *rgbogo);
    int Cpq_GetRGBOGO(const struct tcon_rgb_ogo_s *rgbogo);
    int Cpq_LoadGamma(vpp_gamma_curve_t gamma_curve);
    int Cpq_SetGammaTbl_R(unsigned short red[256]);
    int Cpq_SetGammaTbl_G(unsigned short green[256]);
    int Cpq_SetGammaTbl_B(unsigned short blue[256]);
    int Cpq_SetGammaOnOff(int onoff);
    int Cpq_SetCABC(const db_cabc_param_t *pCABC);
    int SetCabc(void);
    int Cpq_SetAAD(const db_aad_param_t *pAAD);
    int SetAad(void);
    int SetDnlpMode(int level);
    int GetDnlpMode();
    int Cpq_SetVENewDNLP(const ve_dnlp_curve_param_t *pDNLP);
    int Cpq_SetDNLPStatus(ve_dnlp_state_t status);
    int FactorySetDNLPCurveParams(source_input_param_t source_input_param, int level, int final_gain);
    int FactoryGetDNLPCurveParams(source_input_param_t source_input_param, int level);
    int FactorySetBlackExtRegParams(source_input_param_t source_input_param, int val);
    int FactoryGetBlackExtRegParams(source_input_param_t source_input_param);
    int FactoryGetBEValFromDB(source_input_param_t source_input_param, int addr);
    int FactorySetBERegDBVal(source_input_param_t source_input_param, int addr, unsigned int reg_val);
    int FactorySetRGBCMYFcolorParams(source_input_param_t source_input_param, int color_type,int color_param,int val);
    int FactoryGetRGBCMYFcolorParams(source_input_param_t source_input_param, int color_type,int color_param);
    int FactorySetNoiseReductionParams(source_input_param_t source_input_param, vpp_noise_reduction_mode_t nr_mode, int addr, int val);
    int FactoryGetNoiseReductionParams(source_input_param_t source_input_param, vpp_noise_reduction_mode_t nr_mode, int addr);
    int FactorySetCTIParams(source_input_param_t source_input_param, int param_type, int val);
    int FactoryGetCTIParams(source_input_param_t source_input_param, int param_type);
    int SetCTIParamsCheckVal(int param_type, int val);
    int MatchCTIRegMask(int param_type);
    int MatchCTIRegAddr(int param_type);
    int FactorySetDecodeLumaParams(source_input_param_t source_input_param, int param_type, int val);
    int FactoryGetDecodeLumaParams(source_input_param_t source_input_param, int param_type);
    int SetDecodeLumaParamsCheckVal(int param_type, int val);
    int SetSharpnessParamsCheckVal(int param_type, int val);
    int MatchSharpnessRegAddr(int param_type, int isHd);
    int FactorySetSharpnessParams(source_input_param_t source_input_param, Sharpness_timing_e source_timing, int param_type, int val);
    int FactoryGetSharpnessParams(source_input_param_t source_input_param, Sharpness_timing_e source_timing, int param_type);
    int SetEyeProtectionMode(tv_source_input_t source_input, int enable, int is_save);
    int GetEyeProtectionMode(tv_source_input_t source_input);
    int Cpq_SSMReadNTypes(int id, int data_len, int offset);
    int Cpq_SSMWriteNTypes(int id, int data_len, int data_buf, int offset);
    int Cpq_GetSSMActualAddr(int id);
    int Cpq_GetSSMActualSize(int id);
    int Cpq_SSMRecovery(void);
    int Cpq_GetSSMStatus();
    int SetFlagByCfg(void);
    int SetPLLValues(source_input_param_t source_input_param);
    int SetCVD2Values(void);
    int SetCurrentSourceInputInfo(source_input_param_t source_input_param);
    source_input_param_t GetCurrentSourceInputInfo();
    int GetHistParam(ve_hist_t *hist);
    bool isFileExist(const char *file_name);

    int GetRGBPattern();
    int SetRGBPattern(int r, int g, int b);
    int FactorySetDDRSSC (int step);
    int FactoryGetDDRSSC(void);
    int FactorySetLVDSSSC (int step);
    int FactoryGetLVDSSSC(void);
    int SetLVDSSSC(int step);
    int SetGrayPattern(int value);
    int GetGrayPattern();
    //HDR
    int SetHDRMode(int mode);
    int GetHDRMode(void);
    int GetSourceHDRType(void);
    void GetChipVersionInfo(char* chip_version);
    tvpq_databaseinfo_t GetDBVersionInfo(db_name_t name);
    void setHdrInfoListener(const sp<PqNotify>& listener);
    int SetCurrentHdrInfo (int hdrInfo);
    int SetCurrentAspectRatioInfo(tvin_aspect_ratio_e aspectRatioInfo);
    int SetDtvKitSourceEnable(bool isEnable);
    //AI
    void AipqInit();
    int SetAipqEnable(bool isEnable);
    int GetAipqEnable(void);
    void enableAipq(bool isEnable);
    int AiParamLoad(void);
    //COLOR SPACE
    int SetColorGamutMode(vpp_colorgamut_mode_t value, int is_save);
    int GetColorGamutMode(void);
    int SaveColorGamutMode(vpp_colorgamut_mode_t value);
    int Cpq_SetColorGamutMode(vpp_colorgamut_mode_t value, source_input_param_t source_input_param);

    //HDR tone mapping
    int SetHDRTMData(int *reGain);
    //HDR TMO
    int Cpq_SetHDRTMOParams(const hdr_tmo_sw_s *phdrtmo);
    int SetHDRTMOMode(hdr_tmo_t mode, int is_save);
    int GetHDRTMOMode();
    int SaveHDRTMOMode(hdr_tmo_t mode);
    //PQ Diff
    char* CalculateFileSha1(const char* filePath);
    int GenerateTargetPQ();

private:
    int VPPOpenModule(void);
    int VPPCloseModule(void );
    int VPPDeviceIOCtl(int request, ...);
    int DIOpenModule(void);
    int DICloseModule(void);
    int DIDeviceIOCtl(int request, ...);
    int AFEDeviceIOCtl ( int request, ... );
    tvin_sig_fmt_t getVideoResolutionToFmt();
    int Cpq_SetXVYCCMode(vpp_xvycc_mode_t xvycc_mode, source_input_param_t source_input_param);
    int pqWriteSys(ConstCharforSysFsNodeIndex index, const char *val);
    int pqReadSys(ConstCharforSysFsNodeIndex index, char *buf, int count);
    void pqTransformStringToInt(const char *buf, int *val);
    unsigned int GetSharpnessRegVal(int addr);
    int Cpq_SetLocalContrastMode(local_contrast_mode_t mode);
    output_type_t CheckOutPutMode(tv_source_input_t source_input);
    hdr_type_t Cpq_GetSourceHDRType(tv_source_input_t source_input);
    bool isCVBSParamValid(void);
    bool isPqDatabaseMachChip();
    int Cpq_GetInputVideoFrameHeight(tv_source_input_t source_input);
    int Cpq_SetVadjEnableStatus(int isvadj1Enable, int isvadj2Enable);
    bool isBootvideoStopped();
    int SetVideoLayerColor(video_layer_color_t signalColor, video_layer_color_t nosignalColor);
    int setVideoScreenColor (int vdin_blending_mask, int y, int u, int v );
    int getSnowStatus();
    bool mInitialized;
    //cfg
    bool mbCpqCfg_seperate_db_enable;
    bool mbCpqCfg_amvecm_basic_enable;
    bool mbCpqCfg_amvecm_basic_withOSD_enable;
    bool mbCpqCfg_contrast_rgb_enable;
    bool mbCpqCfg_contrast_rgb_withOSD_enable;
    bool mbCpqCfg_blackextension_enable;
    bool mbCpqCfg_sharpness0_enable;
    bool mbCpqCfg_sharpness1_enable;
    bool mbCpqCfg_di_enable;
    bool mbCpqCfg_mcdi_enable;
    bool mbCpqCfg_deblock_enable;
    bool mbCpqCfg_nr_enable;
    bool mbCpqCfg_demoSquito_enable;
    bool mbCpqCfg_gamma_enable;
    bool mbCpqCfg_cm2_enable;
    bool mbCpqCfg_whitebalance_enable;
    bool mbCpqCfg_dnlp_enable;
    bool mbCpqCfg_xvycc_enable;
    bool mbCpqCfg_display_overscan_enable;
    bool mbCpqCfg_local_contrast_enable;
    bool mbCpqCfg_hdmi_out_with_fbc_enable;
    bool mbCpqCfg_pq_param_check_source_enable;
    bool mbCpqCfg_ai_enable;
    bool mbCpqCfg_aad_enable;
    bool mbCpqCfg_cabc_enable;
    bool mbCpqCfg_smoothplus_enable;
    bool mbCpqCfg_hdrtmo_enable;
    bool mbCpqCfg_memc_enable;

    CPQdb *mPQdb;
    COverScandb *mpOverScandb;
    SSMAction *mSSMAction;
    SysFs *mSysFs;
    static CPQControl *mInstance;
    CDevicePollCheckThread mCDevicePollCheckThread;
    CDynamicBackLight mDynamicBackLight;
    CConfigFile *mPQConfigFile;

    CHlgToneMapping *mHlgToneMapping;
    sp<PqNotify> mNotifyListener;
    CDolbyVision *mDolbyVision;

    int mAmvideoFd;
    int mDiFd;

    tcon_rgb_ogo_t rgbfrompq[3];
    source_input_param_t mCurentSourceInputInfo;
    tv_source_input_t mSourceInputForSaveParam;
    bool mCurrentHdrStatus;
    unsigned int mHdmiHdrInfo = 0;
    bool mbDtvKitEnable;
    bool mbDatabaseMatchChipStatus;
    mutable Mutex mLock;
    output_type_t mCurentOutputType;
    tvin_aspect_ratio_e mCurentAfdInfo;
    bool mbVideoIsPlaying = false;//video don't playing
    hdr_type_t mCurrentHdrType = HDR_TYPE_NONE;
    bool screenColorEnable = false;
};
#endif
