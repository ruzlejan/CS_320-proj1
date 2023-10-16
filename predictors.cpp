#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <iomanip>



using namespace std;

//1)Always Taken function
pair<int,int> alwaysTakenAccuracy(const string& filename){
//Initialize the predictor to 1
int prediction = 1;

//Open filename and parse values
ifstream traceFile(filename);
//check validity of the file
if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 }; // Return a pair with both values set to -1 to indicate an error
    }

int totalBranchesCounter = 0;
int correctPredictionCounter = 0;

//parse each line from the input.txt
string line;
    while (getline(traceFile, line)) {
      istringstream iss(line);

        // Variables to store extracted values
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;
           if (iss >> programCounterHex >> actualOutcome >> targetAddressHex) {
          // unsigned long long programCounter = stoull(programCounterHex, nullptr, 16);
            //unsigned long long targetAddress = stoull(targetAddressHex, nullptr, 16);
        // Update statistics
        totalBranchesCounter++;
        if (prediction== (actualOutcome == "T" ? 1 : 0)) {
            correctPredictionCounter++;
        }
    
        }
    }
    traceFile.close();
    return{correctPredictionCounter,totalBranchesCounter};

}

//2)Always Non-Taken
pair<int,int> alwaysNonTakenAccuracy(const string& filename){
//Initialize the predictor to 1
int prediction = 0;

//Open filename and parse values
ifstream traceFile(filename);
//check validity of the file
if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 }; // Return a pair with both values set to -1 to indicate an error
    }

int totalBranchesCounter = 0;
int correctPredictionCounter = 0;

//parse each line from the input.txt
string line;
    while (getline(traceFile, line)) {
      istringstream iss(line);

        // Variables to store extracted values
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;
           if (iss >> programCounterHex >> actualOutcome >> targetAddressHex) {
          // unsigned long long programCounter = stoull(programCounterHex, nullptr, 16);
            //unsigned long long targetAddress = stoull(targetAddressHex, nullptr, 16);
        // Update statistics
        totalBranchesCounter++;
        if (prediction== (actualOutcome == "NT" ? 0 : 1)) {
            correctPredictionCounter++;
        }
    
        }
    }
    traceFile.close();
    return{correctPredictionCounter,totalBranchesCounter};

}

//3)Bimodal Predictor with a single bit of history
pair<int,int> oneBitBimodalPrediction(int size, string& filename){
  const int TABLE_SIZE = size;
  vector<int> predictionTable(TABLE_SIZE,1);

//Open filename and parse values
ifstream traceFile(filename);
//check validity of the file
if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 }; // Return a pair with both values set to -1 to indicate an error
    }

    int totalBranchesCounter = 0;
int correctPredictionCounter = 0;


string line;

    while (getline(traceFile, line)) {
      istringstream iss(line);

        // Variables to store extracted values
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;
           if (iss >> programCounterHex >> actualOutcome >> targetAddressHex) {
          // unsigned long long programCounter = stoull(programCounterHex, nullptr, 16);
            //unsigned long long targetAddress = stoull(targetAddressHex, nullptr, 16);
        // Update statistics


            unsigned long long programCounter = stoull(programCounterHex, nullptr, 16);
            int index = programCounter % TABLE_SIZE;

        totalBranchesCounter++;
        bool prediction = predictionTable[index] == 1;
        
        int actual;
        if(actualOutcome=="T"){
          actual = 1;
        }else{
          actual = 0;
        }
        
        if(prediction == actual){
          correctPredictionCounter++;
        }else{
       
          predictionTable[index] = actual;
        }
        
        }
       
    }
     traceFile.close();
    return{correctPredictionCounter,totalBranchesCounter};
}


//helper function to update prediction table and counting the number of branches
void updatePredictionTableAndCounter(int actual, int index, int &totalBranchesCounter, int &correctPredictionCounter,vector<int> &predictionTable){
   totalBranchesCounter++;
            if(actual == 1 && predictionTable[index] == 3){
              correctPredictionCounter++;
            }else if(actual == 0 && predictionTable[index] ==3 ){
              predictionTable[index] = 2;
            }else if(actual == 1 && predictionTable[index] ==2){
              correctPredictionCounter++;
              predictionTable[index] = 3;
            }else if(actual == 0 && predictionTable[index] == 2){
               predictionTable[index] = 1;
            }else if(actual == 1 && predictionTable[index] == 1){
              predictionTable[index] = 2;
            }else if(actual == 0 && predictionTable[index] == 1){
              predictionTable[index] = 0;
              correctPredictionCounter++;
            }else if(actual == 0 && predictionTable[index] == 0){
              correctPredictionCounter++;
            }else if(actual == 1 && predictionTable[index] == 0){
               predictionTable[index] = 1;
            }
}



//4)Bimodal Predictor with 2-bit saturating counters
pair<int,int> twoBitBimodalPrediction(int size, string& filename){
  const int TABLE_SIZE = size;
  vector<int> predictionTable(TABLE_SIZE,3);

//Open filename and parse values
ifstream traceFile(filename);
//check validity of the file
if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 }; // Return a pair with both values set to -1 to indicate an error
    }

    int totalBranchesCounter = 0;
int correctPredictionCounter = 0;


string line;
    while (getline(traceFile, line)) {
      istringstream iss(line);

        // Variables to store extracted values
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;
           if (iss >> programCounterHex >> actualOutcome >> targetAddressHex) {
          // unsigned long long programCounter = stoull(programCounterHex, nullptr, 16);
            //unsigned long long targetAddress = stoull(targetAddressHex, nullptr, 16);
        // Update statistics
          int numBitsToExtract = static_cast<int>(log2(TABLE_SIZE));
          unsigned int programCounter = stoull(programCounterHex, nullptr, 16);
          unsigned int bitmask = (1U << numBitsToExtract) - 1;
           unsigned int index = programCounter & bitmask;

            int actual;
            if(actualOutcome == "T"){
              actual = 1;
            }else{
              actual = 0;
            }

            //this is where you call the function
            updatePredictionTableAndCounter(actual,index,totalBranchesCounter,correctPredictionCounter,predictionTable);
        }
       
    }
 traceFile.close();
return{correctPredictionCounter,totalBranchesCounter};
}




//5)Gshare predictor
pair<int, int> gSharePredictor(int size, string& filename) {
    vector<int> predictionTable(2048, 3);
    int numBitsToExtract = static_cast<int>(log2(size));

    unsigned int GHR = 0;
    unsigned int ghr_bitmask = (1U << numBitsToExtract)-1;
    unsigned int pc_bitmask = (1U << 11)-1;
  

    // Open filename and parse values
    ifstream traceFile(filename);

    if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 }; // Return a pair with both values set to -1 to indicate an error
    }

    int totalBranchesCounter = 0;
    int correctPredictionCounter = 0;

    string line;

    while (getline(traceFile, line)) {
        istringstream iss(line);

        // Variables to store extracted values
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;

        if (iss >> programCounterHex >> actualOutcome >> targetAddressHex) {
            // Extracting the appropriate bits from the program counter
            unsigned int programCounter = stoull(programCounterHex, nullptr, 16);
            
            unsigned int pc_extractedBits = programCounter & ghr_bitmask;
            unsigned int ghr_extractedBits = GHR & ghr_bitmask;

            unsigned int init_index = pc_extractedBits ^ ghr_extractedBits;

            unsigned int pc_table = programCounter & pc_bitmask;

            
            // PC XOR GHR to determine the index
            unsigned int index = init_index|((pc_table >> numBitsToExtract) << numBitsToExtract);
              
        

            int actual;
            if (actualOutcome == "T") {
                actual = 1;
            } else {
                actual = 0;
            }

            updatePredictionTableAndCounter(actual,index,totalBranchesCounter,correctPredictionCounter,predictionTable);
            
            GHR = (GHR << 1) | actual;
           
        }
         
    }

    traceFile.close();
    return {correctPredictionCounter,totalBranchesCounter };
}


//6 tournament prediction
pair<int,int> tournamentPredictor(string& filename){
vector<int> gShareTable(2048,3);
vector<int> bimodalTable(2048,3);
//initialize values to "Prefer Gshare"(0)
vector<int> selectorTable(2048,0);  

//initialize GHR to 0
unsigned int GHR = 0;

    ifstream traceFile(filename);

    if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 }; // Return a pair with both values set to -1 to indicate an error
    }

     int totalBranchesCounter = 0;
    int correctPredictionCounter = 0;

    string line;

     while (getline(traceFile, line)) {
        istringstream iss(line);
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;

          if (iss >> programCounterHex >> actualOutcome >> targetAddressHex){
            totalBranchesCounter++;
            unsigned int programCounter = stoull(programCounterHex, nullptr, 16);
          //extract bimodal and selector index
          int index = programCounter %2048;
          
            int gshareIndex = (programCounter ^ GHR) % 2048;
            bool gsharePredict = gShareTable[gshareIndex] >= 2;

            bool bimodalPredict =  bimodalTable[index] >= 2;

             bool finalPredict = (selectorTable[index] < 2) ? gsharePredict : bimodalPredict;

            if((finalPredict && actualOutcome == "T") || (!finalPredict && actualOutcome == "NT")) {
            correctPredictionCounter++;
        }

        GHR = ((GHR << 1) | (actualOutcome == "T" ? 1 : 0)) & 0x7FF; // Keep only 11 bits


       if(actualOutcome == "T" && gShareTable[gshareIndex] < 3) {
            gShareTable[gshareIndex]++;
        } else if(actualOutcome == "NT" && gShareTable[gshareIndex] > 0) {
            gShareTable[gshareIndex]--;
        }


        if(actualOutcome == "T" && bimodalTable[index] < 3) {
            bimodalTable[index]++;
        } else if(actualOutcome == "NT" && bimodalTable[index] > 0) {
            bimodalTable[index]--;
        }



         if(gsharePredict != bimodalPredict) {
            if(gsharePredict == (actualOutcome == "T") && selectorTable[index] > 0) {
                selectorTable[index]--;
            } else if(bimodalPredict == (actualOutcome == "T") && selectorTable[index] < 3) {
                selectorTable[index]++;
            }
        }

}
}

traceFile.close();    
return {correctPredictionCounter,totalBranchesCounter };

}






//7)BTB
pair<int, int> btbPrediction(const string& filename) {
    vector<int> predictionTable(512, 1);  // One bit bimodal predictor
    vector<unsigned long long> BTB(512,0);
    unsigned int bitmask = (1U << 9) - 1;

    // Open filename and parse values
    ifstream traceFile(filename);

    if (!traceFile) {
        cerr << "Error opening trace file!" << endl;
        return { -1, -1 };
    }

    int correctBTBPrediction = 0;
    int totalAttemptedPredictions = 0;

    string line;

    while (getline(traceFile, line)) {
        istringstream iss(line);

        // Variables to store extracted values
        string programCounterHex;
        string actualOutcome;
        string targetAddressHex;

        if (iss >> programCounterHex >> actualOutcome >> targetAddressHex) {
            unsigned int programCounter = stoul(programCounterHex, nullptr, 16);
            unsigned int actualTargetAddress = stoul(targetAddressHex, nullptr, 16);

               int index = programCounter&bitmask;

           
              if(predictionTable[index] ==1){
                totalAttemptedPredictions++;
                if(BTB[index] != 0 && BTB[index] == actualTargetAddress){
                  correctBTBPrediction++;
                }
              }    

              if(actualOutcome == "T"){
                predictionTable[index] = 1;
                BTB[index] = actualTargetAddress;
              }  else{
                predictionTable[index] = 0;
              }
                   
            
        }
    }

    traceFile.close();
    return { correctBTBPrediction, totalAttemptedPredictions };
}


int main(int argc, char *argv[]) {
    
    string inputFileName = argv[1];
    string outputFileName = argv[2];

    //running 1)Always taken
   pair<int,int> alwaysTakenResult = alwaysTakenAccuracy(inputFileName);
    //running 2) Always Non-Taken
   pair<int,int> alwaysNonTakenResult = alwaysNonTakenAccuracy(inputFileName);
    //running 3)Bimodal Predictor with a single bit of history
  pair<int,int> bimodal_16 = oneBitBimodalPrediction(16,inputFileName);
  pair<int,int> bimodal_32 = oneBitBimodalPrediction(32,inputFileName);
  pair<int,int> bimodal_128 = oneBitBimodalPrediction(128,inputFileName);
  pair<int,int> bimodal_256 = oneBitBimodalPrediction(256,inputFileName);
  pair<int,int> bimodal_512 = oneBitBimodalPrediction(512,inputFileName);
  pair<int,int> bimodal_1024 = oneBitBimodalPrediction(1024,inputFileName);
  pair<int,int> bimodal_2048 = oneBitBimodalPrediction(2048,inputFileName);
    //runnning 4) Bimodal Predictor with 2-bit saturating counters
pair<int,int> bimodal2_16 = twoBitBimodalPrediction(16,inputFileName);
pair<int,int> bimodal2_32 = twoBitBimodalPrediction(32,inputFileName);
pair<int,int> bimodal2_128 = twoBitBimodalPrediction(128,inputFileName);
pair<int,int> bimodal2_256= twoBitBimodalPrediction(256,inputFileName);
pair<int,int> bimodal2_512 = twoBitBimodalPrediction(512,inputFileName);
pair<int,int> bimodal2_1024 = twoBitBimodalPrediction(1024,inputFileName);
pair<int,int> bimodal2_2048 = twoBitBimodalPrediction(2048,inputFileName);
    //running 5)Gshare Predictor
    pair<int,int> gshare_8 = gSharePredictor(8,inputFileName);
    pair<int,int> gshare_16 = gSharePredictor(16,inputFileName);
    pair<int,int> gshare_32 = gSharePredictor(32,inputFileName);
    pair<int,int> gshare_64 = gSharePredictor(64,inputFileName);
    pair<int,int> gshare_128 = gSharePredictor(128,inputFileName);
    pair<int,int> gshare_256 = gSharePredictor(256,inputFileName);
    pair<int,int> gshare_512 = gSharePredictor(512,inputFileName);
    pair<int,int> gshare_1024 = gSharePredictor(1024,inputFileName);
    pair<int,int> gshare_2048 = gSharePredictor(2048,inputFileName);

    //running 6)Tournament Predictor
    pair<int,int> tournamentPred = tournamentPredictor(inputFileName);

    //running 7)Branch Target Predictor
     pair<int,int> btbPred = btbPrediction(inputFileName);


  ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cout << "Error: Could not open output file." << endl;
        return 1;
    }

      outputFile << alwaysTakenResult.first << "," << alwaysTakenResult.second << ";"<< endl;
      outputFile << alwaysNonTakenResult.first << "," << alwaysNonTakenResult.second << ";"<<endl;

      outputFile << bimodal_16.first << "," << bimodal_16.second<< "; ";
      outputFile << bimodal_32.first << "," << bimodal_32.second<< "; ";
      outputFile << bimodal_128.first << "," << bimodal_128.second<< "; ";
      outputFile << bimodal_256.first << "," << bimodal_256.second<< "; ";
      outputFile << bimodal_512.first << "," << bimodal_512.second<< "; ";
      outputFile << bimodal_1024.first << "," << bimodal_1024.second<< "; ";
      outputFile << bimodal_2048.first << "," << bimodal_2048.second<< "; " << endl;


      outputFile << bimodal2_16.first << "," << bimodal2_16.second<< "; ";
      outputFile << bimodal2_32.first << "," << bimodal2_32.second<< "; ";
      outputFile << bimodal2_128.first << "," << bimodal2_128.second<< "; ";
      outputFile << bimodal2_256.first << "," << bimodal2_256.second<< "; ";
      outputFile << bimodal2_512.first << "," << bimodal2_512.second<< "; ";
      outputFile << bimodal2_1024.first << "," << bimodal2_1024.second<< "; ";
      outputFile << bimodal2_2048.first << "," << bimodal2_2048.second<< "; " << endl;


      outputFile << gshare_8.first << "," << gshare_8.second << "; " ;
     outputFile << gshare_16.first << "," << gshare_16.second << "; " ;
     outputFile << gshare_32.first << "," << gshare_32.second << "; " ;
      outputFile << gshare_64.first << "," << gshare_64.second << "; " ;
     outputFile << gshare_128.first << "," << gshare_128.second << "; " ;
     outputFile << gshare_256.first << "," << gshare_256.second << "; " ;
     outputFile << gshare_512.first << "," << gshare_512.second << "; " ;
     outputFile << gshare_1024.first << "," << gshare_1024.second << "; " ;
     outputFile << gshare_2048.first << "," << gshare_2048.second << "; " << endl;

     outputFile << tournamentPred.first << "," << tournamentPred.second << "; " << endl;

    outputFile << btbPred.first << "," << btbPred.second << "; " <<endl;


}
