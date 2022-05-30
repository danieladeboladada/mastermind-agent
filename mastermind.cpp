#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <ctime>

using namespace std;

struct arrange{
    int heuristic_value;
    int appearances;
    int id;
    bool operator<(arrange a) const
    {
        if (heuristic_value == a.heuristic_value){
            if (appearances == a.appearances)
                return id < a.id;
            else 
                return appearances < a.appearances;
        }
        return heuristic_value > a.heuristic_value;
    }
};

void create_answers(vector<int> &guess, int blacks,int whites, vector<bool> &checked, vector<int> &current_answer, vector <vector <int> > &answers){
    int i,j;
    int nc;
    if (blacks != 0){
        nc = guess.size();
        for (i = 0; i < nc; i++){
            if ((!checked[i]) && (guess[i] == current_answer[i] || current_answer[i] == 0)){
                checked[i] = true;
                int temp = current_answer[i];
                current_answer[i] = guess[i];
                create_answers(guess, blacks - 1, whites, checked, current_answer, answers);
                checked[i] = false;
                current_answer[i] = temp;
            }      
        }
    }   
    else if (whites != 0){
        nc = guess.size();
        for(i=0;i < nc;i++){
            if (!checked[i]){
                checked[i] = true;
                for(j=0; j < nc; j++){
                    if (i != j && (guess[i] == current_answer[j] || current_answer[j] == 0)){
                        int temp = current_answer[j];
                        current_answer[j] = guess[i];
                        create_answers(guess,blacks,whites-1,checked,current_answer,answers);
                        current_answer[j] = temp;
                    }
                }
                checked[i] = false;
            }     
        }
    }
        
    else{
        bool sw = false;
        for (i = 0; i<answers.size(); i++){
            if (current_answer == answers[i]){
                sw = true;
                break;
            }
        }
        if (!sw){
            answers.push_back(current_answer);
        }
    }  
}

bool match(vector <int> &possible_answer, vector <int> &current_answer){
    int i;
    for (i=0;i<possible_answer.size();i++){
        if (possible_answer[i] != current_answer[i] && current_answer[i] != 0){
            return false;
        }
    }
    return true;
}

int heuristic(vector <int> &possible_answer, vector <vector <int> > &answers){
    int i;
    int result = 0;

    for(i=0;i<answers.size();i++){
        if (match(possible_answer,answers[i])){ result++; }
    }

    return result;
}

void create_all(int num_color, int num_pick, vector <int> &current, vector <vector <int> > &arrangements){
    if (current.size() >= num_pick){
        arrangements.push_back(current);
        return;
    }

    int i;
    for(i=1;i <= num_color;i++){
        if(find(current.begin(), current.end(),i) == current.end()){
            current.push_back(i);
            create_all(num_color,num_pick,current,arrangements);
            current.pop_back();
        }
    }

}

void create_arrangement(int num_color, int num_pick, vector <int> &arrangement, vector <int> &useless_colors){
    while(arrangement.size() != num_pick){
        int color = rand() % num_color + 1;
        if (find(arrangement.begin(), arrangement.end(), color) == arrangement.end() &&
            find(useless_colors.begin(), useless_colors.end(), color) == useless_colors.end()){
            arrangement.push_back(color);
        }
    }
}

void create_score(vector <int> &guess, vector <int> &arrangement, int &blacks, int &whites){
    int i;
    blacks = whites = 0;
    for (i=0;i<guess.size();i++){
        if(guess[i] == arrangement[i]){
            blacks++;
        }
    }
    for (i=0;i<guess.size();i++){
        if(guess[i] != arrangement[i] && find(arrangement.begin(), arrangement.end(), guess[i]) != arrangement.end()){
            whites++;
        }
    }
}

void create_first_guess(int num_color, int num_pick, int &blacks, int &whites, vector <int> &guess, vector <int> &arrangement, vector <int> &useless_colors){
    int i;
    guess.clear();

    create_arrangement(num_color, num_pick, guess,useless_colors);
    create_score(guess,arrangement,blacks, whites);
    if(blacks == 0 && whites == 0){
        for(i=0;i<guess.size();i++){
            useless_colors.push_back(guess[i]);
        }
    }
}

void print_guess(vector <int> &guess, int black, int white, vector <string> &colors){
    int i;
    for(i=0;i<guess.size();i++){
        if(i != 0) cout << ", ";
        cout << colors[guess[i]]; 
    }
    cout << ". blacks = " << black << ", whites = " << white << endl;
}

void init_game(int num_color, int num_pick,int &num_guesses, vector <vector <int> > &guesses, 
                vector <vector <int> > &answers, vector <int> &blacks, vector <int> &whites ,
                vector <int> &arrangement, vector <int> &useless_colors, vector <string> &colors)
{
    int black, white;
    vector <int> guess;
    vector <int> current_answer(num_pick,0);
    vector <bool> checked(num_pick, false);
    num_guesses = 0;
    do {
        create_first_guess(num_color,num_pick,black,white,guess,arrangement,useless_colors);
        guesses.push_back(guess);
        blacks.push_back(black);
        whites.push_back(white);
        num_guesses++;
        cout << "Guess " << num_guesses << ": ";
        print_guess(guess,black,white,colors);

    } while (black == 0 && white == 0);

    if(black != num_pick)
        create_answers(guess,black,white,checked,current_answer,answers);

}

bool intersection(vector <int> &guess, vector <int> &colors){
    int i;
    for (i=0;i<guess.size();i++){
        if(find(colors.begin(),colors.end(),guess[i]) != colors.end()){
            return true;
        }
    }
    return false;
}

int counting_colors(vector <vector <int> > &guesses, vector <int> &arrangement){
    int i,j;
    int sum = 0;
    for(i=0;i<arrangement.size();i++){
        for(j=0;j<guesses.size();j++){
            if(find(guesses[j].begin(), guesses[j].end(), arrangement[i]) != guesses[j].end()){
                sum++;
            }
        }
    }
    return sum;
}

bool possible_answer(vector <vector <int> > &guesses, vector <int> &blacks, vector <int> &whites, vector <int> &answer){
    int i,j;
    int common_colors;
    for(i=0;i<guesses.size();i++){
        common_colors = 0;
        for(j=0;j<answer.size();j++){
            if(find(guesses[i].begin(),guesses[i].end(), answer[j]) != guesses[i].end()) 
                common_colors++;
        }
        if(blacks[i] + whites[i] < common_colors) return false; 
    }
    return true;
}

void play_computerVScomputer(int num_color,int num_pick, vector <vector <int> > &guesses, 
                            vector <int> &blacks, vector <int> &whites, vector <string> &colors, vector <int> &arrangement){
    vector <int> useless_colors;
    vector < vector <int> > answers;
    vector <vector <int> > arrangements;
    vector <int> current;
    int i,black,white; 
    int num_guesses = 0, id = 0;
    map <arrange, vector <int> > searchtree; 
    map <arrange, vector <int> >::iterator mavi;

    cout << "The computer picks " << endl;  
    for(i=0;i<arrangement.size();i++){
        if(i != 0){
            cout << ", ";
        }
        cout << colors[arrangement[i]];
    }
    cout << endl << "Game starts!" << endl;

    // initializing the game
    init_game(num_color,num_pick,num_guesses,guesses,answers,blacks,whites,arrangement,useless_colors,colors);

    //check if we are lucky
    if(blacks[num_guesses-1] == num_pick){
        cout << "The game is finished in " << num_guesses << " guesses." << endl;
        return;
    }

    //initializing the search tree
    create_all(num_color,num_pick, current,arrangements);
    for(i=0;i<arrangements.size();i++){
        if(!intersection(arrangements[i],useless_colors) && find(guesses.begin(), guesses.end(), arrangements[i]) == guesses.end()){
            arrange a;
            a.heuristic_value = heuristic(arrangements[i],answers);
            a.appearances = counting_colors(guesses, arrangement);
            a.id = ++id;
            if (a.heuristic_value != 0 && possible_answer(guesses,blacks,whites,arrangements[i])) searchtree[a] = arrangements[i];// we might need to change this because of pointers
        }
    }

    // play the game
    while(blacks[num_guesses-1] != num_pick){
        vector <vector <int> > new_answers;
        // suggesting the best guess we have
        vector <int> best_guess = searchtree.begin()->second;
        cout << "The heuristic value = " << searchtree.begin()->first.heuristic_value << " / " << searchtree.size() << endl;
        searchtree.erase(searchtree.begin());
        create_score(best_guess,arrangement,black,white);
        guesses.push_back(best_guess);
        blacks.push_back(black);
        whites.push_back(white);
        num_guesses++;
        cout << "Guess " << num_guesses << ": ";
        print_guess(best_guess,black,white,colors);

        if(black != num_pick){
            map <arrange, vector <int> > new_searchtree;
            for(i=0;i<answers.size();i++){
                vector <bool> checked(num_pick, false);
                create_answers(best_guess,black,white,checked,answers[i],new_answers);
            }
            answers.swap(new_answers);
            for(mavi = searchtree.begin(); mavi != searchtree.end();++mavi){
                arrange a;
                a.heuristic_value = heuristic(mavi->second,answers);
                a.appearances = counting_colors(guesses,mavi->second);
                a.id = ++id;
                if(a.heuristic_value != 0 && possible_answer(guesses,blacks,whites,mavi->second)) new_searchtree[a] = mavi->second;
            }
            searchtree.swap(new_searchtree);
        }
    }
    cout << "The game is finished in " << num_guesses << " guesses." << endl;
}

void play_computerVShuman(int num_color,int num_pick, vector <vector <int> > &guesses, 
                            vector <int> &blacks, vector <int> &whites, vector <string> &colors, vector <int> &arrangement)
{
    vector <int> guess;
    int i,j,black,white; 
    int num_guesses = 0;
    bool ok, rc;
    black = white = 0;
    cout << "Available colors:";
    for(i=0;i<colors.size();i++){
        cout << " " << colors[i];
    }
    cout << endl; 
    while(black != num_pick){
        cout << "Please enter your guess (" << num_pick << " space separated colors):" << endl;
        guess.clear();
        rc = true;
        for(i=0;i<num_pick;i++){
            string c;
            cin >> c;
            if(c[0] >= 'a' && c[0] <= 'z') c[0] -= 32;
            for(j=1;j<c.size();j++){
                if(c[j] >= 'A' && c[j] <= 'Z') c[j] += 32;
            }
            if(find(colors.begin(),colors.end(),c) == colors.end()){
                rc = false;
            }
            else{
                int p = find(colors.begin(),colors.end(),c) - colors.begin();
                guess.push_back(p);
            }
        }
        ok = true;
        for(i=0;i<guess.size();i++){
            if(count(guess.begin(),guess.end(),guess[i]) > 1){
                ok = false;
                break;
            }
        }
        if(!rc){
            cout << "Invalid color! Try again" << endl;
        }
        else if(!ok){
            cout << "You cannot repeat colors! Try again" << endl;
        }
        else {
            create_score(guess,arrangement,black,white);
            guesses.push_back(guess);
            blacks.push_back(black);
            whites.push_back(white);
            num_guesses++;
            cout << "Guess " << num_guesses << ": ";
            print_guess(guess,black,white,colors);
        }
    }
    cout << "The game is finished in " << num_guesses << " guesses." << endl;

}

void init_colors(vector <string> &colors){
    colors.push_back("");
    colors.push_back("Red");
    colors.push_back("Brown");
    colors.push_back("Yellow");
    colors.push_back("Blue");
    colors.push_back("Pink");
    colors.push_back("Violet");
    colors.push_back("Orange");
    colors.push_back("Purple");
    colors.push_back("Green");
    colors.push_back("Cyan");
    colors.push_back("Lavender");
    colors.push_back("Magenta");
    colors.push_back("Indigo");
    colors.push_back("Turquoise");
    colors.push_back("Burgundy");
    colors.push_back("Chartreuse");
    colors.push_back("Beige");
    colors.push_back("Teal");
    colors.push_back("Fuchsia");
    colors.push_back("Hazel");
    colors.push_back("Neruple");
}

int main(){
    vector<int> guess;
    vector<int> current_answer(4,0);
    vector<bool> checked(4, false);
    vector<vector <int> > answers;
    int i,j;
    int num_color = 10, num_pick = 4;
    vector <string> colors;
    vector <vector <int> > guesses;
    vector <int> blacks;
    vector <int> whites;
    vector <int> arrangement;

    srand (time(NULL));

    init_colors(colors);
    // choosing the target
    create_arrangement(num_color, num_pick, arrangement, guess);
    play_computerVShuman(num_color,num_pick,guesses,blacks,whites,colors,arrangement);
    guesses.clear();
    blacks.clear();
    whites.clear();
    // play_computerVScomputer(num_color,num_pick,guesses,blacks,whites,colors,arrangement);

    return 0;
}
    