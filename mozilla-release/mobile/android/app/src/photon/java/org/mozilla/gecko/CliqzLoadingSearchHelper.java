package org.mozilla.gecko;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.view.ViewPropertyAnimator;
import android.view.ViewStub;
import android.widget.ProgressBar;

class CliqzLoadingSearchHelper {
    private static final long SHOW_ANIMATION_DURATION = 500l;
    private static final long HIDE_ANIMATION_DURATION = 200l;
    private final ViewStub loadingSearchStub;
    private View mLoadingSeachContainer;
    private ViewPropertyAnimator mAnimator;

    public CliqzLoadingSearchHelper(@NonNull ViewStub loadingSearchStub) {
        this.loadingSearchStub = loadingSearchStub;
    }

    public void show() {
        if (mLoadingSeachContainer == null) {
            initView();
            // Postpone the animation, at this point the view has no dimension
        }

        if (mAnimator != null) {
            mAnimator.cancel();
        }
        mLoadingSeachContainer.setAlpha(0f);
        mAnimator = mLoadingSeachContainer.animate();
        mAnimator.alpha(1f)
                .setDuration(SHOW_ANIMATION_DURATION)
                .start();
    }

    private void hide() {
        if (mLoadingSeachContainer == null) {
            return;
        }

        if (mAnimator != null) {
            mAnimator.cancel();
        }
        mAnimator = mLoadingSeachContainer.animate();
        mAnimator.alpha(0f)
                .setDuration(HIDE_ANIMATION_DURATION)
                .start();
    }

    @SuppressWarnings("RedundantCast")
    @SuppressLint("ObsoleteSdkInt")
    private void initView() {
        mLoadingSeachContainer = loadingSearchStub.inflate();
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            final Context context = mLoadingSeachContainer.getContext();
            final ProgressBar progressBar =
                    (ProgressBar) mLoadingSeachContainer.findViewById(android.R.id.progress);
            final Drawable drawable = progressBar.getIndeterminateDrawable();
            drawable.setColorFilter(ContextCompat.getColor(context, android.R.color.white),
                    PorterDuff.Mode.SRC_IN);
        }
    }
}
