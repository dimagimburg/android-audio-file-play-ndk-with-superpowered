package com.dima.looper.Models;

import com.dima.looper.EventListeners.MetronomeEventListener;

import java.util.ArrayList;
import java.util.Vector;

public class MetronomeTrack extends Track {

    private int bpm;
    private int beats;
    private int bars;
    private boolean isPlaying;
    private Vector<MetronomeEventListener> listeners = new Vector<>();

    public MetronomeTrack(){

    }

    static {
        System.loadLibrary("looper");
    }

    //private native void onMetronomePlay(boolean play);
    private native void onMetronomeStop();
    private native void onMetronomePause();
    private native void onMetronomeChangeBpm(int bpm);
    private native void onMetronomeChangeBeats(int beats);
    private native void onMetronomeChangeBars(int bars);
    private native void onMetronomeChangeVolume(double volume);

}
