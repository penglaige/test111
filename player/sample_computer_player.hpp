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

    struct expansion_node
    {
        double num_win=0;
        int num_ep_visit=0;
        double ep_win_possibility=0;
    };
    
    struct Selection_node
    {
        double uct_value=0;
        double possibility=0;
        double num_visit=0.0001;
        double num_win=0;
        int num_ep_visit=0;
        expansion_node ep_node[60];
        int num_child_node;
        double bad_value;
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
    void print_score(Board& board);
    int choose(int node_num);
        
    
}  // namespace reversi
