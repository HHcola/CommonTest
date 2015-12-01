package com.example.test.commontest.download;

import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.URL;
import java.util.HashMap;

/**
 * Created by hewei05 on 15/11/3.
 * 采用socket进行下载
 */
public class SingleThread extends Thread{


    private static final String TAG = SingleThread.class.getSimpleName();
    private String mUrl = "";

    private String mFileName;

    private HashMap<String, String> map;
    private String response;
    private String StatusLine;
    private int ResponseCode;
    private String ResponseMessage;
    private long ContentLength;

    public SingleThread() {
        super();
        map = new HashMap<String, String>();
    }

    public void setDownloadUrl(String url) {
        mUrl = url;
    }

    public void setFileName(String fileName) {
        mFileName = fileName;
    }

    @Override
    public void run() {
        Socket socket;

        URL url;
        try {
            if (TextUtils.isEmpty(mUrl)) {
                Log.d(TAG, "mUrl is empty");
                return;
            }
            url = new URL(mUrl);
            Log.d(TAG, "host = " + url.getHost() + " port = " + url.getPort() + " default port = " + url.getDefaultPort());
            SocketAddress socketAddress = new InetSocketAddress(url.getHost(), url.getDefaultPort());
            socket = new Socket();

            // 设置超时
            socket.setSoTimeout(10 * 1000);

            // 建立连结
            socket.connect(socketAddress);
            Log.d(TAG, "connect success");
        } catch (Exception e) {
            e.printStackTrace();
            Log.d(TAG, "connect failed " + e.getMessage());
            return;
        }

        OutputStreamWriter outputStreamWriter = null;
        InputStream inputStream = null;
        if (socket != null && socket.isConnected()) {

            try {
                outputStreamWriter = new OutputStreamWriter(socket.getOutputStream(), "utf-8");
                sendRequest(outputStreamWriter, url);
            } catch (Exception e) {
                e.printStackTrace();
            }

            // 读取数据
            try {
                inputStream = socket.getInputStream();
                saveData(inputStream);
                inputStream.close();
            } catch (Exception e) {
                e.printStackTrace();
                Log.d(TAG, " socket get inputstream failed " + e.getMessage());
                return;
            }

        }

        try {
            if (outputStreamWriter != null) {
                outputStreamWriter.close();
            }

            if (inputStream != null) {
                inputStream.close();
            }

            if (socket != null) {
                Log.d(TAG, "socket close");
                socket.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }


    private void sendRequest(OutputStreamWriter outputStreamWriter, URL url) {
        if (outputStreamWriter == null) {
            Log.d(TAG, "sendRequest error outputStreamWriter null");
            return;
        }

        Log.d(TAG, "sendRequest");
        try {
            BufferedWriter bufferedWriter = new BufferedWriter(outputStreamWriter);
            String CRLF = "\r\n";
            // 请求行 Method + path + HTTP协议版本 CRLF
            bufferedWriter.write("GET " + url.getPath() + " HTTP/1.0" + CRLF);

            // 消息报头
            // accept : 指定媒体类型
            bufferedWriter.write("Accept: " + "*/*" + CRLF);
            // accept-charset: 请求端可以接受的字符集
            bufferedWriter.write("Accept-Charset: " + "*" + CRLF);

            // accept-encoding 限制服务端返回给客户端可以接受的编码格式
            bufferedWriter.write("Accept-Encoding: " + "*" + CRLF);

            // accept-range 限制服务端返回的请求范围，断点续传

            // Content-Encoding  实体主体采用的何种内容编码，从而服务端知道采用何种解码方式


            // Content-Length  实体发送给接受者的大小


            // 指明来请求资源的网络主机和端口号
            bufferedWriter.write("Host: " + url.getHost() + CRLF);
            bufferedWriter.write("Content-Type: application/octet-stream" + CRLF );


            bufferedWriter.write("\r\n");
            bufferedWriter.flush();

            // 请求正文


        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void saveData(InputStream inputStream) {
        if (inputStream == null) {
            Log.d(TAG, "saveData failed inputStream is null");
            return;
        }
        byte[] buffer = new byte[512];

        try {
            File file = new File(mFileName);
            if (!file.exists()) {
                file.delete();
                file.createNewFile();
            }

            getResponse(inputStream);

            FileOutputStream fileOutputStream = new FileOutputStream(mFileName);
            int readSize = 0;
            while ((readSize = inputStream.read(buffer)) != -1) {
                fileOutputStream.write(buffer, 0, readSize);
            }

            fileOutputStream.flush();
            fileOutputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private void getResponse(InputStream instream) throws IOException {
        // 获取HTTP响应头
        byte[] b = new byte[4096];
        int off, val, endcode = 0;
        for (off = 0; off < 4096 && endcode != 0x0d0a0d0a; off++) {
            if((val = instream.read()) == -1)
                break;
            b[off] = (byte) val;
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


}
