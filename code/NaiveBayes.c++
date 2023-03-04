#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <regex>

void toMatrix(std::string filename, std::vector<std::vector<double>> &m);
void printMatrix(std::vector<std::vector<double>> &m);
void printVector(std::vector<double> &v);

void naiveBayes();

int main()
{
    std::vector<std::vector<double>> m;
    toMatrix("titanic_project.csv", m);
    printMatrix(m);

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
        std::getline(file, line, '\n');

        int colNum = std::count(line.begin(), line.end(), ',') + 1;

        for (int i = 1; i < colNum; i++)
        {
            // file.clear();
            // file.seekg(0);
            std::vector<double> v;
            while (std::getline(file, line))
            {
                int pos = line.find(",");
                std::string value = line.substr(0, pos + i);
                std::string regexOut = std::regex_replace(value, std::regex("[^0-9.]"), "");
                v.push_back(std::stod(regexOut));
            }
            m.push_back(v);
            printVector(v);
        }
    }
    std::printf("Matrix Column Size: %d \n", m.size());
    file.close();
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
