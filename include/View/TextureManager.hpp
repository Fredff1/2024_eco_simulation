#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <unordered_map>
#include <string>
#include <math.h>
#include <SDL2/SDL_image.h>

class TextureManager{
private:
    std::unordered_map<std::string,SDL_Texture*> textureMap;
    SDL_Renderer* renderer;
    const std::string resourcePath="./assets/";
public:
    TextureManager(SDL_Renderer* renderer):renderer(renderer){
        
    }
     ~TextureManager(){
        for(auto&entry:textureMap){
            SDL_DestroyTexture(entry.second);
        }
    }


    void setRenderer(SDL_Renderer* renderer){
        this->renderer=renderer;
    }

    SDL_Texture* loadTexture(const std::string& fileName){
        if (textureMap.find(fileName) == textureMap.end()) {
            auto resPath=resourcePath+fileName;
            SDL_Surface* tempSurface = IMG_Load(resPath.c_str());
            if (!tempSurface) {
                printf("Failed to load image: %s\n", IMG_GetError());
                return nullptr;
            }
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
            if (!tex) {
                printf("Failed to create texture: %s\n", SDL_GetError());
                return nullptr;
            }
            textureMap[fileName] = tex;
        }
        return textureMap[fileName];
    }

    // void drawTexture(SDL_Texture* tex,const SDL_Rect* srcRect,const SDL_Rect* destRect){
    //     SDL_RenderCopy(renderer,tex,srcRect,destRect);
    // }

    // void drawTexture(const std::string& fileName,const SDL_Rect* srcRect,const SDL_Rect* destRect){
    //     auto tex=textureMap[fileName];
    //     SDL_RenderCopy(renderer,tex,srcRect,destRect);
    // }


    void drawTexture(SDL_Texture* tex, const SDL_Rect* srcRect, const SDL_Rect* dstRect, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
        SDL_RenderCopyEx(renderer, tex, srcRect, dstRect, angle, center, flip);
    }

    void drawTexture(const std::string& fileName, const SDL_Rect* srcRect, const SDL_Rect* dstRect, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
        auto tex=textureMap[fileName];
        SDL_RenderCopyEx(renderer, tex, srcRect, dstRect, angle, center, flip);
    }

    void drawTexture(const std::string& fileName, const SDL_Rect* dstRect, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
        auto tex=textureMap[fileName];
        int srcWidth=0,srcHeight=0;
        SDL_Rect srcRect;
        SDL_QueryTexture(tex,nullptr,nullptr,&srcWidth,&srcHeight);
        srcRect.x=0,srcRect.y=0;
        double dstRate = static_cast<double>(dstRect->h) / dstRect->w;
        double srcRate = static_cast<double>(srcHeight) / srcWidth;

        // if (dstRate == srcRate) {
        //     // 比率相同，使用整个图像
        //     srcRect.w = srcWidth;
        //     srcRect.h = srcHeight;
        // } else if (dstRate > srcRate) {
        //     // 目标比源纹理更窄和更高
        //     srcRect.h = srcHeight; // 根据目标比率调整高度
        //     srcRect.w = srcHeight/dstRate; // 宽度使用完整纹理宽度
        //     if (srcRect.w > srcWidth) {
        //         srcRect.w = srcWidth; // 防止超出边界
        //     }
        // } else {
        //     // 目标比源纹理更宽和更矮
        //     srcRect.w = srcWidth ; // 根据目标比率调整宽度
        //     srcRect.h = srcWidth*dstRate; // 高度使用完整纹理高度
        //     if (srcRect.h > srcHeight) {
        //         srcRect.h = srcHeight; // 防止超出边界
        //     }
        // }

        if(dstRate-srcRate<0.000000001){
            SDL_RenderCopyEx(renderer, tex,nullptr, dstRect, angle, center, flip);
        }else{
            SDL_RenderCopyEx(renderer, tex,&srcRect, dstRect, 0, center, flip);
        }
        
        
    }

    void deleteTexture(const std::string& fileName) {
        auto it = textureMap.find(fileName);
        if (it != textureMap.end()) {
            SDL_DestroyTexture(it->second);  // 销毁纹理
            textureMap.erase(it);            // 从映射中删除条目
        }
    }



};

#endif /* AF1919FF_48A3_412C_A852_7CFBA7A7DC53 */
