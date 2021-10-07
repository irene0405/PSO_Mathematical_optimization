#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>

#define ROUND 100
#define GENERATION 100
#define POPULATION 100
#define DIMENSION 1
#define MAX_POSITION 100
#define MIN_POSITION -100
#define MAX_VELOCITY 10
#define MIN_VELOCITY -10
#define w 1
#define c1 1
#define c2 1

using namespace std;

long double position[POPULATION][DIMENSION];
long double pBestPosition[POPULATION][DIMENSION];
long double gBestPosition[POPULATION][DIMENSION];
long double lastVelocity[POPULATION][DIMENSION];
long double velocity[POPULATION][DIMENSION];
long double fitness[POPULATION];
long double pBestFitness[POPULATION];
long double gBestFitness;

long double random(long double start, long double end) {
    random_device rd;
    default_random_engine gen = default_random_engine(rd());
    uniform_real_distribution<long double> dis(start, end);
    return dis(gen);
}

void init() {
    gBestFitness = INT_MAX;
    for (int i = 0; i < POPULATION; i++) {
        pBestFitness[i] = INT_MAX;
        for (int j = 0; j < DIMENSION; j++) {
            position[i][j] = random(MIN_POSITION, MAX_POSITION);
            pBestPosition[i][j] = 0;
            gBestPosition[i][j] = 0;
            lastVelocity[i][j] = random(MIN_VELOCITY, MAX_VELOCITY);
            velocity[i][j] = random(MIN_VELOCITY, MAX_VELOCITY);
        }
    }
}

void calc() {
    for (int i = 0; i < POPULATION; i++) {
        fitness[i] = 0;
        for (int j = 0; j < DIMENSION; j++) {
            fitness[i] += abs(position[i][j]);
        }
    }
}

void update() {
    for (int i = 0; i < POPULATION; i++) {
        // fitness < pBestFitness
        if (fitness[i] < pBestFitness[i]) {
            pBestFitness[i] = fitness[i];
            for (int j = 0; j < DIMENSION; j++) {
                pBestPosition[i][j] = position[i][j];
            }
        }

        // pBestFitness < gBestFitness
        if (pBestFitness[i] < gBestFitness) {
            gBestFitness = pBestFitness[i];
            for (int j = 0; j < DIMENSION; j++) {
                gBestPosition[i][j] = pBestPosition[i][j];
            }
        }

        // updateVelocity
        for (int j = 0; j < DIMENSION; j++) {
            velocity[i][j] = w * lastVelocity[i][j] +
                             c1 * random(0, 1) * (pBestPosition[i][j] - position[i][j]) +
                             c2 * random(0, 1) * (gBestPosition[i][j] - position[i][j]);

            if (velocity[i][j] > MAX_VELOCITY)
                velocity[i][j] = MAX_VELOCITY;
            if (velocity[i][j] < MIN_VELOCITY)
                velocity[i][j] = MIN_VELOCITY;
        }
    }
}

void move() {
    for (int i = 0; i < POPULATION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            // lastVelocity = velocity
            lastVelocity[i][j] = velocity[i][j];

            //position = position + velocity
            position[i][j] = position[i][j] + velocity[i][j];

            if (position[i][j] > MAX_POSITION) {
                position[i][j] = MAX_POSITION;
            }

            if (position[i][j] < MIN_POSITION) {
                position[i][j] = MIN_POSITION;
            }
        }
    }
}

int main() {
    long double best_value = INT_MAX, worst_value = -1, total = 0.0;

    // count execution time
    auto start = chrono::steady_clock::now();

    ofstream ofs;
    ofs.open("PSO_Mathematical_optimization-output.txt");
    ofs << "Round: " << ROUND << endl;
    ofs << "Generation: " << GENERATION << endl;
    ofs << "Population: " << POPULATION << endl;
    ofs << "Max velocity: " << MAX_VELOCITY << endl;
    ofs << "Min velocity: " << MIN_VELOCITY << endl;
    ofs << "Inertia weight: " << w << endl;
    ofs << "Particle best weight: " << c1 << endl;
    ofs << "Globe best weight: " << c2 << endl;
    ofs << "============================" << endl;

    for (int j = 0; j < ROUND; j++) {
        init();
        for (int i = 0; i < GENERATION; i++) {
            calc();
            update();
            move();
        }
        if (gBestFitness < best_value) {
            best_value = gBestFitness;
        } else if (gBestFitness > worst_value) {
            worst_value = gBestFitness;
        }
        ofs << "Round: " << j + 1 << " / " << ROUND << " --- " << gBestFitness << endl;
    }
    ofs << "============================" << endl;
    ofs << "Average value: " << (long double) total / ROUND << endl;
    ofs << "Worst value: " << worst_value << endl;
    ofs << "Best value: " << best_value << endl;
    ofs << "============================" << endl;

    // count execution time
    auto end = chrono::steady_clock::now();
    cout << "Time taken: " << chrono::duration<double>(end - start).count() << " s" << endl;

    ofs << "Time taken: " << chrono::duration<double>(end - start).count() << " s" << endl;
    ofs.close();
}