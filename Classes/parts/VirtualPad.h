//
// Created by 藤井 陽介 on 2014/01/24.
//


#ifndef __VirtualPad_H_
#define __VirtualPad_H_

#include "cocos2d.h"


namespace padDirection
{
    /*
    方向を列挙体で指定。
    対応方向は下記参照
    123
    456
    789
     */
    enum kDirection {
        kCenter = 5,
        kUp     = 2,
        kDown   = 8,
        kLeft   = 4,
        kRight  = 6,
        kLeftUp      = 1,
        kLeftDown    = 7,
        kRightUp     = 3,
        kRightDown   = 9,
    };
};

class VirtualPad {

private:
    //Layer
    cocos2d::Layer* Layer;
    //Pad下地
    cocos2d::Sprite* padBack;
    //Pad
    cocos2d::Sprite* padFront;

    //表示フラグ
    bool drawFlag;
    //開始点
    int start_x;
    int start_y;
    //現在位置
    int now_x;
    int now_y;
    //移動量
    int d_x;
    int d_y;
    //移動sin,cos
    float way_x;
    float way_y;
    //最大半径
    int max_r;
    //角度
    float angle;
    //角度テーブル
    float fsin[360];
    float fcos[360];
    //タッチID
    int touchID;


public:
    VirtualPad(cocos2d::Layer* layer);

    void startPad(int x, int y, int touch_id);
    void endPad(int touch_id);
    void update(int x, int y, int touch_id);
    float getCosX();
    float getSinY();
    bool getDrawFlag();
    int get4Way();
    int get8Way();
};


#endif //__VirtualPad_H_
