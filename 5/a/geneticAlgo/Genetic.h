#ifndef GENETIC_H
#define GENETIC_H

#include <cstdlib>
#include <time.h>
#include <algorithm>

class Genetic
{
public:
    Genetic(int maxVal, int maxPopSize, 
            int minInitPopSize, int indivSize, 
            double survivalRate, bool minimize, 
            int (*fitness)(vector<int>));
    vector<int> nextGeneration(void);
    void setMutationRate(double mutationRate);
    void setCrossoverRate(double crossoverRate);

private:
    // private data members
    int maxVal;
    int maxPopSize;
    int minInitPopSize;
    int indivSize;
    double survivalRate;
    bool minimize;
    vector< vector<int> > population;

    // separately initalized parameters (mutation rate, crossover rate, % that survives each generation)
    double mutationRate;
    double crossoverRate;

    // private functions
    int (*fitness)(vector<int> p);  // must return negative number for an infeasible solution.
    vector<int> crossover(vector<int> p1, vector<int> p2);
    vector<int> mutate(vector<int> p);

    vector<int> generateRandomIndiv(void);
    void initPopulation(void);
    bool compFunc(vector<int> a, vector<int> b);
};

Genetic::Genetic(int maxVal, int maxPopSize, 
                 int minInitPopSize, int indivSize, 
                 double survivalRate, bool minimize, 
                 int (*fitness)(vector<int>)) 
{
    this->maxVal = maxVal;
    this->maxPopSize = maxPopSize;
    this->minInitPopSize = minInitPopSize;
    this->indivSize = indivSize;
    this->survivalRate = survivalRate;
    this->minimize = minimize;
    this->fitness = fitness;

    this->mutationRate = 0.03;
    this->crossoverRate = 0.7;

    // seed rng
    int seed = time(NULL);
    std::cout << "Seed is: " << seed << std::endl;
    srand(seed);

    // then initialize population
    this->initPopulation();
}

static double getRandomDec(void) {
    return (double) rand() / (RAND_MAX);
}

// generates next generation, returns most fit solution from that generation
vector<int> Genetic::nextGeneration(void) {
    std::sort(this->population.begin(), this->population.end(), 
              [&](vector<int> a, vector<int> b) -> bool 
                {
                    return minimize ? fitness(a) < fitness(b)
                                    : fitness(a) > fitness(b); 
                });

    int survived = this->survivalRate * this->population.size();
    
    // we produce enough offspring to fill the population until max population
    int numOffspring = this->maxPopSize - survived;
    vector<int> offspring;
    offspring.resize(this->indivSize);

    // we keep only the ones that survived 
    this->population.resize(survived);

    while (numOffspring) {
        if (getRandomDec() <= this->mutationRate) {
            offspring = this->mutate(this->population[rand() % survived]);
            if (this->fitness(offspring) >= 0) {
                this->population.push_back(offspring);
                numOffspring--;
            }
        }

        if (numOffspring && getRandomDec() <= this->crossoverRate) {
            // mate two random surviving offspring. small chance that an individual mates with itself.
            offspring = this->crossover(this->population[rand() % survived], 
                                        this->population[rand() % survived]);

            if (this->fitness(offspring) >= 0) {
                this->population.push_back(offspring);
                numOffspring--;
            }
        }
    }

    return *std::max_element(this->population.begin(), this->population.end(),
                            [&] (vector<int> a, vector<int> b) 
                            {
                                return minimize ? fitness(a) < fitness(b)
                                                : fitness(a) > fitness(b);
                            });
}

void Genetic::setMutationRate(double mutationRate) {
    if (mutationRate >= 0 && mutationRate <= 1) {
        this->mutationRate = mutationRate;
    } else {
        cout << "Invalid mutation rate - needs to be between 0 and 1" << endl;   
    }
}

void Genetic::setCrossoverRate(double crossoverRate) {
    if (crossoverRate >= 0 && crossoverRate <= 1) {
        this->crossoverRate = crossoverRate;
    } else {
        cout << "Invalid crossover rate: needs to be between 0 and 1" << endl;
    }
}

// function to mate p1 and p2 at some random crossover point
vector<int> Genetic::crossover(vector<int> p1, vector<int> p2) {
    int crossoverPoint = rand() % this->indivSize;
    vector<int> offspring;
    offspring.resize(this->indivSize);
    
    for (int i = 0; i < crossoverPoint; i++) {
        offspring[i] = p1[i];
    }

    for (int i = crossoverPoint; i < this->indivSize; i++) {
        offspring[i] = p2[i];
    }

    return offspring;
}

// mutates p at random point. Changes a single element in p
vector<int> Genetic::mutate(vector<int> p) {
    vector<int> offspring = p;
    int mutationPoint = rand() % this->indivSize;
    
    int newVal = offspring[mutationPoint];
    
    while (newVal == offspring[mutationPoint]) {
        newVal = rand() % maxVal;
    }

    // change the value at the mutation point 
    offspring[mutationPoint] = newVal;
    return offspring;
}

// randomly generates an individual
vector<int> Genetic::generateRandomIndiv(void) {
    vector<int> newIndiv;
    for (int i = 0; i < this->indivSize; i++) {
        newIndiv.push_back(rand() % this->maxVal);
    }
    return newIndiv;
}

// initPopulation will produce exactly minInitPopSize solutions that will all be feasible 
void Genetic::initPopulation(void) {

    // resize population and each individual in population to correct size
    this->population.resize(minInitPopSize);

    int feasibleSolutionCount = 0;
    vector<int> generated;
    while (feasibleSolutionCount < this->minInitPopSize) {
        generated = this->generateRandomIndiv();
        if (fitness(generated) >= 0) {
            this->population[feasibleSolutionCount++] = generated;
        }
    }
}

#endif