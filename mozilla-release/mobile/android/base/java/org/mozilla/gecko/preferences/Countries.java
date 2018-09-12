package org.mozilla.gecko.preferences;

import android.content.Context;

import org.mozilla.gecko.R;

import java.util.Arrays;

/**
 * Copyright © Cliqz 2018
 */
public class Countries {
    private String [] mCountries;
    private String [] mCountryCodes;
    private Context mContext;
    Countries(Context context){
        mCountries = context.getResources().getStringArray(R.array.pref_search_regional_entries);
        mCountryCodes = context.getResources().getStringArray(R.array.pref_search_regional_values);
        mContext = context;
    }

    public String getDefaultCountryCode() {
        String locale = mContext.getResources().getConfiguration().locale.getCountry().toLowerCase();
        for(String countryCode:mCountryCodes){
            if(countryCode.equals(locale)){
                return locale;
            }
        }
        return "us";
    }

    public String getCountryName(String countryCode){
        for(int i= 0 ;i < mCountries.length; i++){
            if(mCountryCodes[i].equals(countryCode)){
                return mCountries[i];
            }
        }
        return "usa";
    }
}
