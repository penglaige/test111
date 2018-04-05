#include <vector>
#include <time.h>
#include <math.h>

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
        int best_move;
        double all_visit=0;
        
        time_t start,end;
        start = time(NULL);
        double difftimes = 0.0;
        
        while(difftimes<=2.5){
            selection(board,turn,all_visit);
            all_visit++;
            end = time(NULL);
            difftimes = difftime(end,start);
        }
        
        double best_uct_value = -1000;
        for(int i=0;i<node_num;i++)
        {
            if(selection_node[i].uct_value>=best_uct_value)
            {
                best_uct_value=selection_node[i].uct_value;
                best_move = i;
            }
        }
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
        
        int max=0;
        int selection_move=0;
        for(int node = 0;node<node_num;node++)
        {
            selection_node[node].uct_value = selection_node[node].possibility + sqrt(log(all_visit)/selection_node[node].num_visit);
            if(selection_node[node].uct_value>max){
                max=selection_node[node].uct_value;
                int selection_move = node;
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
        
        if(node_num==0){
            return 0;
        }
        for(int i=0;i<node_num;i++)
        {
            copy(board,board_ep);
            board_ep.placeDisk(legal_moves.at(i),turn);
            turn = getOpponentSide(turn);
            selection_node[selection_move].num_ep_visit+=1;
            int outcome = Simulation(board_ep,turn,0,0);
            Side BLACK = Side::BLACK;
            Side WHITE = Side::WHITE;
            if((turn==BLACK && outcome == 1) && (turn==WHITE && outcome == -1))
            {
                selection_node[selection_move].num_win+=1;
            }
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

    
}  // namespace reversi
