#ifndef MODEL_TO_CONTROLLER_DATA_HPP
#define MODEL_TO_CONTROLLER_DATA_HPP
#include <variant>
#include <algorithm>

enum ModelToControllerDataType{
    NO_MODEL_TO_CONTROLLER_DATA,
};

struct ModelInitData{

};

struct ModelToControllerData{
    std::variant<ModelInitData> data;
    ModelToControllerDataType type=NO_MODEL_TO_CONTROLLER_DATA;
    template<typename T>
    T* getData(){
        return std::get_if<T>(data);
    }

};



#endif /* EEE9BEE3_0B17_4F56_85F9_3423914E6F41 */
