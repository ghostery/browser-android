/*
Copyright © Cliqz 2018
 */
package org.mozilla.gecko.anolysis;

/**
 * Favorites migration metrics
 */
public class FavoritesMigrationMetrics {

    private FavoritesMigrationMetrics() {
        // No instances
    }

    public static void folders(int count, int foldersInRootCount, int maxDepth) {
        new AnalysisBuilder("metrics.favorites.migration.folders")
                .putNumber("count", count)
                .putNumber("rootFolderCount", foldersInRootCount)
                .putNumber("maxDepth", maxDepth)
                .dispatch();
    }
}
