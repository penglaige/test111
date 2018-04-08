#pragma once

#include "board.hpp"
#include "player.hpp"

namespace reversi
{

class SampleComputerPlayer : public Player
{
public:
	SampleComputerPlayer(Side side) : Player(side) {}

	virtual CellPosition thinkNextMove(const Board& board) override;
};
    
    struct ep
    {
        double win=0;
        int visit=0;
        double possibility=0;
    };
    
    struct se
    {
        double uct_value=0;
        int num_win=0;
        double num_visit = 0.0001;
        double win_possibility=0;
        double worst=1;
        double best=0;
        ep ep[60];
        int num_child;
    };
    
    struct be
    {
        int bad_move=0;
        int num_child;
        double all_visit=0.00001;
        se se[60];
    };
    
    static be be[60];
    

    int before(const Board& board,Side turn);
    void selection(const Board& board,Side turn, double all_visit,int sele_node);
    double expansion(Board& board,Side turn,int selection_move,int sele_node);
    int Simulation(Board& board,Side turn, int random_control,int pass_count );
    int Outcome(Board& board);//
    void Choose_best_move(const Board& board,Side turn,int sele_node);
    int generate_random(int i);//
    void copy(const Board& board,Board& board_play);
    void undo_copy(Board& board_play,Board& board_store);
    void print_score(Board& board);
    int choose(int node_num);
        
    
}  // namespace reversi
