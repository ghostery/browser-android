# lint_ignore=E501
config = {
    "products": {
        "installer": {
            "product-name": "Devedition-%(version)s",
            "check_uptake": True,
            "platforms": [
                "linux",
                "linux64",
                "osx",
                "win",
                "win64",
            ],
        },
        "installer-ssl": {
            "product-name": "Devedition-%(version)s-SSL",
            "check_uptake": True,
            "platforms": [
                "linux",
                "linux64",
                "osx",
                "win",
                "win64",
            ],
        },
        "msi": {
            "product-name": "Devedition-%(version)s-msi-SSL",
            "check_uptake": True,
            "platforms": [
                "win",
                "win64",
            ],
        },
        "msi": {
            "product-name": "Devedition-%(version)s-msi-SSL",
            "check_uptake": True,
            "alias": "firefox-devedition-msi-latest-ssl",
            "ssl-only": True,
            "add-locales": True,
            "paths": {
                "win32": {
                    "path": "/devedition/releases/%(version)s/win32/:lang/Firefox%%20Setup%%20%(version)s.msi",
                    "bouncer-platform": "win",
                },
                "win64": {
                    "path": "/devedition/releases/%(version)s/win64/:lang/Firefox%%20Setup%%20%(version)s.msi",
                    "bouncer-platform": "win64",
                },
            },
        },
        "stub-installer": {
            "product-name": "Devedition-%(version)s-stub",
            "check_uptake": True,
            "platforms": [
                "win",
                "win64",
            ],
        },
        "complete-mar": {
            "product-name": "Devedition-%(version)s-Complete",
            "check_uptake": True,
            "platforms": [
                "linux",
                "linux64",
                "osx",
                "win",
                "win64",
            ],
        },
    },
    "partials": {
        "releases-dir": {
            "product-name": "Devedition-%(version)s-Partial-%(prev_version)s",
            "check_uptake": True,
            "platforms": [
                "linux",
                "linux64",
                "osx",
                "win",
                "win64",
            ],
        },
    },
}
