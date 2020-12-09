/*
 * Copyright (c) 2018, Ubuntu Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "poker.h"

//分配一个牌组
poker_deck *malloc_poker_deck(void){
	poker_deck *ret = malloc(sizeof(poker_deck));
	memset(ret,0x00,sizeof(poker_deck));
	return ret;
}
//释放一个牌组
void free_poker_deck(poker_deck *deck){
	return free(deck);
}
//添加一个牌组
poker_deck *add_poker_deck(poker_deck **deck,poker_deck *add){
	poker_deck *res = *deck;
	if(*deck == NULL){
		*deck = malloc_poker_deck();
		memcpy(*deck,add,sizeof(poker_deck));
		return *deck;
	}else{
		while(res->next){
			res = res->next;
		}
		res->next = malloc_poker_deck();
		memcpy(res->next,add,sizeof(poker_deck));
		return res->next;
	}
}
//释放连续牌组
void delete_poker_deck(poker_deck *deck){
	poker_deck *res;
	while(deck){
		res = deck->next;
		free_poker_deck(deck);
		deck = res;
	}
}
//初始化扑克牌
void format_poker_deck(poker_deck *deck,int overage){
	memset(deck,0x00,sizeof(poker_deck));
	deck->overage = overage;
}



//打印扑克牌
void poker_print(int face){
	switch(face/13){
		case suits_clubs	:if(face%13==8) printf("♣10"); else printf("♣%c",poker_char[face%13]);	break;
		case suits_hearts	:if(face%13==8) printf("♥10"); else printf("♥%c",poker_char[face%13]);	break;
		case suits_spades	:if(face%13==8) printf("♠10"); else printf("♠%c",poker_char[face%13]);	break;
		case suits_diamonds	:if(face%13==8) printf("♦10"); else printf("♦%c",poker_char[face%13]);	break;
		default:break;
	}
	printf(" ");
}
//打印牌组
void poker_deck_print(poker_deck *deck){
	int i;
	while(deck){
		for(i = 0;i < deck->licenses + deck->overage;i++){
			poker_print(deck->face[i]);
			if(i==12 || i==25 || i==38 || i==52){
				printf("\n");
			}
		}
		deck = deck->next;
		printf("\n");
	}
	//printf("\n");
}


//初始化牌组
void poker_init(poker_deck *deck){
	int i;
	for(i = 0;i < poker_number;i++){
		deck->face[i]=i;
	}
	deck->licenses = 0;
	deck->overage = poker_number;
}

//洗牌
void poker_shuffle(poker_deck *deck){
	int i,exA,exB,tmp;
	srand(time(0));
	for(i = 0;i < poker_number;i++){
		exA=rand() / poker_rand_interval;
		exB=rand() / poker_rand_interval;
		
		tmp = deck->face[exA];
		deck->face[exA] = deck->face[exB];
		deck->face[exB] = tmp;
		
	}
	printf("\n");
}
//发牌
//参数deck 数量
void poker_licensing(poker_deck *hand,poker_deck *deck,int count){
	int i,k=0;
	for(i = deck->licenses;(i < count)&&(i<poker_number);i++){
		hand->face[k++]=deck->face[i];
	}
	hand->licenses = 0;
	hand->overage  = count;
	deck->licenses = count;
	deck->overage -= deck->licenses;
}
//排序算法
int cmp_digital(const void *a, const void *b) {
   //return (*(int*)a - *(int*)b);
   return poker_get_digital((*(int*)a)) - poker_get_digital((*(int*)b));
}
//排序算法
int cmp_suits(const void *a, const void *b) {
   return (*(int*)a - *(int*)b);
}

//按点数排序
void poker_sort(poker_deck *cards,int type){
	if(type){
		qsort(cards->face, cards->overage, sizeof(int), cmp_suits);
	}else{
		qsort(cards->face, cards->overage, sizeof(int), cmp_digital);
	}
}
//初始化手牌
void poker_load(poker_deck *deck,char *str){
	int i,count=0;//设置长度
	for(i=0;i<strlen(str);){
		//判断花色
		if(!strncmp(&str[i],"♣",3))
			deck->face[count] = suits_clubs*13;
		else if(!strncmp(&str[i],"♥",3))
			deck->face[count] = suits_hearts*13;
		else if(!strncmp(&str[i],"♠",3))
			deck->face[count] = suits_spades*13;
		else if(!strncmp(&str[i],"♦",3))
			deck->face[count] = suits_diamonds*13;
		else
			deck->face[count] = 0;
		//获取点数
		if(str[i+3] == 'J')
			deck->face[count] += 9;
		else if(str[i+3] == 'Q')
			deck->face[count] += 10;
		else if(str[i+3] == 'K')
			deck->face[count] += 11;
		else if(str[i+3] == 'A')
			deck->face[count] += 12;
		else
			deck->face[count] += atoi(&str[i+3]) -2 ;
		//处理扑克10点的长度
		if(str[i+3] == '1')
			i=i+6;
		else
			i=i+5;
		count++;
	}
	deck->licenses = 0;
	deck->overage  = count;	
}

//分组扑克牌
int poker_deck_group(poker_deck *group,poker_deck *hand){
	int i,value,suits;
	for(i=0;i < 4;i++){
		group[i].licenses = 0;
		group[i].overage = 0;
		group[i].next = NULL;
	}
	for(i=0;i < hand->overage;i++){
		value = hand->face[i];//获取值
		suits = poker_get_suits(value);//获取类别
		group[suits].face[group[suits].overage++] = value;//写入值
	}
	/*for(i=0;i < 4;i++){
		printf("grope %d: ",i);
		poker_deck_print(&group[i]);//打印牌组
	}*/
	return 0;
}

//校验手牌相同值的对数 需要事先排序
int check_samedigital(poker_deck *hand,poker_deck *out){
	int digital=poker_get_digital(hand->face[hand->licenses]),i;
	for(i=0;i < hand->overage;i++){
		if(digital != poker_get_digital(hand->face[i+hand->licenses])){
			return i;
		}
		if(out){
			out->face[i] = hand->face[i+hand->licenses];
			out->licenses = 0;
			out->overage = i+1;
		}
	}
	return i;
}

//校验手牌是否连续
//hand 手牌 out连续的数据
//返回连续个数
int check_continuous(poker_deck *hand,poker_deck *out){
	int digital=poker_get_digital(hand->face[hand->licenses]),i;
	for(i=0;i < hand->overage;i++){
		if((digital+i) != poker_get_digital(hand->face[i+hand->licenses])){
			return i;
		}
		if(out){
			out->face[i] = hand->face[i+hand->licenses];
			out->licenses = 0;
			out->overage = i+1;
		}
	}
	return i;
}
//校验手牌是否同一花色
int check_samesuits(poker_deck *hand,poker_deck *out){
	int suits=poker_get_suits(hand->face[hand->licenses]),i;
	for(i=0;i < hand->overage;i++){
		if(suits != poker_get_suits(hand->face[i+hand->licenses])){
			return i;
		}
		if(out){
			out->face[i] = hand->face[i+hand->licenses];
			out->licenses = 0;
			out->overage = i+1;
		}
	}
	return i;
}

//删除一个元素 参数值
int delete_element_value(int *d,int val,int len){
	bool del = false;
	int i;
	for(i=0;i < len;i++){
		if((del == false) && (d[i] == val)){
			del = true;
		}
		if(del){
			d[i]=d[i+1];
		}
	}
	return del?1:0;
}

//从一个牌组删除另一组牌
int poker_delete_hand(poker_deck * deck,poker_deck *hand){
	int i,count;
	for(i=0;i<hand->overage;i++){
		count = delete_element_value(deck->face,hand->face[i],deck->overage);
		deck->overage -= count;
	}
}

//删除一个元素 del表示需要删除的序号
void delete_element_index(int *d,int del,int len){
	int i;
	for(i=0;i < len;i++){
		if(i >= del){
			d[i]=d[i+1];
		}
	}
}
//从一个牌组删除重复点数的扑克牌
int poker_delete_samedigital(poker_deck *deck){
	int i,count;
	
	for(i=0;i<deck->overage;){
		if(poker_get_digital(deck->face[i]) == poker_get_digital(deck->face[i+1])){
			delete_element_index(deck->face,i,deck->overage);
			deck->overage--;
		}else{
			i++;
		}
	}
}

//查找并且删除顺子
//参数牌组 查询的长度
int delete_straight(poker_deck *hand,int slen){
	unsigned char map[poker_number]="";
	unsigned char *mapp=map;
	int digital=poker_get_digital(hand->face[hand->licenses]),i;
	memset(map,0xFF,poker_number);
	for(i=0;i < hand->overage;i++){
		if(digital == poker_get_digital(hand->face[i+hand->licenses])){
			digital++;
			*mapp++ = i+hand->licenses;
			if(--slen==0){
				break;
			}
		}
	}
	for(i=0;i < poker_number;i++){
		if(map[i]==0xFF)break;
		delete_element_index(hand->face,map[i]-i,hand->overage--);//删除元素	
	}
	return i;
}


