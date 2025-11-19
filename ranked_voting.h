#pragma once

#include <map>
#include <vector>
#include <string>
#include <set>

using namespace std;

struct County {
    int code;
    vector<vector<int>> votes;
};

map<int, County> read_votes_from_file(const string& filename);

void run_ranked_voting_overall(const map<int, County>& counties);

bool has_majority(const map<int, int>& first_choice_counts, int total_votes);

void print_vote_counts(const map<int, int>& first_choice_counts, int total_votes);

int find_lowest_candidate(const map<int, int>& first_choice_counts, const set<int>& remaining_candidates);

void print_county_votes(const map<int, County>& counties);
