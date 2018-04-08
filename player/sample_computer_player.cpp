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
    int best_move = before(board,turn);
    return board.getAllLegalMoves(turn).at(best_move);
    
}

    void Choose_best_move(const Board& board,Side turn,int sele_node)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        int node_num = legal_moves.size();
        be[sele_node].num_child=node_num;
        
        if(legal_moves.empty()!=1)
        {
            selection(board,turn,be[sele_node].all_visit,sele_node);
        }
        
    }
    
    int before(const Board& board,Side turn)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        int node_num = legal_moves.size();
        
        time_t start,end;
        start = time(NULL);
        double difftimes = 0.0;
        while(difftimes<=1.8)
        {
            for(int i=0;i<node_num;i++)
            {
                Board board_play;
                copy(board,board_play);
                board_play.placeDisk(legal_moves.at(i),turn);
                turn = getOpponentSide(turn);
                Choose_best_move(board_play,turn,i);
                be[i].all_visit+=1;
                turn = getOpponentSide(turn);
            }
            end =time(NULL);
            difftimes=end-start;
        }
        
        return choose(node_num);
        //
    }

    void selection(const Board& board,Side turn, double all_visit,int sele_node)
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
            be[sele_node].se[node].uct_value = be[sele_node].se[node].win_possibility - 0.2*sqrt(log(all_visit)/be[sele_node].se[node].num_visit);
        }
        double min=1;
        for(int j=0;j<node_num;j++){
            if(be[sele_node].se[j].uct_value<min){
                min=be[sele_node].se[j].uct_value;
                selection_move = j;
            }
        }
        board_play.placeDisk(legal_moves.at(selection_move),turn);
        turn = getOpponentSide(turn);
        be[sele_node].se[selection_move].num_visit+=1;
        be[sele_node].se[selection_move].win_possibility = expansion(board_play,turn,selection_move,sele_node);
        
    }
    
    double expansion(Board& board,Side turn,int selection_move,int sele_node)
    {
        std::vector<CellPosition> legal_moves;
        legal_moves = board.getAllLegalMoves(turn);
        const int node_num = legal_moves.size();
        Board board_ep;
        be[sele_node].se[selection_move].num_child = node_num;
        
        if(legal_moves.empty()==1){
            return 0;
        }
        for(int i=0;i<node_num;i++)
        {
            copy(board,board_ep);
            board_ep.placeDisk(legal_moves.at(i),turn);
            turn = getOpponentSide(turn);
            be[sele_node].se[selection_move].num_visit+=1;
            be[sele_node].se[selection_move].ep[i].visit+=1;
            int outcome = Simulation(board_ep,turn,0,0);
            Side BLACK = Side::BLACK;
            Side WHITE = Side::WHITE;
            if((turn==BLACK && outcome == -1) || (turn==WHITE && outcome == 1))
            {
                be[sele_node].se[selection_move].num_win+=1;
                be[sele_node].se[selection_move].ep[i].win+=1;
            }
            turn = getOpponentSide(turn);
            be[sele_node].se[selection_move].ep[i].possibility=be[sele_node].se[selection_move].ep[i].win/be[sele_node].se[selection_move].ep[i].visit;
        }
        return be[sele_node].se[selection_move].num_win/be[sele_node].se[selection_move].num_visit;
        
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
            //std::cout<<"simulation random_move-node_num:"<<random_move<<"-"<<node_num<<std::endl;
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
        for(int i=0;i<node_num;i++)
        {
            double min=1;
            for(int j=0;j<be[i].num_child;j++)
            {
                std::cout<<i<<"-"<<j<<"-possibility:"<<be[i].se[j].win_possibility<<"\n"<<std::endl;
                if(be[i].se[j].win_possibility<min)
                {
                    min=be[i].se[j].win_possibility;
                    be[i].bad_move=j;
                }
                std::cout<<i<<"-bad_move:"<<be[i].bad_move<<std::endl;
            }
        }
        double max=0;
        int best_move=0;
        for(int k=0;k<node_num;k++)
        {
            int bad_move=be[k].bad_move;
            if(be[k].se[bad_move].num_child!=0)
            {
                for(int m=0;m<be[k].se[bad_move].num_child;m++)
                {
                    std::cout<<k<<"-"<<bad_move<<"-"<<m<<"-possibility:"<<be[k].se[bad_move].ep[m].possibility<<"\n"<<std::endl;
                    if(be[k].se[bad_move].ep[m].possibility>max)
                    {
                        max=be[k].se[bad_move].ep[m].possibility;
                        best_move=k;
                    }
                }
            }
            
        }

        std::cout<<"best move:"<<best_move<<std::endl;
        return best_move;
    }

    
}  // namespace reversi
