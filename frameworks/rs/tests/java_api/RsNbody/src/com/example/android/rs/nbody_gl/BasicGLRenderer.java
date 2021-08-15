package com.example.android.rs.nbody_gl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class BasicGLRenderer implements GLSurfaceView.Renderer {

    private static final String TAG = "BasicGLRenderer";
    private Swarm mSwarm;
    Context mContext;
    private final float[] mMVPMatrix = new float[16];
    private final float[] mProjectionMatrix = new float[16];
    private final float[] mViewMatrix = new float[16];
    private final float[] mRotationMatrix = new float[16];
    private float mAngle;

    GLSurfaceView mGLSurfaceView;

    public BasicGLRenderer(Context context, GLSurfaceView view) {
        mContext = context;
        mGLSurfaceView = view;
    }

    @Override
    public void onSurfaceCreated(GL10 unused, EGLConfig config) {
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES20.glEnable(GLES20.GL_DEPTH_TEST);

        if (mSwarm != null) {
            mSwarm.onPause();
        }
        mSwarm = new Swarm(mContext, mGLSurfaceView);
        mSwarm.onSurfaceCreated();
        mSwarm.onResume();
    }

    @Override
    public void onDrawFrame(GL10 unused) {
        float[] scratch = new float[16];

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        Matrix.setLookAtM(mViewMatrix, 0,
                0, 0, -3, 0f,
                0f, 0f, 0f,
                1.0f, 0.0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);

        mSwarm.draw(mMVPMatrix);

        Matrix.setRotateM(mRotationMatrix, 0, mAngle, 0, 0, 1.0f);
        Matrix.multiplyMM(scratch, 0, mMVPMatrix, 0, mRotationMatrix, 0);

    }

    @Override
    public void onSurfaceChanged(GL10 unused, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        float ratio = (float) width / height;
        Matrix.frustumM(mProjectionMatrix, 0, -ratio / 10, ratio / 10, -.1f, .1f, .1f, 200);
    }

    public static int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }

    public static void checkGlError(String glOperation) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e(TAG, glOperation + ": glError " + error);
            throw new RuntimeException(glOperation + ": glError " + error);
        }
    }

    public float getAngle() {
        return mAngle;
    }

    public void setAngle(float angle) {
        mAngle = angle;
    }

    public void onResume() {
        if (mSwarm != null) {
            mSwarm.onResume();
        }
    }

    public void onPause() {
        if (mSwarm != null) {
            mSwarm.onPause();
        }
    }

    public void onTouchEvent(MotionEvent e) {
        mSwarm.onTouchEvent(e);
    }

    public void onJoystick(float dx, float dy) {
        mSwarm.onJoystick(dx, dy);
    }

    public void onKeyDown(int keyCode, KeyEvent event) {

        mSwarm.onKeyDown(keyCode, event);
    }
}