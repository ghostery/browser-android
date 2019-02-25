package org.mozilla.gecko;

import android.animation.Animator;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.view.ViewPropertyAnimator;
import android.view.ViewStub;
import android.widget.ProgressBar;

import org.mozilla.gecko.util.ThreadUtils;

/**
 * An helper to coordinate the "loading search" UI
 *
 * Copyright © Cliqz 2019
 */
class CliqzLoadingSearchHelper {
    private static final long SHOW_ANIMATION_DURATION = 500L;
    private static final long HIDE_ANIMATION_DURATION = 200L;
    private static final long SHOW_PROGRESS_DELAY = 1000L;
    private final ViewStub loadingSearchStub;
    private View mLoadingSearchContainer;
    private ViewPropertyAnimator mAnimator;
    private boolean mStopped = true;
    private ScheduledShow mLastScheduledShow = null;

    CliqzLoadingSearchHelper(@NonNull ViewStub loadingSearchStub) {
        this.loadingSearchStub = loadingSearchStub;
    }

    /**
     * Start the countdown to show the UI. If called multiple times, it resets the countdown and
     * restarts it.
     */
    @UiThread
    public void start() {
        mStopped = false;
        if (mLastScheduledShow != null) {
            if (mLastScheduledShow.mIsShown) {
                return;
            } else {
                mLastScheduledShow.cancel();
            }
        }

        mLastScheduledShow = new ScheduledShow();
        ThreadUtils.postDelayedToUiThread(mLastScheduledShow, SHOW_PROGRESS_DELAY);
    }

    /**
     * Stop the countdown to show the UI and, if the latter is visible, hides it.
     */
    @UiThread
    public void stop() {
        if (mStopped) {
            return;
        }
        mStopped = true;
        hide();
    }

    private void show() {
        if (mLoadingSearchContainer == null) {
            initView();
        }

        if (mAnimator != null) {
            mAnimator.cancel();
        }
        mLoadingSearchContainer.setAlpha(0f);
        mLoadingSearchContainer.setVisibility(View.VISIBLE);
        mAnimator = mLoadingSearchContainer.animate();
        mAnimator.alpha(1f)
                .setDuration(SHOW_ANIMATION_DURATION)
                .start();
    }

    private void hide() {
        if (mLastScheduledShow != null) {
            mLastScheduledShow.cancel();
            mLastScheduledShow = null;
        }
        if (mLoadingSearchContainer == null) {
            return;
        }

        if (mAnimator != null) {
            mAnimator.cancel();
        }
        mAnimator = mLoadingSearchContainer.animate();
        mAnimator.alpha(0f)
                .setDuration(HIDE_ANIMATION_DURATION)
                .setListener(new HideAnimationListener())
                .start();
    }

    public boolean isStarted() {
        return !mStopped;
    }

    private void initView() {
        mLoadingSearchContainer = loadingSearchStub.inflate();
    }

    private class ScheduledShow implements Runnable {
        private boolean mIsCancelled = false;
        private boolean mIsShown = false;

        @Override
        public void run() {
            if (mIsCancelled || mStopped) {
                return;
            }

            mIsShown = true;
            show();
        }

        void cancel() {
            mIsCancelled = true;
        }

        boolean isShown() {
            return mIsShown;
        }
    }

    private class HideAnimationListener implements Animator.AnimatorListener {

        @Override
        public void onAnimationStart(Animator animation) {

        }

        @Override
        public void onAnimationEnd(Animator animation) {
            setViewAsGone();
        }

        @Override
        public void onAnimationCancel(Animator animation) {
            setViewAsGone();
        }

        private void setViewAsGone() {
            if (mLoadingSearchContainer != null) {
                mLoadingSearchContainer.setVisibility(View.GONE);
            }
        }

        @Override
        public void onAnimationRepeat(Animator animation) {

        }
    }
}
