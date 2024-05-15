#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <unordered_map>
#include <string>
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

    void deleteTexture(const std::string& fileName) {
        auto it = textureMap.find(fileName);
        if (it != textureMap.end()) {
            SDL_DestroyTexture(it->second);  // 销毁纹理
            textureMap.erase(it);            // 从映射中删除条目
        }
    }



};

#endif /* AF1919FF_48A3_412C_A852_7CFBA7A7DC53 */
