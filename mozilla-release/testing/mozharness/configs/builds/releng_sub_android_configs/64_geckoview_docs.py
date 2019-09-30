config = {
    'stage_platform': 'android-geckoview-docs',
    'src_mozconfig': 'mobile/android/config/mozconfigs/android-api-16-frontend/nightly',
    'multi_locale_config_platform': 'android',
    # geckoview-docs doesn't produce a package. So don't collect package metrics.
    'disable_package_metrics': True,
    'postflight_build_mach_commands': [
        ['android',
         'geckoview-docs',
         '--archive',
         '--upload', 'mozilla/geckoview',
         '--upload-branch', 'gh-pages/javadoc/{project}',
         '--upload-message', 'Update {project} javadoc to rev {revision}',
<<<<<<< HEAD
         '--variant', 'withGeckoBinariesRelease',
||||||| merged common ancestors
         '--variant', 'officialWithGeckoBinariesNoMinApiRelease',
=======
>>>>>>> upstream-releases
        ],
    ],
    'max_build_output_timeout': 0,
}
