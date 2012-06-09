#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <utility>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "solid.h"
#include "assets.h"
#include "noise.h"

using namespace std;
using namespace Eigen;

namespace Assets {

//Shape definitions
#include "shapes.h"

//Artwork
#include "artwork/torus.h"

};

//Databases
unordered_map<string, Solid*> artwork_database;

//Retrieve some artwork from the database
Solid* get_artwork(const char* asset_name) {
	assert(artwork_database.find(asset_name) != artwork_database.end());
	return artwork_database[asset_name];
}

//Initialize all the artwork
void initialize_assets() {
	using namespace Assets;

	//Create player artwork
	{
		artwork_database["torus"] = make_torus();
	}
}


