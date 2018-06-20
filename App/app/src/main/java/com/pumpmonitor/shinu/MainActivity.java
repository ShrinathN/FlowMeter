package com.pumpmonitor.shinu;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.DataInputStream;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    //I'll declare all the variables i am going to use here
    byte per_second_pulses = 0;
    byte per_minute_pulses = 0;

    short runtime_today_seconds = 0;
    int total_pulses_today = 0;

    short runtime_yesterday_seconds = 0;
    int total_pulses_yesterday = 0;

    boolean keepRunning = false;

    String statusString = "Not Connected";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView TextView_status = (TextView)findViewById(R.id.textView_status);
        TextView_status.setText(statusString);

        final TextView TextView_perSecondCounter = (TextView)findViewById(R.id.textView_pulses_second);
        final TextView TextView_perMinuteCounter = (TextView)findViewById(R.id.textView_pulses_minute);

        final TextView TextView_runtimeToday = (TextView)findViewById(R.id.textView_runtime_today);
        final TextView TextView_totalPulsesToday = (TextView)findViewById(R.id.textView_total_pulses_today);

        final TextView TextView_runtimeYesterday = (TextView)findViewById(R.id.textView_runtime_yesterday);
        final TextView TextView_totalPulsesYesterday = (TextView)findViewById(R.id.textView_total_pulses_yesterday);

        final Handler uiHandler = new Handler();
        final Runnable uiUpdateRunnable = new Runnable() {
            @Override
            public void run() {
                TextView_perSecondCounter.setText(Integer.toString(per_second_pulses));
                TextView_perMinuteCounter.setText(Integer.toString(per_minute_pulses));

                TextView_runtimeToday.setText(Integer.toString((((runtime_today_seconds >> 8) & 0xff) | ((runtime_today_seconds << 8) & 0xff00)) / 60)
                                + ":" +
                                Integer.toString((((runtime_today_seconds >> 8) & 0xff) | ((runtime_today_seconds << 8) & 0xff00)) % 60));
                TextView_totalPulsesToday.setText(Integer.toString(((total_pulses_today << 24) & 0xff000000) | ((total_pulses_today << 8) & 0x00ff0000) | ((total_pulses_today >> 8) & 0x0000ff00) | ((total_pulses_today >> 24) & 0xff)));

                TextView_runtimeYesterday.setText(Integer.toString((((runtime_yesterday_seconds >> 8) & 0xff) | ((runtime_yesterday_seconds << 8) & 0xff00)) / 60)
                        + ":" +
                        Integer.toString((((runtime_yesterday_seconds >> 8) & 0xff) | ((runtime_yesterday_seconds << 8) & 0xff00)) % 60));
                TextView_totalPulsesYesterday.setText(Integer.toString(total_pulses_yesterday));
                //((total_pulses_yesterday << 24) & 0xff000000) | ((total_pulses_yesterday << 8) & 0x00ff0000) | ((total_pulses_yesterday >> 8) & 0x0000ff00) | ((total_pulses_yesterday >> 24) & 0xff)));

                TextView_status.setText(statusString);

                uiHandler.postDelayed(this,500);
            }
        };
        uiHandler.postDelayed(uiUpdateRunnable,0);
    }

    class dataRetriever implements Runnable
    {
        public void run()
        {
            try
            {
                Socket socket = new Socket("192.168.1.90", 8123);
                if(socket.isConnected())
                    statusString = "Connected";
                else
                    statusString = "Couldn't Connected";
                DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());
                while(keepRunning && socket.isConnected())
                {
                    per_second_pulses = dataInputStream.readByte();
                    per_minute_pulses = dataInputStream.readByte();

                    runtime_today_seconds = dataInputStream.readShort();
                    total_pulses_today = dataInputStream.readInt();

                    runtime_yesterday_seconds = dataInputStream.readShort();
                    total_pulses_yesterday = dataInputStream.readInt();
                }
                dataInputStream.close();
                socket.close();
                statusString = "Not Connected";
            }
            catch(Exception e)
            {
                Toast.makeText(MainActivity.this, e.toString(), Toast.LENGTH_LONG).show();
            }
        }
    }

    public void startFunction(View view)
    {
        try
        {
            keepRunning = true;
            dataRetriever dataRetrieverInstance = new dataRetriever();
            Thread runningThread = new Thread(dataRetrieverInstance);
            runningThread.start();
        }
        catch(Exception e)
        {
            Toast.makeText(MainActivity.this, e.toString(), Toast.LENGTH_LONG).show();
        }
    }

    public void stopFunction(View view)
    {
        keepRunning = false;
    }
}
