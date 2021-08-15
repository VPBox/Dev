package com.android.rs.refocus;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.Bundle;
import androidx.renderscript.RenderScript;
import android.util.Log;
import android.util.Pair;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;

public class MainActivity extends Activity {
    private static final int RS_API = 19;
    private static final String TAG = "MainActivity";

    private LinearLayout mLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mLayout = findViewById(R.id.layoutTopMost);

        Intent intent = getIntent();
        if (intent != null) {

            String s = intent.getType();
            if (s != null && s.indexOf("image/") != -1) {
                Uri data = (Uri) intent.getParcelableExtra(Intent.EXTRA_STREAM);
                if (data != null) {

                    try {
                      RenderScript renderScript = RenderScript.create(getApplicationContext(), RS_API);
                      renderScript.setPriority(RenderScript.Priority.NORMAL);

                      // Get input uri to RGBZ
                      RGBZ current_rgbz = new RGBZ(data, getContentResolver(), this);
                      DepthOfFieldOptions current_depth_options = new DepthOfFieldOptions(current_rgbz);

                      // Set image focus settings
                      current_depth_options.setFocusPoint(0.7f, 0.5f);
                      current_depth_options.setBokeh(2f);

                      RsTaskParams rsTaskParam = new RsTaskParams(this, renderScript, current_depth_options);
                      new RsAsyncTaskRunner().execute(rsTaskParam);
                      return;
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                }
            }
        }


        try {
            RenderScript renderScript = RenderScript.create(getApplicationContext(), RS_API);
            renderScript.setPriority(RenderScript.Priority.NORMAL);

            RGBZ rgbz;
            DepthOfFieldOptions options;
            RsTaskParams task;
/*
            rgbz = new RGBZ(getResourceRef(R.drawable.flower2), getContentResolver(), this);
            options = new DepthOfFieldOptions(rgbz);
            options.setFocusPoint(0.5f, 0.5f);
            options.setBokeh(0.25f);

            task = new RsTaskParams(this, renderScript, options);
            new RsAsyncTaskRunner().execute(task);

            rgbz = new RGBZ(getResourceRef(R.drawable.cup), getContentResolver(), this);
            options = new DepthOfFieldOptions(rgbz);
            options.setFocusPoint(0.5f, 0.5f);
            options.setBokeh(0.25f);

            task = new RsTaskParams(this, renderScript, options);
            new RsAsyncTaskRunner().execute(task);

            rgbz = new RGBZ(getResourceRef(R.drawable.apple), getContentResolver(), this);
            options = new DepthOfFieldOptions(rgbz);
            options.setFocusPoint(0.5f, 0.5f);
            options.setBokeh(0.25f);

            task = new RsTaskParams(this, renderScript, options);
            new RsAsyncTaskRunner().execute(task);
*/
/*
            RGBZ current_rgbz = new RGBZ(getResourceRef(R.drawable.refocusimage),
                                         getContentResolver(), this);
*/
            rgbz = new RGBZ(getResourceRef(R.drawable.flower),
                            getResourceRef(R.drawable.flower_depthmap),
                            getContentResolver(), this);
            options = new DepthOfFieldOptions(rgbz);
            options.setFocusPoint(0.5f, 0.5f);
            options.setBokeh(0.025f);

            task = new RsTaskParams(this, renderScript, options);
            new RsAsyncTaskRunner().execute(task);

/*
            RGBZ current_rgbz =
                    RGBZ.createFromBitmapDepthmap(getResourceRef(R.drawable.balls),
                                                  getResourceRef(R.drawable.balls_depthmap1),
                                                  getContentResolver(), this);
*/
            rgbz = RGBZ.createFromPFMDepthmap(getResourceRef(R.drawable.balls),
                                              getResourceRef(R.drawable.balls_depthmap),
                                              getContentResolver(), this);
            options = new DepthOfFieldOptions(rgbz);
            options.setFocusPoint(0.2f, 0.167f);
            options.setBokeh(2.0f);
            options.setDepthOfField(0.005f);

            task = new RsTaskParams(this, renderScript, options);
            new RsAsyncTaskRunner().execute(task);

            rgbz = RGBZ.createFromPFMDepthmap(getResourceRef(R.drawable.sculpture),
                                              getResourceRef(R.drawable.sculpture_depthmap),
                                              getContentResolver(), this);
            options = new DepthOfFieldOptions(rgbz);
            // Set image focus settings
            options.setFocusPoint(0.33f, 0.66f);
            options.setBokeh(0.1f);
            options.setDepthOfField(0.2f);

            task = new RsTaskParams(this, renderScript, options);
            new RsAsyncTaskRunner().execute(task);

            return;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class RsTaskParams {
        MainActivity mContext;
      RenderScript mRenderScript;
      DepthOfFieldOptions mOptions;

        public ImageView mImageView1;
        public ImageView mImageView2;
        public TextView  mTextView;

      RsTaskParams(MainActivity context,
                   RenderScript renderScript,
                   DepthOfFieldOptions options) {
          mContext = context;
        mRenderScript = renderScript;
        mOptions = options;

        LayoutParams lparams = new LayoutParams(LayoutParams.WRAP_CONTENT,
                                                LayoutParams.WRAP_CONTENT);
        mImageView1 = new ImageView(context);
        mImageView1.setLayoutParams(lparams);
        context.mLayout.addView(mImageView1);

        mImageView2 = new ImageView(context);
        mImageView2.setLayoutParams(lparams);
        context.mLayout.addView(mImageView2);

        mTextView = new TextView(context);
        mTextView.setLayoutParams(lparams);
        context.mLayout.addView(mTextView);
      }

    }

    private class Result {
        public ArrayList<Pair<String,Long>> timingsOld;
        public ArrayList<Pair<String,Long>> timingsNew;
        public double psnr;

        public Result(ArrayList<Pair<String,Long>> timingsOld,
                      ArrayList<Pair<String,Long>> timingsNew,
                      double psnr) {
            this.timingsOld = timingsOld;
            this.timingsNew = timingsNew;
            this.psnr = psnr;
        }
    }

    private class RsAsyncTaskRunner extends AsyncTask<RsTaskParams, ImageView, Result> {
      Bitmap outputImageOld;
      Bitmap outputImage;
      Bitmap outputImageNew;
      TextView textView;

      @Override
      protected Result doInBackground(RsTaskParams... params) {

        RenderScriptTask renderScriptTask = new RenderScriptTask(params[0].mRenderScript, RenderScriptTask.script.f32);
        outputImageOld = outputImage = renderScriptTask.applyRefocusFilter(params[0].mOptions);
        final ImageView v = params[0].mImageView1;
        publishProgress(v);

        RenderScriptTask renderScriptTaskNew = new RenderScriptTask(params[0].mRenderScript, RenderScriptTask.script.d1new);
        outputImageNew = outputImage = renderScriptTaskNew.applyRefocusFilter(params[0].mOptions);
        final ImageView v2 = params[0].mImageView2;
        publishProgress(v2);

        double psnr = ImageCompare.psnr(outputImageOld, outputImage);
        textView = params[0].mTextView;

        return new Result(renderScriptTask.timings, renderScriptTaskNew.timings, psnr);
      }

      protected  void onPostExecute(Result result) {
          StringBuffer buffer = new StringBuffer();
          DecimalFormat formatter = new DecimalFormat("#,###");

          buffer.append("PSNR: " + String.format("%.02f", result.psnr) + "\n");
          long sum1 = 0, sum2 = 0;
          for (int i = 0; i < result.timingsOld.size(); i++) {
              String tag = result.timingsOld.get(i).first;
              long t1 = result.timingsOld.get(i).second.longValue();
              long t2 = result.timingsNew.get(i).second.longValue();
              buffer.append(tag + ":" + formatter.format(t1) + ":" +
                            formatter.format(t2) + ":"  +
                            String.format("%.02f", (double) t1 / t2) + "\n");
              sum1 += t1;
              sum2 += t2;
          }
          buffer.append("Sum (ms):" +
                        String.format("%.02f", (double)sum1 / 1000000) + ":" +
                        String.format("%.02f", (double)sum2 / 1000000) + ":" +
                        String.format("%.02f", (double) sum1 / sum2) + "\n");
          textView.setText(buffer.toString());
      }

      protected void onProgressUpdate(ImageView... progress) {
          progress[0].setImageBitmap(outputImage);
      }
    }

    Uri getLocalRef() {


        File folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);

        Log.v(TAG, "DIRECTORY_DOCUMENTS = " + folder.getAbsolutePath());
        ;
        File f = findJpeg(folder);
        if (f != null) {
            Log.v(TAG, "File = " + f);
            return Uri.fromFile(f);
        }
        return null;
    }

    Uri getResourceRef(int resID) {
        Context context = getApplicationContext();
        Uri path = Uri.parse(ContentResolver.SCHEME_ANDROID_RESOURCE + "://" +
            context.getResources().getResourcePackageName(resID) + '/' +
            context.getResources().getResourceTypeName(resID) + '/' +
            context.getResources().getResourceEntryName(resID));
        return path;
    }

    private File findJpeg(File dir) {

        File[] files = dir.listFiles();
        if (files == null) return null;
        for (int i = 0; i < files.length; i++) {
            if (files[i].isDirectory() && !files[i].getName().startsWith(".")) {
                File ret = findJpeg(files[i]);
                if (ret != null) {
                    Log.v(TAG, "returning " + ret.getAbsolutePath());
                    return ret;
                }
                continue;
            }
            if (files[i].getName().toLowerCase().endsWith(".jpg")) {
                Log.v(TAG, "returning " + files[i].getAbsolutePath());
                return files[i];

            }
        }
        return null;
    }

}
