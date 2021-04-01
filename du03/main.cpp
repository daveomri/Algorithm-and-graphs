#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;




struct Node{
    int litersOfBeer;
    int visited = 0;
    int thisBeer = 0;
    vector <Node*> neighbours;
};

int recurMe(Node * tmp, int notThisBeer, vector<int>&beerPrice){
    if (tmp->neighbours.size() == 1 && tmp->neighbours[0]->visited == 1){
        int lowerPrice = -1;
        int indexOBeer = 0;
        for (int i = 0; i<(int)beerPrice.size(); ++i){
            if (i == notThisBeer){
                continue;
            }
                    
            if (lowerPrice == -1){
                lowerPrice = beerPrice[i];
                indexOBeer = i;
            }
            else if (beerPrice[i] < lowerPrice){
                lowerPrice = beerPrice[i];
                indexOBeer = i;
            }
        }
        
        tmp->visited = 0;
        tmp->thisBeer = indexOBeer;
        return lowerPrice*tmp->litersOfBeer;
    }
    else{
        int bestPrice = 0;
        int curPrice = 0;
        tmp->visited = 1;
        for (int i = 0; i < (int)beerPrice.size(); ++i){
            if (i == notThisBeer){
                continue;
            }         

            curPrice = 0;
            curPrice += tmp->litersOfBeer*beerPrice[i];

            for (int j = 0; j < (int)tmp->neighbours.size(); ++j){
                if (tmp->neighbours[j]->visited == 1){
                    continue;
                }
                curPrice += recurMe(tmp->neighbours[j], i, beerPrice);
            }

            if (bestPrice == 0 || bestPrice > curPrice){
                bestPrice = curPrice;
                tmp->thisBeer = i;
            }
        }
        tmp->visited = 0;
        return bestPrice;
    }
}

void getMeThoseBeers(Node * tmp, vector<int> & beerPrice, int notThisBeer, int mainNode, int & fullPrice){
    int bestPrice = 0;
    int curPrice = 0;
    tmp->visited = 1;

    for (int i = 0; i < (int)beerPrice.size(); ++i){
        if (i == notThisBeer){
            continue;
        }

        curPrice = 0;
        curPrice += tmp->litersOfBeer*beerPrice[i];

        for (int j = 0; j < (int)tmp->neighbours.size(); ++j){
            if (tmp->neighbours[j]->visited == 1){
                continue;
            }
            curPrice += recurMe(tmp->neighbours[j], i, beerPrice);
        }

        if (bestPrice == 0 || bestPrice > curPrice){
            bestPrice = curPrice;
            tmp->thisBeer = i;
        }
    }

    for (int j = 0; j < (int)tmp->neighbours.size(); ++j){
        if (tmp->neighbours[j]->visited == 1){
            continue;
        }
        getMeThoseBeers(tmp->neighbours[j], beerPrice, tmp->thisBeer, 0, fullPrice);
    }    
    if (mainNode == 1){
        fullPrice = bestPrice;
    }
}

int main(void){
    int N = 0;
    int K = 0;
    string line;
    stringstream aa;

    // N and K input
    getline(cin, line);
    aa << line;

    aa >> N >> K;
    // endl of N and K input
    //---------------------------------  
    vector <int> beerPrice;
    
    getline(cin, line);
    stringstream cc(line);
    int tmpInt;
    for (int i = 0; i < K; ++i){
        cc >> tmpInt;
        beerPrice.push_back(tmpInt);
    }

    // end of reading the input for beer price
    //---------------------------------  
    // bars and pubs
    vector <Node*> barNodes;
    barNodes.reserve(N);
    for (int i = 0; i < N; ++i){
        barNodes.push_back(new Node);
    }

    getline(cin, line);
    
    stringstream bb(line);

    int liter = 0;

     for(int i = 0; i<N; ++i){
        bb >> liter;
        barNodes[i]->litersOfBeer = liter;
    }
    
    // end of bar input
    //---------------------------------

    // edges
    int oneNode, secondNode;
    for (int i = 1; i < N; ++i){
        cin >> oneNode >> secondNode;
        barNodes[oneNode-1]->neighbours.push_back(barNodes[secondNode-1]);
        barNodes[secondNode-1]->neighbours.push_back(barNodes[oneNode-1]);
    }

    int bestPrice = 0; 

    getMeThoseBeers(barNodes[0], beerPrice, -1, 1, bestPrice);

    cout << bestPrice << endl;
    string outputData = "";
    for (int i = 0; i < N; ++i){
        outputData += to_string(barNodes[i]->thisBeer+1);
        delete barNodes[i];
        outputData += " ";
    }
    cout << outputData.substr(0, outputData.length()-1) << endl;

    return 0;
}