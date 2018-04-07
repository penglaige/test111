#include <vector>
#include <time.h>
#include <math.h>
#include <iostream>

#include "sample_computer_player.hpp"

namespace reversi
{

CellPosition SampleComputerPlayer::thinkNextMove(const Board& board)
{
    Side turn = getSide();
    int best_move = Choose_best_move(board,turn);
    return board.getAllLegalMoves(turn).at(best_move);
    
}

int Choose_best_move(const Board& board,Side turn)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        int node_num = legal_moves.size();
        int best_move=0;
        double all_visit=0.0001;
        
        time_t start,end;
        start = time(NULL);
        double difftimes = 0.0;
        
        while(difftimes<=1.8){
            selection(board,turn,all_visit);
            all_visit++;
            end = time(NULL);
            difftimes = difftime(end,start);
        }
        
        best_move=choose(node_num);
        std::cout<<"best move: "<<best_move<<std::endl;
        return best_move;
    }

    void selection(const Board& board,Side turn, double all_visit)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        const int node_num = legal_moves.size();
        
        Board board_play;
        Board board_store;
        copy(board,board_play);
        copy(board,board_store);
        
        int selection_move=0;
        for(int node = 0;node<node_num;node++)
        {
            selection_node[node].uct_value = selection_node[node].possibility + 0.2*sqrt(log(all_visit)/selection_node[node].num_visit);
        }
        double max=0;
        for(int j=0;j<node_num;j++){
            if(selection_node[j].uct_value>max){
                max=selection_node[j].uct_value;
                selection_move = j;
            }
        }
        board_play.placeDisk(legal_moves.at(selection_move),turn);
        turn = getOpponentSide(turn);
        selection_node[selection_move].num_visit+=1;
        selection_node[selection_move].possibility = expansion(board_play,turn,selection_move);
        
    }
    
    double expansion(Board& board,Side turn,int selection_move)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        const int node_num = legal_moves.size();
        Board board_ep;
        selection_node[selection_move].num_child_node=node_num;
        //std::cout<<"frst ep:"<<"turn:"<<turn<<std::endl;
        
        if(node_num==0){
            return 0;
        }
        for(int i=0;i<node_num;i++)
        {
            copy(board,board_ep);
            board_ep.placeDisk(legal_moves.at(i),turn);
            turn = getOpponentSide(turn);
            selection_node[selection_move].num_ep_visit+=1;
            selection_node[selection_move].ep_node[i].num_ep_visit+=1;
            int outcome = Simulation(board_ep,turn,0,0);
            Side BLACK = Side::BLACK;
            Side WHITE = Side::WHITE;
            if((turn==BLACK && outcome == 1) || (turn==WHITE && outcome == -1))
            {
                selection_node[selection_move].num_win+=1;
                selection_node[selection_move].ep_node[i].num_win+=1;
            }
            turn = getOpponentSide(turn);
            selection_node[selection_move].ep_node[i].ep_win_possibility=selection_node[selection_move].ep_node[i].num_win/selection_node[selection_move].ep_node[i].num_ep_visit;
        }
        return selection_node[selection_move].num_win/selection_node[selection_move].num_ep_visit;
        
    }
    
    int Simulation(Board& board,Side turn,int random_control,int pass_count)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        const int node_num = legal_moves.size();
        int outcome;
        int random;
        int random_move;
        
        if(node_num==0)
        {
            pass_count++;
            if(pass_count==2){
                outcome = Outcome(board);
                return outcome;
            }
            turn = getOpponentSide(turn);
            return Simulation(board,turn,random_control,pass_count);
        }
        if(node_num >0)
        {
            pass_count = 0;
            random = generate_random(random_control);
            random_control+=random;
            random_move = random % node_num;
            board.placeDisk(legal_moves.at(random_move),turn);
            turn = getOpponentSide(turn);
            return Simulation(board,turn,random_control,pass_count);
        }
        
    }
    
    int Outcome(Board& board)
    {
        int black,white;
        black = board.count(CellState::BLACK);
        white = board.count(CellState::WHITE);
        if(black>white){
            return 1;
        }
        else if(black == white){
            return 0;
        }
        else{
            return -1;
        }
    }
    
    int generate_random(int i)
    {
        int random_number;
        
        srand((unsigned)time(NULL)+(unsigned)i);
        random_number=rand();
        return random_number;
    }
    
    void copy(const Board& board,Board& board_play)
    {
        for(int y = 0;y<board.HEIGHT;y++){
            for(int x=0;x<board.WIDTH;x++){
                CellState state = board.get({x,y});
                board_play.set({x,y},state);
            }
        }
    }
    
    void undo_copy(Board& board_play,Board& board_store)
    {
        for(int y = 0;y<board_play.HEIGHT;y++){
            for(int x = 0;x<board_play.WIDTH;x++){
                CellState state = board_store.get({x,y});
                board_play.set({x,y},state);
            }
        }
    }
    
    void print_score(Board& board)
    {
        int black = board.count(CellState::BLACK);
        int white = board.count(CellState::WHITE);
        
        std::cout<<"black : white = "<<black<<":"<<white<<std::endl;
    }
    
    int choose(int node_num)
    {
        double max=0;
        int best_move=0;
        for(int i=0;i<node_num;i++){
            double bad_value=1;
            for(int j=0;j<selection_node[i].num_child_node;j++)
            {
                if(selection_node[i].ep_node[j].ep_win_possibility<bad_value)
                {
                    bad_value=selection_node[i].ep_node[j].ep_win_possibility;
                    
                }
            }
            selection_node[i].bad_value=bad_value;
        }
        
        for(int k=0;k<node_num;k++){
            if(selection_node[k].bad_value>max){
                max=selection_node[k].bad_value;
                best_move = k;
            }
        }
        return best_move;
    }

    
}  // namespace reversi
