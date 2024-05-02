#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

template<typename T>
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(const T* data) = 0; // 在Model变化时被调用
};

/*被观察的对象继承subject
方法：
attach 添加一个observer到其list中
detach 删除一个observer
notifyObservers 通知所有observer并调用其update方法*/
template<typename T>
class Subject {
private:
    std::list<Observer<T>*> observers;

public:
    ~Subject(){
        for(Observer<T>* obs:observers){
            delete(obs);
        }
    }

    void attach(Observer<T>* obs) {
        observers.push_back(obs);
    }

    void detach(Observer<T>* obs) {
        observers.remove(obs);
    }

    std::list<Observer<T>*> getObservers(){
        return this->observers;
    }

    virtual void notifyObservers()=0;
};



#endif