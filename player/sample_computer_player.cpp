#include <vector>
#include <time.h>

#include "sample_computer_player.hpp"

namespace reversi
{

CellPosition SampleComputerPlayer::thinkNextMove(const Board& board)
{
    Side turn = getSide();
    int best_move = MCTS::Choose_best_move(const Board& board,turn);
    return board.getAllLegalMoves(turn).at(best_move);
    
}

int MCTS::Choose_best_move(const Board& board,Side turn)
    {
        legal_moves = board.getAllLegalMoves(turn);
        int node_num = legal_moves.size();
        int best_move;
        
        time_t start,end;
        start = time(NULL);
        double difftimes = 0.0;
        
        while(difftimes<=3.0){
            void MCTS::Selection(const Board& board,turn);
            end = time(NULL);
            difftimes = difftime(end,start);
        }
        
        double best_uct_value = -1000;
        for(i=0;i<node_num;i++)
        {
            if(selection_node[i].uct_value>=best_uct_value)
            {
                best_uct_value=selection_node[i].uct_value;
                best_move = i;
            }
        }
        return best_move;
    }

    void MCTS::Selection(const Board& board,Side turn)
    {
        legal_moves = board.getAllLegalMoves(turn);
        const node_num = legal_moves.size();
        board.docopy();
        
        int random = generate_random(0);
        random_move = random % node_num;
        
        board.placeDisk(legal_moves.at[random_move],turn)
        Side turn = getOpponentSide(turn)
        selection_node[random_move].num_visit+=1;
        selection_node[random_move].position = legal_moves.at[random_move];
        int outcome = MCTS::Simulation(const Board& board,turn);
        if((getSide()==BLACK && outcome == 1) && (getSide()==WHITE && outcome == -1))
        {
           selection_node[random_move].uct_value+=1;
        }
        board.undocopy();
        
    }
    
    int MCTS::Simulation(const Board& board,Side turn,int random_control,int pass_count)
    {
        legal_moves = board.getAllLegalMoves(turn);
        const node_num = legal_moves.size();
        int outcome;
        int random;
        int random_move;
        
        if(node_num==0)
        {
            pass_count++;
            if(pass_count==2){
                outcome = MCTS::Outcome();
                return outcome;
            }
        }
        if(node_num >0)
        {
            pass_count = 0;
            random = generate_random(random_control);
            random_control+=random;
            random_move = random % node_num;
            board.placeDisk(legal_moves.at[random_move],turn);
            turn = getOpponentSide(turn);
            return MCTS::Simulation(const Board& board,turn);
        }
        
    }
    
    int MCTS::Outcome() const
    {
        int black,white;
        black = board.count(BLACK);
        white = board.count(WHITE);
        if(black>white){
            return 1;
        }
        else if(black = white){
            return 0;
        }
        else{
            retrun -1;
        }
    }
    
    int MCTS::generate_random(int i)
    {
        int random_number;
        
        srand((unsigned)time(NULL)+(unsigned)i);
        random_number=rand();
        return random_number;
    }
    
}  // namespace reversi
