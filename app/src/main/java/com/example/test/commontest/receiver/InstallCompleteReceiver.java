package com.example.test.commontest.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;


/**
 * Created by hewei05 on 15/11/10.
 */
public class InstallCompleteReceiver extends BroadcastReceiver {

    private static final String TAG = InstallCompleteReceiver.class.getSimpleName();
    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "onReceiver InstallCompleteReceiver " + intent.getAction());
        if (intent.getAction() == Intent.ACTION_PACKAGE_ADDED) {
            Uri data = intent.getData();
            String packageName;
            if (data != null) {
                packageName = data.getSchemeSpecificPart();
                Log.d(TAG, "InstallCompleteReceiver  " + packageName);
            }

        }
    }
}
