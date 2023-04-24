/*********************************/
/*Author: William Youse          */
/*Main file: diskScheduler       */
/*Arguments: 1 (Int)             */
/*                               */
/*Purpose: Simulate surface-level*/
/*-analysis of disk header       */
/*-scheduling algorithms         */
/*                               */
/*Date last Modfied: 23/04/2023  */
/*********************************/

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#define MAX_SIZE 1000;

using namespace std;

vector<int> diskHeader;
vector<int> leftVec, rightVec; //split the vector into two different vectors

int randInt;
int fcfsSum = 0, scanSum = 0, cscanSum = 0;
int head = 0;
int useHead = 0;
int currentVal = 0;
int flip = 2;
int tempCount = 0;

string direction;


/*********************************/
/*Function: randomGen            */
/*Purpose: Handle generating the */
/*-random numbers for vector     */
/*-diskHeader                    */
/*                               */
/*Return Type: void              */
/*********************************/

void randomGen() {

    srand(time(0)); //seed the random number gen with current time, so MORE random! (Was generating the same numbers over and over)

    for (int i = 0; i < MAX_SIZE i++) { //actual number generation
        randInt = rand() % 5000;
        diskHeader.push_back(randInt);
    }

}

int main(int argc, char* argv[]) {
    
    head = atoi(argv[1]); //a (relatively messy) conversion of the argv for the start header
    useHead = head; //copying head for later usage in other functions.

    randomGen(); //Call the generator

    //FCFS (First come, first serve) - Simplist of the three, request in order of what comes first (N comparisons, where N = array size)
    
    for (int i = 0; i < diskHeader.size(); i++) {
        currentVal = diskHeader[i]; //grab the current value at i
        fcfsSum += abs(currentVal - useHead); //do the math; the current value - the head value;
        useHead = currentVal; //update head value to previous value for comparison
    }

    //SCAN SETUP AREA -- PREPARE VECTORS FOR SCAN
    sort(diskHeader.begin(), diskHeader.end());

    leftVec.push_back(0); //adding 0 since it needs to be checked now

    for (int i = 0; i < diskHeader.size(); i++) { //split the arrays into left and right
        if (diskHeader[i] < head) {
            leftVec.push_back(diskHeader[i]);
        }

        if (diskHeader[i] > head) {
            rightVec.push_back(diskHeader[i]);
        }
    }

    //sort the two sides now.
    sort(leftVec.begin(), leftVec.end());
    sort(rightVec.begin(), rightVec.end());

    //SCAN "Elevator algorithm" - (N + 1 comparisons, where N = array size)
    useHead = head; //make sure to copy value again
    direction = "left"; //we start by going left of the array

    while (flip--) {
        if (direction == "left") {
            for (int i = leftVec.size() - 1; i >= 0; i--) {
                currentVal = leftVec[i];
                scanSum += abs(currentVal - useHead);
                useHead = currentVal;
            }
            direction = "right";
        } else if (direction == "right") {
            for (int i = 0; i < rightVec.size(); i++) {
                currentVal = rightVec[i];
                scanSum += abs(currentVal - useHead);
                useHead = currentVal;
            }
            direction = "left";
        }
    }

    //C-SCAN "Circular Elevator Algorithm" - (N + 2 comparisons, where N = array size)

    useHead = head; //make sure to copy value again
    //we have to resort the right array here...
    rightVec.push_back(199);
    sort(rightVec.begin(), rightVec.end());

    for (int i = 0; i < rightVec.size(); i++) {
        currentVal = rightVec[i];
        cscanSum += abs(currentVal - useHead);
        useHead = currentVal;
    }

    head = 0;

    for (int i = 0; i < leftVec.size(); i++) {
        currentVal = leftVec[i];
        cscanSum += abs(currentVal - useHead);
        useHead = currentVal;
    }

    //Output control - Just start printing the outputs here.
    
    cout << "  FCFS = " << fcfsSum << endl;
    cout << "  SCAN = " << scanSum << endl;
    cout << "C-SCAN = " << cscanSum << endl;

    return 0;
}