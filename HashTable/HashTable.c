#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETCOUNT 20

struct hashEntry {
    const char *key;
    char *value;
    struct hashEntry *next;
};
typedef struct hashEntry entry;

struct hashTable {
    entry bucket[BUCKETCOUNT];
};
typedef struct hashTable table;

// 初始化哈希表
void initHashTable(table *t) {
    
    if (t == NULL) return;
    
    for (int i = 0; i < BUCKETCOUNT; i++) {
        t->bucket[i].key = NULL;
        t->bucket[i].value = NULL;
        t->bucket[i].next = NULL;
    }
}

// 释放哈希表
void freeHashTable(table *t) {
    entry *e, *ep;
    if (t == NULL) return;
    
    for (int i = 0; i < BUCKETCOUNT; i++) {
        e = &(t->bucket[i]);// 获取i位置对应的entry
        while (e->next != NULL) {
            ep = e->next;
            e->next = ep->next;
            free(ep->key);
            free(ep->value);
            free(ep);
        }
    }
}

// 哈希散列方法函数
size_t keyToIndex(const char *key) {
    
    size_t index, len, i;
    if (key == NULL) return -1;
    
    len = strlen(key);
    index = (size_t)key[0];
    for (i = 1; i < len; i++) {
        index *= 1103515245 + (size_t)key[i];
    }
    
    index >>= 27;
    index &= (BUCKETCOUNT -1);
    return index;
}

//向哈希表中插入数据
size_t insertEntry(table *t, const char *key, const char *value) {
    
    size_t index, vlen1, vlen2;
    entry *e, *ep;
    
    if (t == NULL || key == NULL || value == NULL) return -1;
    
    index = keyToIndex(key);
    if (t->bucket[index].key == NULL) {
        t->bucket[index].key = strdup(key);
        t->bucket[index].value = strdup(value);
    }else {
        e = ep = &(t->bucket[index]);
        while (e != NULL) {// 如果存在
            if (strcmp(e->key, key) == 0) {
                // 找到key 所在，替换值
                vlen1 = strlen(value);
                vlen2 = strlen(e->value);
                if (vlen1 > vlen2) {
                    free(e->value);
                    e->value = (char *)malloc(vlen1+1);
                }
                memcpy(e->value, value, vlen1+1);
                return index; // 插入完成
            }
            
            ep = e;
            e = e->next;
        }// end while
        
        // 如果没有在当前桶中找到
        // 创建条目加入
        e = (entry *)malloc(sizeof(entry));
        e->key = strdup(key);
        e->value = strdup(value);
        e->next = NULL;
        ep->next = e;
    }
    return index;
}

//在哈希表中查找key对应的value
//如果找到了返回value的地址，没找到返回NULL
const char * findValueByKey(const table *t, const char *key) {
    size_t index;
    const entry *e;
    if (t == NULL || key == NULL) return NULL;
    index = keyToIndex(key);
    e = &(t->bucket[index]);
    
    if (e->key == NULL) return NULL;// 此桶还没有元素
    while (e != NULL) {
        if (strcmp(key, e->key) == 0) {
            return e->value; // 找到，返回地址值
        }
        e = e->next;
    }
    return NULL;
}

/*
 在哈希表中查找key对应的entry
 如果找到，返回entry,并将其从哈希表中移除
 如果没找到返回NULL
 */
entry * removeEntry(table *t, char *key) {
    size_t index;
    entry *e, *ep; //查找的时候，把ep作为返回值
    if (t == NULL || key == NULL) return NULL;
    
    index = keyToIndex(key);
    e = &(t->bucket[index]);
    
    while (e != NULL) {
        if (strcmp(key, e->key) == 0) {
            
            // 如果是桶的第一个
            if (e == &(t->bucket[index])) {
                //如果这个桶有两个或以上元素
                //交换第一个和第二个，然后移除第二个
                ep = e->next;
                if (ep != NULL) {
                    entry tmp = *e;// 做浅拷贝交换
                    *e = *ep; // 相当于链表的头节点已经移除
                    *ep = tmp; //这就是移除下来的链表头节点
                    ep->next = NULL;
                }else {// //这个桶只有第一个元素
                    ep = (entry*)malloc(sizeof(entry));
                    *ep = *e;
                    e->key = e->value = NULL;
                    e->next = NULL;
                }
                
            }else {
                //如果不是桶的第一个元素
                //找到它的前一个
                ep = &(t->bucket[index]);
                while (ep->next != e) {
                    ep = ep->next;
                }
                
                // 将e从中拿出来
                ep->next = e->next;
                e->next = NULL;
                ep = e;
            }
            
            return ep;
        }
    }
    return NULL;
}
 
// 哈希表打印函数
void printHashTable(table *t) {
    entry *e;
    if (t == NULL) return;
    for (int i = 0; i < BUCKETCOUNT; i++) {
        e = &(t->bucket[i]);
        while (e->key != NULL) {
            printf("\t%s\t=\t%s\n",e->key, e->value);
            if (e->next == NULL) break;
            e = e->next;
        }
    }
}

int main(int argc, const char * argv[]) {
    
    table t;
    initHashTable(&t);
    
    // 添加数据
    insertEntry(&t, "www.baidu.com", "baidu");
    insertEntry(&t, "www.ifeng.com", "ifeng");
    insertEntry(&t, "www.sina.com.cn", "sina");
    
    
    // 查询数据
    const char *value = findValueByKey(&t, "www.ifeng.com");
    if (value != NULL) {
        printf("www.ifeng.com: = %s\n",value);
    }
    
    printHashTable(&t);
    
    // 将哈希表中key对应的节点移除
    entry *e = removeEntry(&t, "www.baidu.com");
    if (e != NULL) {// 释放内存
        free(e->key);
        free(e->value);
        free(e);
        e = NULL;
    }
    printHashTable(&t);

    // 释放
    freeHashTable(&t);
    
    return 0;
}
