#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define mem(list, val) memset(list, (val), sizeof(list))
#define pb push_back

typedef vector<int> _Route;
typedef vector<vector<int> > _RouteSet;
typedef vector<vector<double> > Matrix;

#define MAX 200
#define INF 1e8

int minRouteSize, maxRouteSize, numberOfRoutes;

void readData(string filename, Matrix &matrix)
{
    FILE *fp = fopen(filename.c_str(), "r");
    
    int siz;
    double num;
    fscanf(fp, "%d", &siz);

    vector<double> temp;
    for(int i=0; i<siz; i++){
        for(int j=0; j<siz; j++){
            fscanf(fp, "%lf", &num);
            temp.push_back(num);
        }
        matrix.push_back(temp);
        temp.clear(); 
    }

    fclose(fp);
}

template<class T>
void print_matrix(vector<vector<T> > &m)
{
    int d = m.size();

    for(int i = 0; i < d; i++) {
        int l = m[i].size();
        for(int j = 0; j < l; j++) {
            printf("%d ", (int)m[i][j]);
        }
        printf("\n");
    }
}

bool checkRoute(const _Route &route, const Matrix &distance, int maxRouteSize, int minRouteSize)
{
    /*
    if(route.size() > maxRouteSize)
        return false;
    if(route.size() < minRouteSize)
        return false;
    */
        
    assert(route.size() >= minRouteSize && route.size() <= maxRouteSize);

    int n = distance.size();
    vector<bool> isInRoute(n, false);

    for(int i = 0; i < route.size(); i++) {
        /*
        if(isInRoute[route[i]]) {
            return false;
        }
        */
        assert(!isInRoute[route[i]]);
        isInRoute[route[i]] = true;
    }

    for(int i = 0; i < route.size() - 1; i++) {
        /*
        if(distance[route[i]][route[i + 1]] == -1) {
            return false;
        }
        */
        assert(distance[route[i]][route[i + 1]] != -1); 
    }
    return true;
}

void getRouteSet(Matrix &dist, int numberOfRoutes, _RouteSet &result)
{
	int numberOfNodes = dist.size();
	for(int r=0; r<numberOfRoutes; r++){

	}
}

_Route getRandomRoute(Matrix &dist, vector<int> &chosenCount, int routeNo)
{
	_Route route;

	if(routeNo == 0) {
		route.push_back(rand()%dist.size());
		chosenCount[route[0]]++;
	}
	else {
		int indx = rand()%dist.size();
		while(chosenCount[indx] == 0)
		{
			indx = rand()%dist.size();
		}

		route.push_back(indx);
		chosenCount[indx]++;
	}

	int routeSize = minRouteSize + rand()%(maxRouteSize - minRouteSize);
	vector<bool> used = vector<bool>(dist.size(), false);
	vector<int> candidateNode;
	used[route[0]] = true;

	for(int r = 1;r < routeSize;)
	{
		int curNode = route[route.size()-1];
		vector<double> fitness;
		double total = 0;
		//cout<<r<<endl;
		for(int l = 0;l < dist[curNode].size();l++)
		{
			if(dist[curNode][l] <= 0 || used[l]) continue;

			fitness.push_back(1.0/(chosenCount[l] + 1));
			total += fitness[fitness.size()-1];
			candidateNode.push_back(l);
		}

		if(candidateNode.size() == 0) {	//route of proper size can not be made using last node
			//cout<<route.size()<<' '<<minRouteSize<<endl;
			if(route.size() >= minRouteSize) {
				/*
				fr(int l = 0;l < route.size();l++)
				{
					cout<<route[l]<<' ';
				}
				cout<<endl;
				*/
				return route;
			}

			//chosenCount[route[route.size()-1]]--;
			route.pop_back(); //in next iteration node before current will be chosen to see if can be expanded
			r --; //as one node is discarded and in loop r will be increamented

			if(route.size() == 0) {
				if(routeNo == 0) {
					route.push_back(rand()%dist.size());
					chosenCount[route[0]]++;
				}
				else {
					int indx = rand()%dist.size();
					while(chosenCount[indx] == 0)
					{
						indx = rand()%dist.size();
					}

					chosenCount[indx]++;
				}
				
				r++;
			}

			candidateNode.clear();
			fitness.clear();
			continue;
		}

		for(int l = 0;l < fitness.size();l++)
		{
			fitness[l] /= total;
		}

		for(int l = 1;l < fitness.size();l++)
		{
			fitness[l] += fitness[l-1];
		}

		double p = rand()*1.0/RAND_MAX;

		auto it = lower_bound(fitness.begin(), fitness.end(), p);

		route.push_back(candidateNode[it-fitness.begin()]);

		used[route[route.size()-1]] = true;
		chosenCount[route[route.size()-1]]++;

		candidateNode.clear();
		fitness.clear();
		
		r++;
	}

	/*
	for(int l = 0;l < route.size();l++)
	{
		cout<<route[l]<<' ';
	}
	cout<<endl;
	*/
	return route;
}


int main(int argc, char **argv)
{
    if(argc < 6) {
        printf("Usage: distances_file num_of_routes minRouteSize maxRouteSize num_of_route_sets\n");
        return 1;
    }

    numberOfRoutes = atoi(argv[2]);
    minRouteSize = atoi(argv[3]);
    maxRouteSize = atoi(argv[4]);
    int num_of_route_sets = atoi(argv[5]);

    Matrix dist;

    readData(argv[1], dist);
    //print_matrix<double>(dist);

    
    //cout << minRouteSize << endl;
    vector<_RouteSet> routeSet_sets;
   	vector<_Route> routeSet;
    vector<int> chosenCount = vector<int>(dist.size(), 0);

    for(int l = 0;l < num_of_route_sets;l++)
    {
    	for(int r = 0; r < numberOfRoutes; r++)
    	{
	        //printf("generating %d\n", r);
	        _Route result;
	        result = getRandomRoute(dist, chosenCount, r);
	        routeSet.push_back(result);

	        if(!checkRoute(result, dist, maxRouteSize, minRouteSize)) {
	            printf("Route is dangerous  %d \n", r);
	        }
	        for(int j=0; j <result.size(); j++) cout << result[j] << ' ';
	        cout << "-1\n";
		}
		cout<<endl;
    }



    return 0;
}
