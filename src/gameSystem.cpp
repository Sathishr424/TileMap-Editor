#include "gameSystem.h"
#include <iostream>
#include <String>
#include <ctime>
#include <fstream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


const int width = 1200, height = 600;
RenderWindow window(VideoMode(1200, 600), "SA_TileEditor", Style::Titlebar | Style::Close);

gameSystem::gameSystem()
{
    bg = RectangleShape(Vector2f(width-400, height));
    bg.setFillColor(Color(128,128,128));
    bg.setPosition(0,0);

    ifstream input;
    input.open("res\\lastVisitedTexture.ini");
    string line;
    while (getline(input, line)){
        currentTextureIndex = stoi(line);
    }input.close();

    init();
}

void gameSystem::init()
{
    ifstream input, inputForSize;
    input.open("res\\textures.ini");
    inputForSize.open("res\\sizes.ini");
    string line;
    while (getline(input, line)){
        Texture tTmp;
        tTmp.loadFromFile("res\\" + line);
        textures.push_back(tTmp);
    }input.close();
    line = "";
    while (getline(inputForSize, line)){
        sizes.push_back(stoi(line));
    }inputForSize.close();

    loadExternalFile();

    highlighter = RectangleShape(Vector2f(30,30));
    highlighter.setFillColor(Color(255,0,0,150));

    extraTexture.create(780, height);
    extraSprite.setTexture(extraTexture);
    extraSprite.setPosition(0,0);

    loadTextures();
    lineX = RectangleShape(Vector2f(width-400, 2));
    lineY = RectangleShape(Vector2f(2, height));

    lineX.setFillColor(Color::White);
    lineY.setFillColor(Color::White);

    nextButton = RectangleShape(Vector2f(20, 60));
    nextButton.setFillColor(Color(180, 0, 0));
    previousButton = RectangleShape(Vector2f(20, 85));
    previousButton.setFillColor(Color(0, 180, 0));
    previousButton.setPosition(830, 150);
    nextButton.setPosition(1150, 150);

    sideSpace = RectangleShape(Vector2f(430, height));
    sideSpace.setFillColor(Color::Black);
    sideSpace.setPosition(width-420, 0);

}

void gameSystem::textDisplayToTheScreen(string t, int s, Color c, Vector2f pos)
{
    Font font;
    font.loadFromFile("res\\arial.ttf");
    Text text(t, font, s);
    text.setPosition(pos);
    text.setFillColor(c);
    text.setOrigin(0, 0);
    text.setStyle(Text::Bold);
    text.rotate(-90);
    window.draw(text);
}

void gameSystem::loadExternalFile()
{
    ifstream input;
    input.open("external\\externalFiles.ini");
    string line;
    while (getline(input, line)){
        Texture tTmp;
        forExternal.push_back("external\\" + line);
    }input.close();
}

void gameSystem::loadTextures()
{
    sprite.setScale(1,1);
    currentSubGridValue = 0;
    currentBrush = 0;
    srand(time(0));

    //cTexture = rand()%(textures.size()-1);
    //cTexture = currentTextureIndex;
    currentTexture = textures[currentTextureIndex];
    Vector2u tmp1 = currentTexture.getSize();
    textureSize = Vector2f(tmp1.x, tmp1.y);
    sprite.setTexture(currentTexture);

    checking = int(textureSize.x)%sizes[currentTextureIndex];

    if (checking == 0)
        maxScroll = int((textureSize.x/sizes[currentTextureIndex])*(textureSize.y/sizes[currentTextureIndex]))-140;//+sizes[currentTextureIndex];
    else
        maxScroll = int((textureSize.x/sizes[currentTextureIndex])*(textureSize.y/sizes[currentTextureIndex]))-120;//+sizes[currentTextureIndex];

    //maxScroll = int((textureSize.x/sizes[currentTextureIndex])*(textureSize.y/sizes[currentTextureIndex]))-120;//+sizes[currentTextureIndex];

    //changeScale();

    modifiedSize = Vector2f(sizes[currentTextureIndex], sizes[currentTextureIndex]);
    spriteCuts.clear();

    for (int i=0; i<textureSize.x; i+=modifiedSize.x){
        for (int j=0; j<textureSize.y; j+=modifiedSize.y){
            if (i <= textureSize.x && j <= textureSize.y)
                spriteCuts.push_back(IntRect(i, j, modifiedSize.x, modifiedSize.y));
        }
    }

    changeScale();

    mainGrid.clear();
    subGrid.clear();
    currentSubGridValue = 0;
    vector<int> tmp;
    for (int i=0; i<height; i+=modifiedSize.y){
        tmp.push_back(0);
    }for (int i=0; i<width-400; i+=modifiedSize.x){
        mainGrid.push_back(tmp);
    }

    //cout << mainGrid.size() << "," << mainGrid[0].size() << endl;
    vector <int> tmp2;
    for (int i=0; i<textureSize.y; i+=modifiedSize.y){
        tmp2.push_back(0);
    }
    for (int i=0; i<8; i++){
        subGrid.push_back(tmp2);
    }
    int tmpl = 0;
    for (unsigned int i=0; i<subGrid.size(); i++){
        for (unsigned int j=0; j<subGrid[i].size(); j++){
            subGrid[i][j] = (tmpl+1)+currentSubGridValue;
            tmpl++;
        }
    }

    for (int i=0; i<width-400; i+=modifiedSize.x){
        for (int j=0; j<height; j+=modifiedSize.y){
            srand(time(0));
            mainGrid[int(i/modifiedSize.x)][int(j/modifiedSize.y)] = 0;
        }
    }
}

static void resetGrid(vector<vector<int>> &mainGrid, vector<vector<int>> subGrid, Vector2f modifiedSize)
{
    srand(time(0));
    for (int i=0; i<width-400; i+=modifiedSize.x){
        for (int j=0; j<height; j+=modifiedSize.y){
            mainGrid[int(i/modifiedSize.x)][int(j/modifiedSize.y)] = subGrid[rand() % subGrid.size()][rand() % subGrid[0].size()];
        }
    }
}

void gameSystem::changeScale()
{
    float scaleX = (float)wantedSize / sizes[currentTextureIndex];
    gridScale = Vector2f(scaleX, scaleX);

    sprite.setScale(scaleX, scaleX);

    scaleX = (float)15 / sizes[currentTextureIndex];

    sideScale = Vector2f(scaleX, scaleX);

    modifiedSize = Vector2f(wantedSize,wantedSize);

//    textureSize = Vector2f((textureSize.x/sizes[currentTextureIndex])*wantedSize, (textureSize.y/sizes[currentTextureIndex])*wantedSize);
    textureSize = Vector2f(textures[currentTextureIndex].getSize().x*gridScale.x, textures[currentTextureIndex].getSize().y*gridScale.x);
    //cout << sprite.getGlobalBounds().width << ", " << sprite.getGlobalBounds().height << endl;
}


bool gameSystem::checkPickPoints(int x)
{
    for (unsigned int i=0; i<pickPoints.size(); i++){
        if (pickPoints[i].x == x){
            return false;
        }
    }return true;
}

Vector2i gameSystem::addRows(int x, int y)
{
    Vector2i finalReturn;
    if (x > rowFinderX) finalReturn.x = 1;
    else if (x < rowFinderX) finalReturn.x = -1;
    else finalReturn.x = 0;

    if (y > rowFinderY) finalReturn.y = 1;
    else if (y < rowFinderY) finalReturn.y = -1;
    else finalReturn.y = 0;

    return finalReturn;
}

string gameSystem::randomFileName()
{
    srand(time(0));
    string name = "";
    for (int i=0; i<10; i++){
        name += to_string(rand()%9);
    }

    return name;
}

void gameSystem::nextPage()
{
    pickPoints.clear();
    if (currentSubGridValue > 0){
        if (checking == 0)
            currentSubGridValue-=int(textureSize.y/modifiedSize.y);
        else
            currentSubGridValue-=int(textureSize.y/modifiedSize.y)+1;
        int tmpl = 0;
        for (unsigned int i=0; i<subGrid.size(); i++){
            for (unsigned int j=0; j<subGrid[i].size(); j++){
                subGrid[i][j] = (tmpl+1)+currentSubGridValue;
                tmpl++;
            }
        }
    }
}

void gameSystem::previousPage()
{
    pickPoints.clear();
    if (currentSubGridValue < maxScroll){
        if (checking == 0)
        currentSubGridValue+=int(textureSize.y/modifiedSize.y);
        else
        currentSubGridValue+=int(textureSize.y/modifiedSize.y)+1;
        //currentSubGridValue+=int(textureSize.y/modifiedSize.y);
        int tmpl = 0;
        for (unsigned int i=0; i<subGrid.size(); i++){
            for (unsigned int j=0; j<subGrid[i].size(); j++){
                subGrid[i][j] = (tmpl+1)+currentSubGridValue;
                tmpl++;
            }
        }
    }
}

void gameSystem::multipleSelector(Vector2f pos)
{
    int tmpX = 0, tmpY = 0;
    bool curr = 0;
    for (unsigned int i=0; i<subGrid.size(); i++){
        tmpX+=2;
        tmpY=0;
        for (unsigned int j=0; j<subGrid[i].size(); j++){
            if (pos.x > (i*modifiedSize.x)+870+tmpX && pos.x < (i*modifiedSize.x)+870+tmpX+modifiedSize.x
             && pos.y > (j*modifiedSize.y)+50+tmpY && pos.y < (j*modifiedSize.y)+50+tmpY+modifiedSize.y){
                if (!curr){
                    if (checkPickPoints(subGrid[i][j])){
                        if (pickPoints.size() > 0){
                            Vector2i temp = addRows(tmpX, tmpY);
                            pickPoints.push_back(Vector3i(subGrid[i][j], temp.x, temp.y));
                            rowFinderY = tmpY;
                            rowFinderX = tmpX;
                        }else{
                            pickPoints.push_back(Vector3i(subGrid[i][j], 0, 0));
                            rowFinderY = tmpY;
                            rowFinderX = tmpX;
                        }
                        curr=1;
                    }
                }
            }
            tmpY+=2;
            }
        }
}

void gameSystem::update()
{
    if (((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl)) &&
        Keyboard::isKeyPressed(Keyboard::S))){
          Texture tTexture;
          window.clear();
          window.draw(bg);
          layout();
          if (hasExternal) window.draw(extraSprite);
              for (unsigned int i=0; i<mainGrid.size(); i++){
                for (unsigned int j=0; j<mainGrid[i].size(); j++){
                    if (mainGrid[i][j] != 0){
                        sprite.setTextureRect(spriteCuts[mainGrid[i][j]-1]);
                        sprite.setPosition(i*modifiedSize.x, j*modifiedSize.y);
                        window.draw(sprite);
                    }
                }
            }
          window.setSize(Vector2u(780, height));
          Vector2u tSize = window.getSize();
          tTexture.create(tSize.x, tSize.y);
          //layout();
          window.display();
          tTexture.update(window);
          Image screeenshot = tTexture.copyToImage();
          screeenshot.setPixel(780, tSize.y, Color(255,255,255,0));
          screeenshot.saveToFile("SavedFiles\\" + randomFileName() + ".png");
          window.setSize(Vector2u(width, height));
    }

    if (window.hasFocus()){

    if (textureChanging.getElapsedTime().asMilliseconds() > 60){
    if (Keyboard::isKeyPressed(Keyboard::R)){
        resetGrid(mainGrid, subGrid, modifiedSize);
    }
    if (Keyboard::isKeyPressed(Keyboard::End)){
        if ((unsigned)extraTextureCount < forExternal.size()){
            hasExternal = 1;
            extraTextureCount++;
            extraTexture.loadFromFile(forExternal[extraTextureCount-1]);
        }
    }else if(Keyboard::isKeyPressed(Keyboard::Home)){
        if (extraTextureCount > 0){
            extraTextureCount--;
            extraTexture.loadFromFile(forExternal[extraTextureCount-1]);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::PageDown)){
        pickPoints.clear();
        currentBrush = 0;
        if ((unsigned)currentTextureIndex < textures.size()-1){
            currentTextureIndex++;
            Texture tTexture;
            window.clear();
            window.draw(bg);
            layout();
            if (hasExternal) window.draw(extraSprite);
              for (unsigned int i=0; i<mainGrid.size(); i++){
                for (unsigned int j=0; j<mainGrid[i].size(); j++){
                    if (mainGrid[i][j] != 0){
                        sprite.setTextureRect(spriteCuts[mainGrid[i][j]-1]);
                        sprite.setPosition(i*modifiedSize.x, j*modifiedSize.y);
                        window.draw(sprite);
                    }
                }
            }
            window.setSize(Vector2u(780, height));
            Vector2u tSize = window.getSize();
            tTexture.create(tSize.x, tSize.y);
            //layout();
            window.display();
            tTexture.update(window);
            Image screeenshot = tTexture.copyToImage();
            screeenshot.setPixel(780, tSize.y, Color(255,255,255,0));
            screeenshot.saveToFile("data\\temp.png");
            window.setSize(Vector2u(width, height));
            hasExternal = 1;
            extraTexture.loadFromFile("data\\temp.png");
            loadTextures();
        }
    }if (Keyboard::isKeyPressed(Keyboard::PageUp)){
        pickPoints.clear();
        currentBrush = 0;
        if (currentTextureIndex > 0){
            currentTextureIndex--;
            Texture tTexture;
            window.clear();
            window.draw(bg);
            layout();
            if (hasExternal) window.draw(extraSprite);
              for (unsigned int i=0; i<mainGrid.size(); i++){
                for (unsigned int j=0; j<mainGrid[i].size(); j++){
                    if (mainGrid[i][j] != 0){
                        sprite.setTextureRect(spriteCuts[mainGrid[i][j]-1]);
                        sprite.setPosition(i*modifiedSize.x, j*modifiedSize.y);
                        window.draw(sprite);
                    }
                }
            }
            window.setSize(Vector2u(780, height));
            Vector2u tSize = window.getSize();
            tTexture.create(tSize.x, tSize.y);
            //layout();
            window.display();
            tTexture.update(window);
            Image screeenshot = tTexture.copyToImage();
            screeenshot.setPixel(780, tSize.y, Color(255,255,255,0));
            screeenshot.saveToFile("data\\temp.png");
            window.setSize(Vector2u(width, height));
            hasExternal = 1;
            extraTexture.loadFromFile("data\\temp.png");
            loadTextures();
        }
    }
    textureChanging.restart();
    }

    if (Keyboard::isKeyPressed(Keyboard::Delete)) hasExternal = 0;
    if (Keyboard::isKeyPressed(Keyboard::Z)) pickPoints.clear();

    if (((Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::RShift)) &&
        Keyboard::isKeyPressed(Keyboard::A))){
        for (unsigned int i=0; i<mainGrid.size(); i++){
            for (unsigned int j=0; j<mainGrid[i].size(); j++){
                if (mainGrid[i][j] == 0)
                mainGrid[i][j] = currentBrush;
            }
        }
    }
    else if (((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl)) &&
        Keyboard::isKeyPressed(Keyboard::A))){
        for (unsigned int i=0; i<mainGrid.size(); i++){
            for (unsigned int j=0; j<mainGrid[i].size(); j++){
                mainGrid[i][j] = currentBrush;
            }
        }
    }else if (((Keyboard::isKeyPressed(Keyboard::LAlt) || Keyboard::isKeyPressed(Keyboard::RAlt)) &&
        Keyboard::isKeyPressed(Keyboard::A))){
        for (unsigned int i=0; i<mainGrid.size(); i++){
            for (unsigned int j=0; j<mainGrid[i].size(); j++){
                if (mainGrid[i][j] == currentBrush)
                mainGrid[i][j] = 0;
            }
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Delete) || Keyboard::isKeyPressed(Keyboard::Q)){
        for (unsigned int i=0; i<mainGrid.size(); i++){
            for (unsigned int j=0; j<mainGrid[i].size(); j++){
                mainGrid[i][j] = 0;
            }
        }
    }

    Vector2i tmp = Mouse::getPosition(window);
    Vector2f pos(tmp.x, tmp.y);
    if (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl)){
        if (Mouse::isButtonPressed(Mouse::Left) && pos.x > 800){
            multipleSelector(pos);
        }
    }else if(Mouse::isButtonPressed(Mouse::Middle) && pos.x > 800){
        multipleSelector(pos);
    }


    else if (Mouse::isButtonPressed(Mouse::Left)){
        if (pos.x < 799 && pos.x > 0){
            Vector2f tmpPos(pos.x - (int(pos.x) % int(modifiedSize.x)), (pos.y) - (int(pos.y) % int(modifiedSize.y)));
            if(pickPoints.size() > 0){
                int tX = 0, tY = 0;
            for (unsigned int i=0; i<pickPoints.size(); i++){
                if ((tmpPos.x/modifiedSize.x)+tX < mainGrid.size()-1 && (tmpPos.y/modifiedSize.y)+tY < mainGrid[0].size() &&
                    (tmpPos.x/modifiedSize.x)+tX >= 0 && (tmpPos.y/modifiedSize.y)+tY >= 0){
                    tX += pickPoints[i].y; tY += pickPoints[i].z;
                    mainGrid[(tmpPos.x/modifiedSize.x)+tX][tmpPos.y/modifiedSize.y+tY] = pickPoints[i].x;}
            }}else if (currentBrush != 0) mainGrid[tmpPos.x/modifiedSize.x][tmpPos.y/modifiedSize.y] = currentBrush;
        }
        else if (pos.x > 780 && pos.x < width){
            pickPoints.clear();
            int tmpX = 0, tmpY = 0;
            for (unsigned int i=0; i<subGrid.size(); i++){
                tmpX+=2;
                tmpY=0;
                for (unsigned int j=0; j<subGrid[i].size(); j++){
                    if (pos.x > (i*modifiedSize.x)+870+tmpX && pos.x < (i*modifiedSize.x)+870+tmpX+modifiedSize.x
                         && pos.y > (j*modifiedSize.y)+50+tmpY && pos.y < (j*modifiedSize.y)+50+tmpY+modifiedSize.y){
                                currentBrush = subGrid[i][j];
                    }
                    tmpY+=2;
                    }
                }
            }
        }

    if (pos.x > 830 && pos.x < 830+20 && pos.y > 150 && pos.y < 150+60){
        if (click.getElapsedTime().asMilliseconds() > 60){
            if (Mouse::isButtonPressed(Mouse::Left)){
                nextPage();
            }click.restart();
        }
        previousButton.setFillColor(Color(255, 0, 0));}
    else{
        previousButton.setFillColor(Color(180, 0, 0));}
    if (pos.x > 1150 && pos.x < 1150+20 && pos.y > 150 && pos.y < 150+90){
        if (click.getElapsedTime().asMilliseconds() > 60){
            if (Mouse::isButtonPressed(Mouse::Left)){
                previousPage();
            }click.restart();
        }
        nextButton.setFillColor(Color(0, 255, 0));}
    else{
        nextButton.setFillColor(Color(0, 180, 0));
    }}
}

void gameSystem::renderTextures()
{
    for (unsigned int i=0; i<mainGrid.size(); i++){
        for (unsigned int j=0; j<mainGrid[i].size(); j++){
            if (mainGrid[i][j] != 0){
                sprite.setTextureRect(spriteCuts[mainGrid[i][j]-1]);
                sprite.setPosition(i*modifiedSize.x, j*modifiedSize.y);
                window.draw(sprite);
            }
        }
    }
    window.draw(sideSpace);

    window.draw(nextButton);
    window.draw(previousButton);

    textDisplayToTheScreen("Previous", 15, Color::Black, Vector2f(830, 230));
    textDisplayToTheScreen("Next", 15, Color::Black, Vector2f(1150, 200));

    int tmpX = 0, tmpY = 0;
    for (unsigned int i=0; i<subGrid.size(); i++){
        tmpX+=2;
        tmpY=0;
        for (unsigned int j=0; j<subGrid[i].size(); j++){
            if (subGrid[i][j] != 0){
                sprite.setTextureRect(spriteCuts[subGrid[i][j]-1]);
                sprite.setPosition((i*modifiedSize.x)+870+tmpX, (j*modifiedSize.y)+50+tmpY);
                window.draw(sprite);
                tmpY+=2;
            }
        }
    }

    if (pickPoints.size() > 0){
        for (unsigned int k=0; k<pickPoints.size(); k++){
            int tmpX1 = 0, tmpY1 = 0;
            for (unsigned int i=0; i<subGrid.size(); i++){
                tmpX1+=2;
                tmpY1=0;
                for (unsigned int j=0; j<subGrid[i].size(); j++){
                    if (subGrid[i][j] == pickPoints[k].x){
                        highlighter.setPosition((i*modifiedSize.x)+870+tmpX1, (j*modifiedSize.y)+50+tmpY1);
                        window.draw(highlighter);
                    }tmpY1+=2;
                }
            }
        }
    }else if (currentBrush != 0){
        int tmpX1 = 0, tmpY1 = 0;
        for (unsigned int i=0; i<subGrid.size(); i++){
            tmpX1+=2;
            tmpY1=0;
            for (unsigned int j=0; j<subGrid[i].size(); j++){
                if (subGrid[i][j] == currentBrush){
                    highlighter.setPosition((i*modifiedSize.x)+870+tmpX1, (j*modifiedSize.y)+50+tmpY1);
                    window.draw(highlighter);
                }tmpY1+=2;
            }
        }
    }
}

void gameSystem::layout()
{
    for (int i=0; i<height; i+=modifiedSize.y){
        lineX.setPosition(0, i);
        window.draw(lineX);
    }

    for (int i=0; i<=width-400; i+=modifiedSize.x){
        lineY.setPosition(i, 0);
        window.draw(lineY);
    }
}

void gameSystem::closing()
{
    ofstream output;
    output.open("res\\lastVisitedTexture.ini");
    output << currentTextureIndex;

    output.close();

    window.close();
}

void gameSystem::gameLoop()
{
    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::EventType::Closed) closing();
            if (event.type == Event::MouseWheelScrolled){
                if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel){
                    if (event.mouseWheelScroll.delta == 1) nextPage();
                    else if (event.mouseWheelScroll.delta == -1) previousPage();
                }
            }
        }
        window.clear();
        window.draw(bg);
        layout();
        if (hasExternal) window.draw(extraSprite);
        renderTextures();
        update();
        window.display();
    }
}








