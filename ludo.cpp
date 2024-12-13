#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include "player.h"
using namespace std;

enum PlayerColor {
    r = 0,
    g = 1,
    y = 2,
    b = 3
};

vector<POINT> prevpos = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
//player creation
//----red-----
vector<POINT> redd={{125,375}};
vector<POINT> redTokens = {{125, 125}, {175, 125}, {125, 175}, {175, 175}};
Player red(r, redTokens,prevpos); 

//----green-----
vector<POINT> greenn={{475,125}};
vector<POINT> greenTokens = {{625, 125}, {675, 125}, {625, 175}, {675, 175}};
Player green(g, greenTokens,prevpos); 

//----yellow-----
vector<POINT> yelloww={{725,475}};
vector<POINT> yellowTokens = {{725, 725}, {775, 725}, {725, 775}, {775, 775}};
Player yellow(y, yellowTokens,prevpos); 

//----blue-----
vector<POINT> bluee={{375,725}};
vector<POINT> blueTokens = {{125, 725}, {175, 725}, {125, 775}, {175, 775}};
Player blue(b, blueTokens,prevpos);


vector<POINT> grayy={{375,225},{675,375},{475,675},{175,475}};
vector<Player> allPlayers={red,green,yellow,blue};

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawBoard(HDC hdc);
void DrawTokens(HDC hdc);

int diceValue = 0;
HWND diceButton, pieceButtons[4];

int currentPlayer = 0; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char CLASS_NAME[] = "LudoGame";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Ludo Game", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:

        srand(static_cast<unsigned int>(time(nullptr))); 
        diceButton = CreateWindow("BUTTON", "Roll Dice", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                              850, 100, 100, 50, hwnd, (HMENU)1, nullptr, nullptr);

        for (int i = 0; i < 4; ++i) {
            pieceButtons[i] = CreateWindow(
                "BUTTON", std::to_string(i + 1).c_str(), WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
                850, 250 + i * 60, 50, 50, hwnd, reinterpret_cast<HMENU>(static_cast<intptr_t>(10 + i)), nullptr, nullptr
            );
            ShowWindow(pieceButtons[i], SW_HIDE);
        }

        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { 
            diceValue = rand() % 6 + 1; 

            bool allTokensAtStart = true;
            for (int tokenPos : allPlayers[currentPlayer].pos) {
                if (tokenPos != -1) {
                    allTokensAtStart = false;
                    break;
                }
            }

            if (allTokensAtStart && diceValue != 6) {
                currentPlayer = (currentPlayer + 1) % 4; 
            } else {
                for (int i = 0; i < 4; ++i) {
                    ShowWindow(pieceButtons[i], SW_SHOW);
                }
            }

            InvalidateRect(hwnd, nullptr, TRUE); 
        } 
        if (LOWORD(wParam) >= 10 && LOWORD(wParam) < 14) { 
            int pieceIndex = LOWORD(wParam) - 10;

            allPlayers[currentPlayer].calculatePosition(pieceIndex, diceValue,allPlayers);

            for (int i = 0; i < 4; ++i) {
                ShowWindow(pieceButtons[i], SW_HIDE);
            }

            if(diceValue!=6) currentPlayer = (currentPlayer + 1) % 4;

            InvalidateRect(hwnd, nullptr, TRUE); 
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT textRect = {850, 50, 1050, 150};
        FillRect(hdc, &textRect, (HBRUSH)(COLOR_WINDOW + 1));

        DrawBoard(hdc);
        DrawTokens(hdc);

        char buffer[64];
        sprintf(buffer, "Player %d's Turn - Dice: %d", currentPlayer + 1, diceValue);
        TextOut(hdc, 850, 50, buffer, strlen(buffer));
        
        sprintf(buffer, "Move which piece?");
        TextOut(hdc, 850, 200, buffer, strlen(buffer));

        EndPaint(hwnd, &ps);
    }
    break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void DrawBoard(HDC hdc) {
    int cellSize = 50;

    HBRUSH red_brush = CreateSolidBrush(RGB(255, 0, 0)); // Red for Player 1
    HBRUSH blue_brush = CreateSolidBrush(RGB(0, 0, 255)); // blue for Player 1
    HBRUSH green_brush = CreateSolidBrush(RGB(0, 255, 0)); // gren for Player 1
    HBRUSH yellow_brush = CreateSolidBrush(RGB(255, 255, 0)); // yellow for Player 1
    HBRUSH gray_brush = CreateSolidBrush(RGB(128, 128, 128)); // yellow for Player 1
    HBRUSH black_brush = CreateSolidBrush(RGB(20, 20, 20)); // yellow for Player 1

    RECT red_rect1 = { 50, 50, 350, 350 };
    RECT red_rect3 = { 100, 350, 150, 400 };
    RECT red_rect2 = { 100, 400, 350, 450 };

    RECT yellow_rect1 = {500,500,800,800};
    RECT yellow_rect2 = {700,450,750,500};
    RECT yellow_rect3 = {500,400,750,450};

    RECT green_rect1 = {500,50,800,350};
    RECT green_rect2 = {400,100,450,350};
    RECT green_rect3 = {450,100,500,150};

    RECT blue_rect1 = {50,500,350,800};
    RECT blue_rect2 = {400,500,450,750};
    RECT blue_rect3 = {350,700,400,750};

    RECT neutral1={350,150,400,200};
    RECT neutral2={650,350,700,400};
    RECT neutral3={450,650,500,700};
    RECT neutral4={150,450,200,500};

    RECT center={350,350,500,500};

    FillRect(hdc, &center, black_brush);

    FillRect(hdc, &neutral1, gray_brush);
    FillRect(hdc, &neutral2, gray_brush);
    FillRect(hdc, &neutral3, gray_brush);
    FillRect(hdc, &neutral4, gray_brush);

    FillRect(hdc, &red_rect1, red_brush);
    FillRect(hdc, &red_rect2, red_brush);
    FillRect(hdc, &red_rect3, red_brush);

    FillRect(hdc, &blue_rect1, blue_brush);
    FillRect(hdc, &blue_rect2, blue_brush);
    FillRect(hdc, &blue_rect3, blue_brush);

    FillRect(hdc, &green_rect1, green_brush);
    FillRect(hdc, &green_rect2, green_brush);
    FillRect(hdc, &green_rect3, green_brush);

    FillRect(hdc, &yellow_rect1, yellow_brush);
    FillRect(hdc, &yellow_rect2, yellow_brush);
    FillRect(hdc, &yellow_rect3, yellow_brush);

    for (int i = 0; i <= 15; ++i) {
        MoveToEx(hdc, 50, 50 + i * cellSize, nullptr);
        LineTo(hdc, 800, 50 + i * cellSize);

        MoveToEx(hdc, 50 + i * cellSize, 50, nullptr);
        LineTo(hdc, 50 + i * cellSize, 800);
    }

    DeleteObject(red_brush);
    DeleteObject(blue_brush);
    DeleteObject(green_brush);
    DeleteObject(yellow_brush);
}

void DrawTokens(HDC hdc) {
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));
    HFONT font = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, 
                            CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    HFONT oldFont = (HFONT)SelectObject(hdc, font);

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255)); 

    for (int i = 0; i < 4; ++i) { 
        HBRUSH brush;
        HBRUSH rectBrush=whiteBrush; 
        switch (i) {
            case 0: brush = redBrush; break;
            case 1: brush = greenBrush; break;
            case 2: brush = yellowBrush; break;
            case 3: brush = blueBrush; break;
        }

        for (int j = 0; j < 4; ++j) { 
            int prevX = allPlayers[i].previousTokenPosition[j].x;
            int prevY = allPlayers[i].previousTokenPosition[j].y;
            POINT prevv=allPlayers[i].previousTokenPosition[j];
            bool found=false;
            if(!found){
                for(auto i:redd){
                    if(i.x==prevv.x && i.y==prevv.y){
                        rectBrush=redBrush;
                        // cout<<"red";
                        found=true;
                    }
                }
            }
            if(!found){
                for(auto i:bluee){
                    if(i.x==prevv.x && i.y==prevv.y){
                        rectBrush=blueBrush;
                        // cout<<"b";
                        found=true;
                    }
                }
            }
            if(!found){
                for(auto i:yelloww){
                    if(i.x==prevv.x && i.y==prevv.y){
                        rectBrush=yellowBrush;
                        // cout<<"y";
                        found=true;
                    }
                }
            }
            if(!found){
                for(auto i:greenn){
                    if(i.x==prevv.x && i.y==prevv.y){
                        rectBrush=greenBrush;
                        // cout<<"g";
                        found=true;
                    }
                }
            }
            if(!found){
                for(auto i:grayy){
                    if(i.x==prevv.x && i.y==prevv.y){
                        rectBrush=grayBrush;
                        // cout<<"gra";
                        found=true;
                    }
                }
            }
            
            RECT clearRect = {prevX - 24, prevY - 24, prevX + 24, prevY + 24};
            FillRect(hdc, &clearRect, rectBrush);

            SelectObject(hdc, brush);
            int x = allPlayers[i].tokenPosition[j].x;
            int y = allPlayers[i].tokenPosition[j].y;
            Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);

            char label[2];
            sprintf(label, "%d", j + 1);
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, x - 7, y - 10, label, strlen(label));
        }
    }

    SelectObject(hdc, oldFont);
    DeleteObject(font);
    DeleteObject(redBrush);
    DeleteObject(blueBrush);
    DeleteObject(greenBrush);
    DeleteObject(yellowBrush);
    DeleteObject(whiteBrush);
}

