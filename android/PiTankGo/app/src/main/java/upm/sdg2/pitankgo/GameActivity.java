/*Package------------------------------------------ */
package upm.sdg2.pitankgo;
/*Imports------------------------------------------ */
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.ImageView;
import android.widget.VideoView;
/*Clase corresponiente a la actividad GameActivity */
public class GameActivity extends AppCompatActivity {
    /*Variables de clase*/
    private int page; //Controla la imagen que se esta visualizando
    private ImageView look;
    private WebView webView;
    private ViewGroup.LayoutParams params;
 
    /*Funcion de creacion de la actividad */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /*Inflamos la vista */
        setContentView(R.layout.activity_game);
        /*Recuperamos el link de la actividad de inicio */
        String url=getIntent().getStringExtra("url");
        webView = findViewById(R.id.video);
        /*Cargamos la web en el webView */
        webView.loadUrl(url);
        webView.getSettings().setLoadWithOverviewMode(true);
        webView.getSettings().setUseWideViewPort(true);
        page=0;
        look=findViewById(R.id.look);
        params=webView.getLayoutParams();
    }
    /*Callback del listener del boton izquierdo*/
    public void before(View view) {
        /*Seleccionamos la mirilla que toca y ajustamos el tamaño del webview a ella */
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
    /*Callback del listener del boton derecho*/
    public void next(View view) {
        /*Funciona igual que el otro botón, las mirillas
        se muestran en bucle*/
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
    /*Metodo que transforma de pixeles de densidad independiente a pixeles*/
    private int toPixels(int dps){
        final float scale = getResources().getDisplayMetrics().density;
        return (int) (dps * scale + 0.5f);
    }
}
