//
// Created by 藤井 陽介 on 2014/01/24.
//

#include <iostream>

#include "VirtualPad.h"

using namespace cocos2d;

#define VIRTUAL_PAD_MAX_RATE 40
#define VIRTUAL_PAD_MIN_RATE 10

/**
 *コンストラクタ.
 *@param layer
 */
VirtualPad::VirtualPad(cocos2d::Layer* layer)
{
    //layer
    Layer = layer;
    drawFlag = false;

    CCLOG("VirtualPad: %f, %f", Layer->getContentSize().width,Layer->getContentSize().height);

    //画像の生成
    padBack = Sprite::create("padBack.png");
    
    padBack->setVisible(false);
    padBack->setAnchorPoint(Point(0.5f,0.5f));

    padFront = Sprite::create("padFront.png");
    padFront->setVisible(false);
    padFront->setAnchorPoint(Point(0.5f,0.5f));

    //レイヤーに追加
    Layer->addChild(padBack,10000);
    Layer->addChild(padFront,10001);

    //最大半径
    max_r = VIRTUAL_PAD_MAX_RATE ;

    //角度毎のx,y位置テーブル作成
    for(int i=0;i<360;i++){
        fsin[i]=(float)sin(i*3.1415926535/180);
        fcos[i]=(float)cos(i*3.1415926535/180);
    }
}

/**
 *Padの表示開始
 *@param x
 *@param y
 */
void VirtualPad::startPad(int x,int y,int touch_id)
{
    // 可動範囲内をタッチしたか？

    std::cout << "drawFlag : " << drawFlag << std::endl;
    CCLOG("startPad");
    if(drawFlag)return;


    CCLOG("startedPad");
    drawFlag = true;
    start_x = x;
    start_y = y;

    now_x = x;
    now_y = y;
    d_x = 0;
    d_y = 0;

    touchID = touch_id;

    angle = 0;
    way_x = 0;
    way_y = 0;

    padBack->setVisible(true);
    padBack->setPosition(Point(start_x, start_y));
    padFront->setVisible(true);
    padFront->setPosition(Point(now_x, now_y));
}

/**
 *Padの表示終了
 */
void VirtualPad::endPad(int touch_id)
{
    if(!drawFlag)return;
    if(touch_id != touchID)return;
    drawFlag = false;

    padBack->setVisible(false);
    padFront->setVisible(false);
}

/**
 *Padの表示更新
 *@param x
 *@param y
 */
void VirtualPad::update(int x,int y,int touch_id)
{
    LuaLog("update!!");
    if(!drawFlag)return;
    if(touch_id != touchID)return;
    //移動量
    d_x = x-start_x;
    d_y = y-start_y;

    //角度
    angle = atan2f(d_y, d_x);
    //cos,sin
    way_x = cosf(angle);
    way_y = sinf(angle);

    //円移動範囲外か
    if (sqrt(d_x*d_x + d_y*d_y)> max_r) {
        //制限後のボタン位置
        x = (int)(start_x + max_r * cos(angle));
        y = (int)(start_y + max_r * sin(angle));
    }
    //位置をセット
    now_x = x;
    now_y = y;
    padBack->setPosition(Point(start_x, start_y));
    padFront->setPosition(Point(now_x, now_y));

}

/**
 *cos取得
 *@return パッドの角度から算出したcos
 */
float VirtualPad::getCosX()
{
    return way_x;
}

/**
 *sin取得
 *@return パッドの角度から算出したsin
 */
float VirtualPad::getSinY()
{
    return way_y;
}

/**
 *バーチャルパッド使用中かどうかの取得
 *@return drawFlag trueなら表示中
 */
bool VirtualPad::getDrawFlag()
{
    return drawFlag;
}

/**
 *方向取得
 *@return int 4方向のうちどの方向かを返す.
 *方向は⬇内容で表す 5が中心
 *789
 *456
 *123
 */
int VirtualPad::get4Way()
{

    //移動していないか
    if(d_x*d_x + d_y*d_y< VIRTUAL_PAD_MIN_RATE*VIRTUAL_PAD_MIN_RATE){
        return padDirection::kCenter;
    }
    //上
    if(way_x<=fcos[45]&&way_x>=fcos[135]&&way_y>0){
        return padDirection::kUp;
    }
    //下
    if(way_x<=fcos[315]&&way_x>=fcos[225]&&way_y<0){
        return padDirection::kDown;
    }
    //左
    if(way_y<=fsin[135]&&way_y>=fsin[225]&&way_x<0){
        return padDirection::kLeft;
    }
    //右
    if(way_y<=fsin[45]&&way_y>=fsin[315]&&way_x>0){
        return padDirection::kRight;
    }
    return -1;
}