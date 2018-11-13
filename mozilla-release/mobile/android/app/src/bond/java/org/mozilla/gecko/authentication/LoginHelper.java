package org.mozilla.gecko.authentication;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.ViewStub;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;

import org.mozilla.gecko.R;
import org.mozilla.gecko.Response;
import org.mozilla.gecko.preferences.PreferenceManager;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Copyright © Cliqz 2018
 */
//TODO remove logs
public class LoginHelper implements View.OnClickListener, TalkToServer.ServerCallbacks {

    private static final String LOGTAG = LoginHelper.class.getSimpleName();
    private static final long POLL_INTERVAL = 10 * 1000; //10 seconds

    private Activity mActivity;
    private Button mContinueButton;
    private EditText mEmailInputField;
    private ViewStub mLoginScreenStub;
    private String mSecretKey;
    private PreferenceManager mPreferenceManager;
    private Timer mTimer;
    private ProgressBar mProgressBar;

    @SuppressLint("HardwareIds")
    public LoginHelper(AppCompatActivity activity) {
        mActivity = activity;
        mSecretKey = Settings.Secure.getString(mActivity.getContentResolver(),
                Settings.Secure.ANDROID_ID);
        mPreferenceManager = PreferenceManager.getInstance(activity.getBaseContext());
    }

    public void loginOrRegister() {
        final String emailId = mPreferenceManager.getEmailId();
        if (emailId.isEmpty()) {
            Log.d(LOGTAG, "no email found showing login screen");
            showLoginScreen();
        } else {
            //check if device is activated
            Log.d(LOGTAG, "email id found. checking if device is registered");
            checkDeviceRegistered(emailId);
        }
    }

    @Override
    public void onClick(View view) {
        final String emailId = mEmailInputField.getText().toString();
        if (emailId.isEmpty()) {
            //TODO check for valid email format
            Log.d(LOGTAG, "please enter email");
        } else {
            //register device
            Log.d(LOGTAG, "registering device");
            mPreferenceManager.setEmailId(emailId);
            registerDevice(emailId);
        }
    }

    @Override
    public void onServerReplied(Response serverResponse, int whichCase) {
        switch (whichCase) {
            case TalkToServer.IS_DEVICE_ACTIVE:
                if (serverResponse.getErrorCount() > 0) {
                    showLoginScreen();
                    Log.d(LOGTAG, "Device is not registered. Showing login screen");
                    Log.d(LOGTAG, serverResponse.getErrorList().toString());
                } else {
                    //login successful
                    Log.d(LOGTAG, "login successful");
                }
                break;
            case TalkToServer.REGISTER_DEVICE:
                if (serverResponse.getErrorCount() > 0) {
                    Log.d(LOGTAG, "error registering device");
                    Log.d(LOGTAG, serverResponse.getErrorList().toString());
                } else {
                    Log.d(LOGTAG, "starting polling server for activation");
                    pollServerForActivation();
                }
                break;
            case TalkToServer.WAIT_FOR_ACTIVATION:
                if (serverResponse.getErrorCount() > 0) {
                    Log.d(LOGTAG, "device is still not active");
                } else {
                    Log.d(LOGTAG, "registration successful. hiding login screen");
                    mTimer.cancel();
                    mProgressBar.setVisibility(View.GONE);
                    mLoginScreenStub.setVisibility(View.GONE);
                }
                break;
        }
    }

    private void checkDeviceRegistered(String emailId) {
        new TalkToServer(this, TalkToServer.IS_DEVICE_ACTIVE, emailId, mSecretKey).execute();
    }

    private void registerDevice(String emailId) {
        new TalkToServer(this, TalkToServer.REGISTER_DEVICE, emailId, mSecretKey).execute();
    }

    private void pollServerForActivation() {
        final String emailID = mPreferenceManager.getEmailId();
        final Handler handler = new Handler();
        mTimer = new Timer();
        final TimerTask doAsynchronousTask = new TimerTask() {
            @Override
            public void run() {
                handler.post(new Runnable() {
                    public void run() {
                        new TalkToServer(LoginHelper.this,
                                TalkToServer.WAIT_FOR_ACTIVATION, emailID, mSecretKey).execute();
                    }
                });
            }
        };
        mTimer.schedule(doAsynchronousTask, 0, POLL_INTERVAL); //execute in every 10secs
        mProgressBar.setVisibility(View.VISIBLE);
    }

    private void showLoginScreen() {
        mLoginScreenStub = mActivity.findViewById(R.id.bond_login_screen_stub);
        final LinearLayout loginScreenView = (LinearLayout) mLoginScreenStub.inflate();
        mContinueButton = loginScreenView.findViewById(R.id.bond_continue_button);
        mContinueButton.setOnClickListener(this);
        mEmailInputField = loginScreenView.findViewById(R.id.bond_email_input_field);
        mProgressBar = loginScreenView.findViewById(R.id.progress_bar);
    }
}
