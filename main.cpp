#include <iostream>
#include <vector>
//#include <list>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>

typedef unsigned long long int bignumber;
typedef unsigned long int power;
typedef unsigned long int index_number;
#define maxlimit64 18446744073709551615
#define maxlimit32 4294967295

struct nodeKey{
    bignumber Xn =0;
    power powerCounter = 0;
    power powerOfTwo = 0;
    bignumber Xn1 = 0;
    bool overflowed32 = false;
    bool overflowed64 = false;

    friend std::ostream& operator<<(std::ostream &rhs, const nodeKey abc) {
        rhs << abc.Xn << "," << abc.powerCounter << "," << abc.powerOfTwo << "," << abc.Xn1 << "," << abc.overflowed32 << "," << abc.overflowed64 << ",";
        return rhs;
    }
};
//version_2: remove nodeKey as it is not needed
std::string headerNodeKey(const nodeKey abc, const std::string label){
    std::string output;
    output += label + "_Xn,";
    output += label + "_PCounter,";
    output += label + "_POT,";
    output += label + "_Xn+1,";
    output += label + "_Overflow32,";
    output += label + "_overflowed64,";
    return output;
}

/*might be better to add all the candidates here and use
 * switches to determine if they have been solved
 * *steps to the current value
 * *steps to the next value
 * *next value
 * *is a loop
 */
struct node{
    //*version_2: store current node, and prior candidate, plus root candidate
    //*version_2: add switches to better manage when to do things
    nodeKey current;
    nodeKey Minus1;
    nodeKey Zero; //used to determine if the branch is connected to the root
    index_number distTotal = 0;
    index_number distEven = 0;
    index_number distOdd = 0;
    //bool overflowed = false;
    bool saved = false;

    friend std::ostream& operator<<(std::ostream &rhs, const node abc) {
        rhs
            << abc.current
            << abc.Minus1
            << abc.Zero
            << abc.distTotal << ","
            << abc.distOdd << ","
            << abc.distEven << ",";
            return rhs;
    }

};
std::string headerNode(const node abc, const std::string label){
        std::string output;
        output += headerNodeKey(abc.current,label +"_Current");
        output += headerNodeKey(abc.Minus1,label +"_Prior");
        output += headerNodeKey(abc.Zero,label +"_Root");
        output += label + "_distTotal,";
        output += label + "_distOdd,";
        output += label + "_distEven,";
        return output;
}


void fillInNode(std::pair<bignumber,power> Candidate);


/* each value has two parts to it's key
 * The integer Xn
 * The Count of the powers of two to find the next Xn
 */
std::map<std::pair<bignumber,index_number>, node> foundMap;

//Solve for Xn+1
//*version_2: create and operate directly on node
node SolveForXn1(bignumber Xn, power powerCounter) {
    node output;
    output.current.Xn = Xn;
    if(Xn%3==0) {
        return output;
    }
    output.current.powerCounter = powerCounter;
    output.current.powerOfTwo = 2*powerCounter-1+Xn%3;
    output.current.Xn1 = (Xn*pow(2,(2*powerCounter-1+Xn%3))-1)/3;
    //if Xn1 overflowed, then flag it and set to zero
    if(output.current.Xn1%2==0) {
        output.current.overflowed64 = true;
        output.current.overflowed32 = true;
        output.current.Xn1 = 0;
    }
    return output;
}

bignumber power10(bignumber input){
    bignumber output =1;
    for(bignumber counter =0; counter < input; counter++) {
        output *= 10;
    }
    return output;
}

//std library does not convert strings to big integers
//Supporting the conversion here
bignumber stob(std::string input) {
    bignumber output = 0;
    index_number inputlength = input.length();
    for(index_number position = 0; position < inputlength; position++) {
        if(input.substr(inputlength - position-1,1)=="0") {
                continue;
        } else if(input.substr(inputlength - position-1,1)=="1") {
                output += power10(position);
        } else if(input.substr(inputlength - position-1,1)=="2") {
                output += power10(position)*2;
        } else if(input.substr(inputlength - position-1,1)=="3") {
                output += power10(position)*3;
        } else if(input.substr(inputlength - position-1,1)=="4") {
                output += power10(position)*4;
        } else if(input.substr(inputlength - position-1,1)=="5") {
                output += power10(position)*5;
        } else if(input.substr(inputlength - position-1,1)=="6") {
                output += power10(position)*6;
        } else if(input.substr(inputlength - position-1,1)=="7") {
                output += power10(position)*7;
        } else if(input.substr(inputlength - position-1,1)=="8") {
                output += power10(position)*8;
        } else if(input.substr(inputlength - position-1,1)=="9") {
                output += power10(position)*9;
        } else {
                output = 0;
        }
    }
    return output;
}
std::string getNextCSVString(std::string line, index_number startOfValue) {
    index_number length = line.find(",",startOfValue);
    if(length>startOfValue){
        length=length-startOfValue;
    }
    std::string output = line.substr(startOfValue,length);
    return output;
}

void loadFoundXn(void){
    std::string fileLine;
    node tempNode;
    nodeKey tempNodeKey;
    unsigned int column1 = 0;
    unsigned int length = 0;
    std::string tempspot;

    std::ifstream loadfile("SavedNodes.csv", std::ios::in);
    if(!loadfile) {
        loadfile.close();
        //write header line
        std::ofstream savefile("SavedNodes.csv", std::ios::out);
        savefile << headerNode(foundMap.begin()->second, "") <<'\n';
        savefile.close();
    } else {
        //The first line is the header, skip it
        std::getline(loadfile,fileLine);
        //Get each line after that
        while(std::getline(loadfile,fileLine)) {
            if(fileLine=="") {
                continue;
            }
            column1 = 0;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.Xn = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.powerCounter = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.powerOfTwo = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.Xn1 = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.overflowed32 = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.overflowed64 = stob(tempspot);
            tempNode.current = tempNodeKey;

            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.Xn = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.powerCounter = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.powerOfTwo = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.Xn1 = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.overflowed32 = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.overflowed64 = stob(tempspot);
            tempNode.Minus1 = tempNodeKey;


            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.Xn = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.powerCounter = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.powerOfTwo = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.Xn1 = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.overflowed32 = stob(tempspot);
            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNodeKey.overflowed64 = stob(tempspot);
            tempNode.Zero = tempNodeKey;

            column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNode.distTotal= stob(tempspot);
             column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNode.distOdd = stob(tempspot);
             column1 += tempspot.length() +1;
            tempspot = getNextCSVString(fileLine,column1);
            tempNode.distEven = stob(tempspot);
            tempNode.saved = true;

            if(tempNode.current.Xn !=0) {
                foundMap[std::make_pair(tempNode.current.Xn,tempNode.current.powerCounter)] = tempNode;
            }
            fileLine="";
        }
    }

}

void saveMapToFile() {
    std::ofstream savefile("SavedNodes.csv", std::ios::app);
    //write the header in the load file function
    for(std::map<std::pair<bignumber,index_number>,node>::iterator it = foundMap.begin();it !=foundMap.end(); ++it) {
        if(!it->second.saved){
            savefile <<  it->second << '\n';
        }
    }
    savefile.close();
}

void printMap() {
    std::cout << headerNode(foundMap.begin()->second,"") << "\n";
    for(std::map<std::pair<bignumber,index_number>,node>::iterator it = foundMap.begin();it !=foundMap.end(); ++it) {
        std::cout << it-> second << '\n';
    }
}

void findPriorX(std::pair<bignumber, power> candidate) {
    bignumber tempX =0;
    power tempPow = 0;
    std::pair<bignumber,power> tempMinus1Pair;

    //loop at Xn ==1, means it needs to be set explicitly
    if(candidate.first==1) {
        //could check before setting, but what is faster?
        foundMap[candidate].Minus1.Xn = 1;
        foundMap[candidate].Minus1.powerCounter = 0;
        foundMap[candidate].Minus1.powerOfTwo = 0;
        foundMap[candidate].Minus1.Xn1 = 1;
        foundMap[candidate].Minus1.overflowed32 = false;
        foundMap[candidate].Minus1.overflowed64 = false;
        return void();
    }

    //calculate the rest
    if(foundMap[candidate].Minus1.Xn == 0 && !foundMap[candidate].Minus1.overflowed64) {
        //unknown previous, calculate the prior candidate and then call recursive formulas
        tempX = 3*candidate.first+1;
        while(tempX%2==0) {
            tempPow++;
            tempX=tempX/2;
        }
        if(tempPow == 0) {
            //minus1 candidate oveflowed, set to zero
            std::cout << "overflow of prior node\n";
            foundMap[candidate].Minus1.overflowed64 = true;
            foundMap[candidate].Minus1.Xn = 0;
            foundMap[candidate].Minus1.Xn1 = candidate.first;
            foundMap[candidate].Minus1.powerCounter = 0;
            foundMap[candidate].Minus1.powerOfTwo = 0;
            foundMap[candidate].Minus1.overflowed32 = true;
            return void();
        } else {
            //need to find the pair
            //not the most efficient way to find tempCounter, but % seems to change values when subtracted
            //likely an order of operations math error
            for(power tempCounter = 0; tempCounter<64;tempCounter++){
                if(tempPow == (2*tempCounter)-1+tempX%3) {
                    tempMinus1Pair = std::make_pair(tempX,tempCounter);
                    break;
                }
            }
            //checking before calling the function to speed code, but may be faster the other way
            if(foundMap.count(tempMinus1Pair)==0 && tempMinus1Pair.first!=0) {
                //minus1 candidate is unknown, recursive find it.
                fillInNode(tempMinus1Pair);
            }
            //set the minus1 value
            foundMap[candidate].Minus1.overflowed64 = foundMap[tempMinus1Pair].current.overflowed64;
            foundMap[candidate].Minus1.Xn = foundMap[tempMinus1Pair].current.Xn;
            foundMap[candidate].Minus1.powerCounter = foundMap[tempMinus1Pair].current.powerCounter;
            foundMap[candidate].Minus1.powerOfTwo = foundMap[tempMinus1Pair].current.powerOfTwo;
            foundMap[candidate].Minus1.Xn1 = foundMap[tempMinus1Pair].current.Xn1;
            foundMap[candidate].Minus1.overflowed32 = foundMap[tempMinus1Pair].current.overflowed32;
        }
    } else {
        std::cout << "already found prior\n";
    }

}

void findRootX(std::pair<bignumber, power> candidate) {
    bignumber tempX =0;
    power tempPow = 0;
    std::pair<bignumber,power> tempRootPair;

    if(foundMap[candidate].Zero.Xn == 0 ) {
        if(foundMap[candidate].Minus1.overflowed64) {
            //overflowed
            foundMap[candidate].Zero.Xn = 0;
            foundMap[candidate].Zero.powerCounter = 0;
            foundMap[candidate].Zero.powerOfTwo = 0;
            foundMap[candidate].Zero.Xn1 = candidate.first;
            foundMap[candidate].Zero.overflowed32 = true;
            foundMap[candidate].Zero.overflowed64 = true;
            return void();

        } else if (foundMap[candidate].Minus1.Xn == 1 ) {
            //set root node to zero
            foundMap[candidate].Zero.Xn = 1;
            foundMap[candidate].Zero.powerCounter = 0;
            foundMap[candidate].Zero.powerOfTwo = 0;
            foundMap[candidate].Zero.Xn1 = 2;
            foundMap[candidate].Zero.overflowed32 = false;
            foundMap[candidate].Zero.overflowed64 = false;
            return void();
        } else {
            tempX = 3*foundMap[candidate].Minus1.Xn+1;
            while(tempX%2==0) {
                tempPow++;
                tempX=tempX/2;
            }
            if(tempPow == 0) {
                //set zero to unkown overflow
                foundMap[candidate].Zero.Xn = 0;
                foundMap[candidate].Zero.powerCounter = 0;
                foundMap[candidate].Zero.powerOfTwo = 0;
                foundMap[candidate].Zero.Xn1 = candidate.first;
                foundMap[candidate].Zero.overflowed32 = true;
                foundMap[candidate].Zero.overflowed64 = true;

                return void();
            } else {
                //need to find the pair
                for(power tempCounter = 0; tempCounter<64;tempCounter++){
                    if(tempPow == (2*tempCounter)-1+tempX%3) {
                        tempRootPair = std::make_pair(tempX,tempCounter);
                        break;
                    }
                }
            }
        }
    //calculate prior to minus1 and look it up
    } else if(foundMap[candidate].Zero.Xn != 1) {
        //check to see if zero candidate has a prior value that is not overflowed
        tempRootPair = std::make_pair(foundMap[candidate].Zero.Xn, foundMap[candidate].Zero.powerCounter);

    } else {
        //must be root node of 1, verify
        if(!(foundMap[candidate].Zero.Xn1==1)) {
                        //set root node to zero
            foundMap[candidate].Zero.Xn = 1;
            foundMap[candidate].Zero.powerCounter = 0;
            foundMap[candidate].Zero.powerOfTwo = 0;
            foundMap[candidate].Zero.Xn1 = 1;
            foundMap[candidate].Zero.overflowed32 = false;
            foundMap[candidate].Zero.overflowed64 = false;
            return void();
        }
    }

    //now that the zeroth candidate is found, copy it
    //if root candidate.zero.overflowed, try minus
    //if [rootCandidate].minus1.overflowed, use current
    if(foundMap[tempRootPair].Zero.overflowed64) {
        if(foundMap[tempRootPair].Minus1.overflowed64) {
            foundMap[candidate].Zero.Xn = foundMap[tempRootPair].current.Xn;
            foundMap[candidate].Zero.powerCounter = foundMap[tempRootPair].current.powerCounter;
            foundMap[candidate].Zero.powerOfTwo = foundMap[tempRootPair].current.powerOfTwo;
            foundMap[candidate].Zero.Xn1 = foundMap[tempRootPair].current.Xn1;
            foundMap[candidate].Zero.overflowed32 = foundMap[tempRootPair].current.overflowed32;
            foundMap[candidate].Zero.overflowed64 = foundMap[tempRootPair].current.overflowed64;
            return void();
        } else {
            foundMap[candidate].Zero.Xn = foundMap[tempRootPair].Minus1.Xn;
            foundMap[candidate].Zero.powerCounter = foundMap[tempRootPair].Minus1.powerCounter;
            foundMap[candidate].Zero.powerOfTwo = foundMap[tempRootPair].Minus1.powerOfTwo;
            foundMap[candidate].Zero.Xn1 = foundMap[tempRootPair].Minus1.Xn1;
            foundMap[candidate].Zero.overflowed32 = foundMap[tempRootPair].Minus1.overflowed32;
            foundMap[candidate].Zero.overflowed64 = foundMap[tempRootPair].Minus1.overflowed64;
            return void();
        }

    } else {
        foundMap[candidate].Zero.Xn = foundMap[tempRootPair].Zero.Xn;
        foundMap[candidate].Zero.powerCounter = foundMap[tempRootPair].Zero.powerCounter;
        foundMap[candidate].Zero.powerOfTwo = foundMap[tempRootPair].Zero.powerOfTwo;
        foundMap[candidate].Zero.Xn1 = foundMap[tempRootPair].Zero.Xn1;
        foundMap[candidate].Zero.overflowed32 = foundMap[tempRootPair].Zero.overflowed32;
        foundMap[candidate].Zero.overflowed64 = foundMap[tempRootPair].Zero.overflowed64;
        return void();
    }
    return void();


}

void calculateDistance(std::pair<bignumber, power> candidate) {
    //does this function need to verify the find functions have een cl
    bignumber tempdistOdd = 0;
    bignumber tempdistEven = 0;
    std::pair<bignumber,power> tempMinus1lookup;
    if(!foundMap[candidate].Minus1.overflowed64 && foundMap[candidate].Minus1.powerOfTwo!=0) {


        tempMinus1lookup = std::make_pair(foundMap[candidate].Minus1.Xn, foundMap[candidate].Minus1.powerCounter);
        if(foundMap[tempMinus1lookup].distTotal==0) {
            //prior distances need to be calculated
            calculateDistance(tempMinus1lookup);
        }
        tempdistOdd = foundMap[tempMinus1lookup].distOdd;
        tempdistEven = foundMap[tempMinus1lookup].distEven;
    }
    if(foundMap[candidate].current.Xn==1) {
        foundMap[candidate].distOdd = 0;
    } else {
            foundMap[candidate].distOdd = 1 + tempdistOdd;
    }

    foundMap[candidate].distEven = foundMap[candidate].current.powerOfTwo + tempdistEven;
    foundMap[candidate].distTotal = foundMap[candidate].distOdd + foundMap[candidate].distEven;
}

//maybe just move keys and leave the nodes as lookups
void FillInHistory(std::pair<bignumber,power> candidate) {
    findPriorX(candidate);
    findRootX(candidate);
    calculateDistance(candidate);

}

void fillInNode(std::pair<bignumber,power> Candidate) {
    if(Candidate.first!=0) {
        if(foundMap.count(Candidate)==0){
            foundMap[Candidate]=SolveForXn1(Candidate.first,Candidate.second);
            FillInHistory(Candidate);
        }
    }

}

std::pair<bignumber,power> findCandidate(bignumber limitXn, power limitPow) {
    bignumber output_Xn=1;
    power output_power = 1;
    std::pair<bignumber, power> candidatePair;
    // couild include zero
    for(output_Xn=1; output_Xn <= limitXn; output_Xn+=2) {
        //formula for the power of two is off from the counter for Xn mod3==2
        //the following output_power is a temporary fix
        if(output_Xn%3==1) {
            output_power = 1;
        } else {
            output_power=0;
        }
        for(; output_power < limitPow; output_power++) {
            candidatePair = std::make_pair(output_Xn,output_power);
            //if multiple of 3, just do output_power of 0
            //if previous output_power causes a overflow, then break
            if(foundMap.count(candidatePair)>0) {
                continue;
            } else if((output_power!=0 && output_Xn%3==0) || (output_power>1 && foundMap[std::make_pair(output_Xn,output_power-1)].current.overflowed64)) {
                break;
            }else if(foundMap.count(candidatePair)==0) {
                //didn't find this pair, return as a candidate
                return candidatePair;
            } else {
                continue;
            }
        }
    }

    //this returns the limits and should be taken as there is nothing left
    return std::make_pair(output_Xn, output_power);
}

int main(int argc, char **argv) {
    //needs to clean up the commandline structure
    bignumber AdditionalXnCandidates = 10;
    power twosPowerLimit = 1;
    bignumber XnMaxLimit = 1;
    if(argc < 3) {
        //requires two commandline arguements
        return 1;
    } else {
        AdditionalXnCandidates= stob(argv[1]);
        twosPowerLimit = stob(argv[2]);
        if(argc < 4){
            XnMaxLimit= 99999;
        } else {
            XnMaxLimit = stob(argv[3]);
        }
    }

    //setup problem
    //load known nodes from file
    std::pair<bignumber,power> Candidate;
    //std::map<std::pair<bignumber,power>,node> Candidates;
    loadFoundXn();
    for(index_number count = 0; count < AdditionalXnCandidates; count ++) {
        Candidate = findCandidate(XnMaxLimit, twosPowerLimit);
        fillInNode(Candidate);
    }

    saveMapToFile();
    printMap();
    //saveNodesToFile(foundNodes);
    std::cout << "Hello there" << std::endl;
    return 0;
}
