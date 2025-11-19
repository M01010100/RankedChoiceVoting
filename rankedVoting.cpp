#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "ranked_voting.h"

map<int, County> read_votes_from_file(const string& filename) {
    ifstream file(filename);
    string line;
    map<int, County> counties;

    while (getline(file, line)) {
        istringstream iss(line);
        int county_code;
        iss >> county_code;

        vector<int> ballot(5);
        for (int i = 0; i < 5; ++i) {
            iss >> ballot[i];
        }

        counties[county_code].code = county_code;
        counties[county_code].votes.push_back(ballot);
    }

    return counties;
}

void run_ranked_voting_overall(const map<int, County>& counties) {
    vector<vector<int>> all_votes;
    for (const auto& county_pair : counties) {
        const County& county = county_pair.second;
        all_votes.insert(all_votes.end(), county.votes.begin(), county.votes.end());
    }

    int total_votes = all_votes.size();
    cout << "Total votes: " << total_votes << endl;

    set<int> remaining_candidates = {0, 1, 2, 3, 4};
    while (remaining_candidates.size() > 1) {
        map<int, int> first_choice_counts;
        for (const auto& ballot : all_votes) {
            int first_choice = -1;
            for (int rank : ballot) {
                if (remaining_candidates.count(rank - 1)) {
                    first_choice = rank - 1;
                    break;
                }
            }
            first_choice_counts[first_choice]++;
        }

        print_vote_counts(first_choice_counts, total_votes);

        if (has_majority(first_choice_counts, total_votes)) {
            int highest_candidate = -1;
            int highest_count = 0;
            for (const auto& count : first_choice_counts) {
                if (remaining_candidates.count(count.first) && count.second > highest_count) {
                    highest_candidate = count.first;
                    highest_count = count.second;
                }
            }
            cout << "Winner: Candidate " << char('A' + highest_candidate) << endl;
            break;
        }

        int lowest_candidate = find_lowest_candidate(first_choice_counts, remaining_candidates);
        remaining_candidates.erase(lowest_candidate);
    }
}


bool has_majority(const map<int, int>& first_choice_counts, int total_votes) {
    for (const auto& count : first_choice_counts) {
        if (count.second > total_votes / 2) {
            return true;
        }
    }
    return false;
}

void print_vote_counts(const map<int, int>& first_choice_counts, int total_votes) {
    for (const auto& count : first_choice_counts) {
        cout << "Candidate " << char('A' + count.first) << ": " << count.second
             << " votes (" << (100.0 * count.second / total_votes) << "%)" << endl;
    }
    cout << endl;
}
void print_county_votes(const map<int, County>& counties) {
    cout << "\nCounty Votes:" << endl;
    for (const auto& county_pair : counties) {
        int county_code = county_pair.first;
        int county_votes = county_pair.second.votes.size();
        cout << "County " << county_code << ": " << county_votes << " votes" << endl;
    }
}

int find_lowest_candidate(const map<int, int>& first_choice_counts, const set<int>& remaining_candidates) {
    int lowest_candidate = -1;
    int lowest_count = INT_MAX;

    for (const auto& count : first_choice_counts) {
        if (remaining_candidates.count(count.first) && count.second < lowest_count) {
            lowest_candidate = count.first;
            lowest_count = count.second;
        }
    }

    return lowest_candidate;
}
