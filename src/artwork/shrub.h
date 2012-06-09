struct ShrubFunc {
	Cell operator()(Eigen::Vector3f v) const {
		using namespace Eigen;
		
		Cell result;		
		result.density = sphere(v, 2) + 2*simplexNoise3D(v[0], v[1], v[2], 2);
		
		return result;
	}
};

struct ShrubAttr {
	Vertex operator()(Eigen::Vector3f const& v) const {
		using namespace Eigen;
		
		Vertex result;
		result.position = v;
		result.color = drand48() * Vector3f(0, 1, 0);
		
		return result;
	}
};

Solid* make_shrub() {
	ShrubFunc model;
	ShrubAttr style;
	auto art = new Solid(
		Vector3i(16, 16, 16),
		Vector3f(-2, -2, -2),
		Vector3f( 2,  2,  2));
	setup_solid(*art, model, style);
	return art;
}
