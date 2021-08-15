package com.android.rs.unittest;

import com.android.rs.unittest.UnitTest;

import android.app.ListActivity;
import android.content.Context;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public abstract class RSListActivity extends ListActivity {
    private static final String TAG = RSListActivity.class.getSimpleName();

    protected abstract Iterable<Class<? extends UnitTest>> getUnitTests() throws Exception;

    protected abstract void logStartUnitTest(UnitTest test);
    protected abstract void logEndUnitTest(UnitTest test);

    @Override
    public void onCreate(Bundle bundle) {
        try {
            super.onCreate(bundle);

            List<UnitTest> validUnitTests = new ArrayList<>();

            Iterable<Class<? extends UnitTest>> testClasses = getUnitTests();

            for (Class<? extends UnitTest> testClass : testClasses) {
                UnitTest test = testClass.getDeclaredConstructor(Context.class).newInstance(this);
                validUnitTests.add(test);
            }

            ArrayAdapter<UnitTest> adapter = new ArrayAdapter<UnitTest>(this,
                    android.R.layout.simple_list_item_1, validUnitTests) {
                @Override
                public View getView(int position, View convertView, ViewGroup parent) {
                    View view = super.getView(position, convertView, parent);

                    TextView textView = (TextView) view.findViewById(android.R.id.text1);
                    UnitTest test = getItem(position);
                    String textSuffix = "";
                    int textColor = Color.BLACK;
                    switch (test.getResult()) {
                        case UT_RUNNING:
                            textSuffix = " - RUNNING";
                            break;
                        case UT_SUCCESS:
                            textSuffix = " - PASS";
                            textColor = Color.GREEN;
                            break;
                        case UT_FAIL:
                            textSuffix = " - FAIL";
                            textColor = Color.RED;
                            break;
                    }
                    String text = String.format("%s%s", test.toString(), textSuffix);
                    textView.setText(text);
                    textView.setTextColor(textColor);

                    return view;
                }
            };
            setListAdapter(adapter);

            Runnable run = () -> {
                for (UnitTest unitTest : validUnitTests) {
                    RSListActivity.this.logStartUnitTest(unitTest);
                    unitTest.runTest();
                    RSListActivity.this.logEndUnitTest(unitTest);
                    RSListActivity.this.runOnUiThread(adapter::notifyDataSetChanged);
                }
            };
            new Thread(run).start();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
