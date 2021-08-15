package com.example.android.rs.nbody_gl;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

public class BasicGLSurfaceView extends GLSurfaceView {
    private static final String TAG = "BasicGLSurfaceView";
    private final BasicGLRenderer mRenderer;

    public BasicGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        mRenderer = new BasicGLRenderer(context, this);
        setup();
    }

    public BasicGLSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        mRenderer = new BasicGLRenderer(context, this);
        setup();
    }

    private void setup() {
        setRenderer(mRenderer);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();

    }

    private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
    private float mPreviousX;
    private float mPreviousY;

    @Override
    public void onResume() {
        super.onResume();
        mRenderer.onResume();
    }

    @Override
    public void onPause() {
        mRenderer.onPause();
        super.onPause();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        mRenderer.onTouchEvent(e);
        return true;
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        Log.v(TAG,"onGenericMotionEvent ");
        if (event.isFromSource(InputDevice.SOURCE_CLASS_JOYSTICK)) {
            if (event.getAction() == MotionEvent.ACTION_MOVE) {
                float x = event.getAxisValue(MotionEvent.AXIS_X);
                float y = event.getAxisValue(MotionEvent.AXIS_Y);
                mRenderer.onJoystick(x,y);
                return true;
            }
        }
        return super.onGenericMotionEvent(event);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        mRenderer.onKeyDown(keyCode, event);
         return super.onKeyDown(keyCode, event);
     }


}