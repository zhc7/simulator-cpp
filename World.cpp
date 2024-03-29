//
// Created by zhc7 on 2022/1/20.
//

#include "World.h"


World::World(vector<Entity> entities, string mode, double tic, double total_time) : entities(std::move(entities)),
                                                                                    mode(std::move(mode)), tic(tic),
                                                                                    total_time(total_time) {}

void World::step() {
    for (auto &e: this->entities) {
        e.v = e.a * tic + e.v;
        e.place = e.v * tic + e.place;
    }

    for (int i = 0; i < this->entities.size(); i++) {
        Entity &e1 = entities.at(i);
        for (int j = i + 1; j < this->entities.size(); j++) {
            Entity &e2 = entities.at(j);
            nc::NdArray<double> F = e1.collapse(e2);
            e2.enforce(F);
            e1.enforce(-F);
        }
        e1.enforce(g);
    }

    for (auto &e: this->entities) {
        e.calc_a();
    }
}

vector<byte> World::loop(double record_rate = 0.1) {
    double t = 0;
    vector<byte> record;
    double n = 0;
    while (t < total_time) {
        step();
        if (mode == "display") {
            if (t >= record_rate * n) {
                //TODO: record
                n += 1;
            }
        }
        t += tic;
    }
    return record;
}