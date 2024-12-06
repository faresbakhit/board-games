#include "raylib.h"
#include "rlgl.h"
#include "TicTacToe4x4.h"
#include <algorithm>
#include <sstream>
#include <unistd.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

typedef enum {
    SCREEN_GAME_SELECTOR,
    SCREEN_GAME_4X4,
    SCREEN_GAME_ULTIMATE
} Screen;

struct ButtonState {
    bool pressed = false;
    bool mouseIn = false;
};

int screenWidth = 1280;
int screenHeight = 720;
Vector2 mousePosition = { 0.0f, 0.0f };
Screen screen = SCREEN_GAME_SELECTOR;


// Draw circle outline (Vector version)
void DrawCircleLinesEx(int centerX, int centerY, float radius, float thick, Color color, float endAngle = 360, float startAngle = 0)
{
    Vector2 center { (float)centerX, (float)centerY };
    for (float j = radius; j < radius + thick*2; j += 0.5) {
        DrawCircleSectorLines(center, j, startAngle, endAngle, 100, color);
    }
    DrawCircleV(center, radius, RAYWHITE);
}

Vector2 MeasureTextSize(const char *text, int fontSize)
{
    Vector2 textSize = { 0.0f, 0.0f };

    // Check if default font has been loaded
    if (GetFontDefault().texture.id != 0)
    {
        int defaultFontSize = 10;   // Default Font chars height in pixel
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;

        textSize = MeasureTextEx(GetFontDefault(), text, (float)fontSize, (float)spacing);
    }

    return textSize;
}

class Button {
public:
    Button(const char *text,
           Color textColor = GRAY,
           Color backgroundColor = LIGHTGRAY,
           Color backgroundColorHovered = DARKGRAY,
           int backgroundPadding = 20,
           int fontSize = 40)
        : m_text(text),
          m_textSize(MeasureTextSize(text, fontSize)),
          m_textColor(textColor),
          m_backgroundColor(backgroundColor),
          m_backgroundColorHovered(backgroundColorHovered),
          m_backgroundPadding(backgroundPadding),
          m_fontSize(fontSize) {}

    void Draw()
    {
        Color backgroundColor = m_backgroundColor;
        int x = m_x + m_backgroundPadding;
        int y = m_y + m_backgroundPadding;
        x -= (m_textSize.x+2*m_backgroundPadding)/2;
        y -= (m_textSize.y+2*m_backgroundPadding)/2;
        if (mousePosition.x > x-m_backgroundPadding
            && mousePosition.x < x+m_textSize.x+m_backgroundPadding
            && mousePosition.y > y-m_backgroundPadding
            && mousePosition.y < y+m_textSize.y+m_backgroundPadding) 
        {
            if (!m_mouseIn) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            m_mouseIn = true;
        }
        else
        {
            if (m_mouseIn) SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            m_mouseIn = false;
        }
        if (m_mouseIn)
        {
            backgroundColor = m_backgroundColorHovered;
            m_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        }
        else
        {
            m_pressed = false;
        }
        DrawRectangle(
            x-m_backgroundPadding+10, y-m_backgroundPadding+10,
            m_textSize.x+m_backgroundPadding*2, m_textSize.y+m_backgroundPadding*2,
            m_backgroundColorHovered);
        x += animationFrames;
        y += animationFrames;
        if (m_mouseIn) {
            if (animationFrames < 11) {
                animationFrames += 1;
            }
        }
        else if (animationFrames > 0) {
            animationFrames -= 1;
        }
        DrawRectangle(
            x-m_backgroundPadding, y-m_backgroundPadding,
            m_textSize.x+m_backgroundPadding*2, m_textSize.y+m_backgroundPadding*2,
            backgroundColor);
        DrawText(m_text, x, y, m_fontSize, m_textColor);
    }

    bool IsPressed()
    {
        return m_pressed;
    }

    void SetPosition(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

private:
    bool m_pressed = false;
    bool m_mouseIn = false;
    int m_x = 0;
    int m_y = 0;
    int animationFrames = 0;
    const char *m_text;
    const Color m_backgroundColor;
    const Color m_backgroundColorHovered;
    const Color m_textColor;
    const Vector2 m_textSize;
    const int m_backgroundPadding;
    const int m_fontSize;
};

class Input {
public:
    Input(const char *caption, Color captionColor) : m_caption(caption), m_captionColor(captionColor) {}

    void Draw()
    {
        Vector2 textSize = m_minTextSize;
        Vector2 maxTextSize;

        if (m_mouseIn && m_letterCount < M_MAX_INPUT_CHARS) {
            maxTextSize = MeasureTextSize(m_text, m_fontSize);
            maxTextSize.x += MeasureText("_", m_fontSize);
            maxTextSize.x += 8;
        } else {
            maxTextSize = MeasureTextSize(m_text, m_fontSize);
        }

        textSize.x = std::max(textSize.x, maxTextSize.x);
        textSize.y = std::max(textSize.y, maxTextSize.y);

        int x = m_x + m_backgroundPadding;
        int y = m_y + m_backgroundPadding;
        x -= (textSize.x+2*m_backgroundPadding)/2;
        y -= (textSize.y+2*m_backgroundPadding)/2;

        if (mousePosition.x > x-m_backgroundPadding
            && mousePosition.x < x+textSize.x+m_backgroundPadding
            && mousePosition.y > y-m_backgroundPadding
            && mousePosition.y < y+textSize.y+m_backgroundPadding) 
        {
            if (!m_mouseIn) SetMouseCursor(MOUSE_CURSOR_IBEAM);
            m_mouseIn = true;
        }
        else
        {
            if (m_mouseIn) SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            m_mouseIn = false;
        }

        if (m_mouseIn) {
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (m_letterCount < M_MAX_INPUT_CHARS))
                {
                    m_text[m_letterCount] = (char)key;
                    m_text[m_letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    m_letterCount++;
                    m_alert = false;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                m_letterCount--;
                if (m_letterCount < 0) m_letterCount = 0;
                m_text[m_letterCount] = '\0';
            }
        }
        DrawRectangle(
            x-m_backgroundPadding, y-m_backgroundPadding,
            textSize.x+m_backgroundPadding*2, textSize.y+m_backgroundPadding*2,
            RAYWHITE);
        DrawRectangleLines(
            x-m_backgroundPadding, y-m_backgroundPadding,
            textSize.x+m_backgroundPadding*2, textSize.y+m_backgroundPadding*2,
            m_alert ? RED : (m_mouseIn ? m_captionColor : GRAY));
        Vector2 captionTextSize = MeasureTextSize(m_caption, m_captionFontSize);
        DrawText(m_caption, x-m_backgroundPadding, y-m_backgroundPadding-8-captionTextSize.y, m_captionFontSize, m_captionColor);
        DrawText(m_text, x, y, m_fontSize, m_captionColor);
        if (m_mouseIn && m_letterCount < M_MAX_INPUT_CHARS) DrawText("_", x + MeasureText(m_text, m_fontSize) + 8, y, m_fontSize, m_textColor);
    }

    void SetPosition(int x, int y) {
        m_x = x;
        m_y = y;
    }

    int GetLetterCount() {
        return m_letterCount;
    }

    void ActivateAlert() {
        m_alert = true;
    }
private:
    static const int M_MAX_INPUT_CHARS = 7;
    bool m_alert = false;
    bool m_mouseIn = false;
    int m_x = 0;
    int m_y = 0;
    char m_text[M_MAX_INPUT_CHARS];
    int m_letterCount = 0;
    const Color m_captionColor;
    const char *m_caption;
    const int m_captionFontSize = 30;
    const int m_backgroundPadding = 20;
    const Color m_backgroundColor = GRAY;
    const Color m_outlineColor = DARKGRAY;
    const Color m_textColor = DARKGRAY;
    const int m_fontSize = 60;
    const Vector2 m_minTextSize = {270, 60};
};

struct TicTacToe4x4AnimationData {
    char cell;
    float angle;
};

TicTacToe4x4_Board<char>* ticTacToe4x4Board = nullptr;
TicTacToe4x4AnimationData ticTacToe4x4BoardAngles[4][4] = {};
bool gameOver = false;
int gameOverTimer = 0;
int currentPlayer = 1;

void SetScreen(Screen scr)
{
    screen = scr;
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    if (screen == SCREEN_GAME_4X4) {
        currentPlayer = 1;
        ticTacToe4x4Board = new TicTacToe4x4_Board<char>();
        gameOver = false;
        gameOverTimer = 0;
    }
}

static Input inputPlayer1Name("Player 1 Name", MAROON);
static Input inputPlayer2Name("Player 2 Name", DARKBLUE);

void DrawGameSelector()
{
    static Button buttonSelectGame4x4("4x4 TIC-TAC-TOE", WHITE, BLUE, DARKBLUE);
    static Button buttonSelectGameUltimate("ULTIMATE TIC-TAC-TOE", WHITE, RED, MAROON);
    inputPlayer1Name.SetPosition(screenWidth/2 - 250, screenHeight/2 + 200);
    inputPlayer2Name.SetPosition(screenWidth/2 + 250, screenHeight/2 + 200);
    buttonSelectGame4x4.SetPosition(screenWidth/2, screenHeight/2 - 50);
    buttonSelectGameUltimate.SetPosition(screenWidth/2, screenHeight/2 - 200);
    buttonSelectGame4x4.Draw();
    buttonSelectGameUltimate.Draw();
    inputPlayer1Name.Draw();
    inputPlayer2Name.Draw();
    if (buttonSelectGame4x4.IsPressed()) {
        bool p1ok = inputPlayer1Name.GetLetterCount() > 0, p2ok = inputPlayer2Name.GetLetterCount() > 0;
        if (!p1ok) inputPlayer1Name.ActivateAlert();
        if (!p2ok) inputPlayer2Name.ActivateAlert();
        if (p1ok && p2ok) SetScreen(SCREEN_GAME_4X4);
    }
    if (buttonSelectGameUltimate.IsPressed()) {}
}

void DrawGame4x4()
{
    const int width = 100;
    const int inset = 20;
    const char *const *const board = ticTacToe4x4Board->getboard();
    if (gameOver) {
        gameOverTimer += 1;
    }
    if (gameOverTimer > 500) {
        SetScreen(SCREEN_GAME_SELECTOR);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bool selected = false;
            int x = j*width+screenWidth/2-width*2;
            int y = i*width+screenHeight/2-width*2;
            if (mousePosition.x > x
                && mousePosition.x < x+width
                && mousePosition.y > y
                && mousePosition.y < y+width) {
                selected = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            }
            DrawRectangle(x, y, width, width, RAYWHITE);
            DrawRectangleLines(x, y, width, width, GRAY);
            float p1 = 0.0f, p2 = 0.0f;
            switch (board[i][j]) {
                case 'X':
                    ticTacToe4x4BoardAngles[i][j].cell = 'X';
                    if (ticTacToe4x4BoardAngles[i][j].angle < 360.0f) ticTacToe4x4BoardAngles[i][j].angle += 3.0f*3.0f - ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f))
                                                                                                       * ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f));
                    else ticTacToe4x4BoardAngles[i][j].angle = 360.0f;
                    if (ticTacToe4x4BoardAngles[i][j].angle < 180.0f) {
                        p1= ticTacToe4x4BoardAngles[i][j].angle/180;
                        p2 = 0.0f;
                    }
                    else if (ticTacToe4x4BoardAngles[i][j].angle < 360.0f) {
                        p1=1.0f;
                        p2= (ticTacToe4x4BoardAngles[i][j].angle-180)/180;
                    } else {
                        p1 = 1.0f;
                        p2 = 1.0f;
                    }
                    DrawLineEx({(float)x+inset,(float)y+inset},{(float)x+inset+(width-2*inset)*p1,(float)y+inset+(width-2*inset)*p1},15.0f,RED);
                    if (p1 >= 1.0f) DrawLineEx({(float)x+(width-inset),(float)y+(inset)},{(float)x+(width-inset)+(2*inset-width)*p2,(float)y+inset+(width-2*inset)*p2},15.0f,RED);
                    break;
                case 'O':
                    ticTacToe4x4BoardAngles[i][j].cell = 'O';
                    if (ticTacToe4x4BoardAngles[i][j].angle < 360.0f) ticTacToe4x4BoardAngles[i][j].angle += 3.0f*3.0f - ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f))
                                                                                                       * ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f));
                    else ticTacToe4x4BoardAngles[i][j].angle = 360.0f;
                    DrawCircleLinesEx(x+width/2, y+width/2, 25.0f, 5.0f, BLUE, ticTacToe4x4BoardAngles[i][j].angle);
                    break;
                default:
                    switch (ticTacToe4x4BoardAngles[i][j].cell) {
                        case 'O':
                            if (ticTacToe4x4BoardAngles[i][j].angle > 0) {
                                ticTacToe4x4BoardAngles[i][j].angle -= 3.0f*3.0f - ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f))
                                                                               * ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f));
                                DrawCircleLinesEx(x+width/2, y+width/2, 25.0f, 5.0f, BLUE, ticTacToe4x4BoardAngles[i][j].angle);
                            }
                            else {
                                ticTacToe4x4BoardAngles[i][j].angle = 0.0f;
                                ticTacToe4x4BoardAngles[i][j].cell = 0;
                            }
                            break;
                        case 'X':
                            if (ticTacToe4x4BoardAngles[i][j].angle > 0.0f) ticTacToe4x4BoardAngles[i][j].angle -= 3.0f*3.0f - ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f))
                                                                                                               * ((ticTacToe4x4BoardAngles[i][j].angle-180.0f)*(3.0f/181.0f));
                            else {
                                ticTacToe4x4BoardAngles[i][j].angle = 0.0f;
                                ticTacToe4x4BoardAngles[i][j].cell = 0;
                                break;
                            }
                            if (ticTacToe4x4BoardAngles[i][j].angle < 180.0f) {
                                p1= ticTacToe4x4BoardAngles[i][j].angle/180;
                                p2 = 0.0f;
                            }
                            else if (ticTacToe4x4BoardAngles[i][j].angle < 360.0f) {
                                p1=1.0f;
                                p2= (ticTacToe4x4BoardAngles[i][j].angle-180)/180;
                            } else {
                                p1 = 1.0f;
                                p2 = 1.0f;
                            }
                            DrawLineEx({(float)x+(width-inset),(float)y+(inset)},{(float)x+(width-inset)+(2*inset-width)*(p2),(float)y+inset+(width-2*inset)*(p2)},15.0f,RED);
                            if (p1) DrawLineEx({(float)x+inset,(float)y+inset},{(float)x+inset+(width-2*inset)*(p1),(float)y+inset+(width-2*inset)*(p1)},15.0f,RED);
                            break;
                    }
            }
            if (!gameOver && selected) {
                char sym = currentPlayer == 1 ? 'X': 'O';
                if (ticTacToe4x4Board->update_board(i, j, sym)) currentPlayer = currentPlayer == 1 ? 2 : 1;
            }
        }
    }
    std::ostringstream oss;
    if (ticTacToe4x4Board->game_is_over()) {
        gameOver =true;
        if (ticTacToe4x4Board->is_draw()) {
            oss << "Draw!";
        } if (ticTacToe4x4Board->is_win()) {
            oss << "Player " << (currentPlayer == 1 ? 2 : 1) << " WINS!";
        }
    } else {
        oss << "Player " << currentPlayer << " Turn";
    }
    std::string text = oss.str();
    DrawText(text.c_str(), screenWidth/2-MeasureText(text.c_str(), 80)/2, 40, 80, (gameOver * currentPlayer) ? RED : BLUE);
}

void UpdateDrawFrame()
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    mousePosition = GetMousePosition();
    
    BeginDrawing();

    ClearBackground(WHITE);
    DrawText("faresbakhit/board-games\nFCAI-CU", 10, 10, 10, LIGHTGRAY);

    if (screen == SCREEN_GAME_SELECTOR) {
        DrawGameSelector();
    } else if (screen == SCREEN_GAME_4X4) {
        DrawGame4x4();
    } else if (screen == SCREEN_GAME_ULTIMATE) {
    
    }

    EndDrawing();
}

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "faresbakhit/board-games");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();

    return 0;
}
