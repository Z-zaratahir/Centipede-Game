// FOR RUNNING:
// g++ -o run 23i0802_CSA.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio && ./run

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream> // file handling
#include <string>  // for the menu part
#include <sstream> //using this to convert an integer to a string or vice versa

using namespace std;
using namespace sf;

// for the main display menu
void drawMenu(RenderWindow &window, int selectedOption)
{
    // Creating font and text for menu options
    Font font;
    Text playText, highScoreText, instructionsText, exitText;
    font.loadFromFile("Fonts/GA1.ttf");

    // For the contents of the menu-------------------

    // play
    playText.setFont(font);
    playText.setCharacterSize(32);
    playText.setFillColor(selectedOption == 0 ? Color::Red : Color::Black);
    playText.setString("1. Play");
    playText.setPosition(320, 240);

    // high score
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(32);
    highScoreText.setFillColor(selectedOption == 1 ? Color::Red : Color::Black);
    highScoreText.setString("2. High Score");
    highScoreText.setPosition(320, 300);

    // instructions
    instructionsText.setFont(font);
    instructionsText.setCharacterSize(32);
    instructionsText.setFillColor(selectedOption == 2 ? Color::Red : Color::Black);
    instructionsText.setString("3. Instructions");
    instructionsText.setPosition(320, 360);

    // exit
    exitText.setFont(font);
    exitText.setCharacterSize(32);
    exitText.setFillColor(selectedOption == 3 ? Color::Red : Color::Black);
    exitText.setString("4. Exit");
    exitText.setPosition(320, 420);

    // Drawing/displaying the menu options
    window.draw(playText);
    window.draw(highScoreText);
    window.draw(instructionsText);
    window.draw(exitText);
}

// instruction part of menu
void drawInstructions(RenderWindow &window, Font &font)
{
    // Creating text objects for each instruction
    Text title, moveInstruction, eatInstruction, avoidInstruction, returnInstruction;

    // Setting the font for each text object
    title.setFont(font);
    moveInstruction.setFont(font);
    eatInstruction.setFont(font);
    avoidInstruction.setFont(font);
    returnInstruction.setFont(font);

    // Setting character size for the text
    title.setCharacterSize(40);
    moveInstruction.setCharacterSize(24);
    eatInstruction.setCharacterSize(24);
    avoidInstruction.setCharacterSize(24);
    returnInstruction.setCharacterSize(24);

    // Setting text color
    title.setFillColor(Color::Black);
    moveInstruction.setFillColor(Color::Black);
    eatInstruction.setFillColor(Color::Black);
    avoidInstruction.setFillColor(Color::Black);
    returnInstruction.setFillColor(Color::Black);

    // Setting the string for the text
    title.setString("Game Instructions\n\n");
    moveInstruction.setString("Use arrow keys to move the player.");
    eatInstruction.setString("Shoot The mushroom and the centipede to increase score.");
    avoidInstruction.setString("Avoid colliding with the Centipede.");
    returnInstruction.setString("Press Backspace to return to the menu.");

    // Positioning the text on the screen
    title.setPosition(150, 50);
    moveInstruction.setPosition(150, 150);
    eatInstruction.setPosition(150, 200);
    avoidInstruction.setPosition(150, 250);
    returnInstruction.setPosition(150, 300);

    // Drawing each the text to the window
    window.draw(title);
    window.draw(moveInstruction);
    window.draw(eatInstruction);
    window.draw(avoidInstruction);
    window.draw(returnInstruction);
}

// copying the characters from the source string to the destination string
void strcpy(char dest[], const char src[])
{ // custom string copy=strcpy
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// limiting to a limit (n)
void strncpy(char dest[], const char src[], int n)
{
    int i = 0;
    while (src[i] != '\0' && i < n)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// for swapping variables
void swap(int &a, int &b)
{
    // swapping variables
    int temp = a;
    a = b;
    b = temp;
}

// for sorting highscores
void sortScores(int scores[], char names[][100], int count)
{
    // using simple selection sort algorithm for finding max
    for (int i = 0; i < count - 1; i++)
    {
        int maxIdx = i;
        for (int j = i + 1; j < count; j++)
        {
            if (scores[j] > scores[maxIdx])
            {
                maxIdx = j;
            }
        }
        if (maxIdx != i)
        {
            swap(scores[i], scores[maxIdx]);
            char temp[100];
            strcpy(temp, names[i]);
            strcpy(names[i], names[maxIdx]);
            strcpy(names[maxIdx], temp);
        }
    }
}

// storing highscores
int readHighScores(const string &filename, int scores[], char names[][100])
{
    // for reading high scores
    ifstream file(filename);
    string line;
    int count = 0;

    while (getline(file, line) && count < 1000)
    {
        stringstream ss(line);
        string name;
        int score;
        if (getline(ss, name, ',') && ss >> score)
        {
            strncpy(names[count], name.c_str(), 100 - 1);
            names[count][100 - 1] = '\0'; // Ensures null-termination
            scores[count] = score;
            count++;
        }
    }

    sortScores(scores, names, count);
    return count; // Number of scores read
}

// dispalying highscores
void drawHighScores(RenderWindow &window, Font &font, int scores[], char names[][100], int scoreCount)
{
    // for dispalying high scores
    Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setFillColor(Color::Black);
    title.setString("Top 5 High Scores\n\n");
    title.setPosition(200, 50);
    window.draw(title);

    int displayCount = min(scoreCount, 5);
    for (int i = 0; i < displayCount; ++i)
    {
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::Black);
        string scoreStr = to_string(i + 1) + ". " + names[i] + " - " + to_string(scores[i]);
        scoreText.setString(scoreStr);
        scoreText.setPosition(200, 100 + i * 50);
        window.draw(scoreText);
    }
}

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;    // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

void drawPlayer(RenderWindow &window, float player[], Sprite &playerSprite);
void moveBullet(float bullet[], Clock &bulletClock, float mushroom[][3], int &score);
void drawBullet(RenderWindow &window, float bullet[], Sprite &bulletSprite);
void drawMushroom(RenderWindow &window, float mushroom[], Sprite &mushroomSprite);

// for declaring length and speed of centipede
void resetCentipede(float centipede[][3], int centipedeLength, float &centipedeSpeed)
{
    // setting speed
    for (int i = 0; i < centipedeLength; ++i)
    {
        centipede[i][x] = i * boxPixelsX;
        centipede[i][y] = 0;
        centipede[i][exists] = true;
    }

    centipedeSpeed += 10;
}

// checking if bullet hit the mushroom
void checkBulletMushroomCollision(float bullet[], float mushroom[][3], int boxPixelsX, int boxPixelsY, int &score)
{
    for (int i = 0; i < 25; ++i)
    {
        if (mushroom[i][exists] &&
            bullet[exists] &&
            bullet[x] >= mushroom[i][x] && bullet[x] < mushroom[i][x] + boxPixelsX &&
            bullet[y] >= mushroom[i][y] && bullet[y] < mushroom[i][y] + boxPixelsY)
        {
            cout << "Hit mushroom " << i << endl;
            score += 1;
            mushroom[i][exists] = false;
            bullet[exists] = false;
        }
    }
}

// checking if bullet hit the centipede
bool checkBulletCentipedeCollision(float bullet[], float centipede[][3], int centipedeLength, int boxPixelsX, int boxPixelsY, int &score)
{
    bool flag = false;
    for (int i = 0; i < centipedeLength; ++i)
    {
        // check if bullet is within the centipede segment range and if the segment exists (not destroyed) within a range as bullet changes it frame and a different frame is used for the centipede
        if (centipede[i][exists] &&
            bullet[exists] &&
            bullet[x] >= centipede[i][x] && bullet[x] < centipede[i][x] + boxPixelsX &&
            bullet[y] >= centipede[i][y] && bullet[y] < centipede[i][y] + boxPixelsY)
        {
            cout << "Hit centipede segment " << i << endl;
            score += 10;
            bullet[exists] = false;
            centipede[i][exists] = false;
            flag = true;
        }
    }
    return flag;
}

// checking if player touches the centipede
bool checkif_centiped_collides_with_player(float centipede[][3], float player[])
{
    // player is not allowed to touch the centipede
    for (int i = 0; i < 12; i++)
    {
        if (centipede[i][exists] &&
            player[x] >= centipede[i][x] && player[x] < centipede[i][x] + boxPixelsX &&
            player[y] >= centipede[i][y] && player[y] < centipede[i][y] + boxPixelsY)
        {
            return true;
        }
    }
    return false;
}

// for scores
string TO_STRING(int x) // Converts int to string
{
    // for the score
    string str = "";
    while (x)
    {
        str += (x % 10) + '0';
        x /= 10;
    }
    return str;
}

// everything that happens inside the game
int MainGame(RenderWindow &window, string playername)
{
    window.clear();

    // window(VideoMode(resolutionX, resolutionY), "Centipede", Style::Close | Style::Titlebar);

    srand(time(0));
    int score = 0;

    // Initializing Background Music.
    Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(95);

    // Initializing Background.
    Texture backgroundTexture;
    Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

    // Initializing sound effects for bullet
    sf::Music bulletMusic;
    bulletMusic.openFromFile("Sound Effects/fire1.wav");

    // Initializing sound effects for centipede
    sf::Music centipedeMusic;
    centipedeMusic.openFromFile("Sound Effects/newBeat.wav");

    // Initializing sound effects for flea
    sf::Music fleaMusic;
    fleaMusic.openFromFile("Sound Effects/flea.wav");

    // Initializing sound effects for scorpion
    sf::Music scorpionMusic;
    scorpionMusic.openFromFile("Sound Effects/scorpion.wav");

    // Initializing sound effects for scorpion
    sf::Music spiderMusic;
    spiderMusic.openFromFile("Sound Effects/spider.wav");

    // Initializing Player and Player Sprites.
    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameColumns / 1.5) * boxPixelsY;
    Texture playerTexture;
    Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Bullet and Bullet Sprites.
    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initalize mushroom and mushroom sprites
    float mushroom[25][3] = {};
    for (int i = 0; i < 25; i++)
    {
        mushroom[i][x] = rand() % gameColumns * boxPixelsX;
        // Placing mushrooms above 1/3 of the grid
        mushroom[i][y] = rand() % (int)(gameRows / 1.5) * boxPixelsY;
        mushroom[i][exists] = true;
    }
    Texture mushroomTexture;
    Sprite mushroomSprite;
    mushroomTexture.loadFromFile("Textures/mushroom.png");
    mushroomSprite.setTexture(mushroomTexture);
    mushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initialize centipede segments
    const int centipedeLength = 12;
    float centipede[centipedeLength][3];

    for (int i = 0; i < centipedeLength; ++i)
    {
        centipede[i][x] = i * boxPixelsX;
        centipede[i][y] = 0;
        centipede[i][exists] = true;
    }

    // For the movement of the centipede
    bool moveRight = true;
    bool moveLeft = false;
    bool moveDown = false;

    Texture centipedeSegmentTexture;
    centipedeSegmentTexture.loadFromFile("Textures/c_body_left_walk.png");
    Sprite centipedeSegmentSprites[centipedeLength];

    Texture centipedeHeadTexture;
    centipedeHeadTexture.loadFromFile("Textures/headofcenti.png");
    Sprite centipedeHeadSprite;
    centipedeHeadSprite.setTexture(centipedeHeadTexture);
    centipedeHeadSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing sprites for the centipede
    for (int i = 0; i < centipedeLength; ++i)
    {
        centipedeSegmentSprites[i].setTexture(centipedeSegmentTexture);
        centipedeSegmentSprites[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
    }

    float centipedeSpeed = 16;

    // FLEAS------------------------

    // Creating the fleas sprites
    float flea[3] = {};
    flea[x] = rand() % gameColumns * boxPixelsX;
    flea[y] = 0;
    flea[exists] = true;

    bool fleaLeft = true;

    // SCORPION------------------------

    // Creating the scorpion sprites
    float scorpion[3] = {};
    scorpion[x] = rand() % gameColumns * boxPixelsX;
    scorpion[y] = 0;
    scorpion[exists] = true;

    bool scorpionLeft = true;

    // SPIDER------------------------

    // Creating the spider sprites
    float spider[3] = {};
    spider[x] = rand() % gameColumns * boxPixelsX;
    spider[y] = 0;
    spider[exists] = true;

    bool spiderLeft = true;

    // Displaying name and score on top left side
    Font font;
    font.loadFromFile("Fonts/GA1.ttf");

    Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(24);
    nameText.setFillColor(Color::White);
    nameText.setString("Name: " + playername);
    nameText.setPosition(10, 10);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setString("Score: " + TO_STRING(score));
    scoreText.setPosition(10, 40);

    while (window.isOpen())
    {

        // FLEAS---------------------------------

        // For moving the flea
        if (flea[exists])
        {

            /*fleaMusic.stop();
             fleaMusic.play();
             fleaMusic.setVolume(20);*/

            flea[y] += 5; // Adjust the speed of the flea as needed

            // For checking whether the flea has reached the bottom of the grid or not
            if (flea[y] > resolutionY)
            {
                flea[exists] = false; // Flea disappears when it reaches the bottom
            }

            // Check if the flea should leave a trail of mushrooms
            int fleaGridX = static_cast<int>(flea[x] / boxPixelsX);
            int fleaGridY = static_cast<int>(flea[y] / boxPixelsY);

            // Check if there are at least three mushrooms in the player movement area
            int mushroomsInArea = 0;
            for (int i = 0; i < 3; ++i)
            {
                int mushroomGridX = static_cast<int>(mushroom[i][x] / boxPixelsX);
                int mushroomGridY = static_cast<int>(mushroom[i][y] / boxPixelsY);

                // Check if the mushroom is in the player movement area
                if (mushroomGridY >= gameRows / 1.5)
                {
                    ++mushroomsInArea;
                }
            }

            if (mushroomsInArea >= 3)
            {
                // Leave a trail of mushrooms
                if (fleaGridX >= 0 && fleaGridX < gameColumns && fleaGridY >= 0 && fleaGridY < gameRows)
                {
                    mushroom[fleaGridX][fleaGridY] = true;
                }
            }
        }

        // SCORPION---------------------------------

        // For moving the scorpion
        if (scorpion[exists])
        {
            scorpionMusic.stop();
            scorpionMusic.play();
            scorpionMusic.setVolume(10);

            scorpion[y] += 4; // speed

            // For checking whether the scorpion has reached the bottom of the grid or not
            if (scorpion[y] > resolutionY)
            {
                scorpion[exists] = false; // scorpion disappears when it reaches the bottom
            }
        }

        // SPIDER---------------------------------

        // For moving the spider
        if (spider[exists])
        {
            spiderMusic.stop();
            spiderMusic.play();
            spiderMusic.setVolume(10);

            spider[y] += 4; // Adjust the speed of the spider as needed

            // For checking whether the spider has reached the bottom of the grid or not
            if (spider[y] > resolutionY)
            {
                spider[exists] = false; // spider disappears when it reaches the bottom
            }

            // Check if the spider should leave a trail of mushrooms
            /*int scorpionGridX = static_cast<int>(scorpion[x] / boxPixelsX);
            int scorpionGridY = static_cast<int>(scorpion[y] / boxPixelsY);

            // Check if there are at least three mushrooms in the player movement area
            int mushroomsInArea = 0;
            for (int i = 0; i < 3; ++i)
            {
                int mushroomGridX = static_cast<int>(mushroom[i][x] / boxPixelsX);
                int mushroomGridY = static_cast<int>(mushroom[i][y] / boxPixelsY);

                // Check if the mushroom is in the player movement area
                if (mushroomGridY >= gameRows / 1.5)
                {
                    ++mushroomsInArea;
                }
            }

            if (mushroomsInArea >= 3)
            {
                // Leave a trail of mushrooms
                if (scorpionGridX >= 0 && scorpionGridX < gameColumns && scorpionGridY >= 0 && scorpionGridY < gameRows)
                {
                    mushroom[scorpionGridX][scorpionGridY] = true;
                }
            }*/
        }

        // CENTIPEDE----------------------------

        // For the motion / movement of the entire centipede
        for (int i = 0; i < centipedeLength; i++)
        {
            centipedeMusic.stop();
            centipedeMusic.play();
            centipedeMusic.setVolume(8);

            if (moveRight)
            {
                centipede[i][x] += centipedeSpeed;
            }
            else if (moveLeft)
            {
                centipede[i][x] -= centipedeSpeed;
            }

            if (moveDown)
            {
                centipede[i][y] += boxPixelsY;
                if (i == centipedeLength - 1)
                    moveDown = false;
            }
            if ((centipede[i][x] >= resolutionX - boxPixelsX) || (centipede[i][x] <= 0))
            {
                moveDown = true;
                moveRight = !moveRight;
                moveLeft = !moveLeft;
            }
        }

        // Draw the entire centipede as one unit
        for (int i = 0; i < centipedeLength; ++i)
        {
            if (centipede[i][exists])
            {
                if (i == 0)
                {
                    centipedeHeadSprite.setPosition(centipede[i][x], centipede[i][y]);
                    window.draw(centipedeHeadSprite);
                }
                else
                {
                    centipedeSegmentSprites[i].setPosition(centipede[i][x], centipede[i][y]);
                    window.draw(centipedeSegmentSprites[i]);
                }
                centipedeSegmentSprites[i].setPosition(centipede[i][x], centipede[i][y]);
                window.draw(centipedeSegmentSprites[i]);
            }

            // Calling function to check collision between bullet and centipede segments
            if (checkBulletCentipedeCollision(bullet, centipede, centipedeLength, boxPixelsX, boxPixelsY, score))
            {
                for (int i = 0; i < 25; i++)
                {
                    if (mushroom[i][exists] == false) // If mushroom is destroyed, respawn it at a random location
                    {
                        mushroom[i][exists] = true;
                        mushroom[i][x] = rand() % gameColumns * boxPixelsX;
                        mushroom[i][y] = rand() % (int)(gameRows / 1.5) * boxPixelsY;
                    }
                }
            }
        }

        bool allSegmentsDestroyed = true;

        // Check if all centipede segments are destroyed
        for (int i = 0; i < centipedeLength; ++i)
        {
            if (centipede[i][exists])
            {
                allSegmentsDestroyed = false;
                break;
            }
        }

        if (allSegmentsDestroyed) // If all segments are destroyed the game ends and the player wins and score is saved
        {
            window.clear();
            Font font;
            font.loadFromFile("Fonts/GA1.ttf");

            Text winText;
            winText.setFont(font);
            winText.setCharacterSize(40);
            winText.setFillColor(Color::Black);
            winText.setString("You Win!\n\n");
            winText.setPosition(200, 50);
            window.draw(winText);

            fstream file;
            file.open("highscores.csv", ios::app);
            file << playername << "," << score << endl;
            file.close();

            system("pause");
            return 0;
        }

        window.draw(backgroundSprite);
        drawPlayer(window, player, playerSprite);

        if (flea[exists])
        {
            // texture and sprite for the flea
            Texture fleaTexture;
            Sprite fleaSprite;

            fleaTexture.loadFromFile("Textures/flea.png");
            fleaSprite.setTexture(fleaTexture);
            fleaSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

            fleaSprite.setPosition(flea[x], flea[y]);

            window.draw(fleaSprite);
        }

        if (scorpion[exists])
        {
            // texture and sprite for thescorpion
            Texture scorpionTexture;
            Sprite scorpionSprite;

            scorpionTexture.loadFromFile("Textures/scorpion.png");
            scorpionSprite.setTexture(scorpionTexture);
            scorpionSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

            scorpionSprite.setPosition(scorpion[x], scorpion[y]);

            window.draw(scorpionSprite);
        }

        if (spider[exists])
        {
            // texture and sprite for the spider
            Texture spiderTexture;
            Sprite spiderSprite;

            spiderTexture.loadFromFile("Textures/spider_and_score.png");
            spiderSprite.setTexture(spiderTexture);
            spiderSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

            spiderSprite.setPosition(spider[x], spider[y]);

            window.draw(spiderSprite);
        }

        moveBullet(bullet, bulletClock, mushroom, score);
        // For displaying the mushrooms ( randomly )
        for (int i = 0; i < 25; i++)
        {
            if (mushroom[i][exists] == true)
            {
                drawMushroom(window, mushroom[i], mushroomSprite);
            }
        }

        // For displaying the bullet and its movement
        if (bullet[exists] == true)
        {
            moveBullet(bullet, bulletClock, mushroom, score);
            drawBullet(window, bullet, bulletSprite);
        }

        if (flea[exists])
        {
            // bullet and flea are withing a specific range as bullet changes it frame and a different frame is used for the flea
            if (bullet[x] >= flea[x] && bullet[x] < flea[x] + boxPixelsX && bullet[y] >= flea[y] && bullet[y] < flea[y] + boxPixelsY)
            {
                flea[exists] = false;
                bullet[exists] = false;
                score += 5;
            }
        }

        if (scorpion[exists])
        {
            // bullet and scorpion are withing a specific range as bullet changes it frame and a different frame is used for the scorpion
            if (bullet[x] >= scorpion[x] && bullet[x] < scorpion[x] + boxPixelsX && bullet[y] >= scorpion[y] && bullet[y] < scorpion[y] + boxPixelsY)
            {
                scorpion[exists] = false;
                bullet[exists] = false;
                score += 1000;
            }
        }

        if (spider[exists])
        {
            // bullet and spider are withing a specific range as bullet changes it frame and a different frame is used for the spider
            if (bullet[x] >= spider[x] && bullet[x] < spider[x] + boxPixelsX && bullet[y] >= spider[y] && bullet[y] < spider[y] + boxPixelsY)
            {
                spider[exists] = false;
                bullet[exists] = false;
                score += 300;
            }
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                return 0;
            }
            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape)
                {
                    return 0;
                }
                if (e.key.code == Keyboard::Space)
                {
                    if (bullet[exists] == false)
                    {
                        bulletMusic.stop();
                        bulletMusic.play();
                        bulletMusic.setVolume(50);
                        bullet[exists] = true;
                        bullet[x] = player[x];
                        bullet[y] = player[y] - boxPixelsY;
                    }
                }

                // For enabling the Up key
                if (e.key.code == Keyboard::Up && player[y] > (gameColumns / 1.5) * boxPixelsY)
                {
                    player[y] -= boxPixelsY;
                }

                // For enabling the Down key
                if (e.key.code == Keyboard::Down && player[y] < resolutionY - boxPixelsY)
                {
                    player[y] += boxPixelsY;
                }

                // For enabling the left key
                if (e.key.code == Keyboard::Left && player[x] > 0)
                {
                    player[x] -= boxPixelsX;
                }

                // For enabling the Right key
                if (e.key.code == Keyboard::Right && player[x] < resolutionX - boxPixelsX)
                {
                    player[x] += boxPixelsX;
                }
            }
            // calling function to check if the bullet hits the mushroom
            checkBulletMushroomCollision(bullet, mushroom, boxPixelsX, boxPixelsY, score);

            // Check if the centipede collides with the player and end the game if it does
            if (checkif_centiped_collides_with_player(centipede, player))
            {
                window.clear();
                Font font;
                font.loadFromFile("Fonts/GA1.ttf");

                Text loseText;
                loseText.setFont(font);
                loseText.setCharacterSize(40);
                loseText.setFillColor(Color::Black);
                loseText.setString("You Lose!\n\n");
                loseText.setPosition(200, 50);
                window.draw(loseText);

                fstream file;
                file.open("highscores.csv", ios::app);
                file << playername << "," << score << endl;
                file.close();

                system("pause");
                return 0;
            }

            // Check if the flea collides with the player and end the game if it does
            if (flea[exists] &&
                player[x] >= flea[x] && player[x] < flea[x] + boxPixelsX &&
                player[y] >= flea[y] && player[y] < flea[y] + boxPixelsY)
            {
                window.clear();
                Font font;
                font.loadFromFile("Fonts/GA1.ttf");

                Text loseText;
                loseText.setFont(font);
                loseText.setCharacterSize(40);
                loseText.setFillColor(Color::Black);
                loseText.setString("You Lose!\n\n");
                loseText.setPosition(200, 50);
                window.draw(loseText);

                fstream file;
                file.open("highscores.csv", ios::app);
                file << playername << "," << score << endl;
                file.close();

                system("pause");
                return 0;
            }

            // Check if the scorpion collides with the player and end the game if it does
            if (scorpion[exists] &&
                player[x] >= scorpion[x] && player[x] < scorpion[x] + boxPixelsX &&
                player[y] >= scorpion[y] && player[y] < scorpion[y] + boxPixelsY)
            {
                window.clear();
                Font font;
                font.loadFromFile("Fonts/GA1.ttf");

                Text loseText;
                loseText.setFont(font);
                loseText.setCharacterSize(40);
                loseText.setFillColor(Color::Black);
                loseText.setString("You Lose!\n\n");
                loseText.setPosition(200, 50);
                window.draw(loseText);

                fstream file;
                file.open("highscores.csv", ios::app);
                file << playername << "," << score << endl;
                file.close();

                system("pause");
                return 0;
            }

            // Check if the spider collides with the player and end the game if it does
            if (spider[exists] &&
                player[x] >= spider[x] && player[x] < spider[x] + boxPixelsX &&
                player[y] >= spider[y] && player[y] < spider[y] + boxPixelsY)
            {
                window.clear();
                Font font;
                font.loadFromFile("Fonts/GA1.ttf");

                Text loseText;
                loseText.setFont(font);
                loseText.setCharacterSize(40);
                loseText.setFillColor(Color::Black);
                loseText.setString("You Lose!\n\n");
                loseText.setPosition(200, 50);
                window.draw(loseText);

                fstream file;
                file.open("highscores.csv", ios::app);
                file << playername << "," << score << endl;
                file.close();

                system("pause");

                return 0;
            }
        }

        // Display name and score
        scoreText.setString("Score: " + TO_STRING(score));
        window.draw(nameText);
        window.draw(scoreText);

        window.display();
        window.clear();
    }
}

// for menu display
int main()
{

    RenderWindow window(VideoMode(resolutionX, resolutionY), "The Centipede Game");
    window.setFramerateLimit(10);
    Font font;
    font.loadFromFile("Fonts/GA1.ttf");

    Texture backgroundTexture;

    // Create a sprite for the background
    Sprite background;
    // background.setTexture(backgroundTexture);
    background.setScale(static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
                        static_cast<float>(resolutionY) / backgroundTexture.getSize().y);

    int selectedOption = 0;
    bool menuSelected = true;
    bool gameStarted = false;
    bool isEnteringName = false;
    string playername;
    window.draw(background);

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }

            // Handle name input
            if (isEnteringName && event.type == Event::TextEntered)
            {
                if (event.text.unicode < 128) // Standard ASCII characters
                {
                    if (event.text.unicode == 8 && !playername.empty()) // Backspace
                    {
                        playername.pop_back();
                    }
                    else if (event.text.unicode >= 32) // Exclude control characters
                    {
                        playername += static_cast<char>(event.text.unicode);
                    }
                }
            }
        }
        window.clear();

        // Main menu navigation
        if (menuSelected)
        {

            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                selectedOption = (selectedOption - 1 + 4) % 4;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                selectedOption = (selectedOption + 1) % 4;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                if (selectedOption == 0) // "Play" selected
                {
                    isEnteringName = true; // Enable name entering mode
                    menuSelected = false;

                    backgroundTexture.loadFromFile("Textures/menubackground.png");
                    backgroundTexture.setSmooth(true);
                    background.setTexture(backgroundTexture);
                    background.setScale(static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
                                        static_cast<float>(resolutionY) / backgroundTexture.getSize().y);
                    window.draw(background);
                }
                else if (selectedOption == 1) // "High Score" selected
                {
                    window.clear();
                    backgroundTexture.loadFromFile("Textures/menubackground.png");
                    backgroundTexture.setSmooth(true);
                    background.setTexture(backgroundTexture);
                    background.setScale(static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
                                        static_cast<float>(resolutionY) / backgroundTexture.getSize().y);
                    window.draw(background);

                    // Handle high score display
                    // Display high score
                    int scores[50];
                    char names[50][100];
                    int scoreCount = readHighScores("highscores.csv", scores, names);
                    drawHighScores(window, font, scores, names, scoreCount);

                    window.display();
                    while (!Keyboard::isKeyPressed(Keyboard::BackSpace))
                    {
                    }
                    menuSelected = true;
                }
                else if (selectedOption == 2) // "Instructions" selected
                {
                    // Handle instructions display
                    // Display instructions
                    window.clear();

                    backgroundTexture.loadFromFile("Textures/menubackground.png");
                    backgroundTexture.setSmooth(true);
                    background.setTexture(backgroundTexture);
                    background.setScale(static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
                                        static_cast<float>(resolutionY) / backgroundTexture.getSize().y);
                    window.draw(background);

                    drawInstructions(window, font);
                    window.display();
                    while (!Keyboard::isKeyPressed(Keyboard::BackSpace))
                    {
                    }
                    menuSelected = true;
                }
                else if (selectedOption == 3) // "Exit" selected
                {
                    window.close();
                }
            }
        }
        else if (isEnteringName)
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                // Start the game with the entered player name
                MainGame(window, playername);
                isEnteringName = false;
                menuSelected = true;
                playername.clear(); // Clear the name for the next round
                // window.clear();
            }
            else if (Keyboard::isKeyPressed(Keyboard::BackSpace))
            {
                // Exit name entering mode without starting the game
                isEnteringName = false;
                menuSelected = true;
            }
        }

        window.clear();

        if (menuSelected)
        {
            backgroundTexture.loadFromFile("Textures/menubackground.png");
            backgroundTexture.setSmooth(true);
            background.setTexture(backgroundTexture);
            background.setScale(static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
                                static_cast<float>(resolutionY) / backgroundTexture.getSize().y);
            window.draw(background);

            // Draw the main menu
            drawMenu(window, selectedOption);
        }
        else if (isEnteringName)
        {

            // Draw player's name input field
            sf::Text nameText("Enter Name: " + playername, font, 24);
            nameText.setFillColor(sf::Color::White);
            nameText.setPosition(200, 300);
            window.draw(nameText);
        }

        window.display();
    }

    return 0;
}

// displaying mushrooms
void drawMushroom(RenderWindow &window, float mushroom[], Sprite &mushroomSprite)
{
    if (mushroom[exists])
    {
        mushroomSprite.setPosition(mushroom[x], mushroom[y]);
        window.draw(mushroomSprite);
    }
}

// displaying player
void drawPlayer(RenderWindow &window, float player[], Sprite &playerSprite)
{
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}

// displaying bullet's movement
void moveBullet(float bullet[], Clock &bulletClock, float mushroom[][3], int &score)
{
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();
    bullet[y] -= 150;

    //  For the player to shoot the mushroom
    for (int i = 0; i < 25; i++)
    {
        if (mushroom[i][exists] &&
            bullet[exists] &&
            bullet[x] >= mushroom[i][x] && bullet[x] < mushroom[i][x] + boxPixelsX &&
            bullet[y] >= mushroom[i][y] && bullet[y] < mushroom[i][y] + boxPixelsY)
        {
            bullet[exists] = false;
            mushroom[i][exists] = false;
            score += 1;
            break;
        }
    }
    if (bullet[y] < -32)
        bullet[exists] = false;
}

// displaying bullet
void drawBullet(RenderWindow &window, float bullet[], Sprite &bulletSprite)
{
    if (bullet[exists])

    {
        bulletSprite.setPosition(bullet[x], bullet[y]);
        window.draw(bulletSprite);
    }
}