//
// Created by 藤井 陽介 on 2014/03/11.
//

#include "QuestScene.h"

USING_NS_CC;

QuestScene::QuestScene()
: _virtualPad(new VirtualPad(this))
{
    // コンストラクタ
}

QuestScene::~QuestScene()
{
    // デストラクタ
}

Scene* QuestScene::scene()
{
    Scene *scene = Scene::create();
    QuestScene *layer = QuestScene::create();
    scene->addChild(layer);
    return scene;
}

bool QuestScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    // ---------------------
    // バーチャルパッドとタッチイベント
    // ---------------------
    // バーチャルパッド
//    _virtualPad = new VirtualPad(this);

    // TouchEvent settings
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();

    listener->onTouchesBegan = CC_CALLBACK_2(QuestScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(QuestScene::onTouchesMoved, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(QuestScene::onTouchesCancelled, this);
    listener->onTouchesEnded = CC_CALLBACK_2(QuestScene::onTouchesEnded, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto winSize = Director::getInstance()->getWinSize();

    // ---------------------
    // タイルマップを生成
    // ---------------------
    auto pTiledMap = TMXTiledMap::create("th_cobit_rouka_1f.tmx");

    TMXObjectGroup *objectGroup = pTiledMap->getObjectGroup(Entrances);

    ValueMap spawnPoint = objectGroup->getObject("Start");

    float x = spawnPoint.at("x").asInt();
    float y = spawnPoint.at("y").asInt();

    std::cout << "Start from x：" << x << std::endl;
    std::cout << "Start from y：" << y << std::endl;
    Point start = Point(x, y);
//    pTiledMap->setPosition(start);

//    pTiledMap->setPosition(Point::ZERO);

    std::cout << "Start from x：" << x << std::endl;
    std::cout << "Start from y：" << y << std::endl;

    m_baseMapSize = pTiledMap->getMapSize();
    m_baseTileSize = pTiledMap->getTileSize();
    m_baseContentSize = pTiledMap->getContentSize();

    MapIndex startIndex = pointToIndex(start);

    std::cout << "startIndex from x：" << startIndex.x << std::endl;
    std::cout << "startIndex from y：" << startIndex.y << std::endl;
    MapIndex tileIndex = mapIndexToTileIndex(startIndex);

    std::cout << "tileIndex from x：" << tileIndex.x << std::endl;
    std::cout << "tileIndex from y：" << tileIndex.y << std::endl;

    pTiledMap->setPosition(Point(winSize.width/2 - x, winSize.height/2 - y));
    this->addChild(pTiledMap, QuestScene::zTiledMapIndex, QuestScene::kTiledMapTag);

    m_mapManager.init(0, (int)m_baseMapSize.height, 0, (int)m_baseMapSize.width);

    // 障害物をmapManagerに適応する
    auto pColisionLayer = pTiledMap->getLayer("Colision");
    for (int x = 0; x < m_baseMapSize.width; x++)
    {
        for (int y = 0; y < m_baseMapSize.height; y++)
        {
            if (pColisionLayer->getTileAt(Point(x, y)))
            {
                MapIndex mapIndex = {x, y, MoveDirectionType::MOVE_NONE};
                auto tileMapIndex = mapIndexToTileIndex(mapIndex);
                m_mapManager.addObstacle(&tileMapIndex);
            }
        }
    }

    // ---------------------
    // プレイヤー生成
    // ---------------------
    ActorSprite::ActorDto actorDto;
    actorDto.playerId = 22;
    actorDto.imageResId = 1015;
    actorDto.name = "峰　凛太郎";

    // HP
    actorDto.hitPoint = 15;
    actorDto.hitPointLimit = 15;
    // MP
    actorDto.magicPoint = 15;
    actorDto.magicPointLimit = 15;
    // 正気度
    actorDto.sanity = 60;
    actorDto.sanityLimit = 99;

    // パラメーター
    actorDto.actorStr = 11;
    actorDto.actorCon = 8;
    actorDto.actorPow = 12;
    actorDto.actorDex = 9;
    actorDto.actorApp = 14;
    actorDto.actorSize = 16;
    actorDto.actorInt = 17;
    actorDto.actorEdu = 13;

    ActorMapItem actorMapItem;
    actorMapItem.mapDataType = MapDataType::PLAYER;
    // 画面の中心（固定）
    actorMapItem.mapIndex = pointToIndex(Point(winSize.width/2, winSize.height/2));
    actorMapItem.seqNo = 1;
    actorMapItem.moveDist = 1;
    actorMapItem.attackDist = 1;
    actorMapItem.moveDone = false;
    actorMapItem.attackDone = false;

    auto actorSprite = ActorSprite::createWithActorDto(actorDto);
    actorSprite->setPosition(indexToPoint(actorMapItem.mapIndex)); // 画面の中心
    actorSprite->setActorMapItem(actorMapItem);
//    actorSprite->runBottomAction();
    // プレイヤーは画面中心にくるのでmapLayerに追加しない
    this->addChild(actorSprite, QuestScene::zActorBaseIndex, (QuestScene::kActorBaseTag + actorMapItem.seqNo));

    CCLOG("init finished");

    return true;
}

#pragma mark
#pragma mark マップ座標変換

Point QuestScene::indexToPoint(MapIndex mapIndex)
{
    return indexToPoint(mapIndex.x, mapIndex.y);
}

Point QuestScene::indexToPoint(int mapIndex_x, int mapIndex_y)
{
    // タイルサイズを考慮
    return Point(m_baseTileSize.width * mapIndex_x + m_baseTileSize.width * 0.5, m_baseTileSize.height * mapIndex_y + m_baseTileSize.height * 0.5);
}

MapIndex QuestScene::pointToIndex(Point point)
{
    // タイルサイズを考慮
    point.x = point.x - m_baseTileSize.width * 0.5;
    point.y = point.y - m_baseTileSize.height * 0.5;
    return touchPointToIndex(point);
}

MapIndex QuestScene::touchPointToIndex(Point point)
{
    MapIndex mapIndex;
    mapIndex.x = (int)(point.x / m_baseTileSize.width);
    mapIndex.y = (int)(point.y / m_baseTileSize.height);
    return mapIndex;
}

Point QuestScene::indexToPointNotTileSize(MapIndex mapIndex)
{
    return indexToPointNotTileSize(mapIndex.x, mapIndex.y);
}

Point QuestScene::indexToPointNotTileSize(int mapIndex_x, int mapIndex_y)
{
    return Point(m_baseTileSize.width * mapIndex_x, m_baseTileSize.height * mapIndex_y);
}

MapIndex QuestScene::mapIndexToTileIndex(MapIndex mapIndex)
{
    MapIndex tileIndex;
    tileIndex.x = mapIndex.x;
    tileIndex.y = (int)(m_baseMapSize.height - mapIndex.y - 1);
    return tileIndex;
}

#pragma mark
#pragma mark タッチイベント
void QuestScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesBegan");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();

        _virtualPad->startPad((int)location.x,(int)location.y,pTouch->getID());
    }

}

void QuestScene::onTouchesMoved(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesMoved");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();
        _virtualPad->update((int)location.x,(int)location.y,pTouch->getID());
    }
//    this->schedule(schedule_selector(QuestScene::setPlayerPosition), 0.1f);
//    this->schedule(schedule_selector(QuestScene::setViewPlayerCenter));

}

void QuestScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesEnded");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();
        _virtualPad->endPad(pTouch->getID());
    }
//    this->unschedule(schedule_selector(QuestScene::setPlayerPosition));
//    this->unschedule(schedule_selector(QuestScene::setViewPlayerCenter));
}

void QuestScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesCancelled");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();
        _virtualPad->endPad(pTouch->getID());
    }
//    this->unschedule(schedule_selector(QuestScene::setPlayerPosition));
//    this->unschedule(schedule_selector(QuestScene::setViewPlayerCenter));
}