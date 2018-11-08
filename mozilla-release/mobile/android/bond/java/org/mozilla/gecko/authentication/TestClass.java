package org.mozilla.gecko.authentication;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.util.Log;

import org.mozilla.gecko.BondV1Grpc;
import org.mozilla.gecko.Error;
import org.mozilla.gecko.ErrorCode;
import org.mozilla.gecko.Response;
import org.mozilla.gecko.UserAuth;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.concurrent.TimeUnit;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;

/**
 * @author Ravjit Uppal
 */
public class TestClass {

    private static final String LOGTAG = "GrpcPingPong";

    public void test() {
        new TalkToServer().execute();
    }

    @SuppressLint("StaticFieldLeak")
    private class TalkToServer extends AsyncTask<Void, Void, Response> {
        private ManagedChannel channel;

        @Override
        protected Response doInBackground(Void... voids) {
            String host = "ambassador.dev.k8s.eu-central-1.clyqz.com";
            int port = 443;
            try {
                channel = ManagedChannelBuilder.forAddress(host, port)
                        .build();
                BondV1Grpc.BondV1BlockingStub stub = BondV1Grpc.newBlockingStub(channel);
                UserAuth userAuth = UserAuth.newBuilder().setUsername("ravjituppal@gmail.com").setPassword("dj_sucks_big_time").build();
                Response response = stub.registerDevice(userAuth);
                return response;
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
                if (channel != null) {
                    channel.shutdown().awaitTermination(1, TimeUnit.SECONDS);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            if (result.getErrorCount() > 0) {
                Log.d(LOGTAG, "error found");
                Log.d(LOGTAG, result.getErrorList().toString());
            } else {
                Log.d(LOGTAG, "success");
            }
        }
    }
}
