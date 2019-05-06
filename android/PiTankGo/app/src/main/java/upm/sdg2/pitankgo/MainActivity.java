/*Package------------------------------------------ */
package upm.sdg2.pitankgo;
/*Imports------------------------------------------ */
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.webkit.URLUtil;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.Toolbar;
/*Clase corresponiente a la actividad MainActivity.java */
public class MainActivity extends AppCompatActivity {
    /*Variables de clase*/
    String url="";
    EditText ipeT;
     /*Funcion de creacion de la actividad */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ipeT=findViewById(R.id.eT);
    }
    /*Funcion callback del botón de empezar*/
    public void empezar(View view) {
        fillUrl();
        /*Si el formato de la url es válido, comenzamos a jugar
            Nota:esto no garantiza que se conecte a un host, si no hay un servidor mostrando una web en esa url
            se iniciará la siguiente actividad y no se verá nada*/
        if(URLUtil.isValidUrl(url)){
            Intent i=new Intent(this,GameActivity.class);
            i.putExtra("url",url);
            startActivityForResult(i,0);
        }else{
            /*Lanzamos un mensaje diciendo que el formato de la url no es correcto*/
            Toast.makeText(this,"No se ha podido establecer conexión con la Url proporcionada", Toast.LENGTH_LONG).show();
        }
    }
    /*Crea el URL en formato string a partir de la ip introducida*/
    private void fillUrl() {
        url="http://"+ipeT.getText().toString()+":8080/?action=stream";
    }
}
