package com.example.test.commontest.jniDemo;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;

import com.example.test.commontest.R;

import java.io.File;

public class JniDemoActivity extends Activity {

    static {
        System.loadLibrary("myLib");
    }

    public static void  startJniDemoActivity(Context context) {
        Intent intent = new Intent(context, JniDemoActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jni_demo);

        Button button = (Button) findViewById(R.id.jni_test);
        button.setText(getStringFromJni());

        createFileTest();
    }

    private void createFileTest() {
        String fileName = Environment.getExternalStorageState() + File.separator + "jni.txt";
        createFile(fileName);
    }

    public native String getStringFromJni();

    public native String createFile(String fileName);

    private native void nativeStartTcpServer(String ip, int port, String message) throws Exception;

}
