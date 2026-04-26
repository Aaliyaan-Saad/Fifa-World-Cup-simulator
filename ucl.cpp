#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

class Team {
private:
    string name;

    float attack;
    float midfield;
    float defense;

    int points;
    int goalsScored;
    int goalsConceded;
    int wins;
    int draws;
    int losses;
    int matchesPlayed;

    double formMultiplier;

    vector<string> opponentsPlayed;

public:
    Team(string n, int att, int mid, int def) {
        name = n;
        attack = att;
        midfield = mid;
        defense = def;

        points = 0;
        goalsScored = 0;
        goalsConceded = 0;
        wins = draws = losses = matchesPlayed = 0;

        formMultiplier = 1.0;
    }

    string getName() const { return name; }
    float getAttack() const { return attack; }
    float getMidfield() const { return midfield; }
    float getDefense() const { return defense; }
    int getPoints() const { return points; }
    int getGoalDifference() const { return goalsScored - goalsConceded; }
    int getGoalsScored() const { return goalsScored; }
    double getForm() const { return formMultiplier; }

    bool hasPlayed(const string& opponentName) const {
        for (const auto& opp : opponentsPlayed) {
            if (opp == opponentName)
                return true;
        }
        return false;
    }

    void addOpponent(const string& opponentName) {
        opponentsPlayed.push_back(opponentName);
    }

    void updateStats(int goalsFor, int goalsAgainst) {
        matchesPlayed++;
        goalsScored += goalsFor;
        goalsConceded += goalsAgainst;

        if (goalsFor > goalsAgainst) {
            wins++;
            points += 3;
            updateForm(true);
        }
        else if (goalsFor == goalsAgainst) {
            draws++;
            points += 1;
            updateForm(false);
        }
        else {
            losses++;
            updateForm(false, true);
        }
    }

    void updateForm(bool win, bool loss = false) {
        if (win)
            formMultiplier += 0.02;
        else if (loss)
            formMultiplier -= 0.02;

        if (formMultiplier > 1.10) formMultiplier = 1.10;
        if (formMultiplier < 0.90) formMultiplier = 0.90;
    }

    double getOverallStrength() const {
        return (attack * 0.33 + midfield * 0.34 + defense * 0.33) * formMultiplier;
    }

    void display() const {
        cout << name << " | Pts: " << points
             << " | GD: " << getGoalDifference()
             << " | GS: " << goalsScored
             << " | Form: " << formMultiplier
             << endl;
    }
};

// ---------- Probability helpers ----------
double random01() {
    return static_cast<double>(rand()) / RAND_MAX;
}

double calculateTeamProbability(const Team& t1, const Team& t2) {
    // Form is already included inside getOverallStrength().
    double effectiveStrength1 = t1.getOverallStrength();
    double effectiveStrength2 = t2.getOverallStrength();

    // Squaring creates a clearer gap between strong and weak teams,
    // so we do not get too many unrealistic upsets.
    double weightedStrength1 = effectiveStrength1 * effectiveStrength1;
    double weightedStrength2 = effectiveStrength2 * effectiveStrength2;

    return weightedStrength1 / (weightedStrength1 + weightedStrength2);
}

int generateGoals(double teamProbability) {
    const int chances = 5;
    const double conversionFactor = 0.45;

    double scoringProbability = teamProbability * conversionFactor;
    int goals = 0;

    for (int i = 0; i < chances; i++) {
        if (random01() < scoringProbability) {
            goals++;
        }
    }

    return goals;
}

// ---------- Match function ----------
void playMatch(Team& t1, Team& t2) {
    double team1Probability = calculateTeamProbability(t1, t2);
    double team2Probability = 1.0 - team1Probability;

    int goals1 = generateGoals(team1Probability);
    int goals2 = generateGoals(team2Probability);

    cout << t1.getName() << " " << goals1 << " - " << goals2 << " " << t2.getName() << endl;

    t1.updateStats(goals1, goals2);
    t2.updateStats(goals2, goals1);

    t1.addOpponent(t2.getName());
    t2.addOpponent(t1.getName());
}

class Group {
private:
    string groupName;
    vector<Team> teams;

public:
    Group(string name, vector<Team> t) {
        groupName = name;
        teams = t;
    }

    void playGroupMatches() {
        cout << "\n--- Matches in " << groupName << " ---\n";

        playMatch(teams[0], teams[1]);
        playMatch(teams[0], teams[2]);
        playMatch(teams[0], teams[3]);
        playMatch(teams[1], teams[2]);
        playMatch(teams[1], teams[3]);
        playMatch(teams[2], teams[3]);
    }

    void displayTable() {
        vector<Team> sortedTeams = teams;

        sort(sortedTeams.begin(), sortedTeams.end(), [](Team a, Team b) {
            if (a.getPoints() != b.getPoints())
                return a.getPoints() > b.getPoints();
            if (a.getGoalDifference() != b.getGoalDifference())
                return a.getGoalDifference() > b.getGoalDifference();
            return a.getGoalsScored() > b.getGoalsScored();
        });

        cout << "\n--- " << groupName << " Table ---\n";
        for (const auto& team : sortedTeams) {
            team.display();
        }
    }

    vector<Team> getTopTwo() {
        vector<Team> sortedTeams = teams;

        sort(sortedTeams.begin(), sortedTeams.end(), [](Team a, Team b) {
            if (a.getPoints() != b.getPoints())
                return a.getPoints() > b.getPoints();
            if (a.getGoalDifference() != b.getGoalDifference())
                return a.getGoalDifference() > b.getGoalDifference();
            return a.getGoalsScored() > b.getGoalsScored();
        });

        vector<Team> topTwo;
        topTwo.push_back(sortedTeams[0]);
        topTwo.push_back(sortedTeams[1]);

        return topTwo;
    }
};

class Knockout {
public:
    Team playKnockoutMatch(Team t1, Team t2, string stage) {
        double team1Probability = calculateTeamProbability(t1, t2);
        double team2Probability = 1.0 - team1Probability;

        int goals1 = generateGoals(team1Probability);
        int goals2 = generateGoals(team2Probability);

        cout << t1.getName() << " " << goals1 << " - " << goals2 << " " << t2.getName() << endl;

        t1.updateStats(goals1, goals2);
        t2.updateStats(goals2, goals1);
        
        // double strength1 = t1.getOverallStrength();
        // double strength2 = t2.getOverallStrength();

        // int goals1 = rand() % 3;
        // int goals2 = rand() % 3;

        // if (strength1 > strength2)
        //     goals1 += rand() % 2;
        // else if (strength2 > strength1)
        //     goals2 += rand() % 2;

        // cout << "\n--- " << stage << " ---\n";
        // cout << t1.getName() << " " << goals1 << " - " << goals2 << " " << t2.getName() << endl;

         if (goals1 > goals2) {
             cout << t1.getName() << " qualifies!\n"<<endl;
             return t1;
        }
        else if (goals2 > goals1) {
            cout << t2.getName() << " qualifies!\n"<<endl;
            return t2;
        }

        cout << "Draw. Penalties!\n"<<endl;

        int pens1 = rand() % 5 + 1;
        int pens2 = rand() % 5 + 1;

        while (pens1 == pens2) {
            pens1 = rand() % 5 + 1;
            pens2 = rand() % 5 + 1;
        }

        cout << t1.getName() << " " << pens1 << " - " << pens2 << " " << t2.getName() << " (Penalties)\n"<<endl;

        if (pens1 > pens2) {
            cout << t1.getName() << " wins on penalties!\n"<<endl;
            return t1;
        }
        else {
            cout << t2.getName() << " wins on penalties!\n"<<endl;
            return t2;
        }
    }

    vector<Team> playRound(vector<Team> teams, string roundName) {
        vector<Team> winners;

        for (int i = 0; i < teams.size(); i += 2) {
            Team winner = playKnockoutMatch(teams[i], teams[i + 1], roundName);
            winners.push_back(winner);
        }

        return winners;
    }

    void playTournament(vector<Team> qualifiedTeams) {
        vector<Team> currentTeams = qualifiedTeams;
        int roundNumber = 1;

        while (currentTeams.size() > 1) {
            string roundName;

            if (currentTeams.size() == 2)
                roundName = "Final";
            else if (currentTeams.size() == 4)
                roundName = "Semifinals";
            else if (currentTeams.size() == 8)
                roundName = "Quarterfinals";
            else
                roundName = "Knockout Round " + to_string(roundNumber);

            currentTeams = playRound(currentTeams, roundName);
            roundNumber++;
        }

        cout << "\n============================\n";
        cout << "Champion: " << currentTeams[0].getName() << endl;
        cout << "============================\n";
    }
};

int main() {
    srand(time(0));

    vector<Team> groupATeams = {
        Team("Qatar", 68, 60, 69),
        Team("Ecuador", 66, 68, 69),
        Team("Senegal", 85, 81, 82),
        Team("Netherlands", 88, 85, 87),
    };

    vector<Team> groupBTeams = {
        Team("England", 91, 90, 88),
        Team("Iran", 71, 70, 72),
        Team("USA", 75, 74, 76),
        Team("Wales", 82, 81, 80),
    };

    vector<Team> groupCTeams={
        Team("Argentina", 92, 91, 89),
        Team("Saudi Arabia", 68, 69, 62),
        Team("Mexico", 80, 80, 79),
        Team("Poland", 85, 82, 81),
    };

    vector<Team> groupDTeams={
        Team("France", 94, 95, 90),
        Team("Australia", 72, 74, 76),
        Team("Denmark", 84, 82, 85),
        Team("Tunisia", 69, 65, 69),
    };

    vector<Team> groupETeams={
        Team("Spain", 90, 90, 90),
        Team("Costa Rica", 60, 53, 67),
        Team("Germany", 91, 89, 91),
        Team("Japan", 81, 81, 81),
    };

    vector<Team> groupFTeams={
        Team("Belgium", 86, 85, 84),
        Team("Canada", 68, 67, 74),
        Team("Morocco", 79, 82, 85),
        Team("Croatia", 85, 87, 84),
    };

    vector<Team> groupGTeams={
        Team("Brazil", 91, 89, 88),
        Team("Serbia", 79, 80, 80),
        Team("Switzerland", 79, 82, 83),
        Team("Cameroon", 76, 75, 64),
    };
    vector<Team> groupHTeams={
        Team("Portugal", 91, 92, 89),
        Team("Ghana", 63, 66, 67),
        Team("Uruguay", 86, 85, 84),
        Team("South Korea", 82, 72, 71),
    };

    // ====================passing vecotrs and making them group objects===============

    Group groupA("Group A", groupATeams);
    Group groupB("Group B", groupBTeams);
    Group groupC("Group C", groupCTeams);
    Group groupD("Group D", groupDTeams);
    Group groupE("Group E", groupETeams);
    Group groupF("Group F", groupFTeams);
    Group groupG("Group G", groupGTeams);
    Group groupH("Group H", groupHTeams);

    //============ running group stage ===============

    cout<<R"(
 __      __            .__       .___ _________                  _________.__              .__          __                
/  \    /  \___________|  |    __| _/ \_   ___ \ __ ________    /   _____/|__| _____  __ __|  | _____ _/  |_  ___________ 
\   \/\/   /  _ \_  __ \  |   / __ |  /    \  \/|  |  \____ \   \_____  \ |  |/     \|  |  \  | \__  \\   __\/  _ \_  __ \
 \        (  <_> )  | \/  |__/ /_/ |  \     \___|  |  /  |_> >  /        \|  |  Y Y  \  |  /  |__/ __ \|  | (  <_> )  | \/
  \__/\  / \____/|__|  |____/\____ |   \______  /____/|   __/  /_______  /|__|__|_|  /____/|____(____  /__|  \____/|__|   
       \/                         \/          \/      |__|             \/          \/                \/                   )"<<endl;

    cout<<"press y to start simulation"<<endl;
    char yes;
    cin>>yes;

    if (yes=='y')
    {

        
        groupA.playGroupMatches();
        groupB.playGroupMatches();
        groupC.playGroupMatches();
        groupD.playGroupMatches();
        groupE.playGroupMatches();
        groupF.playGroupMatches();
        groupG.playGroupMatches();
        groupH.playGroupMatches();

        char  yes4;
        cout<<endl<<endl<<endl;

        cout<<"do you want to display tables?"<<endl;

        cin>>yes4;
        if (yes4=='y')
        {
            //=================== Dislaying tables =======================
            groupA.displayTable();    
            groupB.displayTable();
            groupC.displayTable();
            groupD.displayTable();
            groupE.displayTable();
            groupF.displayTable();
            groupG.displayTable();
            groupH.displayTable();

            

            vector<Team> topA = groupA.getTopTwo();
            vector<Team> topB = groupB.getTopTwo();
            vector<Team> topC = groupC.getTopTwo();
            vector<Team> topD = groupD.getTopTwo();
            vector<Team> topE = groupE.getTopTwo();
            vector<Team> topF = groupF.getTopTwo();
            vector<Team> topG = groupG.getTopTwo();
            vector<Team> topH = groupH.getTopTwo();



             cout<<"do you wish to get the qualified teams press y if so."<<endl;
            char yes2;
            cin>>yes2;

            if (yes2=='y')
            {
                cout << "\nQualified from Group A:\n";
                cout << topA[0].getName() << endl;
                cout << topA[1].getName() << endl;

                cout << "\nQualified from Group B:\n";
                cout << topB[0].getName() << endl;
                cout << topB[1].getName() << endl;

                cout << "\nQualified from Group C:\n";
                cout << topC[0].getName() << endl;
                cout << topC[1].getName() << endl;

                cout << "\nQualified from Group D:\n";
                cout << topD[0].getName() << endl;
                cout << topD[1].getName() << endl;

                cout << "\nQualified from Group E:\n";
                cout << topE[0].getName() << endl;
                cout << topE[1].getName() << endl;

                cout << "\nQualified from Group F:\n";
                cout << topF[0].getName() << endl;
                cout << topF[1].getName() << endl;

                cout << "\nQualified from Group G:\n";
                cout << topG[0].getName() << endl;
                cout << topG[1].getName() << endl;

                cout << "\nQualified from Group H:\n";
                cout << topH[0].getName() << endl;
                cout << topH[1].getName() << endl;


                cout<<endl;

                cout<<"press y to simulate knockout rounds"<<endl;
                char yes3;
                cin>>yes3;

                if (yes3=='y')
                {
                    vector<Team> qualifiedTeams;
                    qualifiedTeams.push_back(topA[0]); // A1
                    qualifiedTeams.push_back(topB[1]); // B2
                    qualifiedTeams.push_back(topB[0]); // B1
                    qualifiedTeams.push_back(topA[1]); // A2
                    qualifiedTeams.push_back(topC[0]); // C1
                    qualifiedTeams.push_back(topD[1]); // D2
                    qualifiedTeams.push_back(topD[0]); // D1
                    qualifiedTeams.push_back(topC[1]); // C2
                    qualifiedTeams.push_back(topE[0]); // E1
                    qualifiedTeams.push_back(topF[1]); // F2
                    qualifiedTeams.push_back(topF[0]); // F1
                    qualifiedTeams.push_back(topE[1]); // E2
                    qualifiedTeams.push_back(topG[0]); // G1
                    qualifiedTeams.push_back(topH[1]); // H2
                    qualifiedTeams.push_back(topH[0]); // H1
                    qualifiedTeams.push_back(topG[1]); // G2
                    

                    Knockout knockout;
                    knockout.playTournament(qualifiedTeams);
                }
                else
                {
                    cout<<"Simulation ended by user";
                    return 0;
                }
                

            }
            else
            {
                cout<<"Simulation ended by user";
                return 0;
            }
        }
        else
        {
            cout<<"simulation ended by user";
        }
        
        

        
        

        

       
        

        



        

        return 0;
    }
    else
    {
        cout<<"Simulation ended by user";
        return 0;
    }
    
    

    

    
}

