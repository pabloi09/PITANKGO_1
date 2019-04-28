package upm.sdg2.pitankgo;

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

public class MainActivity extends AppCompatActivity {
    String url="";
    EditText ipeT;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ipeT=findViewById(R.id.eT);
        ipeT.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                if (actionId == EditorInfo.IME_ACTION_DONE) {
                    empezar(null);//match this behavior to your 'Send' (or Confirm) button

                }
                return true;
            }
        });
    }

    public void empezar(View view) {
        fillUrl();
        if(URLUtil.isValidUrl(url)){
            Intent i=new Intent(this,GameActivity.class);
            i.putExtra("url",url);
            startActivityForResult(i,0);
        }else{
            Toast.makeText(this,"No se ha podido establecer conexión con la Url proporcionada", Toast.LENGTH_LONG).show();
        }
    }

    private void fillUrl() {
        url="http://"+ipeT.getText().toString()+":8080/?action=stream";
    }
}
