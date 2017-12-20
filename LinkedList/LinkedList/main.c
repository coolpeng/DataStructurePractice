//
//  main.c
//  LinkedList
//
//  Created by Edward on 2017/12/19.
//  Copyright © 2017年 coolpeng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

// 非循环单项链表

#define LEN (sizeof(UrlInfo))

struct UrlInfo {
    int urlType;
    struct UrlInfo *next;
};

struct Node {
    int data;
    char url[1024];
    struct Node *nextNode;
};

typedef struct Node single_node;

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    single_node *headerNode = NULL; //1、首先，当然是定义一个头指针
    headerNode = malloc(sizeof(single_node)); //2、然后分配内存空间
    
    if (headerNode == NULL) {
        printf("malloc failure\n");
    }
    
    memset(headerNode,0,sizeof(single_node));//3、清一下
    headerNode->data = 200; // 给链表节点的数据赋值
    headerNode->nextNode = NULL;
    
    printf("node_ptr: %d\nnode_ptr: %p\n",headerNode->data,headerNode->nextNode);
    
    
    int var = 10;// 实际变量的声明
    int *ip;// 指针变量的声明
    ip = &var;// 在指针变量中存储 var 的地址
    
    printf("ip: %p\n&var: %p\n*ip: %d\n",ip,&var,*ip);
    
    
    
    return 0;
}
