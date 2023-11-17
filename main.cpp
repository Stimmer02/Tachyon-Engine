
#include <functional>

using Function = std::function<void()>;

enum EventType{

};

class UIBuilder
{
private:
    /* data */
public:
    UIBuilder();
    UIBuilder* AssignEventManager(){
        //costam
        return this;
    }




    template<typename T>
    T* Build(){
        //costam
        return new T();
    }
    ~UIBuilder();
};
