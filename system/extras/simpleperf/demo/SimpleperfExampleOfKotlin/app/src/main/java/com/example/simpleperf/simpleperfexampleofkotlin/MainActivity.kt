package com.example.simpleperf.simpleperfexampleofkotlin

import android.support.v7.app.AppCompatActivity
import android.os.Bundle

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        createBusyThread()
    }

    fun createBusyThread() {
        object : Thread() {
            var i = 0

            override fun run() {
                while (true) {
                    i = callFunction(i)
                }
            }

            fun callFunction(i: Int): Int {
                return i + 1
            }
        }.start()
    }
}
