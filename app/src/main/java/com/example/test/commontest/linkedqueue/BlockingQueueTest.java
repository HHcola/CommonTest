package com.example.test.commontest.linkedqueue;

import android.util.Log;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingDeque;

/**
 * Created by hewei05 on 15/8/23.
 */
public class BlockingQueueTest {


    public static final String TAG = BlockingQueueTest.class.getSimpleName();

    public class Basket {
        // 定义队列的容量
        BlockingQueue<String> basket = new LinkedBlockingDeque<>(3);

        // 定义生产者
        public void produce() throws InterruptedException {
            basket.put("An apple");
        }
        // 定义消费者
        public String consume() throws InterruptedException {
            return basket.take();
        }
    }


    // 定义生产者
    public class Producer implements Runnable {

        private String source;
        private Basket basket;

        public Producer(String source, Basket basket) {
            this.source = source;
            this.basket = basket;
        }

        @Override
        public void run() {
            try {
                while (true) {
                    Log.d(TAG, "生产者生产苹果 : " + source);
                    basket.produce();
                    Log.d(TAG, "生产者生产苹果结束 : " + source);

                    Thread.sleep(300);
                }
            } catch (InterruptedException e) {
                 e.printStackTrace();
            }

        }
    }


    // 定义苹果消费者
    class Consumer implements Runnable {
        private String instance;
        private Basket basket;

        public Consumer(String instance, Basket basket) {
            this.instance = instance;
            this.basket = basket;
        }

        public void run() {
            try {
                while (true) {
                    // 消费苹果
                    Log.d(TAG, "消费者准备消费苹果：" + instance);
                    Log.d(TAG, basket.consume());
                    Log.d(TAG, "!消费者消费苹果完毕：" + instance);
                    // 休眠1000ms
                    Thread.sleep(1000);
                }
            } catch (InterruptedException ex) {
                System.out.println("Consumer Interrupted");
            }
        }
    }

    public static void testQueue() {
        BlockingQueueTest test = new BlockingQueueTest();

        Log.d(TAG, "Queue Begin");
        // 建立一个装苹果的篮子
        Basket basket = test.new Basket();

        ExecutorService service = Executors.newCachedThreadPool();
        Producer producer = test.new Producer("生产者001", basket);
        Producer producer2 = test.new Producer("生产者002", basket);
        Consumer consumer = test.new Consumer("消费者001", basket);
        service.submit(producer);
        service.submit(producer2);
        service.submit(consumer);
        // 程序运行5s后，所有任务停止
        try {
            Thread.sleep(1000 * 5);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        service.shutdownNow();
        Log.d(TAG,"shutdown");
    }
}
