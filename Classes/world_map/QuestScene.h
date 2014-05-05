//
// Created by 藤井 陽介 on 2014/03/11.
//


#ifndef __QuestScene_H_
#define __QuestScene_H_

#include "cocos2d.h"
#include "ActorSprite.h"
#include "Config.h"
#include "Dice.h"
#include "VirtualPad.h"
#include "MapManager.h"

class QuestScene : public cocos2d::Layer
{
protected:
    enum Tag {
        kTiledMapTag          = 1,
        kActorBaseTag         = 100000,
        kMiniMapTag           = 150000,
        kStatusBarTag         = 200000,
        kStatusBar2Tag        = 200001,
        kGameLogTag           = 210000,
        kItemListTag          = 220000,
        kItemDetailTag        = 221000,
        kMenuTag              = 300000,
        kModalTag             = 900000,
    };
    enum zIndex {
        zTiledMapIndex   = 1,
        zActorBaseIndex  = 100000,
        zMiniMapIndex    = 150000,
        zStatusBarIndex  = 200000,
        zStatusBar2Index = 200001,
        zGameLogIndex    = 210000,
        zItemListIndex   = 220000,
        zItemDetailIndex = 221000,
        zMenuIndex       = 300000,
        zModalIndex      = 900000,
    };

private:
    Dice dice;
    VirtualPad *_virtualPad;

    // ゲーム管理
//    GameStatus m_gameStatus;
    int m_TurnCount;
//    void changeGameStatus(GameStatus gameStatus);
    void enemyTurn();

    // マップベース情報
    cocos2d::Size m_baseContentSize;
    cocos2d::Size m_baseMapSize;
    cocos2d::Size m_baseTileSize;

    // マップ制御
    MapManager m_mapManager;

    // マップ座標変換
    cocos2d::Point indexToPoint(int mapIndex_x, int mapIndex_y);
    cocos2d::Point indexToPoint(MapIndex mapIndex);
    cocos2d::Point indexToPointNotTileSize(int mapIndex_x, int mapIndex_y);
    cocos2d::Point indexToPointNotTileSize(MapIndex mapIndex);

    MapIndex pointToIndex(cocos2d::Point point);
    MapIndex touchPointToIndex(cocos2d::Point point);
    // MapIndexからtileIndexに変換
    MapIndex mapIndexToTileIndex(MapIndex mapIndex);

    // キャラクター初期位置




public:
    // コンストラクタ、デストラクタ
    QuestScene();
    virtual ~QuestScene();
    static cocos2d::Scene* scene();

    virtual bool init();

    CREATE_FUNC(QuestScene);
    // マルチタッチ
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
};


#endif //__QuestScene_H_
