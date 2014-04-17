package com.example.xmpp;

import java.io.File;

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManager;
import org.jivesoftware.smack.Connection;
import org.jivesoftware.smack.ConnectionConfiguration;
import org.jivesoftware.smack.ConnectionConfiguration.SecurityMode;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.SASLAuthentication;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;

import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class MainActivity extends ActionBarActivity {

	Connection connection;
	TextView tv;
	ChatManager chatmanager;
	MessageListener  messagelistener = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		if (savedInstanceState == null) {
			getSupportFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
		tv = (TextView) findViewById(R.id.tv);
		
		 //tv.append("\nStarting Connection");
		 ConnectionConfiguration config = new
		 ConnectionConfiguration("tagbin.in", 5222);
		 config.setCompressionEnabled(false);
		 config.setSASLAuthenticationEnabled(true);
		 config.setSecurityMode(SecurityMode.disabled);
		 SASLAuthentication.supportSASLMechanism("PLAIN", 0);

		 connection = new XMPPConnection(config);
		 Thread t = new Thread(){
			 
			 public void run(){
				 try {
						connection.connect();
						//tv.append("\n Connected");
					} catch (XMPPException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
						Log.d("XMPP", "Connected to " + connection.getHost());
						//tv.append( "Connected to " + connection.getHost());
					}
					if (connection.isConnected()) {
						Log.d("XMPP", "Connected to " + connection.getHost()+" "+connection.getServiceName());
						try {
							connection.login("USERNAME", "PASSWORD");  //dont include @DOMAIN
						} catch (XMPPException e) {
							// TODO Auto-generated catch block
							
							e.printStackTrace();
							Log.d("XMPP",
									"Login Failed " + e.getMessage() + " "
											+ e.getLocalizedMessage());
							//Log.d("XMPP","Login Failed Continue "+e.getStreamError().toString());
						}

					} else
						Log.d("XMPP", "Not Connected");

					if (connection.isAuthenticated()) {
						Log.d("XMPP", "Connected  after lgin");
					
					} else
						Log.d("XMPP", "Not Connected after login");
			 }
		 };
		 t.start();
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {
		

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);	 
			 
			return rootView;
		}
	}

}
