package upm.sdg2.pitankgo;

import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.ImageView;
import android.widget.VideoView;

public class GameActivity extends AppCompatActivity {
    private int page;
    private ImageView look;
    private WebView webView;
    private ViewGroup.LayoutParams params;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);
        String url=getIntent().getStringExtra("url");
        webView = findViewById(R.id.video);
        webView.loadUrl(url);
        webView.getSettings().setLoadWithOverviewMode(true);
        webView.getSettings().setUseWideViewPort(true);
        page=0;
        look=findViewById(R.id.look);
        params=webView.getLayoutParams();
    }

    public void before(View view) {
        switch (page){
            case 0:
                page=3;
                params.width=toPixels(640);
                look.setImageResource(R.drawable.mira4);
                break;
            case 1:
                page--;
                params.width=toPixels(380);
                look.setImageResource(R.drawable.mira);
                break;
            case 2:
                page--;
                params.width=toPixels(640);
                look.setImageResource(R.drawable.mira2);
                break;
            case 3:
                page--;
                params.width=toPixels(640);
                look.setImageResource(R.drawable.mira3);
                break;
        }
        webView.setLayoutParams(params);
    }

    public void next(View view) {
        switch (page){
            case 0:
                page++;
                params.width=toPixels(640);
                look.setImageResource(R.drawable.mira2);
                break;
            case 1:
                page++;
                params.width=toPixels(640);
                look.setImageResource(R.drawable.mira3);
                break;
            case 2:
                page++;
                params.width=toPixels(640);
                look.setImageResource(R.drawable.mira4);
                break;
            case 3:
                page=0;
                params.width=toPixels(380);
                look.setImageResource(R.drawable.mira);
                break;
        }
        webView.setLayoutParams(params);
    }
    private int toPixels(int dps){
        final float scale = getResources().getDisplayMetrics().density;
        return (int) (dps * scale + 0.5f);
    }
}
