/*
 * Copyright (C) 2015 The Android Open Source Project
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
 * limitations under the License
 */

package com.android.tv.settings.system.development;

import android.content.Context;
import android.debug.IAdbManager;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.leanback.app.GuidedStepSupportFragment;
import androidx.leanback.widget.GuidanceStylist;
import androidx.leanback.widget.GuidedAction;

import com.android.tv.settings.R;

import java.util.List;

@Keep
public class AdbKeysDialog extends GuidedStepSupportFragment {
    private static final String TAG = "AdbKeysDialog";

    @NonNull
    @Override
    public GuidanceStylist.Guidance onCreateGuidance(Bundle savedInstanceState) {
        return new GuidanceStylist.Guidance(
                getString(R.string.clear_adb_keys),
                getString(R.string.adb_keys_warning_message),
                null,
                null);
    }

    @Override
    public void onCreateActions(@NonNull List<GuidedAction> actions, Bundle savedInstanceState) {
        final Context context = getContext();
        actions.add(new GuidedAction.Builder(context)
                .clickAction(GuidedAction.ACTION_ID_OK).build());
        actions.add(new GuidedAction.Builder(context)
                .clickAction(GuidedAction.ACTION_ID_CANCEL).build());
    }

    @Override
    public void onGuidedActionClicked(GuidedAction action) {
        if (action.getId() == GuidedAction.ACTION_ID_OK) {
            try {
                IBinder b = ServiceManager.getService(Context.ADB_SERVICE);
                IAdbManager service = IAdbManager.Stub.asInterface(b);
                service.clearDebuggingKeys();
            } catch (RemoteException e) {
                Log.e(TAG, "Unable to clear adb keys", e);
            }
            getFragmentManager().popBackStack();
        } else {
            getFragmentManager().popBackStack();
        }
    }
}
