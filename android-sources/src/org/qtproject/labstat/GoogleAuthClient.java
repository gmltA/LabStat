package org.qtproject.labstat;

import com.google.android.gms.auth.GoogleAuthUtil;
import com.google.android.gms.auth.GoogleAuthException;
import com.google.android.gms.auth.UserRecoverableAuthException;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.drive.DriveApi.DriveContentsResult;
import com.google.android.gms.drive.DriveContents;
import com.google.android.gms.drive.DriveFile;
import com.google.android.gms.drive.DriveId;
import com.google.android.gms.drive.DriveResource.MetadataResult;
import com.google.android.gms.drive.Metadata;
import com.google.android.gms.drive.MetadataChangeSet;
import com.google.android.gms.drive.OpenFileActivityBuilder;

import android.app.Activity;
import android.accounts.Account;
import android.accounts.AccountManager;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.os.Bundle;
import android.util.Log;

import java.io.IOException;
import android.os.AsyncTask;

public class GoogleAuthClient extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static final String TAG = "GoogleAuthClient";
    protected static final int REQUEST_CODE_RESOLUTION = 1;
    protected static String mAccountName;
    private static GoogleAuthClient thisActivity;

    static GoogleAuthClient getActivity() { return thisActivity; }

    public GoogleAuthClient() {
        thisActivity = this;
    }

    public String processDriveAuth(long authClientAddr)
    {
        Account[] accounts = AccountManager.get(this).getAccountsByType("com.google");
        if (accounts.length == 0) {
            Log.d(TAG, "Must have a Google account installed");
            return "";
        }
        mAccountName = accounts[0].name;

        final long authClientAddress = authClientAddr;

        String token = "";
        AsyncTask<Void, Void, String> task = new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                String token = null;

                try {
                    token = GoogleAuthUtil.getToken(getApplicationContext(), mAccountName, "oauth2:https://www.googleapis.com/auth/drive https://www.googleapis.com/auth/userinfo.email");
                } catch (IOException transientEx) {
                    // Network or server error, try later
                    Log.e(TAG, transientEx.toString());
                } catch (UserRecoverableAuthException e) {
                    Log.e(TAG, e.toString());
                    Intent recover = e.getIntent();
                    startActivityForResult(recover, REQUEST_CODE_RESOLUTION);
                } catch (GoogleAuthException authEx) {
                    Log.e(TAG, authEx.toString());
                }

                return token;
            }

            @Override
            protected void onPostExecute(String token) {
                Log.i(TAG, "Access token retrieved:" + token);
                passToken(token, authClientAddress);
            }

        };
        task.execute();

        return mAccountName;
    }

    private static native void passToken(String token, long authClientAddress);
}
