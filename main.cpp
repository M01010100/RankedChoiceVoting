#include <iostream>
#include "ranked_voting.h"

int main() {
    const string filename = "votes.txt";
    
    map<int, County> counties = read_votes_from_file(filename);
    
    print_county_votes(counties);
    
    cout<<"\nVote breakdown:\n"<<endl;
    
    run_ranked_voting_overall(counties);
}
