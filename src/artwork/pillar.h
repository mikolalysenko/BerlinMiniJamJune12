struct PillarFunc {
	Cell operator()(Eigen::Vector3f v) const {
		using namespace Eigen;
		
		Cell result;		
		result.density = cylinder(v, Vector3f(0, 1, 0), 4);
		
		return result;
	}
};

struct PillarAttr {
	Vertex operator()(Eigen::Vector3f const& v) const {
		using namespace Eigen;
		
		Vertex result;
		result.position = v;
		result.color = drand48() * Vector3f(0.9, 0.9, 0.9) * drand48();
		
		return result;
	}
};

Solid* make_pillar() {
	PillarFunc model;
	PillarAttr style;
	auto art = new Solid(
		Vector3i(10, 32, 10),
		Vector3f(-5, -10, -5),
		Vector3f( 5,  10,  5));
	setup_solid(*art, model, style);
	return art;
}
