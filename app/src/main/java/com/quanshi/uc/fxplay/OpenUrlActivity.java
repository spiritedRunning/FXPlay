package com.quanshi.uc.fxplay;


import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class OpenUrlActivity extends AppCompatActivity {

    private Button playFileBtn;
    private Button rtmpBtn;
    private EditText fileUrlEt;
    private EditText rtmpEt;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.openurl);

        fileUrlEt = findViewById(R.id.fileurl);
        playFileBtn = findViewById(R.id.playvideo);
        rtmpEt = findViewById(R.id.rtmpurl);
        rtmpBtn = findViewById(R.id.playrtmp);

        playFileBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Open(fileUrlEt.getText().toString());
                finish();
            }
        });

        rtmpBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Open(rtmpEt.getText().toString());
                finish();
            }
        });
    }


    public native void Open(String url);
}
