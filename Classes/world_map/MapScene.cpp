//
// Created by 藤井 陽介 on 2014/02/11.
//

#include "MapScene.h"
#include "Config.h"

USING_NS_CC;

MapScene::MapScene()
{
    // コンストラクタ
}

MapScene::~MapScene()
{
    // デストラクタ
}

Scene* MapScene::scene()
{

    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    MapScene *layer = MapScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool MapScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    // バーチャルパッド
    _virtualPad = new VirtualPad(this);

    _tileMap = TMXTiledMap::create(_mapFile);
    this->addChild(_tileMap,0);

    TMXObjectGroup *objectGroup = _tileMap->getObjectGroup(Entrances);

    // tmx上のSpawnPointの座標を取得
    ValueMap spawnPoint = objectGroup->getObject("SpawnPoint");

    int x = spawnPoint.at("x").asInt();
    int y = spawnPoint.at("y").asInt();

    // テクスチャアトラスからのキャラ生成
    _frameCache = SpriteFrameCache::getInstance();
    _frameCache->addSpriteFramesWithFile(Player_Plist);

    _player = Sprite::createWithSpriteFrameName("male_walkcycle_e_01.png");
    // プレイヤーをタグ識別
//    _player->setTag(kPlayerTag);

    // スプライトに座標セット
    _player->setPosition(Point(x,y));
    _player->setScale(1);
    _player->setAnchorPoint(Point(0.5,0));

    Point playerPos = _player->getPosition();

    std::cout << "init from x：" << playerPos.x << std::endl;
    std::cout << "init from y：" << playerPos.y << std::endl;
    _tileMap->addChild(_player, 1, kPlayerTag);

    std::cout << "added from x：" << playerPos.x << std::endl;
    std::cout << "added from y：" << playerPos.y << std::endl;

    auto playerPos2 = ((Sprite*)_tileMap->getChildByTag(kPlayerTag))->getPosition();


    std::cout << "from2 x：" << playerPos2.x << std::endl;
    std::cout << "from2 y：" << playerPos2.y << std::endl;
    _player->setPosition(Point(x,y));
    _meta = _tileMap->getLayer("Meta");
    // プレイヤーの目からは見えなくする
    _meta->setVisible(false);

    // 画面の表示座標をセット
    this->setViewPlayerCenter(0.1f);

    // タッチイベント
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();

    listener->onTouchesBegan = CC_CALLBACK_2(MapScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(MapScene::onTouchesMoved, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(MapScene::onTouchesCancelled, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MapScene::onTouchesEnded, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MapScene::setViewPlayerCenter(float dt)
{
    CCLOG("setViewPlayerCenter");
//    auto playerPos = ((Sprite*)_tileMap->getChildByTag(kPlayerTag))->getPosition();
//    Point playerPos = ((Sprite *)this->getChildByTag(kPlayerTag))->getPosition();

    auto playerPos = _player->getPosition();

    std::cout << "from x：" << playerPos.x << std::endl;
    std::cout << "from y：" << playerPos.y << std::endl;

    auto winSize = Director::getInstance()->getWinSize();
    std::cout << "width：" << winSize.width << std::endl;
    std::cout << "height：" << winSize.height << std::endl;

    int x = MAX(playerPos.x, winSize.width/2);
    int y = MAX(playerPos.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);

    auto viewPoint = Point(winSize.width/2 - x, winSize.height/2 - y);

    std::cout << "viewPoint x：" << viewPoint.x << std::endl;
    std::cout << "viewPoint y：" << viewPoint.y << std::endl;

    _tileMap->setPosition(viewPoint);

}
void MapScene::setPlayerPosition(float dt)
{

    CCLOG("setPlayerPosition");
    int way = -1,d_x = 0,d_y = 0;
    if(_virtualPad->getDrawFlag()){
        // 方向取得
        way = _virtualPad->get4Way();
        CCLOG("VirtualPad get4Way is : %d", way);

        switch (way) {
            // 真ん中
            case padDirection::kCenter : break;
                // 上
            case padDirection::kUp : d_y = 1; break;
                // 下
            case padDirection::kDown : d_y = -1; break;
                // 左
            case padDirection::kLeft : d_x = -1; break;
                // 右
            case padDirection::kRight : d_x = 1; break;
        }
    }

    auto from = _player->getPosition();
//    auto from = ((Sprite*)_tileMap->getChildByTag(kPlayerTag))->getPosition();

    std::cout << "from x：" << from.x << std::endl;
    std::cout << "from y：" << from.y << std::endl;
    float x, y;
    if ( abs(d_x) > abs(d_y) ) {
        if (d_x > 0) {
            x = from.x + _tileMap->getTileSize().width;
        } else {
            x = from.x - _tileMap->getTileSize().width;
        }
        y = from.y;
    } else {
        if (d_y > 0) {
            y = from.y + _tileMap->getTileSize().height;
        } else {
            y = from.y - _tileMap->getTileSize().height;
        }
        x = from.x;
    }

//    from.setPoint(x, y);
    auto newPosition = Point(x, y);
    std::cout << "newPosition x：" << newPosition.x << std::endl;
    std::cout << "newPosition y：" << newPosition.y << std::endl;

    Point tileCoord = this->tileCoordForPosition(newPosition);
//    int tileGid = _meta->getTileGIDAt(tileCoord);

    // 衝突判定
//    if (tileGid) {
//        Dictionary *properties = _tileMap->propertiesForGID(tileGid);
//        if (properties) {
//            CCString *collision = new CCString();
//            *collision = *properties->valueForKey("Collidable");
//            if (collision && (collision->compare("true") == 0)) {
//                this->playHeroMoveAnimationFromPosition(from, newPosition);
//                return;
//            }
//        }
//    }

    this->playHeroMoveAnimationFromPosition(from, newPosition);
    MoveTo* moveTo = MoveTo::create(0.5f, newPosition);
    _player->runAction(moveTo);
    this->setViewPlayerCenter(0.1f);

    std::cout << "10D10===========================" << std::endl;
    dice.roll(10, 10);
    int result = dice.getRollResult();
    dice.reset();
    std::cout << "合計値 : " << result << std::endl;

    if (this->isEncountered(result)) {
        std::cout << "敵が現れた！！" << std::endl;
    }
}

void MapScene::playHeroMoveAnimationFromPosition(Point from, Point to)
{
    char* direction = "n";
    if(to.x > from.x){
        direction = "e";
    }else if(to.x < from.x){
        direction = "w";
    }else if(to.y < from.y){
        direction = "s";
    }
    auto pAnimation = Animation::create();
    for (int i = 0; i < 9; ++i) {

        auto spriteFrameName = StringUtils::format("male_walkcycle_%s_%02d.png",direction, (i + 1));
        auto pFrame = _frameCache->getSpriteFrameByName(spriteFrameName);

        pAnimation->addSpriteFrame(pFrame);
//        std::cout <<  spriteFrameName.get_allocator() << std::endl;
    }
    pAnimation->setDelayPerUnit(0.05f);
//    pAnimation->setLoops(-1);
    auto animate = Animate::create(pAnimation);
    _player->runAction(animate);
}

Point MapScene::tileCoordForPosition(Point position)
{

    Point tilePoint = position;

    // タイルの幅
    float fTileWidth = _tileMap->getTileSize().width;
    // タイルの高さ
    float fTileHeight = _tileMap->getTileSize().height;
    // タイルの行数
    float fTileRows = _tileMap->getMapSize().height;


    // タップ座標をタイル座標に変換
    tilePoint.x = (int)(tilePoint.x / fTileWidth);
    tilePoint.y = (int)((fTileRows * fTileHeight - tilePoint.y) / fTileHeight);

    return tilePoint;
}

void MapScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesBegan");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();

        _virtualPad->startPad((int)location.x,(int)location.y,pTouch->getID());
    }

}

void MapScene::onTouchesMoved(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesMoved");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();
        _virtualPad->update((int)location.x,(int)location.y,pTouch->getID());
    }
    // プレイヤーをこの関数で移動
    this->schedule(schedule_selector(MapScene::setPlayerPosition), 0.1f);
    this->schedule(schedule_selector(MapScene::setViewPlayerCenter));

}

void MapScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesEnded");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();
        _virtualPad->endPad(pTouch->getID());
    }
    this->unschedule(schedule_selector(MapScene::setPlayerPosition));
    this->unschedule(schedule_selector(MapScene::setViewPlayerCenter));
}

void MapScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event *pEvent)
{
    CCLOG("onTouchesCancelled");
    for (auto &item: touches)
    {
        auto pTouch = item;
        auto location = pTouch->getLocation();
        _virtualPad->endPad(pTouch->getID());
    }
    this->unschedule(schedule_selector(MapScene::setPlayerPosition));
    this->unschedule(schedule_selector(MapScene::setViewPlayerCenter));
}