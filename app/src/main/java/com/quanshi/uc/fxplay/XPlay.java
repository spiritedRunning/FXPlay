package com.quanshi.uc.fxplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


/**
 * Created by zhen.liu on 30,Aug,201    8.
 */
public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }



    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // android 8.0设置
        setRenderer(this);

        InitView(holder.getSurface());
    }


    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1) {

    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }


    public native void InitView(Object surface);
}
