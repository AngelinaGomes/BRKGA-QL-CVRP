#include "LocalSearch.h"
#include "Decoder.h"
#include <cmath> // std::abs

TSol LocalSearch(TSol &s, int const& numVertices, vector<vector<double>> const& dist)
{
    // TODO

    return s;
}

TSol LocalSearch2(TSol &s, vector<Vehicle> &v, int const& numVertices, vector<vector<double>> const& dist)
{
    // we use a Random Variable Neighborhood Descent (RVND) as local search
    int k = 1;

    // predefined number of neighborhood moves
    std::vector <int> NSL;
    std::vector <int> NSLAux;
    for (int i=1; i<=4; i++)
    {
        NSL.push_back(i);
        NSLAux.push_back(i);
    }

    //printf("\nHeuristicas: ");
    while (!NSL.empty())
	{
        // current objective function
        double foCurrent = s.fo;

        // randomly choose a neighborhood
        int pos = irand(0,NSL.size()-1);
        k = NSL[pos];

        switch (k) {
        case 1: 
            s = LS1(s, v, numVertices, dist); // swap heuristic intra-route
            break;

        case 2:
            s = LS2(s, v, numVertices, dist); // shift heuristic intra-route
            break;

        case 3:
            s = LS3(s, v, numVertices, dist); // swap heuristic inter-route
            break;

        case 4:
            s = LS4(s, v, numVertices, dist); // shift heuristic inter-route
            break;

        case 5:
            s = LS5(s, v, numVertices, dist); // 2-Opt heuristic intra-route
            break;
        default:
            break;
        }

        // return to first neighborhood if better the current solution
        if (s.fo < foCurrent) {
            NSL.clear();
            NSL = NSLAux;
        }
        // next neighborhood, otherwise
        else {
            NSL.erase(NSL.begin()+pos); /**< Remove N(n) from NSL. */
        }
	} //end while

 	return s;
}


TSol LS1(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist) // Swap
{
    s.fo = 0;
    for(int i = 0; i < v.size(); i++) {
        // Calls swap function
        if(v[i].route.size() > 1) {
            lsSwap(v[i].route, v[i].route.size(), dist);
            depotFirst(v[i].route);
            s.fo += setDistGetFo(v[i].route, dist);
        }
    }
    return s;
}

TSol LS2(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist) // Shift
{
    s.fo = 0;
    for(int i = 0; i < v.size(); i++) {
        // Calls shift function
        if(v[i].route.size() > 1) {
            lsShift(v[i].route, v[i].route.size(), dist);
            depotFirst(v[i].route);
            s.fo += setDistGetFo(v[i].route, dist);
        }
    }
    return s;
}

TSol LS3(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist) // Shift
{
    s.fo = 0;
    for(int i = 0; i < v.size()-1; i++) {
        for(int j = i + 1; j < v.size(); j++) {
            // Calls swap function
            if(v[i].route.size() > 1 && v[j].route.size() > 1) {
                lsSwapInter(v[i].route, v[j].route, v[i].route.size(), v[j].route.size(), dist);
                depotFirst(v[i].route);
                depotFirst(v[j].route);
            }
        }
    }

    for(int i = 0; i < v.size(); i++)
        s.fo += setDistGetFo(v[i].route, dist);

    return s;
}

TSol LS4(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist) // Shift
{
    s.fo = 0;
    for(int i = 0; i < v.size(); i++) {
        int j = (i + 1) % v.size();
        while (j != i) {
            // Calls shift function
            if(v[i].route.size() > 1 && v[j].route.size() > 1) {
                lsShiftInter(v[i].route, v[j].route, v[i].route.size(), v[j].route.size(), dist);
                depotFirst(v[i].route);
                depotFirst(v[j].route);
            }
        }
    }
    
    for(int i = 0; i < v.size(); i++)
        s.fo += setDistGetFo(v[i].route, dist);

    return s;
}


TSol LS5(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist) // 2-Opt
{
    s.fo = 0;
    for(int i = 0; i < v.size(); i++) {
        // Calls 2-opt function
        if(v[i].route.size() > 4) {
            ls2opt(v[i].route, v[i].route.size(), dist);
            depotFirst(v[i].route);
            s.fo += setDistGetFo(v[i].route, dist);
        }
    }
    return s;
}

double rand(const double min, const double max)
{
    return ((double)(rand()%10000)/10000.0)*(max-min)+min;
    //return uniform_real_Distribution<double>(min, max)(rng);
}

int irand(const int min, const int max)
{
    return (int)rand(0,max-min+1.0) + min;
}

void depotFirst(vector<Customer> &route) {
    //make depot be the first point on the route
    int index;
    for(int i = 0; i < route.size(); i++) {
        if(route[i].point == 0) {
            index = i; /**< index of depot */
            break;
        }
    }
    vector <Customer> route2(route.begin(), route.begin() + index);
    vector <Customer> route1(route.begin() + index, route.end());
    route.clear();
    vector<Customer>::iterator it;
    it = route.end();
    it = route.insert(it, route1.begin(), route1.end());
    it = route.end();
    it = route.insert(it, route2.begin(), route2.end());
}

double setDistGetFo(vector<Customer> &route, vector<vector<double>> const& dist) {
    route[0].dist = dist[route.back().point][route[0].point];
    double fo = route[0].dist;
    for(int i = 1; i < route.size(); i++) {
        route[i].dist = dist[route[i-1].point][route[i].point];
        fo += route[i].dist;
    }
    return fo;
}


void lsSwap(vector <Customer> &route, int n, vector<vector<double>> const& dist) { // Swap
    int i = 0, j = 0, Mi1= 0, Mj = 0;
    double fo;

    for (i = 0; i < n-1; i++) {
        for (j = i + 1; j < n; j++) {
            int vi  = route[i].point;
			int viM = route[(i+n-1)%n].point;
            int viP = route[(i+1)%n].point;
			int vj  = route[j].point;
            int vjM = route[(j+n-1)%n].point;
			int vjP = route[(j+1)%n].point;

            double fo = - dist[viM][vi]
                        - dist[vi][viP]
                        - dist[vjM][vj]
                        - dist[vj][vjP]
                        + dist[viM][vj]
                        + dist[vj][viP]
                        + dist[vjM][vi]
                        + dist[vi][vjP];
            
            if (fo < 0) {
                //swap
                int aux = route[i].point;
                route[i].point = route[j].point;
                route[j].point = aux;               
            }
        }//for j
    }//for i
}

void lsShift(vector <Customer> &route, int n, vector<vector<double>> const& dist) { // Shift
    int i = 0, j = 0, Mi1= 0, Mj = 0;
    double fo;

    for (i = 0; i < n; i++) {
        j = (i+1)%n;
        while ( ((j+1)%n) != i ) {
            int vi  = route[i].point;
            int viP = route[(i+1)%n].point;
			int viM = route[(i+n-1)%n].point;
			int vj  = route[j%n].point;
			int vjP = route[(j+1)%n].point;

            double fo = - dist[viM][vi]
                        - dist[vi][viP]
                        - dist[vj][vjP]
                        + dist[vj][vi]
                        + dist[vi][vjP]
                        + dist[viM][viP];
            
            if (fo < 0) {                
                //insert node
                Customer aux;
                aux.point = route[i].point;
                // aux.dist = route[i].dist;

                route.insert(route.begin()+((j+1)%n), aux);
                if (i < ((j+1)%n))
                    route.erase(route.begin()+i);
                else
                    route.erase(route.begin()+(i+1));
            }
            j++;
        }//while
    }//for
}

void lsSwapInter(vector <Customer> &r1, vector <Customer> &r2, int n, int m, vector<vector<double>> const& dist) { // Swap inter-route
    int i = 0, j = 0, Mi1= 0, Mj = 0;
    double fo;

    for (i = 1; i < n; i++) {
        for (j = 1; j < m; j++) {
            int vi  = r1[i].point;
			int viM = r1[(i+n-1)%n].point;
            int viP = r1[(i+1)%n].point;
			int vj  = r2[j].point;
            int vjM = r2[(j+m-1)%m].point;
			int vjP = r2[(j+1)%m].point;

            double fo = - dist[viM][vi]
                        - dist[vi][viP]
                        - dist[vjM][vj]
                        - dist[vj][vjP]
                        + dist[viM][vj]
                        + dist[vj][viP]
                        + dist[vjM][vi]
                        + dist[vi][vjP];
            
            if (fo < 0) {
                //swap
                int aux = r1[i].point;
                r1[i].point = r2[j].point;
                r2[j].point = aux;
            }
        }//for j
    }//for i
}

void lsShiftInter(vector <Customer> &r1, vector <Customer> &r2, int n, int m, vector<vector<double>> const& dist) { // Shift inter-route
    int i = 0, j = 0, Mi1= 0, Mj = 0;
    double fo;

    for (i = 1; i < n; i++) {
        for (j = 0; j < m; j++) {
            int vi  = r1[i].point;
            int viP = r1[(i+1)%n].point;
			int viM = r1[(i+n-1)%n].point;
			int vj  = r2[j].point;
			int vjP = r2[(j+1)%m].point;

            double fo = - dist[viM][vi]
                        - dist[vi][viP]
                        - dist[vj][vjP]
                        + dist[vj][vi]
                        + dist[vi][vjP]
                        + dist[viM][viP];
            
            if (fo < 0) {                
                //insert node
                Customer aux;
                aux.point = r1[i].point;
                // aux.dist = route[i].dist;

                r2.insert(r2.begin()+((j+1)%m), aux);
                r1.erase(r1.begin()+i);
            }
        }//for j
    }//for i
}

void ls2opt(vector <Customer> &route, int n, vector<vector<double>> const& dist) { // 2-Opt
    int t = n, i = 0, j = 0, Mi1= 0, Mj = 0;
    double foOpt = 0;

    for (i = 0; i < t; i++) {
        j = i + 2;
        while (((j+1)%t) != i) {
            int vi  = route[i].point;
            int vi1 = route[(i+1)%t].point;
            int vj  = route[j%t].point;
            int vj1 = route[(j+1)%t].point;

            foOpt = - dist[vi][vi1]
                    - dist[vj][vj1]
                    + dist[vi][vj]
                    + dist[vi1][vj1];

            if (foOpt < 0) {
                // first improvement strategy
                Mi1 = (i+1)%t;
                Mj  = j%t;

                int inicio = Mi1,
                    fim = Mj;

                int tam, p1, p2, aux;

                if(inicio > fim)
                    tam = t - inicio + fim + 1;
                else
                    tam = fim - inicio + 1;

                p1=inicio;
                p2=fim;

                for(int k=0; k < tam/2; k++) {
                    aux = route[p1%t].point;
                    route[p1%t].point = route[p2%t].point;
                    route[p2%t].point = aux;

                    p1 = (p1 == t-1)?0:p1+1;
                    p2 = (p2 == 0)?t-1:p2-1;
                }
                //s.fo = s.fo + foOpt;
            }
            j++;
        }//while
    }//for
}