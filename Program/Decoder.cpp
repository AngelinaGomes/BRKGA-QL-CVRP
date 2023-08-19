#include "Decoder.h"

double randomicoD(double min, double max) {
    return ((double)(rand()%10000)/10000.0)*(max-min)+min;
    //return uniform_real_distribution<double>(min, max)(rng);
}

int irandomicoD(int min, int max) {
    return (int)randomicoD(0,max-min+1.0) + min;
}

void decDepotFirst(vector<Customer> &route) {
    // make depot be the first point on the route
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

vector <Customer> dec2opt(vector <Customer> route, int n, vector<vector<double>> const& dist) // 2-Opt
{
    int t = 0, i = 0, j = 0, Mi1= 0, Mj = 0;

    float foOpt = 0;

    t = n; // use a circular list
    for (i = 0; i < t; i++)
    {
        j = i + 2;
        while (((j+1)%t) != i)
        {
            int vi  = route[i].point;
			int vi1 = route[(i+1)%t].point;
			int vj  = route[j%t].point;
			int vj1 = route[(j+1)%t].point;

			foOpt = - dist[vi][vi1]
					- dist[vj][vj1]
					+ dist[vi][vj]
					+ dist[vi1][vj1];

			if (foOpt < 0)
			{
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

				for(int k=0; k < tam/2; k++)
				{
					aux = route[p1%t].point;
					route[p1%t].point = route[p2%t].point;
					route[p2%t].point = aux;

					p1 = (p1 == t-1)?0:p1+1;
					p2 = (p2 == 0)?t-1:p2-1;
				}
			}
			j++;
        }//while
    }//for

    return route;
}

vector <Customer> decShift(vector <Customer> route, int n, vector<vector<double>> const& dist, float fo) // Shift
{
    int t = 0, i = 0, j = 0, Mi1= 0, Mj = 0;

    float foShift = fo;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int vi  = route[i].point;
			int vip = route[(i+n-1)%n].point;
            int vis = route[(i+1)%n].point;
			int vj  = route[j].point;
            int vjp = route[(j+n-1)%n].point;
			int vjs = route[(j+1)%n].point;

            float foAux = foShift - route[i].dist - route[(i+1)%n].dist - route[j].dist - route[(j+1)%n].dist + dist[vip][vj] + dist[vj][vis] + dist[vjp][vi] + dist[vi][vjs];
            
            if (foAux < foShift) {
                foShift = foAux;
                //swap
                
                int aux = route[i].point;
                route[i].point = route[j].point;
                route[j].point = aux;
                //update distances
                route[i].dist = dist[vip][vj];
                route[(i+1)%n].dist = dist[vj][vis];
                route[j].dist = dist[vjp][vi];
                route[(j+1)%n].dist = dist[vi][vjs];                
            }
        }//for j
    }//for i

    return route;
}


TSol Decoder(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist)
{
    // store the random-key sequence of current solution
    TSol temp = s;
    
    int numDecoders = 4;

    // create a initial solution of the problem
    s.fo = 0;
    for (int i=0; i<n; i++)
        s.vec[i].sol = i + 1;
    
    int dec = floor(s.vec[n].rk*numDecoders)+1;
    // int dec = 3;

    double totalSize = 0.0; /** Total size. */
    for (auto const& v: vertex)
        totalSize += v.size;
    int minVehicles = capacity > 0.0 ? ceil(totalSize / capacity) : -1; /** Minimum number of vehicles. */

    switch (dec) {
        case 1: // sort decoder
            s = Dec1(s, v, n, capacity, vertex, dist);
            break;

        case 2: // cheapest insertion decoder
            s = Dec2(s, v, n, capacity, vertex, dist);
            break;
        
        case 3: // 2-opt
            s = Dec3(s, v, n, capacity, vertex, dist, minVehicles);
            break;

        case 4: // shift
            s = Dec4(s, v, n, capacity, vertex, dist, minVehicles);
            break;

        default:
            break;
    }

    // return initial random-key sequence and maintain the solution sequence
    for (int i=0; i<n; i++)
    {
        s.vec[i].rk = temp.vec[i].rk;
    }

    return s;
}

TSol Dec1(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist)
{
    // sort random-key vector
    sort(s.vec.begin(), s.vec.end() - 1, [](const TVecSol &lhs, const TVecSol &rhs) { return lhs.rk < rhs.rk; });

    // Null.
    s.fo = 0.0;

    Vehicle curVec; /**< Current vehicle. */
    Customer curCus{}; /**< Current customer. */

    curVec.load = 0.0; /**< Vehicle load. */
    curVec.route.push_back(curCus);

    for (int i = 0; i < n; i++) {
        curCus.point = s.vec[i].sol;
        if ((curVec.load + vertex[curCus.point].size) <= capacity) {
            curVec.load += vertex[curCus.point].size;
            curCus.dist = dist[curVec.route.back().point][curCus.point]; /**< Distance from the last customer in the vehicle so far to the current customer. */
            curVec.route.push_back(curCus);
            s.fo +=  curCus.dist;
        } else {
            curVec.route[0].dist = dist[curVec.route.back().point][0]; /**< Distance from the last customer back to depot. */
            s.fo += dist[curVec.route.back().point][0];
            v.push_back(curVec); /**< Store the currente vehicle. */
            /** Clear the vehicle and start a new one. */
            curVec.route.clear();
            Customer depot{};
            curVec.route.push_back(depot);
            curCus.dist = dist[0][curCus.point]; /**< Set distance form depot to current customer. */
            curVec.route.push_back(curCus);
            curVec.load = vertex[curCus.point].size;
            s.fo += curCus.dist;
        }
    }
    curVec.route[0].dist = dist[curVec.route.back().point][0];
    s.fo += dist[curVec.route.back().point][0];
    v.push_back(curVec); /**< Store the last vehicle. */

    return s;
}

TSol Dec2(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist)
{
    // sort random-key vector
    sort(s.vec.begin(), s.vec.end() - 1, [](const TVecSol &lhs, const TVecSol &rhs) { return lhs.rk < rhs.rk; });

    Vehicle curVec; /**< Current vehicle. */
    Customer curCus{}; /**< Current customer. */

    curVec.load = 0.0; /**< Vehicle load. */
    curVec.route.push_back(curCus);
    double dif = 0; /**< Difference between existing path and possible new path. */

    /** Inserting first current on the first vehicle */
    curCus.point = s.vec[0].sol;
    curCus.dist = dist[0][curCus.point];
    curVec.load = vertex[curCus.point].size;
    curVec.route.push_back(curCus);
    s.fo = curCus.dist;

    for (int i = 1; i < n; i++)
    {
        curCus.point = s.vec[i].sol;
        if ((curVec.load + vertex[curCus.point].size) <= capacity)
        {
            curVec.load += vertex[curCus.point].size;
            /** Search for the cheapest position to insert the curNode */
            double bestDif = dist[0][curCus.point] + dist[curCus.point][curVec.route[1].point] - curVec.route[1].dist;
            int bestPos = 1;
            
            /** Check which position is the cheapest. */
            for (int j = 2; j < curVec.route.size(); j++) {
                dif = dist[curVec.route[j-1].point][curCus.point] + dist[curCus.point][curVec.route[j].point] - curVec.route[j].dist;
                if (dif < bestDif) {
                    bestDif = dif;
                    bestPos = j;
                }
            }
            dif = dist[curVec.route.back().point][curCus.point] + dist[curCus.point][0] - dist[curVec.route.back().point][0]; /**< Check last position. */
            
            /** insert customer in the cheapest position */
            if (dif < bestDif) { // end of the vehicle
                curCus.dist = dist[curVec.route.back().point][curCus.point];
                curVec.route.push_back(curCus);
                s.fo += curCus.dist;
            } else { // other position
                curCus.dist = dist[curVec.route[bestPos-1].point][curCus.point];
                curVec.route.insert(curVec.route.begin() + bestPos, curCus);
                curVec.route[bestPos+1].dist = dist[curCus.point][curVec.route[bestPos+1].point];
                s.fo += bestDif;
            }
        } else {
            curVec.route[0].dist = dist[curVec.route.back().point][0]; /**< Distance from the last customer back to depot. */
            s.fo += dist[curVec.route.back().point][0];
            v.push_back(curVec);  /**< Store the currente vehicle. */
            /** Clear the vehicle and start a new one. */
            curVec.route.clear();
            Customer depot{};
            curVec.route.push_back(depot);
            curCus.dist = dist[0][curCus.point]; /**< Set distance from depot to current customer. */
            curVec.route.push_back(curCus);
            curVec.load = vertex[curCus.point].size;
            s.fo += curCus.dist;
        }
    }
    curVec.route[0].dist = dist[curVec.route.back().point][0];
    s.fo += dist[curVec.route.back().point][0];
    v.push_back(curVec); /**< Store the last vehicle. */
    
    return s;

}

TSol Dec3(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist, int const& minVehicles)
{
    // sort random-key vector
    sort(s.vec.begin(), s.vec.end() - 1, [](const TVecSol &lhs, const TVecSol &rhs) { return lhs.rk < rhs.rk; });

    // Null.
    s.fo = 0.0;

    Vehicle tempVec{}; /**< Points to add to new vehicle. */
    
    int m = floor(n / minVehicles); /**< Initial estimate of the number of customers in each vehicle. */
    
    /** Assigning customers to predetermined vehicles. */
    for(int i = 0; i < minVehicles; i++) {
        int k = m*i; /**< Index in s.vec to the first customer of current vehicle. */
        Customer curCus{}; /**< Initialized with 0. */
        Vehicle curVec{}; /**< Points on current vehicle. */
        curVec.route.push_back(curCus);

        if(i == minVehicles - 1)
            m += n % minVehicles; /**< Assigning extra customers to the last vehicle. */
        
        for(int j = 0; j < m; j++) {
            int index = j + k; /**< Index of the current customer on s.vec. */
            curCus.point = s.vec[index].sol;
            if ((curVec.load + vertex[curCus.point].size) <= capacity) {
                curVec.route.push_back(curCus);
                curVec.load += vertex[curCus.point].size;
            } else {
                tempVec.route.push_back(curCus);
                tempVec.load += vertex[curCus.point].size;
            }
        }
        v.push_back(curVec); /**< Store the currente vehicle. */
    }
    /** Assigning customers to extra vehicles. */
    if(tempVec.load <= capacity) {
        Customer depot{};
        auto vecPos = tempVec.route.begin();
        vecPos = tempVec.route.insert(vecPos, depot);
        v.push_back(tempVec);
    } else {
        Customer depot{};
        Vehicle curVec{};
        curVec.route.push_back(depot);
        for(int i = 0; i < tempVec.route.size(); i++) {
            Customer curCus = tempVec.route[i];
            if(curVec.load + vertex[curCus.point].size <= capacity) {
                curVec.route.push_back(curCus);
                curVec.load += vertex[curCus.point].size;
            } else {
                v.push_back(curVec);
                curVec.route.clear();
                depot.point = 0;
                curVec.route.push_back(depot);
                curVec.route.push_back(curCus);
                curVec.load = vertex[curCus.point].size;
            }
        }
        v.push_back(curVec);
    }
    
    /** Calls 2-opt function for all vehicles. */
    for(int i = 0; i < v.size(); i++) {
        float fo;
        if(v[i].route.size() > 4) {
            v[i].route = dec2opt(v[i].route, v[i].route.size(), dist); /**< apply 2-opt for all vehicles. */
            decDepotFirst(v[i].route);
        }
        v[i].route[0].dist = dist[v[i].route.back().point][v[i].route[0].point];
        s.fo += v[i].route[0].dist;
        for(int j = 1; j < v[i].route.size(); j++) {
            v[i].route[j].dist = dist[v[i].route[j-1].point][v[i].route[j].point];
            s.fo += v[i].route[j].dist;
        }
    }

    return s;
}


TSol Dec4(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist, int const& minVehicles)
{
    // sort random-key vector
    sort(s.vec.begin(), s.vec.end() - 1, [](const TVecSol &lhs, const TVecSol &rhs) { return lhs.rk < rhs.rk; });

    // Null.
    s.fo = 0.0;

    //vector <Vehicle> vehicles; /**< Routes of all vehicles. */
    Vehicle tempVec{}; /**< Points to add to new vehicle. */
    
    int m = floor(n / minVehicles); /**< Initial estimate of the number of customers in each vehicle. */
    
    /** Assigning customers to predetermined vehicles. */
    for(int i = 0; i < minVehicles; i++) {
        int k = m*i; /**< Index in s.vec to the first customer of current vehicle. */
        Customer curCus{}; /**< Initialized with 0. */
        Vehicle curVec{}; /**< Points on current vehicle. */
        curVec.route.push_back(curCus);

        if(i == minVehicles - 1)
            m += n % minVehicles; /**< Assigning extra customers to the last vehicle. */
        
        for(int j = 0; j < m; j++) {
            int index = j + k; /**< Index of the current customer on s.vec. */
            curCus.point = s.vec[index].sol;
            if ((curVec.load + vertex[curCus.point].size) <= capacity) {
                curVec.route.push_back(curCus);
                curVec.load += vertex[curCus.point].size;
            } else {
                tempVec.route.push_back(curCus);
                tempVec.load += vertex[curCus.point].size;
            }
        }
        // vehicles.push_back(curVec); /**< Store the currente vehicle. */
        v.push_back(curVec); /**< Store the currente vehicle. */
    }
    /** Assigning customers to extra vehicles. */
    if(tempVec.load <= capacity) {
        Customer depot{};
        auto vecPos = tempVec.route.begin();
        vecPos = tempVec.route.insert(vecPos, depot);
        // vehicles.push_back(tempVec);
        v.push_back(tempVec);
    } else {
        Customer depot{};
        Vehicle curVec{};
        curVec.route.push_back(depot);
        for(int i = 0; i < tempVec.route.size(); i++) {
            Customer curCus = tempVec.route[i];
            if(curVec.load + vertex[curCus.point].size <= capacity) {
                curVec.route.push_back(curCus);
                curVec.load += vertex[curCus.point].size;
            } else {
                // vehicles.push_back(curVec);
                v.push_back(curVec);
                curVec.route.clear();
                depot.point = 0;
                curVec.route.push_back(depot);
                curVec.route.push_back(curCus);
                curVec.load = vertex[curCus.point].size;
            }
        }
        // vehicles.push_back(curVec);
        v.push_back(curVec);
    }
    
    /** Calls shift function for all vehicles. */
    for(int i = 0; i < v.size(); i++) {
        float fo;
        v[i].route[0].dist = dist[v[i].route.back().point][v[i].route[0].point];
        fo += v[i].route[0].dist;
        for(int j = 1; j < v[i].route.size(); j++) {
            v[i].route[j].dist = dist[v[i].route[j-1].point][v[i].route[j].point];
            fo += v[i].route[j].dist;
        }
        if(v[i].route.size() > 1) {
            v[i].route = decShift(v[i].route, v[i].route.size(), dist, fo); /**< apply shift for all vehicles. */

            decDepotFirst(v[i].route);
        }
        v[i].route[0].dist = dist[v[i].route.back().point][v[i].route[0].point];
        s.fo += v[i].route[0].dist;
        for(int j = 1; j < v[i].route.size(); j++) {
            v[i].route[j].dist = dist[v[i].route[j-1].point][v[i].route[j].point];
            s.fo += v[i].route[j].dist;
        }
    }

    return s;
}
