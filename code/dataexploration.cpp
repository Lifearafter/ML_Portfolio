#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>

#include <tuple>

double sumVector(std::vector<double> &v);
double meanVector(std::vector<double> &v);
double medianVector(std::vector<double> &v);
double covarianceVector(std::vector<double> &v, std::vector<double> &v2);
double correlationVector(std::vector<double> &v, std::vector<double> &v2);
double varianceVector(std::vector<double> &v);

std::tuple<double, double> rangeVector(std::vector<double> &v);

void toVector(std::string filename, std::vector<double> &v, int column);
void printVector(std::vector<double> &v);

int main()
{
    std::vector<double> medv;
    std::vector<double> rm;

    std::string filename = "Boston.csv";

    printf("\nRunning functions for rm vector ...... \n");

    toVector(filename, rm, 0);

    double sum = sumVector(rm);
    printf("Sum: %.9g \n", sum);

    double mean = meanVector(rm);
    printf("Mean: %.9g \n", mean);

    double median = medianVector(rm);
    printf("Median: %.9g \n", median);

    double rangeMin, rangeMax;

    std::tie(rangeMin, rangeMax) = rangeVector(rm);
    printf("Range: %.9g %.9g \n", rangeMin, rangeMax);

    printf("\nRunning functions for medv vector ...... \n");

    toVector(filename, medv, 1);

    sum = sumVector(medv);
    printf("Sum: %.9g \n", sum);

    mean = meanVector(medv);
    printf("Mean: %.9g \n", mean);

    median = medianVector(medv);
    printf("Median: %.9g \n", median);

    std::tie(rangeMin, rangeMax) = rangeVector(medv);
    printf("Range: %.9g %.9g \n", rangeMin, rangeMax);

    printf("\nRunning functions for covariance and correlation ...... \n");

    double covariance = covarianceVector(medv, rm);
    printf("Covariance: %.9g \n", covariance);

    double correlation = correlationVector(medv, rm);
    printf("Correlation: %.9g \n", correlation);

    printf("\nEnd of Program \n");

    return 0;
}

void toVector(std::string filename, std::vector<double> &v, int column)
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

        if (column == 0)
        {
            while (std::getline(file, line))
            {
                int pos = line.find(",");
                std::string value = line.substr(0, pos);
                v.push_back(std::stod(value));
            }
        }
        else if (column == 1)
        {
            while (getline(file, line))
            {
                int pos = line.find(",");
                std::string value = line.substr(pos + 1, line.length());
                v.push_back(std::stod(value));
            }
        }
    }
    printf("Vector size: %d \n", v.size());
    file.close();
}

double sumVector(std::vector<double> &v)
{
    double sum = 0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v.at(i);
    }

    return sum;
}

double meanVector(std::vector<double> &v)
{
    double mean = sumVector(v) / v.size();

    return mean;
}

double medianVector(std::vector<double> &v)
{
    std::vector<double> tempVector(v);

    sort(tempVector.begin(), tempVector.end());

    if (tempVector.size() % 2 == 0)
    {
        return (tempVector.at(tempVector.size() / 2 - 1) + tempVector.at(tempVector.size() / 2)) / 2;
    }
    else
    {
        return tempVector.at(tempVector.size() / 2);
    }
}

std::tuple<double, double> rangeVector(std::vector<double> &v)
{
    double min = v.at(0);
    double max = v.at(0);

    for (int i = 0; i < v.size(); i++)
    {

        if (v.at(i) < min)
        {
            min = v.at(i);
        }
        else if (v.at(i) > max)
        {
            max = v.at(i);
        }
    }

    return std::make_tuple(max, min);
}

double covarianceVector(std::vector<double> &v, std::vector<double> &v2)
{
    double covariance = 0;
    double meanMedv = meanVector(v);
    double meanRM = meanVector(v2);

    for (int i = 0; i < v.size(); i++)
    {
        covariance += ((v.at(i) - meanMedv) * (v2.at(i) - meanRM) / (v.size() - 1));
    }

    return covariance;
}

double varianceVector(std::vector<double> &v)
{
    double variance = 0;
    double mean = meanVector(v);

    for (int i = 0; i < v.size(); i++)
    {
        variance += pow((v.at(i) - mean), 2) / (v.size() - 1);
    }

    return variance;
}

double correlationVector(std::vector<double> &v, std::vector<double> &v2)
{
    double correlation = 0;
    double covariance = covarianceVector(v, v2);
    double varianceMedv = varianceVector(v);
    double varianceRM = varianceVector(v2);

    double sigmaMedv = sqrt(varianceMedv);
    double sigmaRM = sqrt(varianceRM);

    correlation = covariance / (sigmaMedv * sigmaRM);

    return correlation;
}

void printVector(std::vector<double> &v)
{
    int counter = 0;
    while (v.size() > 0 && counter < (v.size()))
    {
        printf("%.9g, ", v.at(counter));
    }
}