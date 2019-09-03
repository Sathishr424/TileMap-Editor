#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H
#include <iostream>
#include <vector>
#include <String>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


class gameSystem
{
    public:
        gameSystem();
        void gameLoop();
        void init();
        void loadTextures();
        void layout();
        void update();
        void renderTextures();
        void textDisplayToTheScreen(string t, int s, Color c, Vector2f pos);
        void changeScale();
        bool checkPickPoints(int x);
        Vector2i addRows(int,int);
        string randomFileName();
        void loadExternalFile();
        void nextPage();
        void previousPage();
        void multipleSelector(Vector2f);
        void closing();

    private:
        RectangleShape bg, lineX, lineY, nextButton, previousButton, sideSpace, highlighter;
        Texture extraTexture;
        Sprite extraSprite;
        vector<Texture> textures;
        vector<string> forExternal;
        vector<vector<int>> mainGrid, subGrid;
        vector<IntRect> spriteCuts;
        vector<int> sizes;
        Sprite sprite;
        Texture currentTexture;
        int cTexture;
        Vector2f textureSize, modifiedSize;
        int currentSubGridValue = 0;
        Clock click, textureChanging;
        int currentTextureIndex = 3;
        int currentBrush = 0;
        int maxScroll;
        int wantedSize = 30;
        int checking;
        int extraTextureCount = 0;
        Vector2f gridScale, sideScale;
        vector<Vector3i> pickPoints;
        bool clicking=0, hasExternal = 0;
        int rowFinderX=0, rowFinderY = 0;
};

#endif // GAMESYSTEM_H
