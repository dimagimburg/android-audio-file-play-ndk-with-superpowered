package com.dima.looper;

import android.app.Instrumentation;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;

import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity {

    boolean isPlaying = false;
    private Timer fakeTouchTimer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Get the device's sample rate and buffer size to enable low-latency Android audio output, if available.
        String samplerateString = null, buffersizeString = null;
        if (Build.VERSION.SDK_INT >= 17) {
            AudioManager audioManager = (AudioManager) this.getSystemService(Context.AUDIO_SERVICE);
            samplerateString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
            buffersizeString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
        }
        Log.e("------------------ 1",samplerateString);
        Log.e("------------------ 1",buffersizeString);
        if (samplerateString == null) samplerateString = "44100";
        if (buffersizeString == null) buffersizeString = "512";
        Log.e("------------------ 2",samplerateString);
        Log.e("------------------ 2",buffersizeString);

        AssetFileDescriptor fileDescriptor = getResources().openRawResourceFd(R.raw.lycka);
        int fileStartOffset = (int) fileDescriptor.getStartOffset();
        int fileLength = (int) fileDescriptor.getLength();
        try {
            fileDescriptor.getParcelFileDescriptor().close();
        } catch (IOException e) {
            android.util.Log.d("", "Close error.");
        }

        SetTempFolder(getCacheDir().getAbsolutePath());
        Metronome(Integer.parseInt(samplerateString),Integer.parseInt(buffersizeString),getPackageResourcePath(),fileStartOffset,fileLength,120,4,4);
        findViewById(R.id.playPauseButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                playPause();
            }
        });

    }

    public void playPause(){
        isPlaying = !isPlaying;

        // Sending fake touches every second helps sustaining CPU rate.
        // This is not necessary for this little app, but might be helpful for projects with big audio processing needs.
        if (isPlaying) {
            TimerTask fakeTouchTask = new TimerTask() {
                public void run() {
                    try {
                        Instrumentation instrumentation = new Instrumentation();
                        instrumentation.sendKeyDownUpSync(KeyEvent.KEYCODE_BACKSLASH);
                    } catch(java.lang.Exception e) {
                        assert true;
                    }
                }
            };
            fakeTouchTimer = new Timer();
            fakeTouchTimer.schedule(fakeTouchTask, 1000, 1000);
        } else {
            fakeTouchTimer.cancel();
            fakeTouchTimer.purge();
        }

        onMetronomePlay(isPlaying);
        if(isPlaying){
            ((Button) findViewById(R.id.playPauseButton)).setText("playing");
        } else {
            ((Button) findViewById(R.id.playPauseButton)).setText("stopped");
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onRestart() {
        super.onRestart();
    }

    // TODO: make this method Metronome's model
    private native void SetTempFolder(String path);
    private native void Metronome(int sampleRate, int bufferSize, String apkPath, int fileStartOffset, int fileLength, int bpm, int beats, int bars);
    private native void onMetronomePlay(boolean play);

    static {
        System.loadLibrary("looper");
    }
}
