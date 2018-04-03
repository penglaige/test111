#include <vector>
#include <time.h>

#include "sample_computer_player.hpp"

namespace reversi
{

CellPosition SampleComputerPlayer::thinkNextMove(const Board& board)
{
    //int node_num = board.getAllLegalMoves(getSide()).size();
    int best_move = MCTS::Choose_best_move(const Board& board);
    return board.getAllLegalMoves(getSide()).at(best_move);
    
}

int MCTS::Choose_best_move(const Board& board)
    {
        legal_moves = board.getAllLegalMoves(getSide());
        int node_num = legal_moves.size();
        int best_move;
        
        time_t start,end;
        start = time(NULL);
        double difftimes = 0.0;
        
        //MCTS::original();
        
        while(difftimes<=3.0){
            void Selection(const Board& board);
            end = time(NULL);
            difftimes = difftime(end,start);
        }
        
        double best_uct_value = -1000;
        for(i=0,i<node_num,i++)
        {
            if(selection_node[i].uct_value>=best_uct_value)
            {
                best_uct_value=selection_node[i].uct_value;
                best_move = i;
            }
        }
        return best_move;
    }
    
void Selection(const Board& board)
    {
        
    }
    
}  // namespace reversi
