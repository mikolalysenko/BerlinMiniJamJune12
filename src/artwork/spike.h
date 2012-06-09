struct SpikeFunc {
	Cell operator()(Eigen::Vector3f v) const {
		using namespace Eigen;
		
		Cell result;		
		result.density = bicone(v, Vector3f(0, 1, 0), 1, 3);
		
		return result;
	}
};

struct SpikeAttr {
	Vertex operator()(Eigen::Vector3f const& v) const {
		using namespace Eigen;
		
		Vertex result;
		result.position = v;
		result.color = Vector3f(1, sin(4 * v[2] * v[1]), 0);
		
		return result;
	}
};

Solid* make_spike() {
	SpikeFunc model;
	SpikeAttr style;
	auto art = new Solid(
		Vector3i(16, 16, 16),
		Vector3f(-5, -5, -5),
		Vector3f( 5,  5,  5));
	setup_solid(*art, model, style);
	return art;
}
