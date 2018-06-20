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

private:
    // private data members
    int maxVal;
    int maxPopSize;
    int minInitPopSize;
    int indivSize;
    double survivalRate;
    bool minimize;
    vector< vector<int> > population;

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

    // seed rng
    int seed = time(NULL);
    std::cout << "Seed is: " << seed << std::endl;
    srand(seed);

    // then initialize population
    this->initPopulation();
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
        if (rand() % 2) {
            // mutate a random offspring. 
            offspring = this->mutate(this->population[rand() % survived]);
        } else {
            // mate two random surviving offspring. small chance that an individual mates with itself.
            offspring = this->crossover(this->population[rand() % survived], 
                                        this->population[rand() % survived]);
        }

        // check if the offspring is feasible. 
        if (this->fitness(offspring) > 0) {
            this->population.push_back(offspring);
            numOffspring--;
        }
    }

    return *std::max_element(this->population.begin(), this->population.end(),
                            [&] (vector<int> a, vector<int> b) 
                            {
                                return minimize ? fitness(a) < fitness(b)
                                                : fitness(a) > fitness(b);
                            });
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
        if (fitness(generated) > 0) {
            this->population[feasibleSolutionCount++] = generated;
        }
    }
}

#endif