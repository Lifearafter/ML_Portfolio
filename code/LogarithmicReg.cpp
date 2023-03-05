#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include<math.h>
#include  <bits/stdc++.h>
#include <cmath>
#include <chrono>
using namespace std;





double sigmoid ( double z, double e){
    z = -1*z;
    return 1/(1 + (pow(e, z)));
}


int main( int argc, char ** argv)
{
    auto start = std::chrono::high_resolution_clock::now();

    ifstream inFS;
    string line;
    string id_in, pclass_in,survived_in,sex_in, age_in;
    const int MAX_LEN= 2000;
  
    vector <double> testSexV (MAX_LEN);
     vector <double> testSurV (MAX_LEN);
    vector <double> trainSexV (MAX_LEN);
    vector <double> trainSurV (MAX_LEN);

    vector<vector<double>> m1;
    vector<vector<double>> m2;


    
    vector <double> weight;
    vector <double> err;
   

    weight.push_back(1);
    weight.push_back(1);
    
    double learningRate = 0.0001;

    cout<<"Openning file titanic_project.csv. "<<endl;

    inFS.open("titanic_project.csv");
    if(!inFS.is_open()) {
        cout<<" Could not open file."<<endl;
        return 1;
    }


    cout<<"Reading line 1" <<endl;
    getline(inFS,line);

    cout<<"heading: "<<line<<endl;
    int numObservations= 0;

    while(inFS.good()) { 



        getline(inFS,id_in, ',');
        getline(inFS,pclass_in, ','); 
        getline(inFS,survived_in, ',');
        getline(inFS,sex_in, ','); 
        getline(inFS,age_in, '\n');


      //Take the first 800 to be in test vector
      if ( numObservations < 800)
        {
            trainSurV.at(numObservations) = stof(survived_in);
            trainSexV.at(numObservations) = stof(sex_in);
        }

        // take the rest of the observations in as testV
          int i = 0;
       if ( (numObservations >= 800) && (numObservations < 1046)&& i <246 )
        {
          testSexV.at(i) = stof(sex_in); 
          testSurV.at(i) = stof(sex_in);
          i++;
        }


           numObservations++;


        if(numObservations ==1046)
        {
            break;
        } 
    }


    trainSexV.resize(800);
    trainSurV.resize(800);
    testSexV.resize(246);
    testSurV.resize(246);
    

// Push to matrices
// create temp vectors, w0
vector < double> temp1;

int j= 1.0;
for ( int i = 0; i< 800; i++)
{
    temp1.push_back(j);
}
m1.push_back(temp1);
m1.push_back(trainSexV);
m2.push_back(temp1);
m2.push_back(testSexV);

/***************testing for matrices m1 m2***************/
/*
for ( int j = 0; j< 2;j++){
    for ( int i = 700; i< trainSexV.size(); i++)
    {
        cout<<m1[j][i]<<endl;
    }
}
 cout<<" m2 " <<endl;
 for ( int j = 700; j< 2; j++) {
    for ( int i = 0; i< trainSexV.size(); i++)
    {
        cout<<m2[j][i]<<endl;
    }
 }

*/

/***************************training***************3*/

    vector <double> probability;
    double time;
    time= 0;
    double i;
    double e =  2.71828182845904523536;
    // calculating probability vectors
while ( time < 20) {
     for ( int j = 0; j < 800; j++)
        {   
       // cout<<m1[j][0]<<" "<<m1[j][1]<<endl;break;
        //cout<<m1[j][0]*weight[0]<<endl; break;
     //  cout<< m1[0][j]<<endl;
       //cout<< m1[1][j]<<endl;
    //cout<<weight[1]<<endl;
    //   cout<<weight[0]<<endl;
            i = m1[0][j]*weight[0]+m1[1][j]*weight[1];
           
            probability.push_back(sigmoid (i,e));
             //cout<< "prob is "<<probability[j]<<endl;
        }
       
      


    /****************Get error vector *************/
    for ( int i = 0; i < 800; i++)
    {
        err.push_back(trainSurV[i]-probability[i]);
    }
   /* for ( int i = 0; i < 800; i++)
    {
        cout<<err[i]<<endl;
    } */

    /***********updates weights using error and transpose matrix************/
    //cout<<weight[0]<<weight[1]<<endl;
    for ( int j = 0; j< 800; j++){
       
            weight[0] = (weight[0] + (0.0001* (m1[0][j]*err[j] ))); 

            weight[1] = (weight[1]+ (0.0001* (m1[1][j]*err[j])));
           //cout<<" weight now is "<<weight[0]<<" " << weight[1]<<endl;
    }

     time++;
}  // End of while loop
    // FInal Weight after iteration:
    cout<<" weight is "<<weight[0]<<" " << weight[1]<<endl; 



    /*****************************Predict************************/
    vector <double> predicted;
    for ( int i = 0; i< 246; i++)
    {
        predicted.push_back ( m2[0][i]*weight[0] + m2[1][i] * weight[1]);
        //cout<<"predicted is "<<predicted[i]<<endl;

    }

    vector <double> probabilityTest;
    for (int i = 0; i < 246; i++)
    {
        probabilityTest.push_back ( pow(e, predicted[i])/(1+pow(e, predicted[i])) );
    }

    
    
    
    double  sumMean;
    for (int i = 0; i < 246; i++){
      if ( predicted[i] == m2[1][i])
      {
        sumMean++;
      }
    } 

   
double TP= 0; double TN= 0; 
for (int i = 0; i < 246; i++)
{   if (predicted[i] > 0.5)
   { 
       TP++;
    }
     TN++;
}
 double accuracy = TP/0.645;

cout<< "Accurary is "<< accuracy;

    

    cout<< " Mean is: "<<sumMean<<endl;


auto end = std::chrono::high_resolution_clock::now();
    
    cout<<"Closing file titanic_project.csv. "<<endl;


    cout<<"Number of records: " <<numObservations<<endl;

    cout<<"\nProgram terminaed.";

printf("Time: %f\n", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0);

    return 0;

}

