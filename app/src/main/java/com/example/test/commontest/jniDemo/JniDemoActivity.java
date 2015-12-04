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
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.test.commontest.R;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class JniDemoActivity extends Activity implements View.OnClickListener{

    static {
        System.loadLibrary("myLib");
    }

    private String mFileName;
    private FileOutputStream mFileOutputStream;

    private HashMap<String, String> map;
    private String response;
    private String StatusLine;
    private int ResponseCode;
    private String ResponseMessage;
    private long ContentLength;


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

        findViewById(R.id.jni_socket_test).setOnClickListener(this);

        createFileTest();
        map = new HashMap<String, String>();
    }

    private void createFileTest() {
        String fileName = Environment.getExternalStorageState() + File.separator + "jni.txt";
        createFile(fileName);
    }


    private void setFileName() {
        File filePath = Environment.getExternalStoragePublicDirectory("socketTEST");
        if (!filePath.exists()) {
            filePath.mkdir();
        }
        mFileName = filePath.getAbsolutePath() + File.separator + "socket.apk";

        try {
            File file = new File(mFileName);
            if (!file.exists()) {
                file.delete();
                file.createNewFile();
            }

            mFileOutputStream = new FileOutputStream(file);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }


    private void downloadFile() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                downloadFileByThread();
            }
        }).start();
    }

    private void downloadFileByThread() {
        String downloadUrl = "http://img.phone.baidu.com/public/uploads/store_7/3/c/1/3c1abde2910f1233b5c8a3fded80d65b.png";

        try {
            URL url = new URL(downloadUrl);
//            String message = getRequestMsg(url);
            String message = "for test";
            List<String> msgList = getRequestListMsg(url);
            Log.d("JNI-DEMO", "message = " + message);
            String [] msg = msgList.toArray(new String[msgList.size()]);
            nativeStartTcpServer(url.getHost(), url.getPath(), 8080,message, msg);
            } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private List<String> getRequestListMsg(URL url) {
        List<String> listMsg = new ArrayList<>();
//        String CRLF = "\r\n";
        String CRLF = "";
        listMsg.add("GET " + url.getPath() + " HTTP/1.0" + CRLF);
        listMsg.add("Accept: " + "*/*" + CRLF);
        listMsg.add("Host: " + url.getHost() + CRLF);
        listMsg.add("Content-Type: application/octet-stream" + CRLF);
        listMsg.add(CRLF);

        return listMsg;
    }


    private String getRequestMsg(URL url) {
        try {
            StringBuffer bufferedWriter = new StringBuffer();

            String CRLF = "\r\n";
            // 请求行 Method + path + HTTP协议版本 CRLF
            bufferedWriter.append("GET " + url.getPath() + " HTTP/1.0" + CRLF);

            // 消息报头
            // accept : 指定媒体类型
            bufferedWriter.append("Accept: " + "*/*" + CRLF);
            // accept-charset: 请求端可以接受的字符集
            bufferedWriter.append("Accept-Charset: " + "*" + CRLF);

            // accept-encoding 限制服务端返回给客户端可以接受的编码格式
            bufferedWriter.append("Accept-Encoding: " + "*" + CRLF);

            // accept-range 限制服务端返回的请求范围，断点续传

            // Content-Encoding  实体主体采用的何种内容编码，从而服务端知道采用何种解码方式


            // Content-Length  实体发送给接受者的大小


            // 指明来请求资源的网络主机和端口号
            bufferedWriter.append("Host: " + url.getHost() + CRLF);
            bufferedWriter.append("Content-Type: application/octet-stream" + CRLF);

            bufferedWriter.append("\r\n");
            return bufferedWriter.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return "";
    }

    public native String getStringFromJni();

    public native String createFile(String fileName);

    private native void nativeStartTcpServer(String host, String path, int port, String strMsg, String[] message) throws Exception;

    public void callBackFromJni(byte[] msg, int length) {
        Log.d("JNI-DEMO", "callBackFromJni");
        saveData(mFileOutputStream, msg, length);
    }




    private void getResponse(byte[] msg) throws IOException {

        // 获取HTTP响应头
        byte[] b = new byte[4096];
        int off, val = 0;
        int endcode = 0;
        for (off = 0; off < 4096 && endcode != 0x0d0a0d0a; off++) {
//            if((val = instream.read()) == -1)
//                break;
            b[off] = msg[off];
            endcode <<= 8;
            endcode |= val;
            // System.out.printf("[%4d, 0x%08x] val = %d\n", off, int32, val);
        }
        //System.out.println("off = " + off);
        if (endcode != 0x0d0a0d0a) // 0x0d0a0d0a: "\r\n\r\n"
            throw new IOException("HTTP response header not found.");

        // 解析响应头
        response = new String(b, 0, off);
        String[] header = response.split("\r\n");
        if (header.length < 1)
            throw new IOException("Illegal response header.");

        StatusLine = header[0];
        parseStatusLine();

        String[] pair;
        for (String line : header) {
            pair = line.split(": ");
            if (pair.length == 2)
                map.put(pair[0], pair[1]);
        }

        try {
            ContentLength = Long.parseLong(map.get("Content-Length"));
        } catch (NumberFormatException e) {
            ContentLength = -1;
        }
    }



    // StatusLine = HTTP-Version SPACE Response-Code SPACE Reason-Phrase
    private void parseStatusLine() throws IOException {
        String[] s = StatusLine.split(" ");
        if(s.length < 3)
            throw new IOException("Illegal response status-line.");
        try {
            ResponseCode = Integer.parseInt(s[1]);
            ResponseMessage = s[2];
            for(int i = 3; i < s.length; i++)
                ResponseMessage += " " + s[i];
        } catch (NumberFormatException e) {
            ResponseCode = -1;
            throw new NumberFormatException("Illegal Response-Code: -1");
        }
    }



    private void saveData(FileOutputStream fileOutputStream, byte[] buffer, int length) {
        try {
            if (fileOutputStream == null) {
                return;
            }
            fileOutputStream.write(buffer, 0, length);
            fileOutputStream.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.jni_socket_test) {
            socketJni();
        }
    }

    private void socketJni() {
        setFileName();
        downloadFile();
    }
}
