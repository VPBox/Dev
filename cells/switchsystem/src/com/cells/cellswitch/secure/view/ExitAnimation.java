package com.cells.cellswitch.secure.view;

import android.graphics.Camera;
import android.graphics.Matrix;
import android.view.animation.Animation;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.Transformation;

public class ExitAnimation extends Animation { 
	int centerX, centerY;
	Camera camera = new Camera();

	@Override
	public void initialize(int width, int height, int parentWidth, int parentHeight) { 
		super.initialize(width, height, parentWidth, parentHeight);
		centerX = width / 2;
		centerY = width / 2;
		setDuration(4 * 1000);
		setInterpolator(new DecelerateInterpolator());
	}

	@Override
	protected void applyTransformation(float interpolatedTime, Transformation t) { 
		final Matrix matrix = t.getMatrix();
		camera.save();

		camera.setLocation(0,0,180);
		camera.rotateY(180 * interpolatedTime);

		camera.getMatrix(matrix);

		matrix.preTranslate(-centerX, -centerY);
		matrix.postTranslate(centerX,centerY);
		camera.restore();
	} 
}