//
// Created by 藤井 陽介 on 2014/03/06.
//


#ifndef __ActorSprite_H_
#define __ActorSprite_H_

#include "cocos2d.h"
#include "MapManager.h"

#include <vector>
#include <list>

class ActorSprite : public cocos2d::Sprite
{
public:
    // 技能
    typedef struct _ActorSkillDto {
        /** 識別ID */
        int skillId;
        /** 技能名 */
        std::string name;
        /** 技能値 */
        int skillPoint;
    } ActorSkillDto;

    // TODO: classにして値変更のデリゲートメソッド作るべき？
    // TODO: HPとかのパラメータはStratgyパターン使ったほうがいいかな？
    typedef struct _ActorDto {
        /** プレイヤーを一意に識別するID. */
        int playerId;
        /** キャライメージのID. */
        int imageResId;
        /** キャラ名. */
        std::string name;

        /** HP. */
        int hitPoint;
        /** HP最大値. */
        int hitPointLimit;
        /** 正気度. */
        int sanity;
        /** 正気度最大値. */
        int sanityLimit;
        /** MP. */
        int magicPoint;
        /** MP最大値. */
        int magicPointLimit;

        /** 基本パラメータ */
        /** STR */
        int actorStr;
        /** CON */
        int actorCon;
        /** POW */
        int actorPow;
        /** DEX */
        int actorDex;
        /** APP */
        int actorApp;
        /** SIZ */
        int actorSize;
        /** INT */
        int actorInt;
        /** EDU */
        int actorEdu;

        /** 装備. */
        //ActorPlayerEquipDto equipDto;
        /** スキルリスト. */
//        std::list<ActorSkillDto> skillDtoVector;
    } ActorDto;

    // ToDo:クラスにした方がいいかも


    // コンストラクタ、デストラクタ
    ActorSprite();
    virtual ~ActorSprite();

    // 初期化とか
    virtual bool initWithActorDto(ActorDto actorDto);
    static ActorSprite* createWithActorDto(ActorDto actorDto);

    void setActorMapItem(ActorMapItem actorMapItem);
    ActorMapItem* getActorMapItem();
    ActorDto* getActorDto();

    void runMoveAction(MapIndex moveMapIndex);
    void runBottomAction();
    void runLeftAction();
    void runRightAction();
    void runTopAction();

private:
    ActorMapItem m_actorMapItem;
    ActorDto m_actorDto;

    cocos2d::FiniteTimeAction* createBottomActorAnimate();
    cocos2d::FiniteTimeAction* createLeftActorAnimate();
    cocos2d::FiniteTimeAction* createRightActorAnimate();
    cocos2d::FiniteTimeAction* createTopActorAnimate();
    cocos2d::FiniteTimeAction* createActorAnimate(int actorId, std::string frameName);
};


#endif //__ActorSprite_H_
