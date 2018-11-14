package org.mozilla.gecko.authentication;

import android.os.AsyncTask;

import org.mozilla.gecko.BondV1Grpc;
import org.mozilla.gecko.Error;
import org.mozilla.gecko.ErrorCode;
import org.mozilla.gecko.RegisterDeviceRequest;
import org.mozilla.gecko.Response;
import org.mozilla.gecko.UserAuth;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.concurrent.TimeUnit;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;

/**
 * Copyright © Cliqz 2018
 */
public class TalkToServer extends AsyncTask<Void, Void, Response> {

    public interface ServerCallbacks {
        void onServerReplied(Response serverResponse, int whichCase);
    }

    private static final String LOGTAG = TalkToServer.class.getSimpleName();
    private static final String HOST = "ambassador.dev.k8s.eu-central-1.clyqz.com";
    private static final int PORT = 443;
    //different endpoints that can be called on the server
    static final int REGISTER_DEVICE = 1;
    static final int IS_DEVICE_ACTIVE = 2;
    static final int WAIT_FOR_ACTIVATION = 3;
    private ManagedChannel mChannel;
    private ServerCallbacks mServerCallbacks;
    private RegisterDeviceRequest mRegisterDeviceRequest;
    private int mWhichCase;

    TalkToServer(ServerCallbacks serverCallbacks, int whichCase, String emailId, String secretKey) {
        mServerCallbacks = serverCallbacks;
        mWhichCase = whichCase;
        final UserAuth userAuth = UserAuth.newBuilder().setUsername(emailId)
                .setPassword(secretKey).build();

        mRegisterDeviceRequest = RegisterDeviceRequest.newBuilder().setAuth(userAuth)
                .setDescription("mobile-android").build();
    }

    @Override
    protected Response doInBackground(Void... voids) {
        try {
            mChannel = ManagedChannelBuilder.forAddress(HOST, PORT).build();
            BondV1Grpc.BondV1BlockingStub stub = BondV1Grpc.newBlockingStub(mChannel);
            switch (mWhichCase) {
                case REGISTER_DEVICE:
                    return stub.registerDevice(mRegisterDeviceRequest);
                case IS_DEVICE_ACTIVE:
                case WAIT_FOR_ACTIVATION:
                    return stub.isDeviceActivated(mRegisterDeviceRequest.getAuth());
                default:
                    return stub.registerDevice(mRegisterDeviceRequest);
            }
        } catch (Exception e) {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            e.printStackTrace(pw);
            pw.flush();
            return Response.newBuilder().addError(Error.newBuilder().setCode(ErrorCode.UNDEFINED)).build();
        }
    }

    @Override
    protected void onPostExecute(Response result) {
        try {
            if (mChannel != null) {
                mChannel.shutdown().awaitTermination(1, TimeUnit.SECONDS);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        mServerCallbacks.onServerReplied(result, mWhichCase);
    }
}
