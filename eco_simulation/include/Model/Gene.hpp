#ifndef GENE_H
#define GENE_H

#include <iostream>
#include <random>
#include <cmath>
#include <map>
#include "Util/RandomUtil.hpp"

#define MIN_TEMPERATURE_RANGE -50
#define TEMPERATURE_RANGE 100

#define MIN_OXY_RANGE 0
#define OXY_RANGE 50

#define MIN_PH_RANGE 0
#define PH_RANGE 14

#define MIN_AGE_RANGE -10000
#define AGE_RANGE 30000

#define MIN_HUNGER_RANGE -1000
#define HUNGER_RANGE 2000

#define MIN_HEALTH_RANGE 0
#define HEALTH_RANGE 100000

#define MIN_REPRODUCTION_RATE 1
#define REPRODUCTION_RANGE 30

enum GeneType{
    TEMPERATURE_GENE,
    OXYGEN_GENE,
    PH_GENE,
    AGE_GENE,
    REPRODUCTION_GENE,
    HUNGER_GENE,
    HEALTH_GENE,
};

class Gene{
private:
    unsigned short temperatureGene;
    unsigned short oxygenGene;
    unsigned short pHGene;
    unsigned short ageGene;
    unsigned short reproductionGene;
    unsigned short hungerGene;
    unsigned short healthGene;
    std::map<GeneType,unsigned short> geneMap;
   

    // 转换基因值到实际环境适应值
    /* Gene can suit the range of ()*/
    static float mapGeneToRange(unsigned short gene, float min, float range, float offset) {
        return static_cast<float>(gene) * range / 65535 + min;
    }

    /* Cast true value to gene value
       Parameters :
       value : the true value
       min : the min value of the real
       range : max-min*/
    static unsigned short mapRangeToGene(float value, float min, float range) {
        return static_cast<unsigned short>((value - min) * 65535 / range);
    }

    /* the after parameters are for compute
    Parameters :
    actualValue :the environment value
    optimalValue :the value that best matches the gene
    threshold : the max distance to tolerate
    scale : how much influence*/
    static float calculateFitness(float actualValue, float optimalValue, float threshold, float scale) {
        // 距离越远适应度下降更快
        float distance = std::abs(actualValue - optimalValue);
        float fitness = std::exp(-scale * distance * distance);
        return fitness;
        //return (distance > threshold) ? (-fitness) : fitness;
    }

public:
    Gene(){
        // Temperature: 映射到 -50 到 50，中点为 0
        float midTemperature = 20;
        temperatureGene = RandomUtil::getRandomInt(
            mapRangeToGene(midTemperature - 10, MIN_TEMPERATURE_RANGE, TEMPERATURE_RANGE),  // 略微小于中点
            mapRangeToGene(midTemperature + 10, MIN_TEMPERATURE_RANGE, TEMPERATURE_RANGE)   // 略微大于中点
        );
        geneMap[TEMPERATURE_GENE]=temperatureGene;
        // Oxygen: 映射到 15 到 35，中点为 25
        float midOxygen = 25;
        oxygenGene = RandomUtil::getRandomInt(
            mapRangeToGene(midOxygen - 2, MIN_OXY_RANGE, OXY_RANGE),
            mapRangeToGene(midOxygen + 2, MIN_OXY_RANGE, OXY_RANGE)
        );
        geneMap[OXYGEN_GENE]=oxygenGene;
        // pH: 映射到 2 到 12，中点为 7
        float midPH = 7;
        pHGene = RandomUtil::getRandomInt(
            mapRangeToGene(midPH - 1, MIN_PH_RANGE, PH_RANGE),
            mapRangeToGene(midPH + 1, MIN_PH_RANGE, PH_RANGE)
        );
        geneMap[PH_GENE]=pHGene;
        float midAge=5000;
        ageGene= RandomUtil::getRandomInt(
            mapRangeToGene(midAge-2000,MIN_AGE_RANGE,AGE_RANGE),
            mapRangeToGene(midAge+2000,MIN_AGE_RANGE,AGE_RANGE)
        );
        geneMap[AGE_GENE]=ageGene;
        float midHUnger=500;
        hungerGene=RandomUtil::getRandomInt(
            mapRangeToGene(midHUnger-100,MIN_HUNGER_RANGE,HUNGER_RANGE),
            mapRangeToGene(midHUnger+100,MIN_HUNGER_RANGE,HUNGER_RANGE)
        );
        geneMap[HUNGER_GENE]=hungerGene;
        float midHealth=40000;
        healthGene=RandomUtil::getRandomInt(
            mapRangeToGene(midHealth-15000,MIN_HEALTH_RANGE,HEALTH_RANGE),
            mapRangeToGene(midHealth+15000,MIN_HEALTH_RANGE,HEALTH_RANGE)
        );
        geneMap[HEALTH_GENE]=healthGene;
        float midReproductionRate=3;
        reproductionGene=mapRangeToGene(midReproductionRate,MIN_REPRODUCTION_RATE,REPRODUCTION_RANGE);
        geneMap[REPRODUCTION_GENE]=reproductionGene;
    }

    Gene(int flag){

    }

    

    // 使用随机数生成器初始化基因
    void randomizeGenes() {
        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 gen(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(0, 65535); // Define the range

        temperatureGene = distr(gen);
        oxygenGene = distr(gen);
        pHGene = distr(gen);
    }

    void randomMutate(unsigned short& gene){
        int signal=RandomUtil::getRandomInt(0,20);
        if(signal!=1){
            return;
        }
        int bit = RandomUtil::getRandomInt(0,15);
        gene^=1<<bit;
    }
    
    std::map<GeneType,unsigned short>& getMap(){
        return this->geneMap;
    }

    void copyGeneTo(Gene& copyTarget){
        auto& targetMap = copyTarget.getMap();  // 获取一次引用，避免多次调用
        for (const auto& pair : geneMap) {
            targetMap[pair.first] = pair.second;
            randomMutate(targetMap[pair.first]);
        }
    }

     

    /* Return a fitness score */
    float calculateActualTemperature(float actualTemperature){
        float optimalTemperature = mapGeneToRange(geneMap[TEMPERATURE_GENE], MIN_TEMPERATURE_RANGE, TEMPERATURE_RANGE, 0);
        return  calculateFitness(actualTemperature, optimalTemperature, 10, 0.0005);
    }
    /* Return a fitness score */
    float calculateActualOxygen(float actualOxygen){
        float optimalOxygen = mapGeneToRange(geneMap[OXYGEN_GENE], MIN_OXY_RANGE, OXY_RANGE, 0);
        return calculateFitness(actualOxygen, optimalOxygen, 5, 0.001);
    }

    /* Return a fitness score */
    float calculateActualPH(float actualPH){
        float optimalPH = mapGeneToRange(geneMap[PH_GENE], MIN_PH_RANGE, PH_RANGE, 0);
        return calculateFitness(actualPH, optimalPH, 5, 0.0015);
    }

    /* Return the max age according to gene*/
    float calculateActualMaxAge(){
        return mapGeneToRange(geneMap[AGE_GENE],MIN_AGE_RANGE,AGE_RANGE,0);
    }
    /* Return the max hunger according to gene*/
    float calculateActualMaxHunger(){
        return mapGeneToRange(geneMap[HUNGER_GENE],MIN_HUNGER_RANGE,HUNGER_RANGE,0);
    }

    float calculateActualMaxHealth(){
        return mapGeneToRange(geneMap[HEALTH_GENE],MIN_HEALTH_RANGE,HEALTH_RANGE,0);
    }

    int calculateReproductionRate(){
        int value=mapGeneToRange(geneMap[REPRODUCTION_GENE],MIN_REPRODUCTION_RATE,REPRODUCTION_RANGE,0);;
        if(value>0){
            return value;
        }
        return 1;
    }

    //  Gene& operator=(const Gene& other) {
    //     if (this == &other) {
    //         // 处理自赋值情况
    //         return *this;
    //     }

    //     // 复制每个基因值
    //     temperatureGene = other.temperatureGene;
    //     oxygenGene = other.oxygenGene;
    //     pHGene = other.pHGene;
    //     ageGene = other.ageGene;
    //     reproductionGene = other.reproductionGene;
    //     hungerGene = other.hungerGene;
    //     healthGene = other.healthGene;

    //     return *this;  // 返回当前对象的引用
    // }
};

#endif 
