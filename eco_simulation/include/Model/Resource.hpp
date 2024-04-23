#ifndef RESOURCE_H
#define RESOURCE_H


class ResourceInAtlas{
private:
    int lightResource;
public:
    ResourceInAtlas(){
        lightResource=0;
    }

    int getLightResource(){
        return lightResource;
    }

    void setLightResource(int lightResourceIn){
        lightResource=lightResourceIn;
    }

    void addLightResource(int input){
        this->lightResource+=input;
        
    }

    void minusLightResource(int input){
        this->lightResource-=input;
        if(this->lightResource<0){
            lightResource=0;
        }
    }
};

#endif