package org.mozilla.gecko.cliqzicons;

import android.support.annotation.Nullable;

import java.util.HashMap;
import java.util.Map;

/**
 * Copyright © Cliqz 2018
 */
public class CliqzIconDatabase {

    private static final Map<String, String[]> db = new HashMap<>();
    private CliqzIconDatabase() {}

    @Nullable
    public static String[] getRulesForDomain(String domain) {
        return db.get(domain);
    }

    static {
        db.put("6pm", new String[] {"$.com"});
        db.put("geocaching", new String[] {"$.com"});
        db.put("here", new String[] {"$.com"});
        db.put("yomonda", new String[] {"$.de"});
        db.put("harmonyfm", new String[] {"$.de"});
        db.put("gov", new String[] {"service.$.uk", "direct.$.uk", "$.uk"});
        db.put("mckinsey", new String[] {"$.com", "$.de"});
        db.put("boobdesign", new String[] {"$.com"});
        db.put("chatvongesternnacht", new String[] {"$.de"});
        db.put("parents", new String[] {"$.com"});
        db.put("livejasmin", new String[] {"$.com"});
        db.put("apnews", new String[] {"$.com"});
        db.put("post", new String[] {"$.de", "$.ch", "$.at"});
        db.put("dailystar", new String[] {"$.co.uk"});
        db.put("apache", new String[] {"spark.$.org"});
        db.put("lesen", new String[] {"$.to"});
        db.put("inforadio", new String[] {"$.de"});
        db.put("exklusiv-muenchen", new String[] {"$.de"});
        db.put("kyoto-u", new String[] {"www.$.ac"});
        db.put("ed", new String[] {"$.ac.uk"});
        db.put("reifen", new String[] {"$.com"});
        db.put("bayernwelle", new String[] {"$.de"});
        db.put("hsbc", new String[] {"$.co.uk"});
        db.put("tesco", new String[] {"$.com"});
        db.put("mitarbeiterangebote", new String[] {"$.de"});
        db.put("kaikkialla", new String[] {"$.com"});
        db.put("hausgold", new String[] {"$.de"});
        db.put("stanford", new String[] {"$.edu"});
        db.put("hihonor", new String[] {"$.com"});
        db.put("fiducia", new String[] {"$.de"});
        db.put("standard", new String[] {"$.co.uk"});
        db.put("917xfm", new String[] {"$.de"});
        db.put("yr", new String[] {"$.no"});
        db.put("programme-tv", new String[] {"$.net"});
        db.put("achgut", new String[] {"$.com"});
        db.put("movie", new String[] {"$.to"});
        db.put("apobank", new String[] {"$.de"});
        db.put("autotrader", new String[] {"$.co.uk"});
        db.put("cineplex", new String[] {"$.com", "$.de"});
        db.put("alsterradio", new String[] {"$.de"});
        db.put("go", new String[] {"abcfamily.$", "disney.$", "oscar.$", "abc.$", "abcnews.$", "espn.$", "watchdisneyxd.$", "secsports.$", "$.de", "$.co"});
        db.put("weather", new String[] {"$.com"});
        db.put("digitec", new String[] {"$.ch"});
        db.put("mitvergnuegen", new String[] {"muenchen.$.com", "$.com"});
        db.put("eminem", new String[] {"$.com"});
        db.put("gfds", new String[] {"$.de"});
        db.put("millenium", new String[] {"$.org"});
        db.put("mit", new String[] {"$.edu"});
        db.put("handelszeitung", new String[] {"$.ch"});
        db.put("porsche-stuttgart", new String[] {"$.de"});
        db.put("argos", new String[] {"$.co.uk"});
        db.put("ok", new String[] {"$.ru", "$.de"});
        db.put("xojane", new String[] {"$.com"});
        db.put("nametests", new String[] {"$.com"});
        db.put("filmpalast", new String[] {"$.net"});
        db.put("news", new String[] {"$.stv.tv"});
        db.put("movie2k", new String[] {"$.tl"});
        db.put("ap", new String[] {"$.org"});
        db.put("ambellis", new String[] {"$.de"});
        db.put("ghostery", new String[] {"$.com"});
        db.put("wish", new String[] {"$.com"});
        db.put("audibene", new String[] {"$.de"});
        db.put("opera", new String[] {"$.com"});
    }
}
