#include <iostream>
#include <string>
#include <queue>

using namespace std;

void createX(int i, string & someQueue){
  someQueue[i] = 'x';
}

int main(){
  int length = 0;
  int jumpNum = 0;
  int myPosition = 0;
  int time = 0;
  int i = 0;
  string leftQueue = "";
  string rightQueue = "";
  queue<int> positionQueue;
  queue<int> timeQueue;
  cin >> length >> jumpNum >> leftQueue >> rightQueue;
  positionQueue.push(myPosition);
  timeQueue.push(0);
  while(!positionQueue.empty()){
    time = timeQueue.front();
    timeQueue.pop();
    myPosition = positionQueue.front();
    positionQueue.pop();
    i = (myPosition<0)?(-myPosition):(myPosition);
    if ( i + 1 >= length || i + jumpNum >= length ){
      cout << ++time << endl;
      return 0;
    }
    else{
      string & currentQueue = (myPosition<0)?(rightQueue):(leftQueue);
      string & otherQueue = (myPosition<0)?(leftQueue):(rightQueue);
      createX(i, currentQueue);
      if ( i-1 >= 0 && currentQueue[i-1] != 'x' && (i != time && i != time+1)){
        positionQueue.push((myPosition<0)?(-i+1):(i-1));
        timeQueue.push(time+1);
        createX(i-1, currentQueue);
      }
      if ( currentQueue[i+1] != 'x' ){
        positionQueue.push((myPosition<0)?(-i-1):(i+1));
        timeQueue.push(time+1);
        createX(i+1, currentQueue);
      }
      if ( otherQueue[i+jumpNum]!= 'x' ){
        positionQueue.push( (myPosition<0)?(i+jumpNum):(-i-jumpNum) );
        timeQueue.push(time+1);
        createX(i+jumpNum, otherQueue);
      }
    }
  }
  cout << "-1" << endl;
  return 0;
}
