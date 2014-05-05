//
// Created by 藤井 陽介 on 2014/02/11.
//


#ifndef __MapScene_H_
#define __MapScene_H_

#include "cocos2d.h"
#include "Config.h"
#include "Dice.h"
#include "VirtualPad.h"

USING_NS_CC;

class MapScene : public Layer
{
protected:
    enum kSprite
    {
        kPlayerTag    = 1,
    };

    enum kParameter
    {
        kEncounterRate = 50, // エンカウント率
    };

private:
    TMXTiledMap *_tileMap;
    TMXLayer    *_meta;
    Sprite      *_player;
    SpriteFrameCache *_frameCache;

    Dice dice;
    VirtualPad *_virtualPad;

    // playerから現在位置の場所を取得
    char* _mapFile = Map_Cobit_Corridor_1F;

public:

    // コンストラクタ、デストラクタ
    MapScene();
    virtual ~MapScene();
    static Scene* scene();

    virtual bool init();

    CREATE_FUNC(MapScene);

    // 画面の視野をプレイヤーの周辺に設定
    void setViewPlayerCenter(float dt);
    // キャラクタの位置をセット
    void setPlayerPosition(float dt);
    // キャラアニメーション
    void playHeroMoveAnimationFromPosition(Point from, Point to);

    // タップした箇所のタイル座標を取得
    Point tileCoordForPosition(Point position);

    // 距離取得
    float getPointLength(Point from, Point to);

    // 距離から継続時間を算出
    float getDurationFromLength(float length);

    // エンカウント判定
    inline bool isEncountered(int rollRsulte)
    {
        return kEncounterRate > rollRsulte ? true :false;
    }

    // マルチタッチ
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *pEvent);

    virtual void onTouchesMoved(const std::vector<Touch*>& touches,Event *pEvent);

    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *pEvent);

    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event *pEvent);

};


#endif //__MapScene_H_
