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

    struct Expansion_node
    {
        int nun_win=0;
        int num_visit=0;
    };
    
    struct Selection_node
    {
        double uct_value=0;
        double possibility=0;
        double num_visit=0.00001;
        double num_win=0;
        int num_ep_visit=0;
    };
    static Selection_node selection_node[60];
    

    
    void selection(const Board& board,Side turn, double all_visit);
    double expansion(Board& board,Side turn,int selection_move);
    int Simulation(Board& board,Side turn, int random_control,int pass_count );
    int Outcome(Board& board);//
    int Choose_best_move(const Board& board,Side turn);
    int generate_random(int i);//
    void copy(const Board& board,Board& board_play);
    void undo_copy(Board& board_play,Board& board_store);
        
    
}  // namespace reversi
