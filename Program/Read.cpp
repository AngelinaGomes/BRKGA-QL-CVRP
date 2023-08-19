#include "Read.h"
#include "Distance.h"
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

/**
 * @brief getJsonFromFile - Gets a json object.
 * @param[in] folder - Folder.
 * @param[in] filename - Filename.
 * @param[out] flagError - True if an error has ocurred.
 * @param[out] errorMessage - Error message.
 * @return Returns a json object.
 *
 * This function uses nlohmann's libraries.
 */
json getJsonFromFile(string const& folder, string const& filename, bool &flagError, string &errorMessage) {

    json js;

    try {
        string filePath = folder + "/" + filename;
        ifstream inFile(filePath.c_str());
        if (inFile.is_open()) {
            string buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            if (!json::accept(buffer)) throw "Error: check for syntax errors without parsing (getJsonFromFile).";
            js = json::parse(buffer);
            inFile.close();
        } else {
            throw "Error: unable to open the file (getJsonFromFile).";
        }
    } catch (const char* message) {
        errorMessage = message;
        flagError = true;
    } catch (json::exception& e) {
        errorMessage = "Error: exception from nlohmann's libraries (getJsonFromFile).";
        flagError = true;
    } catch (...) {
        errorMessage = "Error: default error (getJsonFromFile).";
        flagError = true;
    }

    return js;
}

/**
 * @brief getData - Gets the data from the json file.
 * @param[in] folder - Input folder.
 * @param[in] filename - Input filename.
 * @param[out] name - Instance name.
 * @param[out] region - Region.
 * @param[out] capacity - Vehicle capacity.
 * @param[out] vertex - Vector of vertices.
 * @param[out] errorMessage - Error message.
 * @return Returns true if no error has occurred.
 *
 * This function uses nlohmann's libraries.
 */
void jsonToInstance(json const& jsInstance, string &name, string &region, double &capacity, vector<Vertex> &vertex, bool &flagError, string &errorMessage) {

    // Vertex.
    string id = "";    /**< Id. */
    double lat = 0.0;  /**< Latitude. */
    double lng = 0.0;  /**< Longitude. */
    double size = 0.0; /**< Demand. */

    // Null.
    name = "";
    region = "";
    capacity = 0.0;
    vertex.clear();
    flagError = false;
    errorMessage = "";

    try {

        // If the json object is empty.
        if (jsInstance.empty()) throw "Error: json object is empty (jsonToInstance).";

        // name.
        if (jsInstance.contains("name")) {
            if (jsInstance["name"].is_string()) {
                jsInstance["name"].get_to(name);
                if (name.size() < 1) {
                    throw "Error: <name> is empty (jsonToInstance).";
                }
            } else {
                throw "Error: <name> is not a string (jsonToInstance).";
            }
        } else {
            throw "Error: unable to find <name> (jsonToInstance).";
        }

        // region.
        if (jsInstance.contains("region")) {
            if (jsInstance["region"].is_string()) {
                jsInstance["region"].get_to(region);
                if (region.size() < 1) {
                    throw "Error: <region> is empty (jsonToInstance).";
                }
            } else {
                throw "Error: <region> is not a string (jsonToInstance).";
            }
        } else {
            throw "Error: unable to find <region> (jsonToInstance).";
        }

        // origin.
        if (jsInstance.contains("origin")) {
            if (jsInstance["origin"].is_object()) {

                // Longitude.
                if (jsInstance["origin"].contains("lng")) {
                    if (jsInstance["origin"]["lng"].is_number()) {
                        jsInstance["origin"]["lng"].get_to(lng);
                    } else {
                        throw "Error: <lng> in <origin> is not a number (jsonToInstance).";
                    }
                } else {
                    throw "Error: unable to find <lng> in <origin> (jsonToInstance).";
                }

                // Latitude.
                if (jsInstance["origin"].contains("lat")) {
                    if (jsInstance["origin"]["lat"].is_number()) {
                        jsInstance["origin"]["lat"].get_to(lat);
                    } else {
                        throw "Error: <lat> in <origin> is not a number (jsonToInstance).";
                    }
                } else {
                    throw "Error: unable to find <lat> in <origin> (jsonToInstance).";
                }

                // Assignment.
                id = "";
                size = 0.0;
                vertex.emplace_back(id, lat, lng, size);
            } else {
                throw "Error: <origin> is not an object (jsonToInstance).";
            }
        } else {
            throw "Error: unable to find <origin> (jsonToInstance).";
        }

        // vehicle_capacity.
        if (jsInstance.contains("vehicle_capacity")) {
            if (jsInstance["vehicle_capacity"].is_number()) {
                jsInstance["vehicle_capacity"].get_to(capacity);
                if (capacity <= 0.0) {
                    throw "Error: <vehicle_capacity> must be a positive number (jsonToInstance).";
                }
            } else {
                throw "Error: <vehicle_capacity> is not a number (jsonToInstance).";
            }
        } else {
            throw "Error: unable to find <vehicle_capacity> (jsonToInstance).";
        }

        // deliveries.
        if (jsInstance.contains("deliveries")) {
            if (jsInstance["deliveries"].is_array()) {
                size_t sizeDeliveries = jsInstance["deliveries"].size();
                if (sizeDeliveries > 0) {
                    for (size_t i = 0; i < sizeDeliveries; i++) {
                        if (jsInstance["deliveries"][i].is_object()) {

                            // id.
                            if (jsInstance["deliveries"][i].contains("id")) {
                                if (jsInstance["deliveries"][i]["id"].is_string()) {
                                    jsInstance["deliveries"][i]["id"].get_to(id);
                                    if (id.size() < 1) {
                                        throw "Error: <id> is empty (jsonToInstance).";
                                    }
                                } else {
                                    throw "Error: <id> is not a string (jsonToInstance).";
                                }
                            } else {
                                throw "Error: unable to find <id> (jsonToInstance).";
                            }

                            // point.
                            if (jsInstance["deliveries"][i].contains("point")) {
                                if (jsInstance["deliveries"][i]["point"].is_object()) {
                                    // Longitude.
                                    if (jsInstance["deliveries"][i]["point"].contains("lng")) {
                                        if (jsInstance["deliveries"][i]["point"]["lng"].is_number()) {
                                            jsInstance["deliveries"][i]["point"]["lng"].get_to(lng);
                                        } else {
                                            throw "Error: <lng> in <deliveries> is not a number (jsonToInstance).";
                                        }
                                    } else {
                                        throw "Error: unable to find <lng> in <deliveries> (jsonToInstance).";
                                    }

                                    // Latitude.
                                    if (jsInstance["deliveries"][i]["point"].contains("lat")) {
                                        if (jsInstance["deliveries"][i]["point"]["lat"].is_number()) {
                                            jsInstance["deliveries"][i]["point"]["lat"].get_to(lat);
                                        } else {
                                            throw "Error: <lat> in <deliveries> is not a number (jsonToInstance).";
                                        }
                                    } else {
                                        throw "Error: unable to find <lat> in <deliveries> (jsonToInstance).";
                                    }
                                } else {
                                    throw "Error: <deliveries> does not contain an object (jsonInstance).";
                                }
                            } else {
                                throw "Error: unable to find <point> (jsonToInstance).";
                            }

                            // size.
                            if (jsInstance["deliveries"][i].contains("size")) {
                                if (jsInstance["deliveries"][i]["size"].is_number()) {
                                    jsInstance["deliveries"][i]["size"].get_to(size);
                                    if (size <= 0.0) {
                                        throw "Error: <size> must be a positive number (jsonToInstance).";
                                    }
                                } else {
                                    throw "Error: <size> is not a number (jsonToInstance).";
                                }
                            } else {
                                throw "Error: unable to find <size> (jsonToInstance).";
                            }

                            // Assignment.
                            vertex.emplace_back(id, lat, lng, size);
                        } else {
                            throw "Error: <deliveries> does not contain an object (jsonInstance).";
                        }
                    }
                } else {
                    throw "Error: <deliveries> size must be a positive number (jsonInstance).";
                }
            } else {
                throw "Error: <deliveries> is not an array (jsonInstance).";
            }
        } else {
            throw "Error: unable to find <deliveries> (jsonToInstance).";
        }
    } catch (const char* message) {
        errorMessage = message;
        flagError = true;
    } catch (json::exception& e) {
        errorMessage = "Error: exception from nlohmann's libraries (jsonToInstance).";
        flagError = true;
    } catch (...) {
        errorMessage = "Error: default error (jsonToInstance).";
        flagError = true;
    }
}

/**
 * @brief printJson - Print the json object to a file.
 * @param[in] folder - Folder.
 * @param[in] filename - Filename.
 * @param[in] js - Json object.
 * @param[out] flagError - True if an error has ocurred.
 * @param[out] errorMessage - Error message.
 *
 * This function uses nlohmann's libraries.
 */
void printJson(string const& folder, string const& filename, json const& js, bool &flagError, string &errorMessage) {

    try {
        string filePath = folder + "//" + filename + ".json";
        ofstream outFile(filePath.c_str());
        if (outFile.is_open()) {
            outFile << js.dump(4) << std::endl;
            outFile.close();
        } else {
            throw "Error: unable to open the file (printJson).";
        }
    } catch (const char* message) {
        errorMessage = message;
        flagError = true;
    } catch (json::exception& e) {
        errorMessage = "Error: exception from nlohmann's libraries (printJson).";
        flagError = true;
    } catch (...) {
        errorMessage = "Error: default error (printJson).";
        flagError = true;
    }
}

/**
 * @brief getJsonReport - Gets a json object containing a brief report on the data.
 * @param[in] name - Instance name.
 * @param[in] region - Region.
 * @param[in] capacity - Vehicle capacity.
 * @param[in] vertex - Vector of vertices.
 * @return Returns a json object.
 *
 * This function uses nlohmann's libraries.
 */
json getJsonReport(string const& name, string const& region, double const& capacity, vector<Vertex> const& vertex) {

    json js = json::object();

    // Number of points.
    int numPoints = static_cast<int>(vertex.size());

    // Total size.
    double totalSize = 0.0;
    for (auto const& v: vertex) {
        totalSize += v.size;
    }

    // Minimum number of vehicles.
    int minVehicles = capacity > 0.0 ? ceil(totalSize / capacity) : -1;

    // Fill json.
    js["name"] = name;
    js["region"] = region;
    js["vehicle_capacity"] = capacity;
    js["number_of_points"] = numPoints;
    js["total_size"] = totalSize;
    js["min_vehicles"] = minVehicles;

    return js;
}

/**
 * @brief getJsonDistMatrix - Gets a json object containing the distance matrix.
 * @param[in] name - Instance name.
 * @param[in] region - Region.
 * @param[in] vertex - Vector of vertices.
 * @param[in] dist - Distance matrix.
 * @return Returns a json object.
 *
 * This function uses nlohmann's libraries.
 */
json getJsonDistMatrix(string const& name, string const& region, vector<Vertex> const& vertex, vector<vector<double>> const& dist) {

    json js = json::object();

    // Fill json.
    js["name"] = name;
    js["region"] = region;

    // Array.
    for (size_t i = 0; i < vertex.size(); i++) {
        for (size_t j = 0; j < vertex.size(); j++) {
            // Object.
            json jsObj = json::object();
            jsObj["id1"] = vertex[i].id;
            jsObj["id2"] = vertex[j].id;
            jsObj["distance(id1,id2)"] = dist[i][j];

            // Store.
            js["info"].emplace_back(jsObj);
        }
    }

    return js;
}

/**
 * @brief ReadData - This function is used to read the input data.
 * @param[in] nameTable - Filename.
 * @param[out] name - Instance name.
 * @param[out] region - Region.
 * @param[out] capacity - Vehicle capacity.
 * @param[out] vertex - Vector of vertices.
 * @param[out] dist - Distance matrix.
 *
 * This function uses nlohmann's libraries.
 */
void ReadData(char nameTable[], string &name, string &region, double &capacity, vector<Vertex> &vertex, vector<vector<double>> &dist) {

    // Input.
    string folder = "data"; /**< Current folder. */
    string filename = nameTable; /**< Filename. */

    // Error.
    bool flagError = false;  /**< Error flag. */
    string errorMessage = "No error."; /**< Error message. */

    try {
        // Json object.
        json jsInstance = getJsonFromFile(folder, filename, flagError, errorMessage); /**< Json object. */
        if (flagError) throw errorMessage;
        cout << "\nJson... OK" << endl;

        // Gets the data from the json file.
        jsonToInstance(jsInstance, name, region, capacity, vertex, flagError, errorMessage);
        if (flagError) throw errorMessage;
        cout << "Instance... OK" << endl;

        // Distance matrix.
        dist = getDistMatrixByHaversine_KM(vertex);
        cout << "Distance matrix... OK" << endl;
    } catch (string& message) {
        cout << message << endl;
    } catch (...) {
        cout << "Error - default error (main function)." << endl;
    }
}

/**
 * @brief printReport - Prints a brief report about the input data.
 * @param[in] name - Instance name.
 * @param[in] region - Region.
 * @param[in] capacity - Vehicle capacity.
 * @param[in] vertex - Vector of vertices.
 *
 * This function uses nlohmann's libraries.
 */
void printReport(string const& name, string const& region, double const& capacity, vector<Vertex> const& vertex) {

    // Get the json object.
    json js = getJsonReport(name, region, capacity, vertex);

    // Error.
    bool flagError = false;
    string errorMessage = "";

    // Print json to the file.
    string folder = "output//report";
    printJson(folder, name, js, flagError, errorMessage);
    if (flagError) cout << errorMessage << endl;
}

/**
 * @brief printDistMatrix - Prints the distance matrix.
 * @param[in] name - Instance name.
 * @param[in] region - Region.
 * @param[in] capacity - Vehicle capacity.
 * @param[in] vertex - Vector of vertices.
 *
 * This function uses nlohmann's libraries.
 */
void printDistMatrix(string const& name, string const& region, vector<Vertex> const& vertex, vector<vector<double>> const& dist) {

    // Get the json object.
    json js = getJsonDistMatrix(name, region, vertex, dist);

    // Error.
    bool flagError = false;
    string errorMessage = "";

    // Print json to the file.
    string folder = "output//distance";
    printJson(folder, name, js, flagError, errorMessage);
    if (flagError) cout << errorMessage << endl;
}

/**
 * @brief printSolution - Prints the solution to the json file.
 * @param[in] name - Instance name.
 * @param[in] region - Region.
 * @param[in] capacity - Vehicle capacity.
 * @param[in] vec - Vector of nodes.
 * @param[in] vertex - Vector of vertices.
 * @param[in] dist - Distance matrix.
 *
 * This function uses nlohmann's libraries.
 */
void printSolution(string const& name, string const& region, double const& capacity, vector<int> const& vec, vector<Vertex> const& vertex, vector<vector<double>> const& dist) {

    // Solution.

    double z = 0.0; /**< Objective function value. */
    double load = 0.0; /**< Vehicle load. */
    int prevNode = 0; /**< Previous node. */
    int n = static_cast<int>(vec.size()); /**< Number of customers. */
    int curVehicle = 0; /**< Current vehicle. */
    vector<int> vehicle(n + 1, -1); /**< Vehicle index. */

    for (int i = 0; i < n; i++)
    {
        int curNode = vec[i]; /**< Current node. */
        if ((load + vertex[curNode].size) <= capacity)
        {
            load += vertex[curNode].size;
            vehicle[curNode] = curVehicle;
            z += dist[prevNode][curNode];
            prevNode = curNode;
        } else
        {
            load = vertex[curNode].size;
            curVehicle++;
            vehicle[curNode] = curVehicle;
            z += dist[prevNode][0];
            prevNode = 0;
            z += dist[prevNode][curNode];
            prevNode = curNode;
        }
    }
    z += dist[prevNode][0];

    // Json object.

    json js = json::object();

    // Fill json.
    js["name"] = name;
    js["region"] = region;
    js["vehicle_capacity"] = capacity;
    js["value"] = z;

    // Origin.
    js["origin"]["lng"] = vertex[0].lng;
    js["origin"]["lat"] = vertex[0].lat;

    // Array.
    for (size_t i = 0; i < vec.size(); i++) {
        // Object.
        json jsObj = json::object();
        jsObj["id"] = vertex[vec[i]].id;
        jsObj["lng"] = vertex[vec[i]].lng;
        jsObj["lat"] = vertex[vec[i]].lat;
        jsObj["size"] = vertex[vec[i]].size;
        jsObj["vehicle"] = vehicle[vec[i]];

        // Store.
        js["route"].emplace_back(jsObj);
    }

    // Error.
    bool flagError = false;
    string errorMessage = "";

    // Print json to the file.
    string folder = "output//solution";
    printJson(folder, name, js, flagError, errorMessage);
    if (flagError) cout << errorMessage << endl;
}

/**
 * @brief printSolution - Prints the solution to the json file.
 * @param[in] name - Instance name.
 * @param[in] region - Region.
 * @param[in] capacity - Vehicle capacity.
 * @param[in] vec - Vector of nodes.
 * @param[in] vertex - Vector of vertices.
 * @param[in] dist - Distance matrix.
 *
 * This function uses nlohmann's libraries.
 */
void printSolutionLoggi(string const& name, string const& region, double const& capacity, vector<int> const& vec, vector<Vertex> const& vertex, vector<vector<double>> const& dist, vector<Vehicle> v) {

    // Json object
    json js = json::object();

    // Fill json.
    js["name"] = name;

    for (int i = 0; i < v.size(); i++) {
        json jsObj = json::object();
        jsObj["origin"]["lng"] = vertex[0].lng;
        jsObj["origin"]["lat"] = vertex[0].lat;

        for (int j = 1; j < v[i].route.size(); j++) {
            json jsObj2 = json::object();
            jsObj2["id"] = vertex[v[i].route[j].point].id;
            jsObj2["point"]["lng"] = vertex[v[i].route[j].point].lng;
            jsObj2["point"]["lat"] = vertex[v[i].route[j].point].lat;
            jsObj2["size"] = vertex[v[i].route[j].point].size;

            jsObj["deliveries"].emplace_back(jsObj2);    
        }

        js["vehicles"].emplace_back(jsObj);
    }

    // Error.
    bool flagError = false;
    string errorMessage = "";

    // Print json to the file.
    string folder = "output//solution";
    printJson(folder, name, js, flagError, errorMessage);
    if (flagError) cout << errorMessage << endl;
}

/**
 * @brief FreeMemoryProblem - Frees the memory.
 * @param[out] vertex - Vector of vertices.
 * @param[out] dist - Distance matrix.
 */
void FreeMemoryProblem(vector<Vertex> &vertex, vector<vector<double>> &dist) {
    // specific problem
    vertex.clear();
    dist.clear();
}
