package org.mozilla.gecko.cliqztelemetry;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.mozilla.gecko.AppConstants;
import org.mozilla.gecko.BuildConfig;
import org.mozilla.gecko.preferences.PreferenceManager;

import java.util.Locale;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Copyright © Cliqz 2019
 */
public class Telemetry {

    // This tag is only used to quick filter the telemetry stuff on the console using this command:
    // adb logcat '*:S TELEMETRY_DEBUG'
    private final static String TELEMETRY_TAG = "TELEMETRY_DEBUG";
    private final static String ALPHA_NUMERIC_SPACE =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    private final static String NUMERIC_SPACE = "0123456789";
    private final static int RANDOM_ALPHA_NUMERIC_KEY_LENGTH = 18;
    private final static int RANDOM_NUMERIC_KEY_LENGTH = 6;
    private static final long ENVIRONMENT_SIGNAL_TIME_LIMIT = 60 * 60 * 1000; //one hour
    private static final int BATCH_SIZE = 50;
    private JSONArray mSignalCache = new JSONArray();

    private final ExecutorService executorService = Executors.newSingleThreadExecutor();

    private Context context;

    @SuppressLint("StaticFieldLeak")
    private static Telemetry instance;
    private PreferenceManager mPreferenceManager;

    private Telemetry(Context context) {
        this.context = context;
        mPreferenceManager = PreferenceManager.getInstance(context);
    }

    public static void init(Context context) {
        if (instance == null) {
            //This class never keeps a reference to local context. Only application context.
            instance = new Telemetry(context.getApplicationContext());
            instance.sendEnvironmentSignal();
        }
    }

    public static Telemetry getInstance() {
        return instance;
    }

    private void logError(String type) {
        Log.e(TELEMETRY_TAG, "Error sending telemetry for " + type);
    }

    private void sendEnvironmentSignal() {
        final long timeSinceLastSignal = getUnixTimeStamp() - mPreferenceManager.getTimeOfLastEnvSignal();
        if (timeSinceLastSignal < ENVIRONMENT_SIGNAL_TIME_LIMIT) {
            return;
        }
        try {
            final JSONObject signal = new JSONObject();
            signal.put(TelemetryKeys.TYPE, TelemetryKeys.ENVIRONMENT);
            signal.put(TelemetryKeys.DEVICE, Build.MODEL);
            signal.put(TelemetryKeys.LANGUAGE, getLanguage());
            signal.put(TelemetryKeys.VERSION, AppConstants.CLIQZ_PRIVACY_VERSION);
            signal.put(TelemetryKeys.VERSION_DIST, BuildConfig.VERSION_NAME);
            signal.put(TelemetryKeys.VERSION_OS, Build.VERSION.SDK_INT);
            mPreferenceManager.setTimeOfLastEnvSignal(getUnixTimeStamp());
            saveSignal(signal, false);
        } catch (JSONException e) {
            logError(TelemetryKeys.ENVIRONMENT);
        }
    }

    public void sendHomeCustomizationSignal(String action, String target) {
        try {
            final JSONObject signal = new JSONObject();
            signal.put(TelemetryKeys.TYPE, TelemetryKeys.ONBOARDING);
            signal.put(TelemetryKeys.ACTION, action);
            signal.put(TelemetryKeys.TARGET, target);
            signal.put(TelemetryKeys.CONTEXT, TelemetryKeys.HOME_CUSTOMIZATION);
            signal.put(TelemetryKeys.VIEW, TelemetryKeys.HOME);
            saveSignal(signal, false);
        } catch (JSONException e) {
            logError(TelemetryKeys.HOME_CUSTOMIZATION);
        }
    }

    private synchronized void saveSignal(JSONObject signal, boolean forcePush) {
        addIdentifiers(signal);
        mSignalCache.put(signal);
        if (BuildConfig.DEBUG) {
            try {
                signal.put("cindex", mSignalCache.length());
            } catch (JSONException e) {
                // NOP
            }
            Log.v(TELEMETRY_TAG, signal.toString());
        }
        final boolean shouldSend = (forcePush || mSignalCache.length() > BATCH_SIZE);
        if (shouldSend) {
            sendCachedSignals();
        }
    }

    private synchronized void sendCachedSignals() {
        final JSONArray cache = mSignalCache;
        if(BuildConfig.DEBUG){
            final JSONObject msg = new JSONObject();
            try {
                msg.put("csize", mSignalCache.length());
                Log.v(TELEMETRY_TAG, msg.toString());
            } catch (JSONException e) {
                // NOP
            }
        }
        mSignalCache = new JSONArray();
        final TelemetrySender sender = new TelemetrySender(cache, context);
        executorService.execute(sender);
    }

    /**
     * Posts the telemetry signal sent by the extension, to the logger
     * @param signal Telemetry signal by the extension
     */
    public void saveExtSignal(JSONObject signal) {
        if(signal.length() != 0) {
            saveSignal(signal, false);
        }
    }

    //adds session id. timestamp, sequence number to the signals
    private void addIdentifiers(JSONObject signal) {
        int telemetrySequence = mPreferenceManager.getAutoIncrementSequenceNumber();
        try {
            final String sessionId = mPreferenceManager.getSessionId();
            if (sessionId.isEmpty()) {
                mPreferenceManager.setSessionId(generateSessionID());
            }
            signal.put(TelemetryKeys.SESSION, mPreferenceManager.getSessionId());
            signal.put(TelemetryKeys.TIME_STAMP, getUnixTimeStamp());
            signal.put(TelemetryKeys.TELEMETRY_SEQUENCE, telemetrySequence);
        } catch (JSONException e) {
            Log.e(TELEMETRY_TAG, "Error");
        }
    }

    /**
     * Generates a SessionID as per the CLIQZ standard
     * @see <a href="https://github.com/cliqz/navigation-extension/wiki/Logging#session-id-format</a>
     * @return A newly generated SessionID
     */
    private static String generateSessionID() {
        final String randomAlphaNumericString = generateRandomString(RANDOM_ALPHA_NUMERIC_KEY_LENGTH,
                ALPHA_NUMERIC_SPACE);
        final String randomNumericString = generateRandomString(RANDOM_NUMERIC_KEY_LENGTH, NUMERIC_SPACE);
        final String days = Long.toString(getUnixTimeStamp() / 86400000);
        return randomAlphaNumericString + randomNumericString + "|" + days + "|"
                + AppConstants.CLIQZ_CHANNEL;
    }

    //Returns a random string of length 'length' using characters from the given 'space'
    private static String generateRandomString(int length, String space) {
        final StringBuilder builder = new StringBuilder(length);
        for(int i=0; i < length; i++ ) {
            builder.append(space.charAt((int)Math.floor(Math.random() * space.length())));
        }
        return builder.toString();
    }

    //Returns the current time in milliseconds since January 1, 1970 midningt UTC.
    //This returns the time in UTC regardless of the timezone of the system
    private static long getUnixTimeStamp() {
        return (long)Math.floor(System.currentTimeMillis());
    }

    //returns current language of the device
    private String getLanguage() {
        String language = Locale.getDefault().getLanguage()+"-"+Locale.getDefault().getCountry();
        language = language.replaceAll("_","-");
        return language;
    }
}