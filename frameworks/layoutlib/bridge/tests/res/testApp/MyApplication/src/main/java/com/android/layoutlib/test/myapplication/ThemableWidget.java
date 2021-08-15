package com.android.layoutlib.test.myapplication;

import android.content.Context;
import android.util.AttributeSet;
import android.view.ContextThemeWrapper;
import android.view.LayoutInflater;
import android.widget.FrameLayout;

public class ThemableWidget extends FrameLayout {
    public ThemableWidget(Context context) {
        super(context);

        init();
    }

    public ThemableWidget(Context context, AttributeSet attrs) {
        super(context, attrs);

        init();
    }

    public ThemableWidget(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        init();
    }

    public ThemableWidget(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);

        init();
    }

    private void init() {
        ContextThemeWrapper context = new ContextThemeWrapper(getContext(), getContext().getTheme());
        context.setTheme(R.style.ThemableWidgetStyle);

        LayoutInflater.from(context).inflate(R.layout.themable_widget_layout, this);
    }
}
