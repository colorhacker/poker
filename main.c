/*
 * Copyright (c) 2018, Ubuntu Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "poker.h"




/*十三水规则*/
//特殊牌：至尊清龙判断 108分
//♥2 ♥3 ♥4 ♥5 ♥6 ♥7 ♥8 ♥9 ♥10 ♥J ♥Q ♥K ♥A
int supreme_qinglong(poker_deck *hand){
	if((check_continuous(hand,NULL)==hand->overage) && 
		(check_samesuits(hand,NULL)==hand->overage)){
		return 104;
	}else{
		return 0;
	}
}
//特殊牌：一条龙判断 36分
//♣2 ♥3 ♠4 ♦5 ♥6 ♥7 ♥8 ♥9 ♥10 ♥J ♥Q ♥K ♥A
int one_stop_long(poker_deck *hand){
	if((check_continuous(hand,NULL)==hand->overage)){
		return 36;
	}else{
		return 0;
	}
}
//特殊牌：十二皇族 24分
//♣J ♣Q ♣K ♣A ♥J ♥Q ♥K ♥A ♠J ♠Q ♠K ♠A ♦J ♦Q ♦K ♦A
int twelve_royalty(poker_deck *hand){
	int i;
	for(i=0;i < hand->overage;i++){
		if(poker_get_digital(hand->face[i]) < 9){
			return 0;
		}
	}
	return 24;
}
//特殊牌：三同花顺 20分
//♣J ♣Q ♣K ♥2 ♥3 ♥4 ♥5 ♥6 ♠7 ♠8 ♠9 ♠10 ♠J
int three_flush(poker_deck *hand){
	poker_deck hand_group[4];//分组卡
	int i,count;
	poker_deck_group(hand_group,hand);//分组数据
	for(i=0;i < 4;i++){
		while (hand_group[i].overage){
			count = check_continuous(&hand_group[i],NULL);
			hand_group[i].licenses += count;
			hand_group[i].overage -=count;
			if(count != 0 && count != 3 &&
				count != 5 && count != 8){
				return 0;
			}
		}
	}
	return 20;
}
//特殊牌：六对半 4分
//♣J ♠J ♣K ♠K ♦3 ♥3 ♦4 ♥4 ♦7 ♠7 ♦10 ♠10 ♠2
int six_a_half(poker_deck *hand){
	poker_deck tmp;
	int count,num=0;
	memcpy(&tmp,hand,sizeof(poker_deck));
	while (tmp.overage){
		count = check_samedigital(&tmp,NULL);
		tmp.licenses += count;
		tmp.overage -=count;
		//printf("six_a_half :%d\n",count);
		if((num++ > 7) || (count != 2 && count != 1)){
			return 0;
		}
	}	
	return 4;
}
//特殊牌：四套三条 6分
//♣J ♠J ♦J ♣K ♠K ♦K ♣4 ♦4 ♥4 ♦7 ♠7 ♣7 ♠2
int four_three_groups(poker_deck *hand){
	poker_deck tmp;
	int count,num=0;
	memcpy(&tmp,hand,sizeof(poker_deck));
	while (tmp.overage){
		count = check_samedigital(&tmp,NULL);
		tmp.licenses += count;
		tmp.overage -=count;
		//printf("six_a_half :%d\n",count);
		if((num++ > 5) || (count != 1 && count != 3)){
			return 0;
		}
	}	
	return 6;
}
//特殊牌：清一色 10分
//♣2 ♣3 ♣4 ♣5 ♣6 ♣7 ♣8 ♣9 ♣10 ♣J ♣Q ♣K ♣K 
int of_same_color(poker_deck *hand){
	if(check_samesuits(hand,NULL) != hand->overage){
		return 0;
	}
	return 10;
}
//特殊牌：全小 10分
//♣2 ♣3 ♣4 ♣5 ♣6 ♣7 ♣8 ♦7 ♦6 ♦5 ♦4 ♦3 ♠3
int all_small(poker_deck *hand){
	int i;
	for(i=0;i < hand->overage;i++){
		if(poker_get_digital(hand->face[i]) > 6){
			return 0;
		}
	}
	return 10;

}
//特殊牌：全大 10分
//♣8 ♣9 ♣10 ♣J ♣Q ♣K ♣A ♦8 ♦9 ♦10 ♦J ♦Q ♠8
int all_big(poker_deck *hand){
	int i;
	for(i=0;i < hand->overage;i++){
		if(poker_get_digital(hand->face[i]) < 6){
			return 0;
		}
	}
	return 10;

}
//特殊牌：三顺子 4分
//♣J ♦Q ♣K ♥2 ♥3 ♥4 ♦5 ♥6 ♠4 ♠5 ♠6 ♦7 ♠8
int three_straight(poker_deck *hand){
	poker_deck tmp;
	int i,k,count;
	unsigned char list[][3]={{3,5,5},{5,3,5},{5,5,3}};
	for(i=0;i<(sizeof(list)/3);i++){
		memcpy(&tmp,hand,sizeof(poker_deck));
		for(k=0;k<3;k++){
			count = delete_straight(&tmp,list[i][k]);
			if(count != list[i][k]){
				break;
			}
		}
		if(k==3)return 4;
	}
	return 0;
}
//特殊牌：三同花 3分
//♣J ♣Q ♣K ♥2 ♥3 ♥4 ♥5 ♥6 ♦4 ♦5 ♦6 ♦7 ♦9
int triple_flush(poker_deck *hand){
	poker_deck hand_group[4];//分组卡
	int i;
	poker_deck_group(hand_group,hand);//分组数据
	for(i=0;i < 4;i++){
		if( hand_group[i].overage != 0 &&hand_group[i].overage != 3 &&
			hand_group[i].overage != 5 &&hand_group[i].overage != 8 && 
			hand_group[i].overage != 10){
			return 0;
		}
	}
	return 4;
}
//判断特殊牌
bool find_special_cards(poker_deck *hand_cards){
	int ret=false;
	if(ret){return true;}ret = supreme_qinglong(hand_cards);
	printf("is supreme qinglong? %s \n",ret?"yes":"no");//至尊清龙
	if(ret){return true;}ret = one_stop_long(hand_cards);
	printf("is one stop long? %s \n",ret?"yes":"no");//一条龙
	if(ret){return true;}ret = twelve_royalty(hand_cards);
	printf("is twelve royalty? %s \n",ret?"yes":"no");//十二皇族
	if(ret){return true;}ret = three_flush(hand_cards);
	printf("is three flush? %s \n",ret?"yes":"no");//三同花顺 分离组
	if(ret){return true;}ret = six_a_half(hand_cards);
	printf("is six and a half? %s \n",ret?"yes":"no");//六对半
	if(ret){return true;}ret = four_three_groups(hand_cards);
	printf("is four three groups? %s \n",ret?"yes":"no");//四个三条
	if(ret){return true;}ret = of_same_color(hand_cards);
	printf("is of same color? %s \n",ret?"yes":"no");//清一色
	if(ret){return true;}ret = all_small(hand_cards);
	printf("is all small? %s \n",ret?"yes":"no");//全小
	if(ret){return true;}ret = all_big(hand_cards);
	printf("is all big? %s \n",ret?"yes":"no");//全大
	if(ret){return true;}ret = three_straight(hand_cards);
	printf("is three straight? %s \n",ret?"yes":"no");//三个顺子
	if(ret){return true;}ret = triple_flush(hand_cards);
	printf("is triple flush? %s \n",ret?"yes":"no");//三个同花
	if(ret){return true;}else {return false;}
}



//普通牌：同花顺 查找出同花顺的组合 cnt指定个数
poker_deck *find_flush(poker_deck *hand,int cnt){
	poker_deck *find_deck = NULL;
	poker_deck cont,copy;
	poker_deck hand_group[4];//分组卡
	int i,count,n;
	format_poker_deck(&cont,0);
	format_poker_deck(&copy,cnt);
	poker_deck_group(hand_group,hand);//分组数据
	for(i=0;i < 4;i++){
		while (hand_group[i].overage){
			count = check_continuous(&hand_group[i],&cont);
			hand_group[i].licenses += count;
			hand_group[i].overage -=count;
			for(n=0;n<=count-cnt;n++){
				memcpy(copy.face,&cont.face[n],sizeof(int)*cnt);
				add_poker_deck(&find_deck,&copy);
			}
		}
	}
	printf("\nfind flush %d \n",cnt);
	poker_deck_print(find_deck);//打印牌组
	delete_poker_deck(find_deck);//删除数据
	return find_deck;
}

//普通牌：相同点数扑克 查找出指定相同数目的牌
poker_deck *find_samedigital(poker_deck *hand,int cnt){
	poker_deck *find_deck = NULL;
	poker_deck temp,cont,copy;
	int n,count;
	format_poker_deck(&temp,0);
	format_poker_deck(&cont,0);
	format_poker_deck(&copy,cnt);
	memcpy(&temp,hand,sizeof(poker_deck));
	poker_sort(&temp,poker_face_digital);//排序牌组
	while (temp.overage){
		count = check_samedigital(&temp,&cont);
		temp.licenses += count;
		temp.overage -=count;
		for(n=0;n<=count-cnt;n++){
			memcpy(copy.face,&cont.face[n],sizeof(int)*cnt);
			copy.overage = cnt;
			add_poker_deck(&find_deck,&copy);
		}
	}
	printf("\nfind samedigital %d \n",cnt);
	poker_deck_print(find_deck);//打印牌组
	delete_poker_deck(find_deck);//删除数据
	return find_deck;
}

//普通牌：相同花色扑克 查找出指定相同数目的牌
poker_deck *find_samesuits(poker_deck *hand,int cnt){
	poker_deck *find_deck = NULL;
	poker_deck copy;
	poker_deck hand_group[4];//分组卡
	int i,n;
	format_poker_deck(&copy,cnt);
	poker_deck_group(hand_group,hand);//分组数据
	for(i=0;i < 4;i++){
		for(n=0;n<=hand_group[i].overage-cnt;n++){
			memcpy(copy.face,&hand_group[i].face[n],sizeof(int)*cnt);
			add_poker_deck(&find_deck,&copy);
		}
	}
	printf("\nfind samesuits %d \n",cnt);
	poker_deck_print(find_deck);//打印牌组
	delete_poker_deck(find_deck);//删除数据
	return find_deck;
}

//普通牌：顺子查询 查找出指定相同数目的牌
poker_deck *find_continuous(poker_deck *hand,int cnt){
	poker_deck *find_deck = NULL;
	poker_deck temp,copy;
	int n,count;
	format_poker_deck(&copy,cnt);
	memcpy(&temp,hand,sizeof(poker_deck));
	poker_sort(&temp,poker_face_digital);//排序牌组
	poker_delete_samedigital(&temp);//删除相同点数的扑克牌
	while (temp.overage){
		count = check_continuous(&temp,&copy);
		temp.licenses += count;
		temp.overage -=count;
		for(n=0;n<=count-cnt;n++){
			memcpy(copy.face,&copy.face[n],sizeof(int)*cnt);
			copy.overage = cnt;
			add_poker_deck(&find_deck,&copy);
		}
	}
	printf("\nfind continuous %d \n",cnt);
	poker_deck_print(find_deck);//打印牌组
	delete_poker_deck(find_deck);//删除数据
	return find_deck;

}

//主函数
int main(void){
	poker_deck *deck_cards = malloc_poker_deck();//牌组卡
	poker_deck *hand_cards = malloc_poker_deck();//手牌卡

	poker_init(deck_cards);//初始化牌组
	poker_shuffle(deck_cards);//洗牌
	//♣J ♣Q ♣K ♥2 ♥3 ♥4 ♥5 ♥6 ♥7 ♦5 ♦6 ♠7 ♥9
	poker_load(hand_cards,"♣J ♣Q ♣K ♥2 ♥3 ♥4 ♥5 ♥6 ♥7 ♦5 ♦6 ♠7 ♥9");//♣♥♠♦加载手牌
	//poker_licensing(hand_cards,deck_cards,poker_number/4);//发牌前N张
	poker_sort(hand_cards,poker_face_digital);//排序牌组
	//poker_deck_print(deck_cards);
	poker_deck_print(hand_cards);//打印牌组
	find_special_cards(hand_cards);//查找特殊牌
	
	find_flush(hand_cards,5);//连续且同花的数据
	find_samedigital(hand_cards,4);//查找相同点数的扑克牌组合
	find_samedigital(hand_cards,3);//查找相同点数的扑克牌组合
	find_samedigital(hand_cards,2);//查找相同点数的扑克牌组合
	find_samesuits(hand_cards,5);//查找相同花色的扑克牌组合
	find_continuous(hand_cards,5);//查找指定数量顺子的组合
	
	
	//poker_delete_hand(deck_cards,hand_cards);
	//poker_deck_print(deck_cards);

	delete_poker_deck(deck_cards);
	delete_poker_deck(hand_cards);
	printf("\n");
	return 0;
}

