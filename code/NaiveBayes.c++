#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <stdlib.h>
#include <bits/stdc++.h>

#define TRAIN_SEED 800
#define TEST_SEED 200

void toMatrix(std::string filename, std::vector<std::vector<double>> &m);
void printMatrix(std::vector<std::vector<double>> &m);
void printVector(std::vector<double> &v);

void seperateTranTest(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &train, std::vector<std::vector<double>> &test);

void naiveBayes();

int main()
{
    srand(TRAIN_SEED);
    std::vector<std::vector<double>> m;
    toMatrix("titanic_project.csv", m);

    m.erase(m.begin());

    std::vector<std::vector<double>> train;
    std::vector<std::vector<double>> test;

    seperateTranTest(m, train, test);

    printf("Train Size Survive: %d \n", train.at(0).size());
    printf("Train Size Pclass: %d \n", train.at(1).size());
    printf("Train Size Sex: %d \n", train.at(2).size());
    printf("Train Size Age: %d \n", train.at(3).size());
    // printMatrix(m);

    return 0;
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

void seperateTranTest(std::vector<std::vector<double>> &m, std::vector<std::vector<double>> &train, std::vector<std::vector<double>> &test)
{
    std::vector<std::vector<double>> temp = m;
    std::vector<std::vector<double>> tempTrain;
    std::vector<std::vector<double>> tempTest;

    std::unordered_map<int, int> trainMap;
    std::unordered_map<int, int> testMap; // To store index to avoid duplicates

    int mapKey = 1;
    for (int i = 0; i < 800; i++)
    {

        int index = -1;
        do
        {
            index = rand() % temp.at(i).size();

        } while (trainMap.find(index) != trainMap.end());

        trainMap.insert(std::make_pair(index, mapKey));
        mapKey++;

        for (int j = 0; j < temp.size(); j++)
        {
            if (tempTrain.size() != temp.size())
            {
                std::vector<double> v;
                tempTrain.push_back(v);
            }
            tempTrain.at(j).push_back(temp.at(j).at(index));
            temp.at(j).erase(temp.at(j).begin() + index);
        }
    }

    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.size() == 0)
        {
            break;
        }

        int index = rand() % temp.at(i).size();

        for (int j = 0; j < temp.size(); j++)
        {

            if (tempTest.size() != temp.size())
            {
                std::vector<double> v;
                tempTrain.push_back(v);
            }
            tempTest.at(j).push_back(temp.at(j).at(index));
            temp.at(j).erase(temp.at(j).begin() + index);
        }
    }

    train = tempTrain;
    test = tempTest;
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

void printVector(std::vector<double> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        printf("%.2f ", v.at(i));
    }
    printf("\n");
}
