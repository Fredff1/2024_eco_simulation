#include "MainModel.hpp"

 void MainModel::generateNewData(){
        FrameData data;
        data.h=quadTreeAtlas.getAtlasSize().first;
        data.w=quadTreeAtlas.getAtlasSize().second;
        quadTreeAtlas.update(data);
        newData=data;
    }