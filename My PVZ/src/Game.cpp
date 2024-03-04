#include "Game.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


void Level::loadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
        std::string line;
        int currentWave = 0;

        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue; // Skip empty lines and comments
            }

            std::istringstream iss(line);
            std::string word;
            iss >> word;

            if (word == "LevelNumber:") {
                iss >> levelNumber;
            } else if (word == "MaxWave:") {
                iss >> maxWave;
            } else if (word == "ZombieTypes:") {
                std::string zombieType;
                while (iss >> zombieType) {
                    zombieTypes.push_back(zombieType);
                }
            }else if(word=="SunAmount"){  
                iss>>sun;
            } else if (word == "Waves:") {
                    
                int waveNumber;
                std::string zombieType;
                int row;
                //show the wave which is being editing
                int wave_count=0;
                while (std::getline(file, line) && !line.empty()) {
                    std::istringstream waveStream(line);
                    waveStream>>word;
                    if(word=="Wave"){
                        waveStream>>waveNumber;
                        wave_count++;
                    }else if(word=="zombie"){
                        waveStream>>zombieType>>row;
                        waves[waveNumber-1].zombies.push_back(zombieType);
                        waves[waveNumber-1].zombie_row.push_back(row);
                    }

                    
                }
            } 
            // Add more parsing logic here for other sections if needed
        }
    
}

bool Level::saveLevel(){
    std::ofstream file(savePath);
    if(!file.is_open()){
        return false;
    }
    
    return true;
}

void Game::init_level(){
    playingLevel.init_grid(0);
    playingLevel.isGridGenerated=true;
    playingLevel.loadLevel("./config/LevelInfo.txt");
    plantSlot sun_slot(GUI_factory,0,NoPlantType);
    for(int i=1;i>=0;i--){
        plantSlot tempSLot(GUI_factory,i,PeaShooterType);
        playingLevel.slots.push_back(tempSLot);
    }
    plantSlot tempSLot(GUI_factory,2,SunflowerType);
    playingLevel.slots.push_back(tempSLot);
    if(playingLevel.sun<playerProperty.start_sun){
        playingLevel.sun=playerProperty.start_sun;
    }
    
    
}


void Level::setDefault(){
    zombie_wave_time_trigger=0;
    isPaused=false,chosenPlant=NoPlantType;
    savePath="./config/saved_game.txt",playerState=DEFAULTPLAYERSTATE;
    for(int i=0;i<grid.size();i++){
        grid[i].resize(0);
    }
    grid.resize(0);
    isGridGenerated=false;
    sun=0,isPaused=false;
    
}




bool Level::init_grid(int flag){
    int rows,cols;
    switch(flag){
    case 0:
        rows=5;
        cols=12;//3 for zombies to appear
        grid.resize(rows); // 首先调整外层向量的大小为rows
        for (auto& row : grid) {
            row.resize(cols); // 然后为每行调整大小为cols
        }
        for(int row=0;row<rows;row++){
            for(int col=0;col<cols;col++){
                grid[row][col].plantType=NoPlantType;
                grid[row][col].rowPos=row+1;
                grid[row][col].colPos=col+1;
            }
        }
        return true;
        break;
    }
    return false;
}   

void Game::printGrid(){
    for(const auto&row:playingLevel.grid){
            for(const auto&target:row){
                switch(target.plantType){
                case NoPlantType:
                    cout<<'*'<<' ';
                    break;
                case PeaShooterType:
                    cout<<'A'<<' ';    
                    break;    
                default:
                    cout<<'&'<<' ';
                }
            }
            cout<<endl;
    }
    cout<<endl;
    for(const auto&row:playingLevel.grid){
        for(const auto&target:row){
            for(const auto&target_zombie:target.zombies){
                if(target_zombie->feature.type==BasicZombieType){
                    cout<<'^';
                }
            }
            cout<<"* ";    
        }
        cout<<endl;
    }
}

plantSlot::plantSlot(SDL_factory GUI_factory,int posInput,plantTypeInGrid type_p):state(EMPTY_SLOT), cooldownTime(0){
    slotTextures["slotList"]=GUI_factory.slotTextures["slotList"];
    slotTextures["slot"]=GUI_factory.slotTextures["slot"];
    slotTextures["plant_card"]=GUI_factory.slotTextures["plant_card"];
    slotTextures["plant_card_black"]=GUI_factory.slotTextures["plant_card_black"];
    Pos=posInput;
    position.h=185,position.w=200,position.x=0,position.y=Pos*95+10;
    positions["plant_card"]=position,positions["plant_card"].x+=16,positions["plant_card"].y=60+Pos*100;
    positions["plant_card"].h-=90,positions["plant_card"].w-=45;
    positions["plant_card_cover"]=positions["plant_card"];
    colorOfWord={0,0,0};
    redColorRemainTime=0;
    if(Pos==0){
        slotTextures["Sun_total_bank"]=GUI_factory.slotTextures["Sun_total_bank"];
        position.h=120;
        position.w=125;
        position.x=0;
        position.y=0;
    }
    state=AVAILABLE_SLOT;
    plantType=type_p;
    setPlantTexture(GUI_factory);
    cooldownCount=0;
    cost_word_destRect=positions["plant_card"];
    cost_word_destRect.h=20,cost_word_destRect.x+=1,cost_word_destRect.y+=70;
}

bool plantSlot::is_selected(int mouseX, int mouseY){
    if (mouseX >= positions["plant_card"].x && mouseX <= positions["plant_card"].x + positions["plant_card"].w &&
            mouseY >= positions["plant_card"].y && mouseY <= positions["plant_card"].y + positions["plant_card"].h) {
            // 点击位置在卡槽内
            if (state == AVAILABLE_SLOT) {
                // 处理选中逻辑
                return true;
            }
        }
    return false;
}

void plantSlot::setPlantTexture(SDL_factory GUI_factory){
    switch(plantType){
        case PeaShooterType:
            plantTexture=GUI_factory.plantTextures[PeaShooterType]["PeaShooterTop"];
            textureOnCardRects.second=positions["plant_card"];
            textureOnCardRects.second.x;
            plantCardCost=100;
            cooldownTime=360;
        break;
        case SunflowerType:
            plantTexture=GUI_factory.plantTextures[SunflowerType]["SunflowerTop"];
            textureOnCardRects.second=positions["plant_card"];
            textureOnCardRects.second.x;
            plantCardCost=50;
            cooldownTime=360;
        break;
    }
}


void plantSlot::updateSlotState(){
    if(redColorRemainTime<=30){
        redColorRemainTime++;
    }else{
        colorOfWord={0,0,0};
        redColorRemainTime=0;
    }
    if(cooldownCount>0){
        cooldownCount--;
        positions["plant_card_cover"].h=cooldownCount*positions["plant_card"].h/cooldownTime;
        
    }else{
        cooldownCount=0;
        state=AVAILABLE_SLOT;
        positions["plant_card_cover"]=positions["plant_card"];
        positions["plant_card_cover"].h=0;
    }
}

//加载具体的卡片和植物
void plantSlot::renderPlantCard(SDL_factory& GUI_factory){
    auto flip=SDL_FLIP_NONE;
    SDL_RenderCopyEx(GUI_factory.renderer, slotTextures["plant_card"], nullptr, &positions["plant_card"],0.0,nullptr,flip);
    SDL_RenderCopy(GUI_factory.renderer,plantTexture,nullptr,&textureOnCardRects.second);   
    char sun_buffer[20];
    std::sprintf(sun_buffer,"%d",plantCardCost);     
    GUI_factory.RenderTextCentered(GUI_factory.renderer,GUI_factory.basic_fonts[20] ,sun_buffer, cost_word_destRect,colorOfWord);
    if(cooldownCount>0){
        SDL_RenderCopy(GUI_factory.renderer,slotTextures["plant_card_black"],nullptr,&positions["plant_card_cover"]);   
    }
    //SDL_RenderCopy(GUI_factory.renderer,slotTextures["plant_card_black"],nullptr,&positions["plant_card_black"]);   
}

//加载卡槽背后的栏位
void plantSlot::render(SDL_Renderer* renderer) {
    SDL_RendererFlip flip=SDL_FLIP_NONE;
    if(Pos!=0){ 
        SDL_RenderCopyEx(renderer, slotTextures["slotList"], nullptr, &position,270.0,nullptr,flip);
        // 根据需要渲染状态指示（如冷却遮罩）
    }else{
        SDL_RenderCopyEx(renderer, slotTextures["Sun_total_bank"], nullptr, &position,0.0,nullptr,flip);
        //SDL_RenderCopyEx(renderer, slotTextures["slotList"], nullptr, &position,270.0,nullptr,flip);
    }
        
}




void Game::loadResource(){
    GUI_factory.initialize_sdl();
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
    }
    TTF_Init();
    const std::string path_to_zombie="./assets/pic/zombies";
    const std::string path_to_plant="./assets/pic/plants";
    const std::string path_to_menu="";

    //打开字体
    for(int i=1;i<50;i++){
        GUI_factory.basic_fonts[i]=TTF_OpenFont("./assets/font/Roboto-Black.ttf", i);
    }
    //加载所有图片资源
    GUI_factory.window = GUI_factory.init_window(WINDOW_WIDTH, WINDOW_HEIGHT);
    GUI_factory.renderer = GUI_factory.render_window(GUI_factory.window);
    GUI_factory.zombieTexture[BasicZombieType]["ZombieAlive"]=(IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/basicZombie/basicZombie.png")); 
    GUI_factory.zombieTexture[BasicZombieType]["ZombieHead"]=(IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/basicZombie/basicZombie_head.png")); 
    GUI_factory.zombieTexture[BasicZombieType]["ZombieBody"]=(IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/basicZombie/basicZombie_body.png")); 
    GUI_factory.zombieTexture[ConeHeadZombieType]["ZombieAlive"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/coneHeadZombie/ConeHeadZombie.png");
    GUI_factory.zombieTexture[ConeHeadZombieType]["ConeHead"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/coneHeadZombie/ConeHead.png");
    GUI_factory.zombieTexture[ConeHeadZombieType]["withoutCone"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/coneHeadZombie/BasicZombie.png");
    GUI_factory.zombieTexture[ConeHeadZombieType]["ZombieHead"]=(IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/basicZombie/basicZombie_head.png")); 
    GUI_factory.zombieTexture[ConeHeadZombieType]["ZombieBody"]=(IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/zombies/basicZombie/basicZombie_body.png")); 
    
    GUI_factory.plantTextures[PeaShooterType]["PeaShooterTop"]=(IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/PeaShooter/Peashooter.png"));
    GUI_factory.plantTextures[PeaShooterType]["PeaShooterBase"]=(IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/PeaShooter/shooterBase.png"));
    GUI_factory.plantTextures[PeaShooterType]["peaBullet"]=(IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/PeaShooter/BasicPea.png"));
    GUI_factory.plantTextures[SunflowerType]["SunflowerTop"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/Sunflower/sunflower_top.png");
    GUI_factory.plantTextures[SunflowerType]["SunflowerBase"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/Sunflower/sunflower_base.png");
    GUI_factory.backgroundTextures["GrasslandBackground"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/background1.jpg");
    GUI_factory.backgroundTextures["MainBackground"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/Menu/Menu_background.jpg");
    GUI_factory.buttonTexture["BasicButton_dark"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/Menu/ChooseButton_dark.png");
    GUI_factory.buttonTexture["BasicButton"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/Menu/ChooseButton.png");
    GUI_factory.buttonTexture["level_option"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/Menu/option_menu.png");
    GUI_factory.slotTextures["slotList"]=IMG_LoadTexture(GUI_factory.renderer, "./assets/pic/plants/slot/slot_list.png");;
    GUI_factory.slotTextures["slot"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/slot/slot.png");
    GUI_factory.slotTextures["plant_card"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/slot/plant_card.png");
    GUI_factory.slotTextures["plant_card_black"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/slot/plant_card_black.png");
    //设置为半透明
    SDL_SetTextureBlendMode( GUI_factory.slotTextures["plant_card_black"], SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod( GUI_factory.slotTextures["plant_card_black"], 192);
    GUI_factory.slotTextures["Sun_total_bank"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/plants/slot/SunBank.png");
    GUI_factory.resourceTextures[SUNSHINE_RESOURCE]["sunshine_outer"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/sunshine/sunshine_outer.png");
    GUI_factory.resourceTextures[SUNSHINE_RESOURCE]["sunshine_ball"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/sunshine/sunshine_ball.png");
    GUI_factory.resourceTextures[SUNSHINE_RESOURCE]["sunshine_card"]=IMG_LoadTexture(GUI_factory.renderer,"./assets/pic/sunshine/sunshine_card.png");
    Button button_startGame({300,300,400,200},GUI_factory.buttonTexture["BasicButton"],GUI_factory.buttonTexture["BasicButton_dark"],"Adventure Mode");

    GUI_factory.menu_buttons.insert(std::make_pair("startGame",button_startGame));
    Button button_level_option({1680,0,160,80},GUI_factory.buttonTexture["BasicButton"],GUI_factory.buttonTexture["BasicButton_dark"],"Pause");
    GUI_factory.level_buttons.insert(std::make_pair("Pause",button_level_option));
    Button button_level_pause({720,240,480,540},GUI_factory.buttonTexture["level_option"],GUI_factory.buttonTexture["level_option"]);
    button_level_pause.isAvail=false;
    GUI_factory.level_buttons.insert(std::make_pair("level_option",button_level_pause));
    Button button_level_pause_toMenu({860,450,200,90},GUI_factory.buttonTexture["BasicButton"],GUI_factory.buttonTexture["BasicButton_dark"],"Leave Level");
    Button button_level_pause_backGame({860,560,200,90},GUI_factory.buttonTexture["BasicButton"],GUI_factory.buttonTexture["BasicButton_dark"],"Resume");
    button_level_pause_backGame.isAvail=false,button_level_pause_toMenu.isAvail=false;
    GUI_factory.level_buttons.insert(std::make_pair("level_pause_backGame",button_level_pause_backGame));
    GUI_factory.level_buttons.insert(std::make_pair("level_pause_toMenu",button_level_pause_toMenu));
}

//加载资源以及主菜单 目前只加载资源
void Game::loadMainMenu(){
    for(auto& pair:GUI_factory.menu_buttons){
        pair.second.checkHover(GUI_factory.mouseX,GUI_factory.mouseY);
        pair.second.render(GUI_factory);
    }
}

//row范围1开始，col范围1开始
bool Game::setPlant(plantTypeInGrid plantType,int row,int col){
    unique_ptr<Plant> temp_ptr;
    switch(plantType){
        case PeaShooterType:
        temp_ptr=std::move(plant_factory.createPlant("PeaShooter",row-1,col,backGroundState));
        if(playingLevel.sun>=temp_ptr->feature.cost){
            playingLevel.sun-=temp_ptr->feature.cost;
            playingLevel.grid[row-1][col].plants.push_back(std::move(temp_ptr));
            playingLevel.grid[row-1][col].plantType=plantType;
            playingLevel.playerState=DEFAULTPLAYERSTATE;
            return true;
        }else{
            temp_ptr.reset();
            playingLevel.playerState=DEFAULTPLAYERSTATE;
            return false;
        }
        break;
        case SunflowerType:
        temp_ptr=std::move(plant_factory.createPlant("Sunflower",row-1,col,backGroundState));
        if(playingLevel.sun>=temp_ptr->feature.cost){
            playingLevel.sun-=temp_ptr->feature.cost;
            playingLevel.grid[row-1][col].plants.push_back(std::move(temp_ptr));
            playingLevel.grid[row-1][col].plantType=plantType;
            playingLevel.playerState=DEFAULTPLAYERSTATE;
            return true;
        }else{
            temp_ptr.reset();
            playingLevel.playerState=DEFAULTPLAYERSTATE;
            return false;
        }
        break;
    }
    
    return false;
    
}

void Game::generateZombieWaves(const ZombieWave target_wave){
    int length=target_wave.zombies.size();
    int count=0;
    while(generated_zombie_number<length&&count<3){
        std::string zombieType=target_wave.zombies[generated_zombie_number];
        int rowInfo=target_wave.zombie_row[generated_zombie_number];
        playingLevel.grid[rowInfo-1][11].zombies.push_back(zombie_factory.createZombie(zombieType,rowInfo));
        generated_zombie_number++;
        startGeneratingWaves=true;
        count++;
    }
    if(generated_zombie_number>=length){
        startGeneratingWaves=false;
        generated_zombie_number=0;
        waveNumber++;
    }
    
}

void Game::loadMainGame(int targetLevel){
    
    


}

void Game::updateGameState(){
    switch(gameState){
        case MAIN_MENU:
        backGroundName="MainBackground";
        backGroundState=MAINMENUBACKGROUND;
        break;
        case INLEVEL:
        backGroundName="GrasslandBackground";
        backGroundState=GRASSLANDBACKGROUND;
        break;

    }
}

void Game::mainGame(){
    const int frameDelay = 1000 / FPS; 
    Uint32 frameStart;
    int frameTime;
    isRunning=true;
    loadResource();
    backGroundName="MainBackground";
    SDL_RenderCopy(GUI_factory.renderer, GUI_factory.backgroundTextures[backGroundName], &GUI_factory.backgroundSrcRect, &GUI_factory.backgroundDestRect); // 这里假设你想在全屏幕渲染图片
    // 更新屏幕
    SDL_RenderPresent(GUI_factory.renderer);
    int tick_sum=0;
    gameState=MAIN_MENU;
    while(isRunning){
        
        handleEvents();
        
        updateGameState();
        SDL_RenderClear(GUI_factory.renderer);
        frameStart = SDL_GetTicks(); // 循环开始时的时间
        SDL_RenderCopy(GUI_factory.renderer, GUI_factory.backgroundTextures[backGroundName], &GUI_factory.backgroundSrcRect, &GUI_factory.backgroundDestRect); // 这里假设你想在全屏幕渲染图片
        // 处理输入
        // 更新游戏状态
        if(gameState==INLEVEL||gameState==INLEVEL_PAUSE){
            if(playingLevel.isGridGenerated==false){
                init_level();
            }  
            InLevel("Level 1");
            
        }else if(gameState==MAIN_MENU){
            loadMainMenu();
        }
        
        
        SDL_RenderPresent(GUI_factory.renderer);
        

        frameTime = SDL_GetTicks() - frameStart; // 计算已消耗的时间

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // 等待直到达到目标帧时长
        }
        
    
    }
}



void Game::InLevel(const std::string LevelInfo){
    
    if(!playingLevel.isPaused){
        if(playingLevel.isWave_started&&playingLevel.zombie_wave_time_trigger>=800||startGeneratingWaves){
            playingLevel.zombie_wave_time_trigger=0;
            startGeneratingWaves=true;
            generateZombieWaves(playingLevel.waves[waveNumber-1]);
        }else if(waveNumber>=playingLevel.maxWave-1){
            gameState=MAIN_MENU;
            playingLevel.setDefault();
            waveNumber=0;
        }
        if(playingLevel.zombie_wave_time_trigger>=1800&&playingLevel.isWave_started==false){
            playingLevel.isWave_started=true;
        }
        updateLevelGUI();
        updatePlantBulletAct();
        updateZombieAct();
        updatePlantAct();
        generateSunshine();
        updateSunshineState();
    }
        
        //printGrid();
        // 渲染
        renderPlants();
        renderZombies();
        renderPlantBullet();
        renderSunshine();
        renderLevelGUI();
        highlightPlantTarget();
        playingLevel.zombie_wave_time_trigger++;
}

void Game::updateLevelGUI(){
    
}

void Game::renderLevelGUI(){
    //加载按钮
    for(auto& pair:GUI_factory.level_buttons){
        if(pair.second.isAvail){
            pair.second.checkHover(GUI_factory.mouseX,GUI_factory.mouseY);
            pair.second.render(GUI_factory);
        }   
    }
    //加载卡插栏
    for(auto& temp_slot:playingLevel.slots){
        temp_slot.render(GUI_factory.renderer);
        if(temp_slot.Pos==playingLevel.slots.size()-1){
            auto flip=SDL_FLIP_HORIZONTAL;
            auto temp_rect=temp_slot.position;
            temp_rect.y+=60,temp_rect.x-=0;
            SDL_RenderCopyEx(GUI_factory.renderer,temp_slot.slotTextures["slotList"], nullptr, &temp_rect,270.0,nullptr,flip);
        }
    }
    //加载具体植物卡片
    for(auto&temp_slot:playingLevel.slots){
        temp_slot.updateSlotState();
        //这个if用来显示总的阳光
        if(temp_slot.Pos==0){
            auto temp_rect=temp_slot.position;
            temp_rect.h/=4,temp_rect.y+=3*temp_rect.h;
            char sun_total[40];
            std::sprintf(sun_total,"%d",playingLevel.sun);
            GUI_factory.RenderTextCentered(GUI_factory.renderer,GUI_factory.basic_fonts[20],sun_total,temp_rect,temp_slot.colorOfWord);
            continue;
        }
        temp_slot.renderPlantCard(GUI_factory);
    }
    
}

//不只是移动，包含了僵尸的所有行为
void Game::zombiesMove(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            
            for(auto&target_zombie:cell.zombies){

                cout<<row<<" "<<col<<"zombie here"<<endl;
            }
        }
    }
}

void Game::updateZombieAct(){
    updateZombieState();
    updateZombiePos();
    //检测僵尸是否死去并将其删除
    for (int row = 0; row < playingLevel.grid.size(); ++row) {
        for (int col = 0; col < playingLevel.grid[row].size(); ++col) {
            auto& cell = playingLevel.grid[row][col];
            auto it = cell.zombies.begin();
            while (it != cell.zombies.end()) {
                if ((*it)->zombieState==DEAD_ZOMBIE) {
                    // 收集僵尸及其新位置，这里假设你已经更新了 (*it)->colPos 和 (*it)->rowPos
                    it = cell.zombies.erase(it); // 移除当前僵尸，并安全地移动迭代器到下一个元素
                } else {
                    ++it;
                }
            }
        }
    }
}

//僵尸进行移动 攻击等
void Game::updateZombieState(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            //攻击的逻辑
            for(auto&target_zombie:cell.zombies){
                if(playingLevel.grid[row][col].plants.empty()&&target_zombie->zombieState==ATTACKING){
                        target_zombie->zombieState=MOVING;
                }
                for(int i=col+1;i>col-2;i--){
                    if(i>0&&i<10){
                            //首先检测僵尸是否碰撞植物，如果是状态为攻击
                        for(auto&target_plant:playingLevel.grid[row][i].plants){
                            target_zombie->is_collied_with_plant(target_plant->collisionRects);
                        }
                        //选择攻击对象，优先选择外层的对象
                        if(target_zombie->zombieState==ATTACKING){
                            for(auto&target_plant:playingLevel.grid[row][i].plants){
                                if(target_plant->grid_layer_pos==2){
                                    target_zombie->attack(target_plant);
                                    break;
                                }else if(target_plant->grid_layer_pos==1){
                                    target_zombie->attack(target_plant);
                                    break;
                                }
                            }
                        }
                    }
                    
                    
                }
            }
            //其他行动逻辑
            for(auto&target_zombie:cell.zombies){
                target_zombie->act();
                cout<<row<<" "<<col<<"zombie here"<<endl;
            }
        }
    }
}

void Game::updateZombiePos(){
    //第一次遍历：标记
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_zombie:cell.zombies){
                if(target_zombie->colPos!=col+1){
                    target_zombie->isGridChanged=true;
                }else if(target_zombie->colPos<0){
                    cout<<"game over"<<endl;
                    gameState=MAIN_MENU;
                    playingLevel.loadLevel("./config/LevelInfo.txt");
                }
            }
        }
    }
    //第二次遍历：移动
    std::vector<std::tuple<int, int, unique_ptr<Zombie>>> zombiesToMove;
    for (int row = 0; row < playingLevel.grid.size(); ++row) {
        for (int col = 0; col < playingLevel.grid[row].size(); ++col) {
            auto& cell = playingLevel.grid[row][col];
            auto it = cell.zombies.begin();
            while (it != cell.zombies.end()) {
                if ((*it)->isGridChanged) {
                    // 收集僵尸及其新位置，这里假设你已经更新了 (*it)->colPos 和 (*it)->rowPos
                    zombiesToMove.push_back(std::make_tuple((*it)->rowPos, (*it)->colPos, std::move(*it)));
                    it = cell.zombies.erase(it); // 移除当前僵尸，并安全地移动迭代器到下一个元素
                } else {
                    ++it;
                }
            }
        }
    }

    // 第二步：将僵尸移动到它们的新位置
    for (auto& zombieInfo : zombiesToMove) {
        int newRow = std::get<0>(zombieInfo)-1;
        int newCol = std::get<1>(zombieInfo)-1;
        std::get<2>(zombieInfo)->isGridChanged=false;
        unique_ptr<Zombie> zombie = std::move(std::get<2>(zombieInfo));

        // 确保新位置有效
        if (newRow >= 0 && newRow < playingLevel.grid.size() && newCol >= 0 && newCol < playingLevel.grid[newRow].size()) {
            playingLevel.grid[newRow][newCol].zombies.push_back(std::move(zombie));
        }
    }
}



void Game::updatePlantBulletAct(){
    updateBulletPos();
    updateBulletCollision();
    
    
    
}

void Game::updateBulletPos(){
     for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_bullet:cell.bullets){
                target_bullet->update(backGroundState);
            }
        }
    }
     //第一次遍历：标记
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_bullet:cell.bullets){
                if(target_bullet->colPos!=col+1){
                    target_bullet->isGridChanged=true;
                }
            }
        }
    }
    //第二次遍历：移动
    std::vector<std::tuple<int, int, unique_ptr<PlantBullet>>> bulletToMove;
    for (int row = 0; row < playingLevel.grid.size(); ++row) {
        for (int col = 0; col < playingLevel.grid[row].size(); ++col) {
            auto& cell = playingLevel.grid[row][col];
            auto it = cell.bullets.begin();
            while (it != cell.bullets.end()) {
                if ((*it)->isGridChanged) {
                    // 收集僵尸及其新位置，这里假设你已经更新了 (*it)->colPos 和 (*it)->rowPos
                    bulletToMove.push_back(std::make_tuple((*it)->rowPos-1, (*it)->colPos, std::move(*it)));
                    it = cell.bullets.erase(it); // 移除当前僵尸，并安全地移动迭代器到下一个元素
                } else if((*it)->isActive==false){   
                    (*it).reset();
                    it=cell.bullets.erase(it);
                }else {
                    ++it;
                }
            }
        }
    }

    // 第二步：将弹幕移动到它们的新位置
    for (auto& bulletInfo : bulletToMove) {
        int newRow = std::get<0>(bulletInfo);
        int newCol = std::get<1>(bulletInfo);
        std::get<2>(bulletInfo)->isGridChanged=false;
        unique_ptr<PlantBullet> bullet = std::move(std::get<2>(bulletInfo));

        // 确保新位置有效
        if (newRow >= 0 && newRow < playingLevel.grid.size() && newCol >= 0 && newCol < playingLevel.grid[newRow].size()) {
            playingLevel.grid[newRow][newCol].bullets.push_back(std::move(bullet));
        }
    }
}



void Game::updateBulletCollision(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            //检查每一个弹幕的碰撞,每个三格
            for(auto bulletIt=cell.bullets.begin();bulletIt!=cell.bullets.end();){
                
                for(int target_col=col-1;target_col<col+2;target_col++){
                    if((*bulletIt)->isActive==false)break;
                    if(target_col<0||target_col>=11)continue;
                    auto& temp_cell=playingLevel.grid[row][target_col];
                    for(auto& target_zombie:temp_cell.zombies){
                        //每次攻击前，先更新弹幕的活动状态
                        if((*bulletIt)->isActive==false)break;
                        //防止穿透弹幕对僵尸造成帧伤
                        auto it_1=std::find((*bulletIt)->attack_zombie_ids.begin(),(*bulletIt)->attack_zombie_ids.end(),target_zombie->id);
                        if(it_1!=(*bulletIt)->attack_zombie_ids.end())break;
                        bool can_attack_zombie_body=true;
                        bool flag_bulletCanPierce=((*bulletIt)->pierceAbility==INFINITY_PIERCE||((*bulletIt)->pierceAbility==LIMITED_PIERCE)&&(*bulletIt)->pierceCount<(*bulletIt)->MaxPierceNum);
                        //这个循环用于
                        for(auto&target_protective:target_zombie->protectiveList){
                            if(flag_bulletCanPierce==true&&target_protective.is_pierce_available){
                                //跳过可以穿透的护具
                                continue;
                                //不能穿透的防具不能跳过
                            }else if((*bulletIt)->isActive==true&&target_protective.is_pierce_available==false){
                               //这个循环用于决定是否与僵尸碰撞，如果是，则对其防具造成伤害并终止循环
                               for(auto&zombie_part:target_zombie->collisionRectMap){
                                    if((*bulletIt)->checkCollision(zombie_part.second)&&can_attack_zombie_body==true){
                                         target_protective.health-=(*bulletIt)->damage;
                                        (*bulletIt)->pierceCount++;
                                        (*bulletIt)->updatePierceAbility();
                                        (*bulletIt)->attack_zombie_ids.push_back(target_zombie->id);
                                        can_attack_zombie_body=false;
                                        break;
                                    }
                                } 
                            }                                    
                        }
                        //这个循环用于对僵尸本体造成伤害，如果已经对防具造成了伤害，则这个循环不会触发
                        for(auto&zombie_part:target_zombie->collisionRectMap){
                            if((*bulletIt)->checkCollision(zombie_part.second)&&can_attack_zombie_body==true){
                                target_zombie->feature.health -= (*bulletIt)->damage;
                                (*bulletIt)->pierceCount++;
                                (*bulletIt)->updatePierceAbility();
                                (*bulletIt)->attack_zombie_ids.push_back(target_zombie->id);
                                break;
                            }
                        }
                    }
                }
                if ((*bulletIt)->isActive==false) {
                    // 如果发生了碰撞，从容器中移除弹幕
                     bulletIt = cell.bullets.erase(bulletIt);
                } else {
                    ++bulletIt; // 否则，继续检查下一个弹幕
                }
            }
            
        }
    }
}

void Game::renderZombies(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_zombie:cell.zombies){
                target_zombie->render_zombie(GUI_factory.renderer,backGroundState);
            }
        }
    }
}

//植物的攻击等行动
void Game::updatePlantAct(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_plant:cell.plants){
                
                target_plant->act(playingLevel);
                target_plant->testEnemy(playingLevel.grid);
            }
        }
    }
    for (int row = 0; row < playingLevel.grid.size(); ++row) {
        for (int col = 0; col < playingLevel.grid[row].size(); ++col) {
            auto& cell = playingLevel.grid[row][col];
            auto it = cell.plants.begin();
            while (it != cell.plants.end()) {
                if ((*it)->feature.health<0) {
                    // 收集僵尸及其新位置，这里假设你已经更新了 (*it)->colPos 和 (*it)->rowPos
                    it = cell.plants.erase(it); // 移除当前僵尸，并安全地移动迭代器到下一个元素
                } else {
                    ++it;
                }
            }
            if(cell.plants.empty()){
                cell.plantType=NoPlantType;
            }
        }
    }
}

void Game::generateSunshine(){
    if(playingLevel.sunGenerateCount>=600){
        int tar_x=200+rand()%1600;
        int tar_y=30+rand()%200;
        Sunshine newSunshine(tar_x,tar_y,25,GUI_factory,FALLING_SUNSHINE);
        playingLevel.sunshineList.push_back(newSunshine);
        playingLevel.sunGenerateCount=0;
    }else{
        playingLevel.sunGenerateCount++;
    }
    
}
//更新阳光的状态   
void Game::updateSunshineState(){
    for(auto&sunshine:playingLevel.sunshineList){
        switch(sunshine.state){
            //处理下落逻辑
            case FALLING_SUNSHINE:
            if(sunshine.destRect.y<=sunshine.falling_dest.y){
                sunshine.destRect.y+=2;
            }else{
                sunshine.state=STABLE_SUNSHINE;
            }
            break;
            case MOVING_TO_SUNSHINEBANK:
            
            if(sunshine.destRect.x>=sunshine.gathered_dest.x||sunshine.destRect.y>=sunshine.gathered_dest.y){
                sunshine.destRect.x-=sunshine.x_off;
                sunshine.destRect.y-=sunshine.y_off;
            }else{
                sunshine.state=DISAPPEARING;
                playingLevel.sun+=sunshine.value;
            }
            break;
            case PLANT_GENERATED:
            if(sunshine.destRect.y<=sunshine.falling_dest.y){
                sunshine.destRect.y+=sunshine.y_off;
                sunshine.y_off+=sunshine.acceleration_y;
            }else{
                sunshine.state=STABLE_SUNSHINE;
            }
                
            break;
        }
    }

    for(auto it=playingLevel.sunshineList.begin();it!=playingLevel.sunshineList.end();){
        (*it).disappearCount++;
        if((*it).disappearCount>(*it).disappearMaxTime&&(*it).state==STABLE_SUNSHINE){
            it=playingLevel.sunshineList.erase(it);
        }else if((*it).state==DISAPPEARING){
            it=playingLevel.sunshineList.erase(it);
        }else{
            ++it;
        }
    }
}

void Game::renderSunshine(){
    for(auto&sunshine_tar:playingLevel.sunshineList){
        sunshine_tar.render(GUI_factory.renderer);
    }
}

void Game::renderPlants(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_plant:cell.plants){
                target_plant->renderPlant(GUI_factory.renderer,backGroundState);
            }
        }
    }
}

void Game::renderPlantBullet(){
    for(int row=0;row<playingLevel.grid.size();row++){
        for(int col=0;col<playingLevel.grid[0].size();col++){
            auto& cell=playingLevel.grid[row][col];
            for(auto&target_bullet:cell.bullets){
                if(!target_bullet){
                    continue;
                }
                target_bullet->render(GUI_factory.renderer,backGroundState);
                cout<<"Bullet here"<<row<<col<<endl;
            }
        }
    }
}


void Game::handleEvents(){
    while (SDL_PollEvent(&e) != 0) { // 循环获取所有事件
        SDL_GetMouseState(&GUI_factory.mouseX,&GUI_factory.mouseY);
        switch (e.type) {
            case SDL_QUIT: // 用户请求退出
                isRunning = false;
                break;
            case SDL_KEYDOWN: // 键盘按键按下
                handleKeyDownEvent();
                break;
            case SDL_KEYUP:
                handleKeyUpEvent();
                break;    
            case SDL_MOUSEBUTTONDOWN: // 鼠标按钮按下
                // 处理鼠标点击...
                handleMouseDownEvent();
                break;
            // 处理其他事件类型...
            case SDL_MOUSEBUTTONUP:
                handleMouseUpEvent();
                break;
            case SDL_MOUSEMOTION:
                handleMouseMotionEvent();
                break;    
        
        }
    }
}

void Game::handleKeyDownEvent(){
    switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: // 按下ESC键
        switch(gameState){
            case MAIN_MENU:
            isRunning=false;
            break;
            case INLEVEL :
            playingLevel.isPaused=true;
            GUI_factory.changeLevelPauseState(INLEVEL_PAUSE,gameState);
            break;
            case INLEVEL_PAUSE:
            playingLevel.isPaused=false;
            GUI_factory.changeLevelPauseState(INLEVEL,gameState);
            break;
        }
        break;
        // 处理其他按键...
    }
}

void Game::handleKeyUpEvent(){

}



void Game::handleMouseButtonRight(){
    switch(gameState){
        case MAIN_MENU:

        break;
        case INLEVEL:
        switch (playingLevel.playerState){
            case DEFAULTPLAYERSTATE:

            break;
            case CHOOSE_PLANT:
                playingLevel.playerState=DEFAULTPLAYERSTATE;
                playingLevel.chosenPlant=NoPlantType;
            break;
        }
        break;
    }
}

void Game::handleMouseButtonLeft(){
    switch(gameState){
        case MAIN_MENU:
        for(auto& pair:GUI_factory.menu_buttons){
            if(pair.first=="startGame"&&pair.second.isClicked(GUI_factory.mouseX,GUI_factory.mouseY)){
                gameState=INLEVEL;
                playingLevel.loadLevel(" ");
                break;
            }
        }
        break;
        case INLEVEL:
        switch(playingLevel.playerState){
            case DEFAULTPLAYERSTATE:
            for(auto&pair:GUI_factory.level_buttons){
                if(pair.second.isAvail&&pair.second.isClicked(GUI_factory.mouseX,GUI_factory.mouseY)){
                    if(pair.first=="Pause"&&gameState==INLEVEL){
                        playingLevel.isPaused=true;
                        GUI_factory.changeLevelPauseState(INLEVEL_PAUSE,gameState);
                    }
                }
            }

            //处理阳光
            for(auto& temp_sunshine:playingLevel.sunshineList){
                temp_sunshine.isCollected(GUI_factory.mouseX,GUI_factory.mouseY);
            }
            //处理是否选择植物
            for(auto& temp_slot:playingLevel.slots){
                if(temp_slot.is_selected(GUI_factory.mouseX,GUI_factory.mouseY)&&temp_slot.state==AVAILABLE_SLOT){
                    playingLevel.playerState=CHOOSE_PLANT;
                    playingLevel.chosenPlant=temp_slot.plantType;
                    playingLevel.chosenSlotNum=temp_slot.Pos;
                    
                    break;
                }else if(temp_slot.is_selected(GUI_factory.mouseX,GUI_factory.mouseY)&&temp_slot.state==COOLDOWN_SLOT){

                }
            }
            break;
            case CHOOSE_PLANT:
            for(auto&pair:GUI_factory.level_buttons){
                if(pair.second.isAvail&&pair.second.isClicked(GUI_factory.mouseX,GUI_factory.mouseY)){
                    if(pair.first=="Pause"&&gameState==INLEVEL){
                        playingLevel.isPaused=true;
                        GUI_factory.changeLevelPauseState(INLEVEL_PAUSE,gameState);
                    }
                }
            }
            //处理是否选择植物
            for(auto& temp_slot:playingLevel.slots){
                if(temp_slot.is_selected(GUI_factory.mouseX,GUI_factory.mouseY)&&temp_slot.state==AVAILABLE_SLOT){
                    playingLevel.playerState=CHOOSE_PLANT;
                    playingLevel.chosenPlant=temp_slot.plantType;
                    playingLevel.chosenSlotNum=temp_slot.Pos;
                    
                    break;
                }else if(temp_slot.is_selected(GUI_factory.mouseX,GUI_factory.mouseY)&&temp_slot.state==COOLDOWN_SLOT){

                }
            }
            //检测条件，种植植物
            int mouseRow,mouseCol;
            auto pair=getPosFromXY(GUI_factory.mouseX,GUI_factory.mouseY,backGroundState);
            mouseRow=pair.first,mouseCol=pair.second;
            if(pair.first<0||pair.second<0){
                break;
            }
            if(playingLevel.grid[mouseRow-1][mouseCol].plantType==NoPlantType&&mouseCol<10){
                bool flag_set=setPlant(playingLevel.chosenPlant,mouseRow,mouseCol);
                for(auto&slot:playingLevel.slots){
                    if(slot.Pos==playingLevel.chosenSlotNum){
                        if(flag_set){
                            slot.cooldownCount=slot.cooldownTime;
                            slot.state=COOLDOWN_SLOT;
                        }
                        
                    }
                }
                if(flag_set==false){
                    for(auto&slot:playingLevel.slots){
                        if(slot.Pos==0){
                            slot.colorOfWord={255,0,0};
                        }
                    }
                }
                
            }
            break;
        }
        
        break;
        case INLEVEL_PAUSE:
        for(auto&pair:GUI_factory.level_buttons){
            if(pair.second.isAvail&&pair.second.isClicked(GUI_factory.mouseX,GUI_factory.mouseY)){
                if(pair.first=="level_pause_backGame"){
                    playingLevel.isPaused=false;
                    GUI_factory.changeLevelPauseState(INLEVEL,gameState);
                }else if(pair.first=="level_pause_toMenu"){
                    playingLevel.isPaused=false;
                    GUI_factory.changeLevelPauseState(MAIN_MENU,gameState);
                }
            }
        }

        break;
    }
}

void Game::handleMouseButtonMiddle(){

}
void Game::handleMouseDownEvent(){
    SDL_GetMouseState(&GUI_factory.mouseX,&GUI_factory.mouseY);
    switch(e.button.button){
        case SDL_BUTTON_LEFT:
        handleMouseButtonLeft();
        break;
        case SDL_BUTTON_MIDDLE:
        handleMouseButtonMiddle();
        break;
        case SDL_BUTTON_RIGHT:
        handleMouseButtonRight();
        break;
    }
    
}

void Game::handleMouseUpEvent(){
    SDL_GetMouseState(&GUI_factory.mouseX,&GUI_factory.mouseY);
    switch(gameState){
        case INLEVEL:
        switch(playingLevel.playerState){
            case CHOOSE_PLANT:
            auto pair=getPosFromXY(GUI_factory.mouseX,GUI_factory.mouseY,backGroundState);
            if(pair.first>0&&pair.first<=5&&pair.second>0&&pair.second<=9){
                SDL_SetRenderDrawColor(GUI_factory.renderer,255,0,255,255);
                SDL_Rect targetRect;
                targetRect.x=HOME_LENGTH_PIXEL+(pair.first-1)*GRID_WIDTH_PIXEL;
                targetRect.y=GRID_SKY_HEIGHT/2+(pair.second-1)*GRID_HEIGHT_PIXEL;
                targetRect.h=GRID_HEIGHT_PIXEL,targetRect.w=GRID_WIDTH_PIXEL;
                SDL_RenderFillRect(GUI_factory.renderer, &targetRect);
            }
            break;
        
        }
        break;
    }
}

void Game::handleMouseMotionEvent(){
    SDL_GetMouseState(&GUI_factory.mouseX,&GUI_factory.mouseY);
    switch(gameState){
        case INLEVEL:
        switch(playingLevel.playerState){
            case CHOOSE_PLANT:
            auto pair=getPosFromXY(GUI_factory.mouseX,GUI_factory.mouseY,backGroundState);
            // if(pair.first>0&&pair.first<=5&&pair.second>0&&pair.second<=9){
            //     SDL_SetRenderDrawColor(GUI_factory.renderer,255,0,255,255);
            //     SDL_Rect targetRect;
            //     targetRect.x=HOME_LENGTH_PIXEL+(pair.first-1)*GRID_WIDTH_PIXEL;
            //     targetRect.y=GRID_SKY_HEIGHT/2+(pair.second-1)*GRID_HEIGHT_PIXEL;
            //     targetRect.h=GRID_HEIGHT_PIXEL,targetRect.w=GRID_WIDTH_PIXEL;
            //     SDL_RenderFillRect(GUI_factory.renderer, &targetRect);
            // }
            break;
        
        }
        break;
    }
}

void Game::highlightPlantTarget(){
    if(gameState==INLEVEL&&playingLevel.playerState==CHOOSE_PLANT){
        SDL_GetMouseState(&GUI_factory.mouseX,&GUI_factory.mouseY);
        auto pair=getPosFromXY(GUI_factory.mouseX,GUI_factory.mouseY,backGroundState);
        SDL_SetRenderDrawBlendMode(GUI_factory.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(GUI_factory.renderer,0,0,0,56);
        SDL_Rect targetRect;
        targetRect.x=HOME_LENGTH_PIXEL+(pair.second-1)*GRID_WIDTH_PIXEL;
        targetRect.y=GRID_SKY_HEIGHT*2+(pair.first-1)*GRID_HEIGHT_PIXEL;
        targetRect.h=GRID_HEIGHT_PIXEL,targetRect.w=GRID_WIDTH_PIXEL;
        SDL_RenderFillRect(GUI_factory.renderer, &targetRect);
    }
    
}


   
    
    
            
           
        
    


