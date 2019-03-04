#!/bin/bash

readonly SCRIPT_DIR=`dirname "$0"`
readonly L10N_DIR=`cd "${SCRIPT_DIR}/../l10n" && pwd`
readonly BASE_DIR=`cd "${SCRIPT_DIR}/../mozilla-release/mobile/android/base/locales/en-US/" && pwd`
readonly BASE_PATH="${BASE_DIR}/cliqz_strings.dtd"
readonly BASE_TMP="`mktemp`"

declare -A EXCEPTIONS=( \
    ["action_ok"]="OK" \
    ["cc_enhanced_anti_tracking"]="Anti-Tracking" \
    ["cc_one_trackers"]="&formatD; Tracker" \
    ["cc_smart_blocking"]="Smart Blocking" \
    ["firstrun_urlbar_subtext3"]="&firstrun_urlbar_subtext;" \
    ["ghostery_onboarding_new_tab_title"]="&pref_new_tab_category;" \
    ["myoffrz_title"]="&brandMyOffrz;" \
    ["pref_about_android_open_source_project_summary"]="Apache License 2.0" \
    ["pref_about_crux"]="Crux" \
    ["pref_about_crux_summary"]="Apache License 2.0" \
    ["pref_about_eula"]="EULA" \
    ["pref_about_hp_hosts_ad_server_summary"]="Freeware" \
    ["pref_category_ghostery_control_center"]="Ghostery Control Center" \
    ["pref_category_help_summary"]="FAQ, Support" \
    ["pref_category_human_web"]="Human Web" \
    ["pref_help_faq"]="FAQs" \
    ["pref_help_support"]="Support" \
    ["pref_myoffrz_category"]="&brandMyOffrz;" \
    ["pref_news_notifications"]="News notifications" \
    ["pref_privacy_antitracking"]="Anti-Tracking" \
    ["pref_reset_subscriptions_description"]="Would you like to reset all your subscriptions (e.g. to Bundesliga games)?" \
    ["pref_reset_subscriptions"]="Reset all subscriptions" \
    ["pref_reset_subscriptions_toast"]="All subscriptions reset" \
    ["pref_search_complementary_engine_bing"]="bing" \
    ["pref_search_complementary_engine_duck"]="DuckDuckGo" \
    ["pref_search_complementary_engine_ecosia"]="Ecosia" \
    ["pref_search_complementary_engine_Google"]="Google" \
    ["pref_search_complementary_engine_qwant"]="Qwant" \
    ["pref_search_complementary_engine_start_page"]="StartPage" \
    ["pref_search_complementary_engine_yahoo"]="yahoo" \
    ["pref_subscriptions_notifications_category"]="Subscriptions and Notifications" \
    ["human_web_info_url"]="A URL" \
)

cleanUp() {
    cat "$1"|sed '/^<!--/d'|sed '/^$/d'|sed -e 's/<!ENTITY \(.*\)>$/\1/' > "$2"
}

cleanUp $BASE_PATH $BASE_TMP

find "${L10N_DIR}" -iname "cliqz_strings.dtd"| while read dtd; do
    cleaned="`mktemp`"
    cleanUp "${dtd}" "${cleaned}"
    cat "${BASE_TMP}" "${cleaned}"|sort|uniq -d|while read dup; do
        key=`echo $dup|cut -d' ' -f1`
        exc="${EXCEPTIONS[$key]}"
        if [ -z "${exc}" ]; then
            echo "$key is untraslated in ${dtd}" >>/dev/stderr
        fi
    done
done
