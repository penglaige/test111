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
        double uct_value;
        double num_visit;
        Cellposition position;
        CellState state;
    }
    static Selection_node selection_node[60];
    
    struct Expansion_node
    {
        int nun_win;
        int num_visit;
        Cellposition position;
        CellState state;
    }
    static Expansion_node expansion_node[60];
    
    class MCTS : public Player
    {
    public:
        void set_selection_node(int node);
        void set_expansion_node(int node);
        
        void Selection(const Board& board);
        double Expansion();
        int Simulation();
        bool Outcome() const;
        int Choose_best_move(const Board& board) override;
        
    }
}  // namespace reversi
