#include <vector>
#include <list>
#include <utility> 
#include <iostream>
#include <functional>
#include <string>

#define DEFAULT_CAPACITY 8

using std::cout;
using std::cin;
using std::endl;

//哈希表的单个节点
template<typename Key, typename Value>
class HashNode{
public:
    Key key;
    Value value;
    HashNode* next;
    HashNode(Key key,Value value):key(key),value(value),next(nullptr){}

};

//哈希表的具体结构
template<typename Key, typename Value, typename HashFunction = std::hash<Key>, typename Equal = std::equal_to<Key>>
class HashMap{
private:
    HashNode<Key,Value>**buckets;
    size_t size;
    size_t capacity;
    HashFunction hashFunc;
    Equal equalFunc;
    bool is_rehashing;
    //重哈希操作
    void rehash(){
        is_rehashing=true;
        auto oldBuckets=buckets;
        size_t oldCapacity=capacity;
        capacity*=2;
        buckets=new HashNode<Key,Value>*[capacity]();
        for(size_t i=0;i<oldCapacity;i++){
            HashNode<Key, Value>* node=oldBuckets[i];
            while(node!=nullptr){
                insert(node->key,node->value);
                HashNode<Key, Value>* temp=node;
                node=node->next;
                delete temp;
            }
        }
        delete[] oldBuckets;
        is_rehashing=false;
    }
public:
    //构造函数
    HashMap(size_t capacity=DEFAULT_CAPACITY):capacity(capacity),size(0),is_rehashing(false){
        buckets=new HashNode<Key,Value>*[capacity]();
        for(size_t i=0;i<capacity;i++){
            buckets[i]=nullptr;
        }
    }
    //析构函数
    ~HashMap(){
        for(size_t i=0;i<capacity;i++){
            HashNode<Key, Value>* node=buckets[i];
            while(node!=nullptr){
                HashNode<Key, Value>* temp=node;
                node=node->next;
                delete temp;
            }
        }
        delete[] buckets;
    }    
    //插入
    void insert(const Key& key,const Value& value){
        //这里偷懒使用了std::hash自带的算法来计算Index的位置
        if((1+size*4/3)>capacity&&is_rehashing==false){
            rehash();
            cout<<"rehashed!"<<endl;
        }
        size_t bucketIndex = hashFunc(key) % capacity;
        HashNode<Key,Value>*newNode= new HashNode<Key,Value>(key,value);
        newNode->next=buckets[bucketIndex];
        buckets[bucketIndex]=newNode;
        size++;
    }

    //查找
    Value*findValue(const Key& key){
        size_t bucketIndex = hashFunc(key) % capacity;
        HashNode<Key, Value>* node = buckets[bucketIndex];
        while(node!=nullptr){
            if(equalFunc(node->key,key)){
                return &node->value;
            }
        node = node->next;
        }
        return nullptr;
    }

    //删除
    bool erase(const Key& key){
        size_t bucketIndex = hashFunc(key) % capacity;
        HashNode<Key, Value>* node = buckets[bucketIndex];
        HashNode<Key, Value>* prev = nullptr;
        while(node!=nullptr){
            if(equalFunc(node->key,key)){
                if(prev!=nullptr){
                    prev->next=node->next;
                }else{
                    buckets[bucketIndex]=node->next;
                }
                delete node;
                size--;
                return true;
            }
            prev=node;
            node=node->next;
        }
        return false;
    }

    int getSize(){
        return size;
    }

    int getCapacity(){
        return capacity;
    }
};

int main() {
    // 假设默认容量为较小的值以便快速触发重哈希，例如DEFAULT_CAPACITY设置为5或更小
    HashMap<std::string, std::string> map;

    // 插入足够的元素以触发重哈希
    map.insert("name1", "John Doe");
    map.insert("occupation1", "Software Engineer");
    map.insert("language1", "C++");
    map.insert("name2", "Jane Doe");
    map.insert("occupation2", "Data Scientist");
    // 此处可能已触发重哈希，具体取决于负载因子设置

    // 继续插入更多元素以确保重哈希工作正常
    map.insert("language2", "Python");
    map.insert("name3", "Jim Beam");
    map.insert("occupation3", "Project Manager");

    // 查找元素以验证它们是否仍正确存在
    std::string keys[] = {"name1", "occupation1", "language1", "name2", "occupation2", "language2", "name3", "occupation3"};
    for (const auto& key : keys) {
        std::string* value = map.findValue(key);
        if (value) {
            std::cout << "Found " << key << ": " << *value << std::endl;
        } else {
            std::cout << key << " not found after rehash" << std::endl;
        }
    }

    // 输出当前大小和容量来观察重哈希的效果
    // 注意：需要在HashMap类中添加方法来获取当前的大小和容量，例如 getSize() 和 getCapacity()
    std::cout << "Current size: " << map.getSize() << std::endl;
    std::cout << "Current capacity: " << map.getCapacity() << std::endl;

    return 0;
}