export MOZ_CHROME_MULTILOCALE=`ls -1 ../l10n|paste -s -d " "`
for AB_CD in $MOZ_CHROME_MULTILOCALE; do
    ./mach build chrome-$AB_CD
done
export AB_CD=multi
./mach package
./gradlew :app:assembleWithGeckoBinariesRelease
