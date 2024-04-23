#ifndef ENVIRONMENT_FAC_H
#define ENVIRONMENT_FAC_H
#include "Util/GlobalEnum.hpp"
#include <math.h>


class EnvironmentFactor{
private:
    //0-100
    float oxygenLevel;
    //0-14
    float pHLevel;
    
    float temperature;
public:
    EnvironmentFactor(float oxy,float ph,float temperature):oxygenLevel(oxy),pHLevel(ph),temperature(temperature){}

    // void computeAllStates(float x,float y,AtlasState state){
    //     computeOxygenInAtlas(x,y,state);
    //     computePHInAtlas(x,y,state);
    //     computeTempInAtlas(x,y,state);
    // }

    // Getter and Setter for temperature
    float getTemperature()  { 
        return temperature; }
    void setTemperature(float temp) { temperature = temp; }
    float computeTempInAtlas(float x,float y,AtlasState state){
        float temperature = 0.0;
        // 计算x和y相对于中心的偏移
        float dx = x - ATLAS_FULL_WIDTH/2;
        float dy = y - ATLAS_FULL_HEIGHT/2;
        switch(state){
            case DEFAULT_ATLAS_STATE:
            temperature = 40 - (std::sqrt(dx*dx+dy*dy) / 100);
            break;
        }
        return temperature;
    }

    // Getter and Setter for oxygen level
    float getOxygenLevel() const { return oxygenLevel; }
    void setOxygenLevel(float oxygen) { oxygenLevel = oxygen; }
    float computeOxygenInAtlas(float x,float y,AtlasState state){
        float dx = x - ATLAS_FULL_WIDTH/2;
        float dy = y - ATLAS_FULL_HEIGHT/2;
        switch (state){
            case DEFAULT_ATLAS_STATE:
                oxygenLevel=30-(std::sqrt(dx*dx+dy*dy) / 1000);
                break;
            
            default:
                break;
        }
        return oxygenLevel;
    }

    // Getter and Setter for pH value
    float getPHValue() const { return pHLevel; }
    void setPHValue(float pH) { pHLevel = pH; }
    float computePHInAtlas(float x,float y,AtlasState state){
        float dx = x - ATLAS_FULL_WIDTH/2;
        float dy = y - ATLAS_FULL_HEIGHT/2;
        switch (state){
            case DEFAULT_ATLAS_STATE:
                
                pHLevel=10-(std::sqrt(dx*dx+dy*dy) / 1000);
                break;
            
            default:
                break;
        }
        return pHLevel;
    }
};


#endif
