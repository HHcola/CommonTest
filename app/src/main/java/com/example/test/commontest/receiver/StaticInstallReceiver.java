package com.example.test.commontest.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

public class StaticInstallReceiver extends BroadcastReceiver {
    private static final String TAG = InstallCompleteReceiver.class.getSimpleName();
    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO: This method is called when the BroadcastReceiver is receiving
        // an Intent broadcast.
        Log.d(TAG, "onReceiver StaticInstallReceiver " + intent.getAction());
        if (intent.getAction() == Intent.ACTION_PACKAGE_ADDED) {
            Uri data = intent.getData();
            String packageName;
            if (data != null) {
                packageName = data.getSchemeSpecificPart();
                Log.d(TAG, "StaticInstallReceiver  " + packageName);
            }

        }
    }
}
