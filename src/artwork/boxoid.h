struct BoxoidFunc {
	Cell operator()(Eigen::Vector3f v) const {
		using namespace Eigen;
		
		Cell result;		
		result.density = sin(v[0]) + sin(v[1]) + sin(v[2]);
		
		return result;
	}
};

struct BoxoidAttr {
	Vertex operator()(Eigen::Vector3f const& v) const {
		using namespace Eigen;
		
		Vertex result;
		result.position = v;
		result.color = drand48() * Vector3f(0.9, 0.9, 0.9) * drand48();
		
		return result;
	}
};

Solid* make_boxoid() {
	BoxoidFunc model;
	BoxoidAttr style;
	auto art = new Solid(
		Vector3i(50, 50, 50),
		Vector3f(-12, -12, -12),
		Vector3f( 12,  12,  12));
	setup_solid(*art, model, style);
	return art;
}
