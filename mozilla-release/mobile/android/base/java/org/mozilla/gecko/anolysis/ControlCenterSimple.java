/*
Copyright © Cliqz 2018
 */
package org.mozilla.gecko.anolysis;

public class ControlCenterSimple {

    private ControlCenterSimple() {
        // no instances
    }

    public static void show() {
        new AnalysisBuilder("metrics.controlCenter.show").dispatch();
    }


    public static void pause() {
        new AnalysisBuilder("metrics.controlCenter.click.pause").dispatch();
    }


    public static void resume() {
        new AnalysisBuilder("metrics.controlCenter.click.resume").dispatch();
    }


    public static void trustSite() {
        new AnalysisBuilder("metrics.controlCenter.click.trustSite").dispatch();
    }


    public static void restrictSite() {
        new AnalysisBuilder("metrics.controlCenter.click.restrictSite").dispatch();
    }

}
