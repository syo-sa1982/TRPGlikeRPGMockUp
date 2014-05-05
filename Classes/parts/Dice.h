//
//  Dice.h
//  Dice
//
//  Created by 藤井 陽介 on 2013/12/13.
//  Copyright (c) 2013年 syo-sa. All rights reserved.
//

#ifndef Dice__Dice__
#define Dice__Dice__

#include <cstdlib>
#include <vector>
#include <iostream>


class Dice
{
    // 面数
    enum kSurface
    {
        kDefaultSurface = 6
    };
    
    // 投げる回数
    enum kRollCount
    {
        kDefaultRoll = 1
    };

    
public:
    // コンストラクタ
    Dice();
    // ダイスロール
    // RollCount D Surface (例：3D6 6面ダイスを3回)
    void roll(int Surface = kDefaultSurface, int RollCount = kDefaultRoll);
    // 出目表示
    void display();
    // 出目リセット
    void reset();
    // 出目合計取得
    inline int getRollResult(){ return this->totalScore; };
    // 出目履歴取得
    inline const std::vector<int>& getRollHistory(){ return this->rollHistory; };
private:
    // 履歴
    std::vector<int> rollHistory;
    // 振った回数
    int totalRolls;
    // 出目合計
    int totalScore;
};
#endif /* defined(Dice__Dice__) */
