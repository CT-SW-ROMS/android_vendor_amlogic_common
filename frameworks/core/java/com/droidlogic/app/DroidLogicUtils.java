/*
 * Copyright (c) 2014 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 * Description: JAVA file
 */

package com.droidlogic.app;

import android.content.Context;
import android.net.Uri;
import android.os.SystemProperties;
import android.provider.Settings;
import android.util.Log;

import com.droidlogic.app.SystemControlManager;
import com.droidlogic.app.OutputModeManager;

public class DroidLogicUtils {
    public static final String TAG = "DroidLogicUtils";

    public static SystemControlManager getSystemControlManager() {
        return SystemControlManager.getInstance();
    }

    public static boolean isTv() {
        return getSystemControlManager().getPropertyBoolean("ro.vendor.platform.has.tvuimode", false);
    }

    public static boolean getAudioDebugEnable() {
        return SystemProperties.getBoolean("sys.droidlogic.audio.debug", false);
    }

    public static boolean isBuildLivetv() {
        return getSystemControlManager().getPropertyBoolean("ro.vendor.platform.build.livetv", false);
    }

    public static String audioFormatOutputToString(int value) {
        String temp = "["+value+"]";
        switch (value) {
            case OutputModeManager.DIGITAL_AUDIO_FORMAT_PCM:
                return temp + "FORMAT_PCM";
            case OutputModeManager.DIGITAL_AUDIO_FORMAT_AUTO:
                return temp + "FORMAT_AUTO";
            case OutputModeManager.DIGITAL_AUDIO_FORMAT_MANUAL:
                return temp + "FORMAT_MANUAL";
            case OutputModeManager.DIGITAL_AUDIO_FORMAT_PASSTHROUGH:
                return temp + "FORMAT_PASSTHROUGH";
            default:
                return temp + "INVALID_VALUE";
        }
    }
}
