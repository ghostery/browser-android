package org.mozilla.gecko.authentication;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.provider.Settings;
import android.support.graphics.drawable.VectorDrawableCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.ViewStub;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.mozilla.gecko.Error;
import org.mozilla.gecko.ErrorCode;
import org.mozilla.gecko.R;
import org.mozilla.gecko.Response;
import org.mozilla.gecko.Utils;
import org.mozilla.gecko.preferences.PreferenceManager;

import java.util.Timer;
import java.util.TimerTask;

import static android.view.View.GONE;
import static android.view.View.VISIBLE;

/**
 * Copyright © Cliqz 2018
 */
public class LoginHelper implements View.OnClickListener, TalkToServer.ServerCallbacks {

    private static final String LOGTAG = LoginHelper.class.getSimpleName();
    private static final long POLL_INTERVAL = 10 * 1000; //10 seconds

    private Activity mActivity;
    private ViewStub mLoginScreenStub;
    private ImageView mLoginIcon;
    private TextView mLoginTitle;
    private TextView mLoginDescription;
    private TextView mErrorMessageTextView;
    private EditText mLoginInputField;
    private Button mContinueButton;
    private Button mResendButton;
    private TextView mFooterTextView;

    private String mSecretKey;
    private PreferenceManager mPreferenceManager;
    private Timer mTimer;
    private String mEmailId;
    private LoginStatus mLoginState = LoginStatus.REGISTRATION;

    ;
    @SuppressLint("HardwareIds")
    public LoginHelper(AppCompatActivity activity) {
        mActivity = activity;
        mSecretKey = Settings.Secure.getString(mActivity.getContentResolver(),
                Settings.Secure.ANDROID_ID);
        mPreferenceManager = PreferenceManager.getInstance(activity.getBaseContext());
    }

    public void start() {
        if (!autoLogin()) {
            showLoginScreen();
        }
    }

    private boolean autoLogin() {
        mEmailId = mPreferenceManager.getEmailId();
        if (!mEmailId.isEmpty()) {
            checkDeviceActivated();
            return true;
        }
        return false;
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.login_continue_button) {
            switch (mLoginState) {
                case REGISTRATION:
                    mEmailId = mLoginInputField.getText().toString();
                    if (!Utils.validateEmail(mEmailId)) {
                        mErrorMessageTextView.setText(mActivity.getString(R.string
                                .error_invalid_email));
                    } else {
                        registerDevice();
                    }
                    break;
                case WELCOME:
                    hideLoginScreen();
            }
        } else if (view.getId() == R.id.login_resend_button) {
            resendActivation();
        }
    }

    @Override
    public void onServerReplied(Response serverResponse, TalkToServer.Cases whichCase) {
        switch (whichCase) {
            case IS_DEVICE_ACTIVATED:
                if (serverResponse.getErrorCount() > 0) {
                    if (serverResponse.getErrorList().get(0).getCode() == ErrorCode.NO_INTERNET_CONNECTION) {
                        showLoginScreen();
                        mLoginInputField.setText(mEmailId);
                        mErrorMessageTextView.setText(mActivity.getString(R.string.error_no_internet_connection));
                    } else {
                        Log.e(LOGTAG, "Device not activated yet");
                        initViews();
                        showActivationScreen();
                    }
                } else {
                    saveEmailId();
                }
                break;
            case REGISTER_DEVICE:
                if (serverResponse.getErrorCount() > 0) {
                    Log.e(LOGTAG, "Error registering device.");
                    final Error error = serverResponse.getErrorList().get(0);
                    Log.e(LOGTAG, error.getMsg());

                    if (error.getCode() == ErrorCode.DEVICE_EXISTS) {
                        showActivationScreen();
                    } else if (error.getCode() == ErrorCode.NO_INTERNET_CONNECTION) {
                        mErrorMessageTextView.setText(mActivity.getString(R.string
                                .error_no_internet_connection));
                    }
                } else {
                    saveEmailId();
                    showActivationScreen();
                }
                break;
            case RESEND_ACTIVATION:
                if (serverResponse.getErrorCount() > 0) {
                    Log.e(LOGTAG, "can't resend the activation again");
                    if (serverResponse.getErrorList().get(0).getCode() == ErrorCode
                            .NO_INTERNET_CONNECTION) {
                        mErrorMessageTextView.setText(mActivity.getString(R.string
                                .error_no_internet_connection));
                    } else {
                        mErrorMessageTextView.setText(mActivity.getString(R.string
                                .error_resend_activation));
                    }
                }
                break;
            case WAIT_FOR_ACTIVATION:
                if (serverResponse.getErrorCount() > 0) {
                    Log.e(LOGTAG, "device is still not active");
                } else {
                    mTimer.cancel();
                    saveEmailId();
                    showWelcomeScreen();
                }
                break;
        }
    }

    private void checkDeviceActivated() {
        new TalkToServer(this, TalkToServer.Cases.IS_DEVICE_ACTIVATED, mEmailId, mSecretKey).execute();
    }

    private void registerDevice() {
        new TalkToServer(this, TalkToServer.Cases.REGISTER_DEVICE, mEmailId, mSecretKey).execute();
    }

    private void resendActivation() {
        new TalkToServer(LoginHelper.this,
                TalkToServer.Cases.RESEND_ACTIVATION, mEmailId, mSecretKey)
                .execute();
    }

    private void pollServerForActivation() {
        final Handler handler = new Handler();
        mTimer = new Timer();
        final TimerTask doAsynchronousTask = new TimerTask() {
            @Override
            public void run() {
                handler.post(new Runnable() {
                    public void run() {
                        new TalkToServer(LoginHelper.this,
                                TalkToServer.Cases.WAIT_FOR_ACTIVATION, mEmailId, mSecretKey)
                                .execute();
                    }
                });
            }
        };
        //we should define a limit for how long we keep polling
        mTimer.schedule(doAsynchronousTask, 0, POLL_INTERVAL); //execute in every 10secs
    }

    private void initViews() {
        mLoginScreenStub = mActivity.findViewById(R.id.bond_login_screen_stub);
        mLoginScreenStub.setLayoutResource(R.layout.bond_login_screen);
        final LinearLayout loginScreenView = (LinearLayout) mLoginScreenStub.inflate();
        mLoginIcon = (ImageView) loginScreenView.findViewById(R.id.login_icon);
        mLoginTitle = (TextView) loginScreenView.findViewById(R.id.login_title);
        mLoginDescription = (TextView) loginScreenView.findViewById(R.id.login_description);
        mErrorMessageTextView = (TextView) loginScreenView.findViewById(R.id.login_error_message);
        mLoginInputField = loginScreenView.findViewById(R.id.login_input_field);
        mContinueButton = loginScreenView.findViewById(R.id.login_continue_button);
        mContinueButton.setOnClickListener(this);
        mResendButton = loginScreenView.findViewById(R.id.login_resend_button);
        mResendButton.setOnClickListener(this);
        mFooterTextView = (TextView) loginScreenView.findViewById(R.id.login_footer_text);
    }

    private void setViewsValues(LoginStatus loginState, int iconId, int titleId, int descriptionId,
                                int inputFieldHintId, int inputFieldVisibility, int resendVisibility
            , int footerTextId, int footerTextVisibility) {
        mLoginState = loginState;
        final Drawable drawable = VectorDrawableCompat.create(mActivity.getResources(), iconId, null);
        mLoginIcon.setImageDrawable(drawable);
        mLoginTitle.setText(mActivity.getString(titleId));
        mLoginDescription.setText(mActivity.getString(descriptionId));
        mLoginInputField.setVisibility(inputFieldVisibility);
        if (inputFieldVisibility == VISIBLE) {
            mLoginInputField.setHint(mActivity.getString(inputFieldHintId));
        }
        mResendButton.setVisibility(resendVisibility);
        mFooterTextView.setVisibility(footerTextVisibility);
    }

    private void showLoginScreen() {
        initViews();
        setViewsValues(LoginStatus.REGISTRATION, R.mipmap.lumen_logo, R.string.browserName, R.string
                .login_description, R.string.login_input_hint, VISIBLE, GONE, R.string
                .login_footer, VISIBLE);
    }

    private void showActivationScreen() {
        setViewsValues(LoginStatus.ACTIVATION, R.drawable.login_circle, R.string
                        .login_activation_title, R.string.login_activation_description, -1, GONE, VISIBLE,
                -1, GONE);
        pollServerForActivation();
    }

    private void showWelcomeScreen() {
        setViewsValues(LoginStatus.WELCOME, R.drawable.login_welcome, R.string.login_welcome_title,
                R.string.login_welcome_description, -1, GONE, GONE, -1, GONE);
    }

    private void hideLoginScreen() {
        mLoginScreenStub.setVisibility(View.GONE);
    }

    private void saveEmailId() {
        mPreferenceManager.setEmailId(mEmailId);
    }

    private enum LoginStatus {REGISTRATION, ACTIVATION, WELCOME}

}
