# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

profiles-title = Informations sur les profils
profiles-subtitle = Cette page vous aide à gérer vos profils. Chaque profil est un environnement isolé contenant un historique, des marque-pages, des paramètres et des modules qui lui sont propres.
profiles-create = Créer un nouveau profil
profiles-restart-title = Redémarrer
profiles-restart-in-safe-mode = Redémarrer avec les modules désactivés…
profiles-restart-normal = Redémarrer normalement…
# Variables:
#   $name (String) - Name of the profile
profiles-name = Profil : { $name }
profiles-is-default = Profil par défaut
profiles-rootdir = Répertoire racine
# localDir is used to show the directory corresponding to
# the main profile directory that exists for the purpose of storing data on the
# local filesystem, including cache files or other data files that may not
# represent critical user data. (e.g., this directory may not be included as
# part of a backup scheme.)
# In case localDir and rootDir are equal, localDir is not shown.
profiles-localdir = Répertoire local
profiles-current-profile = Il s’agit du profil en cours d’utilisation ; il ne peut pas être supprimé.
profiles-in-use-profile = Ce profil est utilisé par une autre application et ne peut pas être supprimé.
profiles-rename = Renommer
profiles-remove = Supprimer
profiles-set-as-default = Définir comme profil par défaut
profiles-launch-profile = Lancer le profil dans un nouveau navigateur
profiles-yes = oui
profiles-no = non
profiles-rename-profile-title = Renommer un profil
# Variables:
#   $name (String) - Name of the profile
profiles-rename-profile = Renommer le profil { $name }
profiles-invalid-profile-name-title = Nom de profil invalide
# Variables:
#   $name (String) - Name of the profile
profiles-invalid-profile-name = Le nom de profil « { $name } » n’est pas autorisé.
profiles-delete-profile-title = Supprimer un profil
# Variables:
#   $dir (String) - Path to be displayed
profiles-delete-profile-confirm =
    Supprimer un profil l’enlèvera de la liste des profils disponibles et ne peut être annulé.
    Il est aussi possible de choisir de supprimer les fichiers de données du profil, comprenant vos paramètres, certificats et toutes vos données personnelles. Cette option supprimera le dossier « { $dir } » et ne pourra pas être annulée.
    Désirez-vous supprimer les fichiers de données du profil ?
profiles-delete-files = Supprimer les fichiers
profiles-dont-delete-files = Ne pas supprimer les fichiers
profiles-delete-profile-failed-title = Erreur
profiles-delete-profile-failed-message = Une erreur s’est produite lors de la tentative de suppression de ce profil.
profiles-opendir =
    { PLATFORM() ->
        [macos] Ouvrir dans le Finder
        [windows] Ouvrir le dossier
       *[other] Ouvrir le répertoire
    }
