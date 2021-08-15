package com.example.android.rs.nbody_gl;

import android.content.Context;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.Float2;
import android.renderscript.Float3;
import android.renderscript.RenderScript;
import android.renderscript.Type;
import androidx.core.view.MotionEventCompat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.Arrays;
import java.util.Objects;

public class Swarm {
    private static final String TAG = "Swarm";
    private static final boolean USE_BB = true;
    public static final int SWARM_SIZE = 1000; // max is 2730= 2^32/12

    private final String vertexShaderCode =
            "uniform   mat4 u_MVP;" +
                    "attribute vec4 a_Position;" +
                    "attribute vec4 a_Color;" +
                    "varying   vec4 v_Color;" +
                    "varying   vec4 v_loc;" +
                    "varying   vec3 v_pos;" +

                    "void main() {" +
                    "  v_Color = a_Color;" +
                    "  gl_Position = u_MVP * a_Position;" +
                    "v_loc = a_Position;" +
                    " v_pos = gl_Position.xyz;" +
                    "}";

    private final String fragmentShaderCode =
            "precision mediump float;" +
                    "varying vec4 v_Color;" +
                    "varying vec4 v_loc;" +
                    "varying vec3 v_pos;" +

                    "void main() {" +

                    "  gl_FragColor = v_Color ;" +
                    "}";

    GLSurfaceView mGLSurfaceView;
    boolean mDrawSwarm = false;

    private volatile FloatBuffer vertexBuffer;
    private final ShortBuffer drawListBuffer;
    private final FloatBuffer mColorBuffer;
    private final int mProgram;
    private final Context mContext;
    Background mBackground = new Background();
    private int mPositionHandle;
    private int mColorHandle;
    private int mMVPMatrixHandle;
    static final int[] sTetrahedronIndex = {0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3};

    static final int FLOATS_PER_VERTEX = 4;
    static final int FLOATS_PER_COLOR = 3;
    static final int VERTEX_PER_TETRAHEDRON = 4;
    static final int BYTES_PER_FLOAT = 4;
    volatile float mTriArray[] = new float[SWARM_SIZE * FLOATS_PER_VERTEX * VERTEX_PER_TETRAHEDRON];
    float mColorArray[] = new float[SWARM_SIZE * FLOATS_PER_COLOR * VERTEX_PER_TETRAHEDRON];
    short mIndexArray[] = new short[SWARM_SIZE * sTetrahedronIndex.length];

    private final int vertexStride = FLOATS_PER_VERTEX * BYTES_PER_FLOAT; // 4 bytes per vertex
    private final int colorStride = FLOATS_PER_COLOR * BYTES_PER_FLOAT; // 4 bytes per vertex
    private boolean mOld = true;
    private Object lock = new Object();

    public Swarm(Context context, GLSurfaceView view) {

        mContext = context;
        mGLSurfaceView = view;
        double arc = 2 * Math.PI / mTriArray.length;
        for (int i = 0; i < mTriArray.length; i += FLOATS_PER_VERTEX) {
            int p = i;
            int v = ((i / FLOATS_PER_VERTEX) % VERTEX_PER_TETRAHEDRON) == 0 ? 0 : 1;
            int c = (i / FLOATS_PER_VERTEX) / VERTEX_PER_TETRAHEDRON;
            double s = (2 + c % 7) / 4.;
            mTriArray[p++] = (float) (s * v * Math.sin(i * arc));
            mTriArray[p++] = (float) (s * v * Math.cos(i * arc));
            mTriArray[p++] = (float) ((Math.random() - .5) * 1.8);
            mTriArray[p] = 0;
        }

        float[] hsv = new float[]{.9f, .99f, .8f};
        Arrays.fill(mColorArray, 0, 12, .1f);
        for (int i = 12; i < mColorArray.length; i += FLOATS_PER_COLOR) {
            if (((i / 3) % 4) == 0) {
                mColorArray[i + 2] = mColorArray[i + 1] = mColorArray[i] = 0.8f;
                continue;
            }
            hsv[0] = (float) Math.random() * 360;
            int color = Color.HSVToColor(hsv);
            mColorArray[i] = (color & 255) / 255.f;
            color >>= 8;
            mColorArray[i + 1] = (color & 255) / 255.f;
            color >>= 8;
            mColorArray[i + 2] = (color & 255) / 255.f;
        }
        // Build Tetrahedrons
        for (short i = 0; i < mIndexArray.length; i++) {
            int tet = i / sTetrahedronIndex.length;
            int index = i % sTetrahedronIndex.length;
            int offset = tet * VERTEX_PER_TETRAHEDRON;
            mIndexArray[i] = (short) (offset + sTetrahedronIndex[index]);
        }

        ByteBuffer bb = ByteBuffer.allocateDirect(mTriArray.length * 4);   // (# of coordinate values * 4 bytes per float)
        bb.order(ByteOrder.nativeOrder());


        ByteBuffer dlb = ByteBuffer.allocateDirect(mIndexArray.length * 2);
        dlb.order(ByteOrder.nativeOrder());
        drawListBuffer = dlb.asShortBuffer();
        drawListBuffer.put(mIndexArray);
        drawListBuffer.position(0);

        ByteBuffer colBB = ByteBuffer.allocateDirect(mColorArray.length * 4);
        colBB.order(ByteOrder.nativeOrder());
        mColorBuffer = colBB.asFloatBuffer();
        mColorBuffer.put(mColorArray);
        mColorBuffer.position(0);

        int vertexShader = BasicGLRenderer.loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        int fragmentShader = BasicGLRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        mProgram = GLES20.glCreateProgram();             // create empty OpenGL Program
        GLES20.glAttachShader(mProgram, vertexShader);   // add the vertex shader to program
        GLES20.glAttachShader(mProgram, fragmentShader); // add the fragment shader to program
        GLES20.glLinkProgram(mProgram);                  // create OpenGL program executables
    }

    synchronized void setTriangles(FloatBuffer v) {
        vertexBuffer = v;
        mGLSurfaceView.requestRender();
    }

    int count = 0;
    long time = System.nanoTime();

    public synchronized void draw(float[] mvpMatrix) {
         if (!mDrawSwarm || vertexBuffer == null) return;
        mTriArray[2] = 0;
        synchronized (vertexBuffer) {
            vertexBuffer.position(0);
            GLES20.glUseProgram(mProgram);

            mPositionHandle = GLES20.glGetAttribLocation(mProgram, "a_Position");
            GLES20.glEnableVertexAttribArray(mPositionHandle);

            GLES20.glVertexAttribPointer(mPositionHandle, 3, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

            mColorHandle = GLES20.glGetAttribLocation(mProgram, "a_Color");
            GLES20.glEnableVertexAttribArray(mColorHandle);
            GLES20.glVertexAttribPointer(mColorHandle, FLOATS_PER_COLOR, GLES20.GL_FLOAT, false, colorStride, mColorBuffer);

            mMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "u_MVP");
            checkGlError("glGetUniformLocation");

            GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mvpMatrix, 0);
            checkGlError("glUniformMatrix4fv");

            GLES20.glDrawElements(
                    GLES20.GL_TRIANGLES, mIndexArray.length,
                    GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

            GLES20.glDisableVertexAttribArray(mPositionHandle);
            GLES20.glFinish();
        }
        count++;
        if (count > 100) {
            Log.v(TAG, " gl " + count / ((System.nanoTime() - time) * 1E-9) + " fps");
            count = 0;
            time = System.nanoTime();
        }
    }

    private static void checkGlError(String glOperation) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e(TAG, glOperation + ": glError " + error);
            throw new RuntimeException(glOperation + ": glError " + error);
        }
    }

    public void onResume() {
        mBackground = new Background();
        mBackground.execute();
    }

    public void onPause() {
        mDrawSwarm = false;
        mBackground.keepRunning = false;
    }

    public void onSurfaceCreated() {
        mDrawSwarm = true;
    }

    public void onTouchEvent(MotionEvent e) {
        final int action = MotionEventCompat.getActionMasked(e);
        Log.v(TAG, "action =" + action);
        switch (action) {
            case MotionEvent.ACTION_MOVE:
                if (e.getEventTime() - e.getDownTime() > 100) {
                    float w = mGLSurfaceView.getWidth();
                    float h = mGLSurfaceView.getHeight();

                    mDrag_x = (w / 2 - e.getX()) * 4;
                    mDrag_y = (h / 2 - e.getY()) * 4;
                    mDrag_z = 0;
                    mDrag = true;
                }
                break;
            case MotionEvent.ACTION_DOWN:
                break;
            case MotionEvent.ACTION_UP:
                if (e.getEventTime() - e.getDownTime() < 200) {
                    restartSimulation();
                }

        }
    }

    public void onJoystick(float dx, float dy) {
        if (Math.abs(dx) + Math.abs(dy) > 0.1) {
            float w = mGLSurfaceView.getWidth();
            float h = mGLSurfaceView.getHeight();
            mDrag_x = dx * w * 2;
            mDrag_y = dy * h * 2;
            mDrag_z = 0;
            mDrag = true;
        }
    }


    float mDrag_x, mDrag_y, mDrag_z;
    volatile boolean mDrag = false;

    static final int P_CTR_M = 0;
    static final int P_MAX_M = 1;
    static final int P_MIN_M = 2;
    static final int P_INIT_V = 3;
    static final int P_DIR_X = 4;
    static final int P_DIR_Y = 5;
    static final int P_DIR_Z = 6;
    static final int P_RAD_MIN = 7;
    static final int P_RAD_MAX = 8;
    static final float[][] sModes = {
            {1000, 800, 300, 0, 1, 1, 1, 1300, 1900},
            {1000000, 8, 3, 1800, 1, 1, 1, 1300, 1900},
            {1000000, 8, 7, 1800, 0, 0, 1, 1300, 1900},
            {1000, 800, 300, 0, 1, 1, 1, 1800, 1900},
            {1000000, 8, 3, 1800, 1, 1, 1, 1800, 1900},
            {1000000, 8, 7, 1000, 0, 0, 1, 1800, 1900},
            {1000000, 8, 7, 1800, 0, 1, 0, 1800, 1900},
    };
    int mModeNo = 0;
    volatile float[] mMode = sModes[mModeNo];

    private void restartSimulation() {
        Log.v(TAG, "restartSimulation " + mModeNo);

        mDrawSwarm = false;
        mBackground.keepRunning = false;
        mModeNo = (mModeNo + 1) % sModes.length;
        mMode = sModes[mModeNo];
        mBackground = new Background();
        mBackground.execute();
        mDrawSwarm = true;
    }

    public void onKeyDown(int keyCode, KeyEvent event) {
        restartSimulation();
    }


    class Background extends AsyncTask<Void, FloatBuffer, Void> {
        boolean keepRunning = true;
        int count = 0;
        long time = System.nanoTime();


        @Override
        protected Void doInBackground(Void... params) {
            RenderScript rs = RenderScript.create(mContext);
            float[] hsv = new float[3];
            hsv[1] = 0.8f;
            hsv[2] = 0.9f;

            Allocation posAlloc = Allocation.createSized(rs, Element.F32_4(rs), SWARM_SIZE);
            Allocation velAlloc = Allocation.createSized(rs, Element.F32_4(rs), SWARM_SIZE);
            int triAllocSize = mTriArray.length / 4;
            Type.Builder builder = new Type.Builder(rs, Element.F32_4(rs));

            Allocation triAlloc1 = Allocation.createSized(rs, Element.F32_4(rs), triAllocSize);
            Allocation triAlloc2 = triAlloc1;
            FloatBuffer triBuff1,triBuff2,triBuff;
            if (USE_BB) {
                triAlloc2 = Allocation.createSized(rs, Element.F32_4(rs), triAllocSize);
                triBuff1 = triAlloc1.getByteBuffer().order(ByteOrder.LITTLE_ENDIAN).asFloatBuffer();
                triBuff2 = triAlloc2.getByteBuffer().order(ByteOrder.LITTLE_ENDIAN).asFloatBuffer();
            }
            else {
                triBuff1 = ByteBuffer.allocateDirect(mTriArray.length *4).order(ByteOrder.nativeOrder()).asFloatBuffer();
                triBuff2 = ByteBuffer.allocateDirect(mTriArray.length *4).order(ByteOrder.nativeOrder()).asFloatBuffer();
            }

            ScriptC_nbody nbody = new ScriptC_nbody(rs);
            nbody.set_positions(posAlloc);
            nbody.set_velocities(velAlloc);
            nbody.set_triangles1(triAlloc1);
            nbody.set_triangles2(triAlloc2);
            Log.v(TAG, " node = " + mMode + "  " + sModes[mModeNo] + " " + mModeNo);
            nbody.set_CENTER_MASS(mMode[P_CTR_M]);
            nbody.set_OBJECT_MAX_MASS(mMode[P_MAX_M]);
            nbody.set_OBJECT_MIN_MASS(mMode[P_MIN_M]);
            nbody.set_INITAL_VEL(mMode[P_INIT_V]);
            nbody.set_INITAL_DIR(new Float3(mMode[P_DIR_X], mMode[P_DIR_Y], mMode[P_DIR_Z]));
            nbody.set_RADIUS_RANGE(new Float2(mMode[P_RAD_MIN], mMode[P_RAD_MAX]));
            nbody.forEach_fill_pos(posAlloc);
            boolean dir = false;
            try {
                Thread.sleep(500);
                int triBufferPos = 0;
                while (keepRunning) {
                    count++;
                    dir = !dir;

                    triBuff = (dir) ? triBuff1 : triBuff2;
                    long time = System.nanoTime();

                    if (mDrag) {
                        Log.v(TAG, "Drag =  ... " + mDrag_x + ", " + mDrag_y);
                        nbody.invoke_setCenterMassPos(mDrag_x, mDrag_y, mDrag_z);
                        mDrag = false;
                    }
                    synchronized (triBuff) {
                        if (dir)
                            nbody.forEach_simulate1(posAlloc, posAlloc);
                        else
                            nbody.forEach_simulate2(posAlloc, posAlloc);

                        rs.finish();
                        if (!USE_BB) {
                            ( (dir) ?triAlloc1:triAlloc1).copyTo(mTriArray);
                            triBuff.position(0);
                            triBuff.put(mTriArray);
                            triBuff.position(0);
                        }
                        publishProgress(triBuff);
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            posAlloc.destroy();
            velAlloc.destroy();
            triAlloc1.destroy();
            if (triAlloc1 != triAlloc2) {
                triAlloc2.destroy();
            }
            nbody.destroy();
            return null;
        }

        @Override
        protected void onProgressUpdate(FloatBuffer... values) {
            if (count > 100) {
                Log.v(TAG, " " + count / ((System.nanoTime() - time) * 1E-9) + " fps");
                count = 0;
                time = System.nanoTime();
            }
            setTriangles(values[0]);
        }
    }
}
