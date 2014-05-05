//
// Created by 藤井 陽介 on 2014/03/06.
//

#include "MapManager.h"


void MapManager::init(int top, int bottom, int left, int right)
{
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;

//    clearCursor();
    clearMapItemArray(&m_mapObjectDataArray);
    clearMapItemArray(&m_mapDropItemDataArray);

    m_mapMovePointList.clear();
}



/**
 * 障害物の追加
 */
void MapManager::addObstacle(MapIndex* pMapIndex)
{
    // TODO: とりあえずactorと同じにする。。。大丈夫か？
    ActorMapItem mapItem = createNoneMapItem<ActorMapItem>(pMapIndex->x, pMapIndex->y);
    mapItem.mapDataType = MapDataType::OBSTACLE;
    mapItem.seqNo = 0;
    mapItem.moveDone = false;
    mapItem.attackDone = false;

    m_mapObjectDataArray[pMapIndex->x][pMapIndex->y] = mapItem;
}
