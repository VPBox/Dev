package com.android.layoutlib.test.myapplication.widgets;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

import java.io.IOException;
import java.io.InputStream;

public class AssetView extends View {
    public AssetView(Context context) {
        super(context);
        init(context);
    }

    public AssetView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public AssetView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(context);
    }

    private void init(Context context) {
        try {
            InputStream istr = context.getAssets().open("asset.png");
            setBackground(Drawable.createFromStream(istr, null));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
