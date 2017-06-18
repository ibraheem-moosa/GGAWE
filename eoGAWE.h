// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// eoGAWE.h
// (c) GeNeura Team, 2000 - EEAAX 1999 - Maarten Keijzer 2000
/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _eoGAWE_h
#define _eoGAWE_h

#include <eoInvalidateOps.h>
#include <eoContinue.h>
#include <eoPop.h>
#include <eoSelectOne.h>
#include <eoSelectPerc.h>
#include <eoSelectNumber.h>
#include <eoEvalFunc.h>
#include <eoAlgo.h>
#include <apply.h>
#include <iostream>
#include <cmath>
#include "inputData.h"

#include <unordered_map>
#include <tuple>

extern unordered_map<string, tuple<double, double, double, double, double, double> > calculated_fitnesses;




/** The Simple Genetic Algorithm,
 *
 * Needs a selector (class eoSelectOne) a crossover (eoQuad, i.e. a
 * 2->2 operator) and a mutation with their respective rates, of
 * course an evaluation function (eoEvalFunc) and a continuator
 * (eoContinue) which gives the stopping criterion. Performs full
 * generational replacement.
 *
 * @ingroup Algorithms
 */
template <class EOT>
class eoGAWE : public eoAlgo<EOT>
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    eoGAWE(
           eoSelectOne<EOT>& _select,
           eoQuadOp<EOT>& _cross, float _crate,
           eoMonOp<EOT>& _mutate, float _mrate,
           eoEvalFunc<EOT>& _eval,
           eoContinue<EOT>& _cont)
    : cont(_cont),
    mutate(_mutate),
    mutationRate(_mrate),
    cross(_cross),
    crossoverRate(_crate),
    select(_select),
    eval(_eval)
    {
    }

    eoGAWE(
           //eoSelectPerc<EOT>& _select,
           eoSelectNumber<EOT>& _select,
           eoQuadOp<EOT>& _cross,
           eoMonOp<EOT>& _mutate,
           eoEvalFunc<EOT>& _eval,
           eoContinue<EOT>& _cont,
           unsigned _elite)
    : cont(_cont),
    mutate(_mutate),
    cross(_cross),
    select(_select),
    elite(_elite),
    eval(_eval)
    {
        //best = NULL;
    }
#pragma GCC push_options
#pragma GCC optimize ("O3")

	
	void operator()(eoPop<EOT>& _pop)
	{
		eoPop<EOT> R;
		eoPop<EOT> Q;
		int gen = 0;
		prevBest = best.fitness();
		int stableCounter = 0;
		do
		{
			double start_time = clock();
			R.clear();	
			for (unsigned i = 0; i < _pop.size(); i++)
			{
				for(unsigned j = 0; j < parameters["neighborhoodSize"]; j++)
				{
					R.push_back(_pop[i]);
                    R.back().set_parent_id(_pop[i].id);
				}
			}
			for(unsigned i = 0; i < R.size(); i++)
			{
				if(mutate(R[i]))
					R[i].invalidate();
				//R[i].fitness();
			}
			apply<EOT > (eval, R); //Assess Fitness
			//std::cout << "mutation done\n";
			select(R, Q);
			// std::cout <<"Q after breed"<<Q;
			//std::cout << "selection done\n";
			// std::cout <<"Best indi:"<<Q[Q.size() - elite]<<std::endl;
			std::cout << "Gen:" << gen << std::endl;
			EOT competitor;
			double max_fitness = -INFINITY;
			for(auto individual : Q){
				if(individual.fitness() > max_fitness)
				{
					competitor = individual;
					max_fitness = individual.fitness();
				}
			}

			if (best < competitor) //using adjusted fitness
			{
				best = competitor;
			}
			// std::cout << best.D[0] << ',' << best.D[1] << ',' << best.D[2] << ',' << best.Dun << ',' << best.ATT << std::endl;

			if (actualBest.actualFitness < competitor.actualFitness)
			{
				actualBest = competitor;
			}
            /*
            std::cout << actualBest.id << std::endl;
            for(unsigned i = 0; i < _pop.size(); i++)
            {
                std::cout << _pop[i].id << " " ;
            }
            std::cout << std::endl;
            */
			std::cout << actualBest.D[0] << ',' << actualBest.D[1] << ',' << actualBest.D[2] << ',' << actualBest.Dun << ',' << actualBest.ATT  << ',' << actualBest.fitness()<< std::endl;
			double average_fitness = 0.0;
			for(auto individual : _pop) {
				average_fitness += individual.fitness();
			}
			average_fitness /= _pop.size();
			double std_dev = 0.0;
			for(auto individual : _pop) {
				std_dev += (individual.fitness() - average_fitness) * (individual.actualFitness - average_fitness);
			}
			std_dev /= _pop.size();
			std_dev = sqrt(std_dev);
			std::cout << std_dev << std::endl;

			_pop.swap(Q);
            for(int i = 0; i < _pop.size(); i++){
                _pop[i].set_id(i);
                printf("%d %d %d %lf\n", i, _pop[i].parent_id, _pop[i].mutation_applied, _pop[i].fitness());
            }
			//apply<EOT > (eval, _pop); //Assess Fitness

			double fitnessImprovementRatio = (best.fitness() - prevBest) / fabs(prevBest);  
			if (fitnessImprovementRatio < this->minFitnessRatio)
			{
				stableCounter++;
				if (stableCounter == this->stableCount)
				{
					//break;
				}
				prevBest = best.fitness();
				printf("ratio: %lf stable count: %d\n", fitnessImprovementRatio, stableCounter);
			}
			else
			{
				prevBest = best.fitness();
				stableCounter--;
				if(stableCounter < 0)
					stableCounter = 0;
			}
			gen++;
			//printf("%d\n", _pop.size());
			printf("Time: %lf\n", (clock() - start_time) / CLOCKS_PER_SEC);
			printf("Unique individuals seen: %ld\n", calculated_fitnesses.size());
		}while (genCount--);
	}
#pragma GCC pop_options
private:

    eoContinue<EOT>& cont;
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> mutate;
    float mutationRate;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoInvalidateQuadOp<EOT> cross;
    float crossoverRate;
    //eoSelectPerc<EOT> select;
    eoSelectNumber<EOT> select;
    unsigned elite;
    eoEvalFunc<EOT>& eval;

public:
    EOT best;
    EOT actualBest;
    EOT initial;
    int genCount;
    int stableCount;
    double prevBest;
    double minFitnessRatio;
};

#endif
