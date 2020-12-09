#ifndef POKER_H
#define POKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// 2 3 4 5 6 7 8 9 10 J Q  K  A
// 0 1 2 3 4 5 6 7 8  9 10 11 12
#define poker_char "23456789XJQKA"
//获取数值 以及牌面
#define poker_get_digital(x) (x%13)
#define poker_get_suits(x) 	 (x/13)
//牌面类型
typedef enum _poker_suits{
	suits_clubs,//梅花
	suits_hearts,//红桃
	suits_spades,//黑桃
	suits_diamonds,//方块砖石
	suits_joker//小丑 王牌
}poker_suits;

//扑克牌个数
#define poker_face_digital		0
#define poker_face_suits		1

#define poker_number 			52
#define poker_rand_interval		(RAND_MAX / poker_number) 
//
typedef struct _poker_deck{
	int face[poker_number];//digital A 2 3 4 5 6 7 8 9 10 J Q K suits clubs hearts spades diamonds
	int licenses;//已经发出的扑克牌
	int overage;//剩余扑克牌
	struct _poker_deck *next;//指向下一个扑克牌
}poker_deck;//扑克牌类型

poker_deck *malloc_poker_deck(void);
void free_poker_deck(poker_deck *deck);
poker_deck *add_poker_deck(poker_deck **deck,poker_deck *add);
void delete_poker_deck(poker_deck *deck);
void format_poker_deck(poker_deck *deck,int overage);


void poker_print(int face);//打印牌面
void poker_deck_print(poker_deck *deck);//打印牌组
void poker_init(poker_deck *deck);//初始化牌组
void poker_shuffle(poker_deck *deck);//洗牌
void poker_licensing(poker_deck *hand,poker_deck *deck,int count);//发牌

void poker_sort(poker_deck *cards,int type);//排序顺发 type 1数字 或者 0花色
void poker_load(poker_deck *deck,char *str);//加载牌组
int poker_deck_group(poker_deck *group,poker_deck *hand);//扑克牌分组
int check_samedigital(poker_deck *hand,poker_deck *out);//校验相同对数 需要先排序
int check_continuous(poker_deck *hand,poker_deck *out);//校验手牌是否连续
int check_samesuits(poker_deck *hand,poker_deck *out);//校验手牌是否相同花色
int poker_delete_hand(poker_deck * deck,poker_deck * hand);//从一个牌组删除另一组牌
int poker_delete_samedigital(poker_deck *deck);//删除重复点数的手牌
int delete_straight(poker_deck *hand,int slen);//查找顺子 然后删除找到的顺子


#endif /* OBJECT_POOL_H */
