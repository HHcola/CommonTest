package com.example.test.commontest.webview;

import android.content.Context;
import android.view.View;
import android.webkit.WebView;

/**
 * Created by hewei05 on 15/11/12.
 */
public class WebviewManager {

    private Context mContext;
    private WebView mWebView;
    public WebviewManager(Context context, View rootView) {
        mContext = context;
        initWebView();
    }

    public void showWebView() {
        if (mWebView == null) {
            return;
        }
        mWebView.loadUrl("file:///android_asset/html/test.html");
    }

    private void initWebView() {
        mWebView.getSettings().setJavaScriptEnabled(true);
//        mWebView.addJavascriptInterface(new JSInterface(), "jsInterface");
    }

}
