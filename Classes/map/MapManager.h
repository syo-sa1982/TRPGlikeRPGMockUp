//
// Created by 藤井 陽介 on 2014/03/06.
//


#ifndef __MapManager_H_
#define __MapManager_H_

#include <iostream>
#include <vector>
#include <list>

enum MapDataType {
    NONE        = 0,
    MOVE_DIST   = 1,
    ATTACK_DIST = 2,
    MAP_ITEM    = 3,
    PLAYER      = 4,
    ENEMY       = 5,
    OBSTACLE    = 6,
    MOVE_STEP_DIST = 10, // 移動経路
    SELECTED_DIST  = 11, // 選択位置
};

enum MoveDirectionType {
    MOVE_NONE  = -1,
    MOVE_DOWN  = 0,
    MOVE_LEFT  = 1,
    MOVE_RIGHT = 2,
    MOVE_UP    = 3
};

typedef struct _MapIndex {
    int x;
    int y;
    MoveDirectionType moveDictType;
} MapIndex;

typedef struct _MapItem {
    /** 短形マップ上の区分. */
    MapDataType mapDataType;

    MapIndex mapIndex;

    /** 移動可能距離. */
    int moveDist;
    /** 攻撃可能距離. */
    int attackDist;
} MapItem;

typedef struct _ActorMapItem : public MapItem {
    /** プレイヤーを一意に識別するID. */
    int seqNo;
    /** 移動済みフラグ. */
    bool moveDone;
    /** 攻撃済みフラグ. */
    bool attackDone;
} ActorMapItem;

typedef struct _DropMapItem : public MapItem {
    /** ドロップアイテムを一意に識別するID. */
    int seqNo;
    /** アイテムID */
    int itemId;
} DropMapItem;


class MapManager {

private:
    // マップカーソル一時データ
    std::vector<std::vector<MapItem>> m_mapCursorDataArray;
    // マップオブジェクトデータ（ドロップアイテム等）
    std::vector<std::vector<DropMapItem>> m_mapDropItemDataArray;
    // マップオブジェクトデータ（キャラ、障害物）
    std::vector<std::vector<ActorMapItem>> m_mapObjectDataArray;
    // マップ移動カーソルリスト
    std::list<MapIndex> m_mapMoveCursorList;
    // マップ移動経路リスト
    std::list<MapIndex> m_mapMovePointList;

    int m_top;
    int m_bottom;
    int m_left;
    int m_right;

    //void clearMapItemArray(std::vector<std::vector<MapItem>> *pMapItemArray);
    //void clearActorMapItemArray(std::vector<std::vector<ActorMapItem>> *pActorMapItemArray);

    template <typename TYPE>
    void clearMapItemArray(std::vector<std::vector<TYPE>> *pMapItemArray)
    {
        pMapItemArray->clear();
        for (int x = 0; x < m_right; x++)
        {
            std::vector<TYPE> mapItemArray;
            for (int y = 0; y < m_bottom; y++)
            {
                TYPE noneMapItem = createNoneMapItem<TYPE>(x, y);
                mapItemArray.push_back(noneMapItem);
            }
            pMapItemArray->push_back(mapItemArray);
        }
    }

    template <typename TYPE>
    TYPE createNoneMapItem(int x, int y)
    {
        TYPE mapItem;
        mapItem.mapDataType = MapDataType::NONE;
        mapItem.mapIndex = {x, y, MoveDirectionType::MOVE_NONE};
        mapItem.moveDist = 0;
        mapItem.attackDist = 0;
        return mapItem;
    }
    void findDist(int x, int y, int dist, bool first);
    void findMovePointList(int moveX, int moveY, int moveDist, MapItem* moveToMapItem);
    bool chkMove(int mapPointX, int mapPointY, int dist);
    bool chkMovePoint(int mapPointX, int mapPointY, int dist, MapDataType ignoreMapDataType);
    void addDistCursor(int mapPointX, int mapPointY, int dist);

    std::string logOutString(MapItem mapItem);

public:

    void DEBUG_LOG_MAP_ITEM_LAYER(); // デバッグ用のマップログ出力

    void init(int top, int bottom, int left, int right);

    std::list<MapIndex> createActorFindDist(MapIndex mapIndex, int dist);
    std::list<MapIndex> createMovePointList(MapIndex* moveFromMapIndex, MapItem* moveToMapItem);
    void clearCursor();

    void addActor(ActorMapItem* pActorMapItem);
    void moveActor(ActorMapItem* actorMapItem, MapIndex* moveMapIndex);
    void addDropItem(DropMapItem* pDropMapItem);
    void addObstacle(MapIndex* pMapIndex);
    void removeMapItem(MapItem* pRemoveMapItem);
//    void removeMapItem(MapItem removeMapItem);

    ActorMapItem* getActorMapItem(MapIndex* pMapIndex);
    MapItem* getMapItem(MapIndex* pMapIndex);
    ActorMapItem* getActorMapItemById(int seqNo);
    DropMapItem* getDropMapItem(MapIndex* pMapIndex);

    MoveDirectionType checkMoveDirectionType(MapIndex fromMapIndex, MapIndex toMapIndex);
    MapItem searchTargetMapItem(std::list<MapIndex> searchMapIndexList);

    std::list<ActorMapItem> findEnemyMapItem();
};


#endif //__MapManager_H_
