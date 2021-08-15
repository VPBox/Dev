package com.example.simpleperf.simpleperfexampleofkotlin

import android.support.v7.app.AppCompatActivity
import android.os.Bundle

class SleepActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sleep)
        createRunSleepThread();
    }

    fun createRunSleepThread() {
        object : Thread() {
            var counter = 0
            var totalRunTimeInNs: Long = 0
            var totalSleepTimeInNs: Long = 0

            fun RunFunction(): Long {
                var startTimeInNs = System.nanoTime()
                var i = 0
                while (i < 10000000) {
                    counter = callFunction(counter)
                    i++
                }
                return System.nanoTime() - startTimeInNs
            }

            fun SleepFunction(sleepTimeInNs: Long): Long {
                var startTimeInNs = System.nanoTime()
                Thread.sleep(sleepTimeInNs / 1000000, (sleepTimeInNs % 1000000).toInt())
                return System.nanoTime() - startTimeInNs
            }

            override fun run() {
                while (true) {
                    totalRunTimeInNs += RunFunction()
                    if (totalSleepTimeInNs < totalRunTimeInNs) {
                        totalSleepTimeInNs += SleepFunction(totalRunTimeInNs - totalSleepTimeInNs)
                    }
                }
            }

            fun callFunction(i: Int): Int {
                return i + 1
            }
        }.start()
    }
}
