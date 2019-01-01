package com.quanshi.uc.fxplay;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.SeekBar;

public class MainActivity extends AppCompatActivity implements Runnable {
    private static final String TAG = "MainActivity";
    private SeekBar seek;
    private Thread th;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        setContentView(R.layout.activity_main);
        seek = findViewById(R.id.aplayseek);
        seek.setMax(1000);

        th = new Thread(this);
        th.start();
    }

    public void OpenButton(View view) {
        Intent intent = new Intent();
        intent.setClass(MainActivity.this, OpenUrlActivity.class);
        startActivity(intent);
    }

    @Override
    public void run() {
        for (;;) {
            seek.setProgress((int)(PlayPos() * 1000));
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public native double PlayPos();
}
