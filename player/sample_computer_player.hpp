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

    struct Selection_node
    {
        double uct_value=0;
        double num_visit=0.00001;
        Cellposition position;
        CellState state;
    };
    static Selection_node selection_node[60];
    
    struct Expansion_node
    {
        int nun_win=0;
        int num_visit=0;
        Cellposition position;
        CellState state;
    };
    static Expansion_node expansion_node[60];
    
    class MCTS : public Player
    {
    public:
        
        void Selection(const Board& board,Side turn);
        int Simulation(const Board& board,Side turn, int random_control,int pass_count );
        int Outcome() const;//
        int Choose_best_move(const Board& board,Side turn);
        int generate_random(int i);//
        
        
    };
}  // namespace reversi
