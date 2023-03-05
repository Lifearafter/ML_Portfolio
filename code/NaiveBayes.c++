#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <tuple>
#include <chrono>

#define RAND_SEED 800

void toMatrix(std::string filename, std::vector<std::vector<double>> &m);
void printMatrix(std::vector<std::vector<double>> &m);
void printVector(std::vector<double> &v);
void compareVectors(std::vector<double> &v1, std::vector<double> &v2);
void print_map(std::unordered_map<int, int> const &m);

void seperateTrainTest(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &train, std::vector<std::vector<double>> &test, std::vector<int> &trainIndex, std::vector<int> &testIndex);

std::tuple<double, double> calcApriori(std::vector<std::vector<double>> &m, std::vector<int> &index);
std::tuple<int, int> calcSurvDie(std::vector<std::vector<double>> &m);

void quallikelihood(std::vector<std::vector<double>> &m, std::vector<int> &index, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex);
void quatlikelihood(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var);

double probDensity(double x, double mean, double var);

std::tuple<double, double> naiveBayes(int pclass, int sex, double age, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori);

double specificity(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori);
double accuracy(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori);
double sensitivity(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori);

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    srand(RAND_SEED);
    std::vector<std::vector<double>> m;
    toMatrix("titanic_project.csv", m);

    m.erase(m.begin());

    std::vector<std::vector<double>> train;
    std::vector<std::vector<double>> test;
    std::vector<int> trainIndex;
    std::vector<int> testIndex;

    seperateTrainTest(m, train, test, trainIndex, testIndex);

    printf("Train Size: %d \n", train.at(0).size());
    printf("Test Size: %d \n", test.at(0).size());

    std::tuple<double, double> apriori = calcApriori(m, trainIndex);

    std::vector<std::vector<double>> lpclass;
    std::vector<std::vector<double>> lsex;

    quallikelihood(train, trainIndex, lpclass, lsex);

    std::vector<std::vector<double>> lage;
    std::vector<double> mean;
    std::vector<double> var;
    quatlikelihood(train, lage, mean, var);

    double acc = accuracy(test, lpclass, lsex, lage, mean, var, apriori);
    double spec = specificity(test, lpclass, lsex, lage, mean, var, apriori);
    double sen = sensitivity(test, lpclass, lsex, lage, mean, var, apriori);

    printf("Accuracy: %f \n", acc);
    printf("Specificity: %f\n", spec);
    printf("Sensitivity: %f\n", sen);

    auto end = std::chrono::high_resolution_clock::now();

    printf("Time: %f\n", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0);

    return 0;
}

std::tuple<double, double> naiveBayes(int pclass, int sex, double age, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori)
{

    double num_s = lpclass.at(1).at(pclass - 1) * lsex.at(1).at(sex) * probDensity(age, mean.at(1), var.at(1)) * std::get<1>(apriori);
    double num_p = lpclass.at(0).at(pclass - 1) * lsex.at(0).at(sex) * probDensity(age, mean.at(0), var.at(0)) * std::get<0>(apriori);

    double den = num_s + num_p;

    return std::make_tuple(num_p / den, num_s / den);
}

double probDensity(double x, double mean, double var)
{
    double p = (1 / sqrt(2 * M_PI * var)) * exp(-pow(x - mean, 2) / (2 * var));
    return p;
}

void toMatrix(std::string filename, std::vector<std::vector<double>> &m)
{
    std::ifstream file;

    file.open(filename);

    if (!file.is_open())
    {
        printf("File not found Error.\n");
    }
    else
    {
        std::string line;
        std::getline(file, line);
        std::regex reg(",");
        std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
        std::sregex_token_iterator end;

        while (iter != end)
        {
            std::vector<double> v;
            m.push_back(v);
            iter++;
        }

        while (std::getline(file, line))
        {
            std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
            std::sregex_token_iterator end;
            int i = 0;
            while (iter != end)
            {
                std::string regexOut = std::regex_replace((*iter).str().c_str(), std::regex("[^0-9.]"), "");
                m.at(i).push_back(std::stod(regexOut));
                iter++;
                i++;
            }
        }

        std::printf("Matrix Column Size: %d \n", m.size());
        file.close();
    }
}

void seperateTrainTest(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &train, std::vector<std::vector<double>> &test, std::vector<int> &trainIndex, std::vector<int> &testIndex)
{
    std::vector<std::vector<double>> temp = m;
    std::vector<std::vector<double>> tempTrain;
    std::vector<std::vector<double>> tempTest;

    std::unordered_map<int, int> mainMap;

    // std::vector<int> testIndex;
    // std::vector<int> trainIndex;

    for (int i = 0; i < 800; i++)
    {

        int index = -1;
        do
        {

            index = rand() % temp.at(0).size();

        } while (mainMap.find(index) != mainMap.end());

        mainMap.insert(std::make_pair(index, 0));
        trainIndex.push_back(index);

        for (int j = 0; j < temp.size(); j++)
        {
            if (tempTrain.size() != temp.size())
            {
                std::vector<double> v;
                tempTrain.push_back(v);
            }
            tempTrain.at(j).push_back(temp.at(j).at(index));
        }
    }
    for (int i = 0; i < temp.at(0).size() - 800; i++)
    {
        int index = -1;
        do
        {
            index = rand() % temp.at(0).size();
        } while (mainMap.find(index) != mainMap.end());

        mainMap.insert(std::make_pair(index, 0));
        testIndex.push_back(index);

        for (int j = 0; j < temp.size(); j++)
        {

            if (tempTest.size() != temp.size())
            {
                std::vector<double> v;
                tempTest.push_back(v);
            }
            tempTest.at(j).push_back(temp.at(j).at(index));
        }
    }

    train = tempTrain;
    test = tempTest;
}

void quallikelihood(std::vector<std::vector<double>> &m, std::vector<int> &index, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex)
{
    std::tuple<int, int> t;
    t = calcSurvDie(m);

    double died = (double)std::get<0>(t);
    double survived = (double)std::get<1>(t);

    lpclass.push_back(std::vector<double>());
    lpclass.push_back(std::vector<double>());

    int richDead = 0;
    int richSurvived = 0;
    int poorDead = 0;
    int poorSurvived = 0;
    int middleDead = 0;
    int middleSurvived = 0;

    int count = 0;

    for (auto i : m.at(1))
    {

        if (i == 0 && m.at(0).at(count) == 1)
        {
            richDead++;
        }
        else if (i == 0 && m.at(0).at(count) == 2)
        {
            middleDead++;
        }
        else if (i == 0 && m.at(0).at(count) == 3)
        {
            poorDead++;
        }
        else if (i == 1 && m.at(0).at(count) == 1)
        {
            richSurvived++;
        }

        else if (i == 1 && m.at(0).at(count) == 2)
        {
            middleSurvived++;
        }

        else if (i == 1 && m.at(0).at(count) == 3)
        {
            poorSurvived++;
        }
        count++;
    }

    std::vector<double> pclassNum;

    pclassNum.push_back(richDead);
    pclassNum.push_back(middleDead);
    pclassNum.push_back(poorDead);

    pclassNum.push_back(richSurvived);
    pclassNum.push_back(middleSurvived);
    pclassNum.push_back(poorSurvived);

    for (int i = 0; i < 2; i++)
    {

        for (int j = 0; j < 3; j++)
        {
            if (i == 0)
            {
                lpclass.at(i).push_back(pclassNum.at(j) / (double)std::get<0>(t));
            }
            else
            {
                lpclass.at(i).push_back(pclassNum.at(j + 3) / (double)std::get<1>(t));
            }
        }
    }

    lsex.push_back(std::vector<double>());
    lsex.push_back(std::vector<double>());

    int mDead = 0;
    int mSurvived = 0;
    int fDead = 0;
    int fSurvived = 0;

    count = 0;

    for (auto i : m.at(1))
    {
        if (i == 0 && m.at(2).at(count) == 0)
        {
            mDead++;
        }
        else if (i == 0 && m.at(2).at(count) == 1)
        {
            fDead++;
        }
        else if (i == 1 && m.at(2).at(count) == 0)
        {
            mSurvived++;
        }
        else if (i == 1 && m.at(2).at(count) == 1)
        {
            fSurvived++;
        }
        count++;
    }

    std::vector<double> sexNum;
    sexNum.push_back(mDead);
    sexNum.push_back(fDead);
    sexNum.push_back(mSurvived);
    sexNum.push_back(fSurvived);

    for (int i = 0; i < 2; i++)
    {

        for (int j = 0; j < 2; j++)
        {
            if (i == 0)
            {
                lsex.at(i).push_back(sexNum.at(j) / (double)std::get<0>(t));
            }
            else
            {
                lsex.at(i).push_back(sexNum.at(j + 2) / (double)std::get<1>(t));
            }
        }
    }
}

void quatlikelihood(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var)
{

    double meanLocal[2] = {0, 0};

    double varLocal[2] = {0, 0};

    double sumDead = 0;
    double sumAlive = 0;

    double numDead = 0;
    double numAlive = 0;

    for (int i = 0; i < m.at(1).size(); i++)
    {
        if (m.at(1).at(i) == 0)
        {
            sumDead += m.at(3).at(i);
            meanLocal[0] += m.at(3).at(i);
            numDead++;
        }
        else
        {
            sumAlive += m.at(3).at(i);
            meanLocal[1] += m.at(3).at(i);
            numAlive++;
        }
    }

    meanLocal[0] = sumDead / numDead;
    meanLocal[1] = sumAlive / numAlive;

    for (int i = 0; i < m.at(1).size(); i++)
    {
        if (m.at(1).at(i) == 0)
        {
            varLocal[0] += pow(m.at(3).at(i) - meanLocal[0], 2);
        }
        else
        {
            varLocal[1] += pow(m.at(3).at(i) - meanLocal[1], 2);
        }
    }

    varLocal[0] = varLocal[0] / numDead;
    varLocal[1] = varLocal[1] / numAlive;

    mean.push_back(meanLocal[0]);
    mean.push_back(meanLocal[1]);

    var.push_back(varLocal[0]);
    var.push_back(varLocal[1]);
}

std::tuple<double, double> calcApriori(std::vector<std::vector<double>> &m, std::vector<int> &index)
{

    std::vector<double> temp;
    std::tuple<int, int> t;
    t = calcSurvDie(m);

    return (std::make_tuple((double)std::get<0>(t) / m.at(1).size(), (double)std::get<1>(t) / m.at(1).size()));
}

std::tuple<int, int> calcSurvDie(std::vector<std::vector<double>> &m)
{
    int survived = 0;
    int died = 0;

    for (auto i : m.at(1))
    {
        if (m.at(1).at(i) == 0)
        {
            died++;
        }
        else
        {
            survived++;
        }
    }
    return (std::make_tuple(died, survived));
}

void printMatrix(std::vector<std::vector<double>> &m)
{
    for (int i = 0; i < m.size(); i++)
    {
        printf("Column %d: ", i);
        for (int j = 0; j < m.at(i).size(); j++)
        {
            printf("%.2f ", m.at(i).at(j));
        }
        printf("\n");
    }
}

void print_map(std::unordered_map<int, int> const &m)
{
    for (auto const &pair : m)
    {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

void compareVectors(std::vector<double> &v1, std::vector<double> &v2)
{
    for (auto i : v1)
    {

        if (std::binary_search(v2.begin(), v2.end(), i) == true)
        {

            printf("Found: %d \n", i);
        }
    }
}

void printVector(std::vector<double> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        printf("%.2f ", v.at(i));
    }
    printf("\n");
}

double accuracy(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori)
{
    double TP = 0;
    double FP = 0;
    double TN = 0;
    double FN = 0;

    for (int i = 0; i < m.at(1).size(); i++)
    {
        std::tuple<double, double> prob = naiveBayes(m.at(0).at(i), m.at(1).at(i), m.at(3).at(i), lpclass, lsex, lage, mean, var, apriori);

        if (std::get<0>(prob) > std::get<1>(prob))
        {
            if (m.at(1).at(i) == 1)
            {
                TP++;
            }
            else
            {
                FP++;
            }
        }
        else
        {
            if (m.at(1).at(i) == 0)
            {
                FN++;
            }
            else
            {
                TN++;
            }
        }
    }

    return ((TP + TN) / (TP + TN + FP + FN));
}

double sensitivity(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori)
{
    double TP = 0;
    double FP = 0;
    double TN = 0;
    double FN = 0;

    for (int i = 0; i < m.at(1).size(); i++)
    {
        std::tuple<double, double> prob = naiveBayes(m.at(0).at(i), m.at(1).at(i), m.at(3).at(i), lpclass, lsex, lage, mean, var, apriori);

        if (std::get<0>(prob) > std::get<1>(prob))
        {
            if (m.at(1).at(i) == 1)
            {
                TP++;
            }
            else
            {
                FP++;
            }
        }
        else
        {
            if (m.at(1).at(i) == 0)
            {
                FN++;
            }
            else
            {
                TN++;
            }
        }
    }
    return (TP / (TP + FN));
}

double specificity(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &lpclass, std::vector<std::vector<double>> &lsex, std::vector<std::vector<double>> &lage, std::vector<double> &mean, std::vector<double> &var, std::tuple<double, double> apriori)
{
    double TP = 0;
    double FP = 0;
    double TN = 0;
    double FN = 0;

    for (int i = 0; i < m.at(1).size(); i++)
    {
        std::tuple<double, double> prob = naiveBayes(m.at(0).at(i), m.at(1).at(i), m.at(3).at(i), lpclass, lsex, lage, mean, var, apriori);

        if (std::get<0>(prob) > std::get<1>(prob))
        {
            if (m.at(1).at(i) == 1)
            {
                TP++;
            }
            else
            {
                FP++;
            }
        }
        else
        {
            if (m.at(1).at(i) == 0)
            {
                FN++;
            }
            else
            {
                TN++;
            }
        }
    }
    return (TN / (TN + FP));
}