#include<bits/stdc++.h>
#include <windows.h>
using namespace std;

vector<POINT>path={{125, 375}, {175, 375}, {225, 375}, {275, 375}, {325, 375}, 
                  {375, 325}, {375, 275}, {375, 225}, {375, 175}, {375, 125}, {375, 75}, {425, 75}, 
                  {475, 75}, {475, 125}, {475, 175}, {475, 225}, {475, 275}, {475, 325}, 
                  {525, 375}, {575, 375}, {625, 375}, {675, 375}, {725, 375}, {775, 375}, 
                  {775, 425}, {775, 475}, {725, 475}, {675, 475}, {625, 475}, {575, 475}, 
                  {525, 475}, {475, 525}, {475, 575}, {475, 625}, {475, 675}, {475, 725}, 
                  {475, 775}, {425, 775}, {375, 775}, {375, 725}, {375, 675}, {375, 625}, 
                  {375, 575}, {375, 525}, {325, 475}, {275, 475}, {225, 475}, {175, 475}, 
                  {125, 475}, {75, 475}, {75, 425}, {75, 375}};

class Player{
    public:
    int player;//0->red 1->green 2=>yellow 3->blue
    vector<POINT> tokenPosition;
    vector<int> pos={-1,-1,-1,-1};
    vector<POINT> previousTokenPosition; // New member to store the previous positions

    // Constructor
    Player(int color, vector<POINT> tokens,vector<POINT> prev)
        : player(color),tokenPosition(tokens), previousTokenPosition(prev) {}
    // Player(int player,vector<POINT> tp){
    //   this->player=player;
    //   this -> tokenPosition=tp;
    // }
    void calculatePosition(int pieceID, int diceValue) {
        if (pieceID < 0 || pieceID > 3) {
            cout<<pieceID;
            MessageBox(nullptr, "Invalid Piece ID!", "Error", MB_OK | MB_ICONERROR);
            return;
        }
        previousTokenPosition=tokenPosition;
        // Convert pieceID (1-based) to index (0-based)
        int index = pieceID;

        // Get the current position of the token
        POINT& currentPos = tokenPosition[index];
        if(pos[index]==-1){
          if(diceValue==6){
            switch(player){
              case 0:
                pos[index]=0;
                currentPos=path[0];
                break;
              case 1:
                pos[index]=13;
                currentPos=path[13];
                break;
              case 2:
                pos[index]=26;
                currentPos=path[26];
                break;
              case 3:
                pos[index]=39;
                currentPos=path[39];
                break;
            }   
            return;
          }
          else{
            cout<<"you can unlock piece only when you have 6!!\nchoose another piece which is unlocked!!";
            return;
          }
        }
        else if(pos[index]+diceValue>56){
          cout<<"Cant move that piece, choose another piece"<<pos[index]<<" "<<index<<"dice:"<<diceValue<<endl;
          return;
        }
        else if(pos[index]+diceValue==56){
          pos[index]+=diceValue;
          currentPos.x=550;
          currentPos.y=550;
          return;
        }
        else{
          pos[index]+=diceValue;
          currentPos=path[pos[index]%52];
          return ;
        }
    }
};