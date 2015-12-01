package com.example.test.commontest.linkedqueue;

import android.util.Log;

import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by hewei05 on 15/8/23.
 */
public class ConcurrentLinkedQueueTest {

    private static final String TAG = BlockingQueueTest.TAG;
    private static ConcurrentLinkedQueue<Integer> queue = new ConcurrentLinkedQueue<Integer>();
    private static int count = 2; // 线程个数
    //CountDownLatch，一个同步辅助类，在完成一组正在其他线程中执行的操作之前，它允许一个或多个线程一直等待。
    private static CountDownLatch latch = new CountDownLatch(count);

    public static void linkedQueueTest() {
        try {
            long timeStart = System.currentTimeMillis();
            ExecutorService es = Executors.newFixedThreadPool(4);
            ConcurrentLinkedQueueTest.offer();
            for (int i = 0; i < count; i++) {
                es.submit(new Poll());
            }
            latch.await(); //使得主线程(main)阻塞直到latch.countDown()为零才继续执行
            Log.d(TAG, "cost time " + (System.currentTimeMillis() - timeStart) + "ms");
            es.shutdown();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 生产
     */
    public static void offer() {
        for (int i = 0; i < 100000; i++) {
            queue.offer(i);
        }
    }


    /**
     * 消费
     *
     * @author 林计钦
     * @version 1.0 2013-7-25 下午05:32:56
     */
    static class Poll implements Runnable {
        public void run() {
            // while (queue.size()>0) {
            while (!queue.isEmpty()) {
                Log.d(TAG, "" + queue.poll());
            }
            latch.countDown();
        }
    }
}
