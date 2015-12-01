package com.example.test.commontest.ui.stamp;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.util.AttributeSet;
import android.view.View;

import com.example.test.commontest.R;

/**
 * Created by hewei05 on 15/8/5.
 */
public class StampView extends View {

    private static final int TEXT_SIZE = 32;  //文字大小
    private static final int OUTSIDE_RING_WIDTH = 10; //圆环宽度
    private static final float TEXT_ANGLE = 120f;  //文字扇形排列的角度
    private static final int RADIUS = 180;  //印章半径
    private static final int SPACING = 36;  //图片与文字间距
    private static final float STAMP_ROTATE = 15f;  //印章旋转角度

    private static final int TEXT_COLOR = 0xFF000000; //文字颜色
    private static final int CIRCLE_COLOR = 0xFFFF0000; //圆环颜色

    private String mText;
    private Bitmap source;

    private final Paint mTextPaint = new Paint();
    private final Paint mCirclePaint = new Paint();

    public StampView(Context context) {
        super(context);
        init();
    }

    public StampView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public StampView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init();
    }

    private void init() {
        mTextPaint.setColor(TEXT_COLOR);
        mTextPaint.setTextAlign(Paint.Align.CENTER);

        mCirclePaint.setColor(CIRCLE_COLOR);
        mCirclePaint.setStyle(Paint.Style.STROKE);
        mCirclePaint.setAntiAlias(true);

        source = BitmapFactory.decodeResource(getResources(), R.drawable.abc_ic_ab_back_mtrl_am_alpha);
    }

    @Override
    protected void onDraw(Canvas Canvas) {
        // 获取宽高
        int width = this.getWidth();
        int height = this.getHeight();

        mTextPaint.setTextSize(TEXT_SIZE);

        float textY = height / 2 - RADIUS + OUTSIDE_RING_WIDTH + TEXT_SIZE;
        mText = mText != null ? mText : "";
        // 把文字拆成字符数组
        char[] chs = mText.toCharArray();

        // 画圆环
        mCirclePaint.setStrokeWidth(OUTSIDE_RING_WIDTH);
        Canvas.drawCircle(width / 2, height / 2, RADIUS - OUTSIDE_RING_WIDTH / 2, mCirclePaint);

        Canvas.save();
        Canvas.rotate(STAMP_ROTATE, width / 2, height / 2);

        // 中间圆形位图的半径
        int radius = RADIUS - OUTSIDE_RING_WIDTH - TEXT_SIZE - SPACING;
        Bitmap image = createCircleImage(source, radius * 2);
        Canvas.drawBitmap(image, (width - image.getWidth()) / 2, (height - image.getHeight()) / 2,
                mCirclePaint);
        image.recycle();

        Canvas.rotate(-TEXT_ANGLE / 2, width / 2, height / 2);
        // 每个文字间的角度间隔
        float spaceAngle = TEXT_ANGLE / (chs.length - 1);
        for(int i = 0; i < chs.length; i++) {
            String s = String.valueOf(chs[i]);
            Canvas.drawText(s, width / 2, textY, mTextPaint);
            Canvas.rotate(spaceAngle, width / 2, height / 2);
        }
        Canvas.restore();
    }

    /**
     * 创建圆形位图
     * @param source 原图片位图
     * @param diameter 圆形位图的直径
     * @return
     */
    private Bitmap createCircleImage(Bitmap source, int diameter) {
        final Paint paint = new Paint();
        paint.setAntiAlias(true);
        paint.setFilterBitmap(true);

        int width = source.getWidth();
        int height = source.getHeight();
        Bitmap clipBitmap;
        if(width > height) {
            int x = (width - height) / 2;
            int y = 0;
            clipBitmap = Bitmap.createBitmap(source, x, y, height, height);
        } else if(width < height) {
            int x = 0;
            int y = (height - width) / 2;
            clipBitmap = Bitmap.createBitmap(source, x, y, width, width);
        } else {
            clipBitmap = source;
        }

        Bitmap scaledBitmap = Bitmap.createScaledBitmap(clipBitmap, diameter, diameter, true);
        Bitmap outputBitmap = Bitmap.createBitmap(diameter, diameter, Bitmap.Config.ARGB_8888);
        Canvas Canvas = new Canvas(outputBitmap);
        Canvas.drawCircle(diameter / 2, diameter / 2, diameter / 2, paint);
        paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_IN));
        Canvas.drawBitmap(scaledBitmap, 0, 0, paint);
//        source.recycle();
        clipBitmap.recycle();
        scaledBitmap.recycle();
        return outputBitmap;
    }

    public void setText(String text) {
        mText = text;
    }

    public String getText() {
        return mText;
    }

    public void setTextColor(int textColor) {
        mTextPaint.setColor(textColor);
    }

    public void setCircleColor(int circleColor) {
        mCirclePaint.setColor(circleColor);
    }

    public void setBitmap(int id) {
        source = BitmapFactory.decodeResource(getResources(), id);
    }
}
