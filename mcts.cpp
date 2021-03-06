#include "mcts.h"

/*
void MCTS::init_with_board(board& b)
{
    root = new Node;
    //root_board = b; //copy a board from main
    root->initNode(nullptr, BOARDVL, b.just_play_color()); //2019.10.10.17:09....
    //root->show_stats();
    //use NULL, BOARDSZ, 0, 0 for last action doesnt matter
    root->expand(b);
    //root->show_stats();
    total = 0;
}

void MCTS::reset(board& b)
{
    //curroot = root; //use game record to find current Node?
    simu_board = b;
    for(Action : b.actions_taken)
    path.clear(); //clear select path
}

Node* MCTS::select()
{
    Node* cur = root;
    path.push_back(cur);
    while (cur->cptr.size()) //if cptr.size() = 0, not expanded or game over
    {
        cur = cur->best_child();
        simu_board.add(cur->pos, cur->color);
        rave_path[cur->color].push_back(cur->pos);
        path.push_back(cur);
    }
    return cur;
}

bool MCTS::roll_out() //the one in the board
{
    bool c = !simu_board.just_play_color(); //c = cur player to move
    //getv update info for rollout by pointer and reference
    simu_board.getv(bpos, wpos, bothpos, bpos_sz, wpos_sz, bothpos_sz);
    //simulate the rest of board, record is in b
    //res == 1 => black(0)player win
    int res = simu_board.simulate(c, bpos, wpos, bothpos, bpos_sz, wpos_sz, bothpos_sz);
    //update the simu actions to my rave_path vec
    //tree sim part in board b/wpath
    for (int i = 0; i < simu_board.bpsize; i++)
        rave_path[0].push_back(simu_board.bpath[i]);
    for (int i = 0; i < simu_board.wpsize; i++)
        rave_path[1].push_back(simu_board.wpath[i]); //wtf this bug//bugged until 2019/10/0 18:22
    return res; //modified so that result is winner
}

void MCTS::backpropogation(bool res)
{
    //update the sim_tree part with path, and child's rave value with rave_path
    //rave_path[0/1][i] ~= path[i*2]
    for (int t = 0; t < path.size(); t++) {
        path[t]->update(res);
        //AMAF
        //if there is action A in the subtree from the afterstate S(color, pos) now
        //then Q(S(now),A) should be updated
        bool c = !path[t]->color; //the cur color to play(= children's color)
        for (int tp = 0; tp < rave_path[c].size(); tp++)
        //tp=t/2=>subtree, tp=0=>all, should not influence the result because actionsi staken
        {
            int k = path[t]->idx[rave_path[c][tp]];
            if (k != -1)
                path[t]->cptr[k]->rave_update(res);
        }
    }
}

int MCTS::best_action(board& init_b, bool color, int simu_per_step)
{
    init_with_board(init_b); //create and expand root(null, basenum, p, c)
    time_t start_t, cur_t;
    start_t = clock();

#ifdef USEROUNDS
    for (int ep = 0; ep < simu_per_step; ep++) {
#endif
        //reset path, board
        reset(init_b);
        //selection
        Node* selected_root = select();
        //expansion
        //expand when the is chosen once before, i.e. the second time
        //let rave value do the job at opening
        bool res, nc;
        if (selected_root->cnt == BASENUM+1) { //testing, 2020.1.16 is better(20 game exp)
            int nc = selected_root->expand(simu_board);
            //if there are children, one step look ahead
            //just expand => ravevalue as heuristic
            if (nc != 0) {
                selected_root = selected_root->best_child();
                simu_board.add(selected_root->pos, selected_root->color);
                rave_path[selected_root->color].push_back(selected_root->pos);
                path.push_back(selected_root);
            }
        }
        //simulation, with the intuition of two-go position is better
        res = roll_out();
        //backpropogation
        backpropogation(res);
    }
    //return result, forget to judge NULL at first
    //best policy is of highest rave_winrate(in opening) and highest winrate(in ending)
    vector<double> pi = root->getPi();
    Action maxA = (root->mvc == nullptr) ? -1 : root->mvc->pos;
    //resign to speed up game process care /0
    if (maxA != -1 && root->mvc->cnt != 0.0 && root->mvc->num / root->mvc->cnt < RESIGN)
        maxA = -1;
#ifdef SHOW
    cerr << endl;
    cur_t = clock();
    cerr << "simulations/sec : " << double(DEFAUT_SIMS) * CLOCKS_PER_SEC / double(cur_t - start_t) << endl;
    //cerr << "winrate(root_num/root/cnt) : " <<  <<endl;
    cerr << "show policy of the board:(double)" << endl;
    for (int ci = 0; ci < BOARDSZ; ci++) {
        for (int ri = 0; ri < BOARDSZ; ri++) {
            cerr << setw(3) << std::setprecision(3) << std::fixed << pi[ci * BOARDSZ + ri] << ' ';
        }
        cerr << endl;
    }
    cerr << "show policy of the board(int):" << endl;
    for (int ci = 0; ci < BOARDSZ; ci++) {
        for (int ri = 0; ri < BOARDSZ; ri++) {
            cerr << setw(3) << std::fixed << int(pi[ci * BOARDSZ + ri]*simu_per_step) << ' ';
        }
        cerr << endl;
    }
#endif
    //if(calc_winrate() < RESIGN) maxA = -1;
    //root->show_stats();
    clear();
    return maxA;
}

double MCTS::calc_winrate()
{
    //cerr << "HI" << endl;
    return root->show_stats();
}

void MCTS::clear()
{
    delete root; //recursively delete by def of ~node
}
*/
