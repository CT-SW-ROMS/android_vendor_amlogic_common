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
 *  @author   tong.li
 *  @version  1.0
 *  @date     2018/03/16
 *  @par function description:
 *  - This is Audio Setting Manager, control surround sound state.
 */
package com.droidlogic.app;

import android.content.Context;
import android.content.ContentResolver;
import android.database.ContentObserver;
import android.hardware.hdmi.HdmiDeviceInfo;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Handler;
import android.provider.Settings;
import android.util.Log;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class AudioSettingManager {
    private static final String TAG                 = "AudioSettingManager";

    private ContentResolver mResolver;
    private OutputModeManager mOutputModeManager;
    private SettingsObserver mSettingsObserver;
    private SystemControlManager mSystemControlManager;
    private AudioManager mAudioManager;

    public static final String PROP_TUNER_AUDIO = "ro.vendor.platform.is.tv";

    public AudioSettingManager(Context context){
        mResolver = context.getContentResolver();
        mSettingsObserver = new SettingsObserver(new Handler());
        mOutputModeManager = OutputModeManager.getInstance(context);
        mSystemControlManager = SystemControlManager.getInstance();
        mAudioManager = (AudioManager) context.getSystemService(context.AUDIO_SERVICE);
    }

    public boolean needSyncDroidSetting(int surround) {
        int format = Settings.Global.getInt(mResolver, OutputModeManager.DIGITAL_AUDIO_FORMAT, -1);
        Log.i(TAG, "needSyncDroidSetting internal audio format:" + format + ", android format:" + surround);
        switch (surround) {
            case OutputModeManager.ENCODED_SURROUND_OUTPUT_AUTO:
                if (format == OutputModeManager.DIGITAL_AUDIO_FORMAT_AUTO ||
                    format == OutputModeManager.DIGITAL_AUDIO_FORMAT_PASSTHROUGH) {
                    return false;
                }
                break;
            case OutputModeManager.ENCODED_SURROUND_OUTPUT_NEVER:
                if (format == OutputModeManager.DIGITAL_AUDIO_FORMAT_PCM)
                    return false;
                break;
            case OutputModeManager.ENCODED_SURROUND_OUTPUT_ALWAYS:
            case OutputModeManager.ENCODED_SURROUND_OUTPUT_MANUAL:
                String subformat = Settings.Global.getString(mResolver,
                        OutputModeManager.DIGITAL_AUDIO_SUBFORMAT);
                String subsurround = getSurroundManualFormats();
                if (subsurround == null)
                    subsurround = "";
                if ((format == OutputModeManager.DIGITAL_AUDIO_FORMAT_MANUAL)
                        && subsurround.equals(subformat)) {
                    return false;
                }

                break;
            default:
                Log.d(TAG, "error surround format");
                break;
        }
        return true;
    }

    public String getSurroundManualFormats() {
        return Settings.Global.getString(mResolver,
                OutputModeManager.ENCODED_SURROUND_OUTPUT_ENABLED_FORMATS);
    }

    public void registerSurroundObserver() {
        String[] settings = new String[] {
                OutputModeManager.ENCODED_SURROUND_OUTPUT,
                OutputModeManager.ENCODED_SURROUND_OUTPUT_ENABLED_FORMATS,
        };
        for (String s : settings) {
            mResolver.registerContentObserver(Settings.Global.getUriFor(s),
                    false, mSettingsObserver);
        }
    }

    private class SettingsObserver extends ContentObserver {
        public SettingsObserver(Handler handler) {
            super(handler);
        }

        @Override
        public void onChange(boolean selfChange, Uri uri) {
            String option = uri.getLastPathSegment();
            final int esoValue = Settings.Global.getInt(mResolver,
                    OutputModeManager.ENCODED_SURROUND_OUTPUT,
                    OutputModeManager.ENCODED_SURROUND_OUTPUT_AUTO);

            if (!needSyncDroidSetting(esoValue)) {
                return;
            }
            if (OutputModeManager.ENCODED_SURROUND_OUTPUT.equals(option)) {
                switch (esoValue) {
                    case OutputModeManager.ENCODED_SURROUND_OUTPUT_AUTO:
                        mOutputModeManager.saveDigitalAudioFormatToHal(
                                OutputModeManager.DIGITAL_AUDIO_FORMAT_AUTO, "");
                        break;
                    case OutputModeManager.ENCODED_SURROUND_OUTPUT_NEVER:
                        mOutputModeManager.saveDigitalAudioFormatToHal(
                                OutputModeManager.DIGITAL_AUDIO_FORMAT_PCM, "");
                        break;
                    case OutputModeManager.ENCODED_SURROUND_OUTPUT_ALWAYS:
                    case OutputModeManager.ENCODED_SURROUND_OUTPUT_MANUAL:
                        mOutputModeManager.saveDigitalAudioFormatToHal(
                                OutputModeManager.DIGITAL_AUDIO_FORMAT_MANUAL, getSurroundManualFormats());
                        break;
                    default:
                        break;
                }
            } else if (OutputModeManager.ENCODED_SURROUND_OUTPUT_ENABLED_FORMATS.equals(option)) {
                mOutputModeManager.saveDigitalAudioFormatToHal(
                        OutputModeManager.DIGITAL_AUDIO_FORMAT_MANUAL, getSurroundManualFormats());
            }
        }
    }


    public void initSystemAudioSetting() {
        Log.d(TAG, "initParameterAfterBoot");

        setWiredDeviceConnectionState(SystemControlEvent.DEVICE_OUT_AUX_DIGITAL,
                mOutputModeManager.isHDMIPlugged() ? 1 : 0, "", "");
                /*setThisValue for dts scale*/
        mOutputModeManager.setDtsDrcScaleSysfs();

        initDigitalAudioFormat();
        if (!mSystemControlManager.getPropertyBoolean("ro.vendor.platform.has.mbxuimode", false)) {
            initDrcModePassthrough();
        }
        setARCLatency(getARCLatency());
        mOutputModeManager.initSoundParametersAfterBoot();
        initVadStatus();
    }

    private void initDigitalAudioFormat () {
        int audioFormat = mOutputModeManager.getDigitalAudioFormatOut();
        switch (audioFormat) {
            case OutputModeManager.DIGITAL_AUDIO_FORMAT_MANUAL:
                mOutputModeManager.setDigitalAudioFormatOut(OutputModeManager.DIGITAL_AUDIO_FORMAT_MANUAL, getAudioManualFormats());
                break;
        case OutputModeManager.DIGITAL_AUDIO_FORMAT_PCM:
        case OutputModeManager.DIGITAL_AUDIO_FORMAT_AUTO:
        default:
            mOutputModeManager.setDigitalAudioFormatOut(audioFormat);
            break;
        }
    }

    public String getAudioManualFormats() {
        String format = Settings.Global.getString(mResolver, OutputModeManager.DIGITAL_AUDIO_SUBFORMAT);
        if (format == null)
            return "";
        else
            return format;
    }

    public void initDrcModePassthrough() {
        final int value = Settings.Global.getInt(mResolver,
                OutputModeManager.DRC_MODE, isTunerAudio() ? OutputModeManager.IS_DRC_RF : OutputModeManager.IS_DRC_LINE);

        switch (value) {
        case OutputModeManager.IS_DRC_OFF:
            mOutputModeManager.enableDobly_DRC(false);
            mOutputModeManager.setDoblyMode(OutputModeManager.LINE_DRCMODE);
            setDrcModePassthroughSetting(OutputModeManager.IS_DRC_OFF);
            break;
        case OutputModeManager.IS_DRC_LINE:
            mOutputModeManager.enableDobly_DRC(true);
            mOutputModeManager.setDoblyMode(OutputModeManager.LINE_DRCMODE);
            setDrcModePassthroughSetting(OutputModeManager.IS_DRC_LINE);
            break;
        case OutputModeManager.IS_DRC_RF:
            mOutputModeManager.enableDobly_DRC(false);
            mOutputModeManager.setDoblyMode(OutputModeManager.RF_DRCMODE);
            setDrcModePassthroughSetting(OutputModeManager.IS_DRC_RF);
            break;
        default:
            return;
        }
    }

    public void setDrcModePassthroughSetting(int newVal) {
        Settings.Global.putInt(mResolver,
                OutputModeManager.DRC_MODE, newVal);
    }

    public void setARCLatency(int newVal) {
        Settings.Global.putInt(mResolver, OutputModeManager.TV_ARC_LATENCY, newVal);
        mOutputModeManager.setARCLatency(newVal);
    }

    public int getARCLatency() {
        return Settings.Global.getInt(mResolver, OutputModeManager.TV_ARC_LATENCY,
                OutputModeManager.TV_ARC_LATENCY_DEFAULT);
    }

    private void setWiredDeviceConnectionState(int type, int state, String address, String name) {
        try {
            Class<?> audioManager = Class.forName("android.media.AudioManager");
            Method setwireState = audioManager.getMethod("setWiredDeviceConnectionState",
                                    int.class, int.class, String.class, String.class);
            setwireState.invoke(mAudioManager, type, state, address, name);
        } catch(ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException ex) {
            ex.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }

    public boolean isTunerAudio() {
        return mSystemControlManager.getPropertyBoolean(PROP_TUNER_AUDIO, false);
    }

    private void initVadStatus() {
        final String VAD_ENABLE_UBOOTENV = "ubootenv.var.ffv_wake";
        final String VAD_ENABLE_PROPERTY = "persist.vendor.sys.vadwake";
        final String STRING_VAD_ON = "on";
        final String STRING_VAD_OFF = "off";

        SystemControlManager scm = SystemControlManager.getInstance();
        String mode = scm.getBootenv(VAD_ENABLE_UBOOTENV, STRING_VAD_OFF);
        String property = scm.getPropertyString(VAD_ENABLE_PROPERTY, STRING_VAD_OFF);

        if (!mode.equals(property)) {
            scm.setProperty(VAD_ENABLE_PROPERTY, mode);
        }
    }
}
