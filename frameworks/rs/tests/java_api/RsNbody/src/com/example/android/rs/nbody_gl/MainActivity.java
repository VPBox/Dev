package com.example.android.rs.nbody_gl;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Toolbar;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    private BasicGLSurfaceView mGLView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mGLView = findViewById(R.id.surfaceView);
        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN;
        decorView.setSystemUiVisibility(uiOptions);
    }

    @Override
    protected void onPause() {
        mGLView.onPause();
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        Log.v(TAG, "KEY " + keyCode);
        return true;
    }
}
