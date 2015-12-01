package com.example.test.commontest;

import android.content.Intent;
import android.content.IntentFilter;
import android.os.Environment;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.example.test.commontest.download.SingleThread;
import com.example.test.commontest.jniDemo.JniDemoActivity;
import com.example.test.commontest.linkedqueue.ConcurrentLinkedQueueTest;
import com.example.test.commontest.receiver.InstallCompleteReceiver;
import com.example.test.commontest.receiver.StaticInstallReceiver;

import java.io.File;


public class MainActivity extends ActionBarActivity implements View.OnClickListener{

    private static final String TAG = MainActivity.class.getSimpleName();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
    }


    private void init() {
        findViewById(R.id.queue_test).setOnClickListener(this);
        findViewById(R.id.socket_test).setOnClickListener(this);
        findViewById(R.id.package_install_test).setOnClickListener(this);
        findViewById(R.id.jni_test).setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.socket_test) {
            socketTest();
        } else if (v.getId() == R.id.queue_test) {
            queueTest();
        } else if (v.getId() == R.id.package_install_test) {
            installBroadTest();
        } else if (v.getId() == R.id.jni_test) {
            JniDemoActivity.startJniDemoActivity(this);
        }
    }


    private void queueTest() {
//        BlockingQueueTest.testQueue();
        ConcurrentLinkedQueueTest.linkedQueueTest();
    }


    private void socketTest() {
        SingleThread singleThread = new SingleThread();
        File filePath = Environment.getExternalStoragePublicDirectory("socketTEST");
        if (!filePath.exists()) {
            filePath.mkdir();
        }
        singleThread.setFileName(filePath.getAbsolutePath() + File.separator + "socket.apk");

//        String url = "http://download.androidapp.baidu.com/public/uploads/store_1/9/e/1/9e1a44de05c164f02254cfc61af7043b.apk";
        String url = "http://img.phone.baidu.com/public/uploads/store_7/3/c/1/3c1abde2910f1233b5c8a3fded80d65b.png";
//        String url = "http://usp1.baidu.com/setting/mbr?p=com.adslib.mobovee&hp=com.enode.catchvideo&l=222c8a228379cafae9000589419cb7ae";
        singleThread.setDownloadUrl(url);
        singleThread.start();
    }


    private void installBroadTest() {
        Log.d(TAG, "installBroadTEST");
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(Intent.ACTION_PACKAGE_ADDED);
        intentFilter.addDataScheme("package");

//        InstallCompleteReceiver installCompleteReceiver = new InstallCompleteReceiver();
//        registerReceiver(installCompleteReceiver, intentFilter);

        StaticInstallReceiver staticInstallReceiver = new StaticInstallReceiver();
        registerReceiver(staticInstallReceiver, intentFilter);
    }

}
