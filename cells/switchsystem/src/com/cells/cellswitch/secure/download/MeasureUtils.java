package com.cells.cellswitch.secure.download;

import android.view.View;

public class MeasureUtils {
    /**
     * Measurement for View
     *
     * @param measureSpec Measurement mode and size
     * @param defaultSize Default size
     * @return
     */
    public static int measureView(int measureSpec, int defaultSize) {
        int measureSize;
        //Get the size and mode specified by the user
        int mode = View.MeasureSpec.getMode(measureSpec);
        int size = View.MeasureSpec.getSize(measureSpec);
        //Return size according to mode
        if (mode == View.MeasureSpec.EXACTLY) {
            //The exact mode (specified size and match_parent) directly returns the specified size
            measureSize = size;
        } else {
            //UNSPECIFIED mode, AT_MOST mode (wrap_content), you need to provide the default size
            measureSize = defaultSize;
            if (mode == View.MeasureSpec.AT_MOST) {
                //In AT_MOST (wrap_content) mode, the minimum value of the measured value and the default value needs to be selected
                measureSize = Math.min(measureSize, defaultSize);
            }
        }
        return measureSize;
    }
}
