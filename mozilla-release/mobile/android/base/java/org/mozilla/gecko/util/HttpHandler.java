package org.mozilla.gecko.util;

import android.support.annotation.Nullable;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Copyright © Cliqz 2018
 */
public class HttpHandler {

    private static final String HEADER_CONTENT_TYPE = "Content-Type";

    private static final String TAG = HttpHandler.class.getSimpleName();


    @Nullable
    public static String sendRequest(String method, URL url, String contentType, String content) {
        HttpURLConnection httpURLConnection = null;
        try {
            httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod(method);
            httpURLConnection.setUseCaches(method.equalsIgnoreCase("METHOD_GET"));
            httpURLConnection.setConnectTimeout(10000);
            httpURLConnection.setReadTimeout(10000);
            httpURLConnection.setRequestProperty(HEADER_CONTENT_TYPE, contentType);
            //TODO uncomment when decompression implemented in server
            if (content != null) {
                DataOutputStream dataOutputStream = new DataOutputStream(httpURLConnection.getOutputStream());
                dataOutputStream.writeBytes(content);
                dataOutputStream.close();
            }
            int responseCode = httpURLConnection.getResponseCode();
            if(responseCode == 200) {
                final InputStream is = new DataInputStream(httpURLConnection.getInputStream());
                final ByteArrayOutputStream baos = new ByteArrayOutputStream();
                final byte[] buffer = new byte[1024];
                int len;
                while ((len = is.read(buffer)) != -1) {
                    baos.write(buffer, 0, len);
                }
                is.close();
                return baos.toString();
            } else {
                throw new RuntimeException("Response code not 200");
            }
        } catch (IOException e) {
            Log.e(TAG, "Error fetching request" + url.toString(), e);
        } catch (RuntimeException e) {
            Log.e(TAG, "Error getting data from server" + url.toString(), e);
        } finally {
            if (httpURLConnection != null) {
                httpURLConnection.disconnect();
            }
        }
        return null;
    }
}
