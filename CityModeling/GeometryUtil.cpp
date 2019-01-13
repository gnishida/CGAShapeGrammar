#include "GeometryUtil.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include "clipper.hpp"

namespace procedural_generation {

	// Triangulation
	// https://doc.cgal.org/latest/Triangulation_2/index.html#Chapter_2D_Triangulations
	// https://doc.cgal.org/latest/Triangulation_2/Triangulation_2_2polygon_triangulation_8cpp-example.html

	struct FaceInfo2 {
		int nesting_level = -1;
		bool in_domain() {
			return nesting_level % 2 == 1;
		}
	};

	using CGALKernel = CGAL::Exact_predicates_exact_constructions_kernel;
	using CGALVertexBase = CGAL::Triangulation_vertex_base_2<CGALKernel>;
	using CGALFaceBaseWithInfo = CGAL::Triangulation_face_base_with_info_2<FaceInfo2, CGALKernel>;
	using CGALFaceBase = CGAL::Constrained_triangulation_face_base_2<CGALKernel, CGALFaceBaseWithInfo>;
	using CGALTriangulationDataStruct = CGAL::Triangulation_data_structure_2<CGALVertexBase, CGALFaceBase>;
	using CGALConstrainedDelaunayTriangulation = CGAL::Constrained_Delaunay_triangulation_2<CGALKernel, CGALTriangulationDataStruct, CGAL::Exact_predicates_tag>;

	static void mark_domains(CGALConstrainedDelaunayTriangulation& ct, CGALConstrainedDelaunayTriangulation::Face_handle start, int index, std::list<CGALConstrainedDelaunayTriangulation::Edge>& border) {
		if (start->info().nesting_level != -1) {
			return;
		}

		std::list<CGALConstrainedDelaunayTriangulation::Face_handle> queue;
		queue.push_back(start);
		while (!queue.empty()) {
			CGALConstrainedDelaunayTriangulation::Face_handle fh = queue.front();
			queue.pop_front();
			if (fh->info().nesting_level == -1) {
				fh->info().nesting_level = index;
				for (int i = 0; i < 3; i++) {
					CGALConstrainedDelaunayTriangulation::Edge e(fh, i);
					CGALConstrainedDelaunayTriangulation::Face_handle n = fh->neighbor(i);
					if (n->info().nesting_level == -1) {
						if (ct.is_constrained(e)) {
							border.push_back(e);
						}
						else {
							queue.push_back(n);
						}
					}
				}
			}
		}
	}

	static void mark_domains(CGALConstrainedDelaunayTriangulation& cdt) {
		for (CGALConstrainedDelaunayTriangulation::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it) {
			it->info().nesting_level = -1;
		}

		std::list<CGALConstrainedDelaunayTriangulation::Edge> border;
		mark_domains(cdt, cdt.infinite_face(), 0, border);
		while (!border.empty()) {
			CGALConstrainedDelaunayTriangulation::Edge e = border.front();
			border.pop_front();
			CGALConstrainedDelaunayTriangulation::Face_handle n = e.first->neighbor(e.second);
			if (n->info().nesting_level == -1) {
				mark_domains(cdt, n, e.first->info().nesting_level + 1, border);
			}
		}
	}

	CGAL::Polygon_2<CGALKernel> ConvertPolygonToCGALPolygon(const Polygon2D& polygon) {
		CGAL::Polygon_2<CGALKernel> cgalPolygon;

		for (const auto& p : polygon) {
			cgalPolygon.push_back(CGALKernel::Point_2(p.x, p.y));
		}

		if (!cgalPolygon.is_simple()) {
			for (const auto& p : polygon) {
				std::cout << "(" << p.x << "," << p.y << ") ";
			}
			std::cout << std::endl;
			throw std::runtime_error("ConvertPolygonToCGALPolygon(): Polygon is not simple.");
		}

		return cgalPolygon;
	}

	CGAL::Polygon_with_holes_2<CGALKernel> ConvertPolygonToCGALPolygonWithHoles(const Polygon2D& polygon) {
		CGAL::Polygon_2<CGALKernel> cgalPolygon = ConvertPolygonToCGALPolygon(polygon);

		CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygonWithHoles(cgalPolygon);
		return cgalPolygonWithHoles;
	}

	CGAL::Polygon_with_holes_2<CGALKernel> ConvertPolygonToCGALPolygonWithHoles(const Polygon2DWithHoles& polygonWithHoles) {
		CGAL::Polygon_2<CGALKernel> cgalPolygon = ConvertPolygonToCGALPolygon(polygonWithHoles.outerContour);

		std::vector<CGAL::Polygon_2<CGALKernel>> cgalHoles;
		for (const auto& hole : polygonWithHoles.holes) {
			CGAL::Polygon_2<CGALKernel> cgalHole = ConvertPolygonToCGALPolygon(hole);
			cgalHoles.push_back(cgalHole);
		}

		CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygonWithHoles(cgalPolygon, cgalHoles.begin(), cgalHoles.end());
		return cgalPolygonWithHoles;
	}


	std::vector<Polygon2D> Triangulate(const Polygon2D& poly) {
		return Triangulate(poly, {});
	}

	std::vector<Polygon2D> Triangulate(const Polygon2D& poly, const std::vector<Polygon2D>& holes) {
		// construct two non-intersecting nested polygons 
		CGAL::Polygon_2<CGALKernel> cgalPolygon;
		for (const auto& p : poly) {
			cgalPolygon.push_back(CGALConstrainedDelaunayTriangulation::Point(p.x, p.y));
		}

		std::vector<Polygon2D> triangulatedPolygons;

		if (cgalPolygon.is_simple()) {
			// insert the polygon into a constrained triangulation
			CGALConstrainedDelaunayTriangulation cdt;
			cdt.insert_constraint(cgalPolygon.vertices_begin(), cgalPolygon.vertices_end(), true);

			// insert the holes into a constrained triangulation
			for (const auto& hole : holes) {
				CGAL::Polygon_2<CGALKernel> cgalHole;
				for (const auto& p : hole) {
					cgalHole.push_back(CGALConstrainedDelaunayTriangulation::Point(p.x, p.y));
				}
				if (cgalHole.is_simple()) {
					cdt.insert_constraint(cgalHole.vertices_begin(), cgalHole.vertices_end(), true);
				}
			}

			// mark facets that are inside the domain bounded by the polygon
			mark_domains(cdt);

			for (auto fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); fit++) {
				if (fit->info().in_domain()) {
					Polygon2D triangulatedPolygon;
					for (size_t i = 0; i < 3; i++) {
						CGALConstrainedDelaunayTriangulation::Vertex_handle vh = fit->vertex(i);
						triangulatedPolygon.push_back(glm::vec2(CGAL::to_double(vh->point().x()), CGAL::to_double(vh->point().y())));
					}

					procedural_generation::RemoveRedundantPoints(triangulatedPolygon);
					if (triangulatedPolygon.size() >= 3) {
						triangulatedPolygons.push_back(triangulatedPolygon);
					}
				}
			}
		}

		return triangulatedPolygons;
	}

	std::vector<Polygon2DWithHoles> UnionPolygons(const Polygon2D& polygon1, const Polygon2D& polygon2) {
		CGAL::Polygon_set_2<CGALKernel> cgalPolygons;

		CGAL::Polygon_2<CGALKernel> cgalPolygon1;
		for (const auto& p : polygon1) {
			cgalPolygon1.push_back(CGALKernel::Point_2(p.x, p.y));
		}
		if (!cgalPolygon1.is_simple()) {
			for (const auto& p : polygon1) {
				std::cout << "(" << p.x << "," << p.y << ") ";
			}
			std::cout << std::endl;
			throw std::runtime_error("UnionPolygons(): Input polygon is not simple.");
		}

		CGAL::Polygon_2<CGALKernel> cgalPolygon2;
		for (const auto& p : polygon2) {
			cgalPolygon2.push_back(CGALKernel::Point_2(p.x, p.y));
		}
		if (!cgalPolygon2.is_simple()) {
			for (const auto& p : polygon2) {
				std::cout << "(" << p.x << "," << p.y << ") ";
			}
			std::cout << std::endl;
			throw std::runtime_error("UnionPolygons() Input polygon is not simple.");
		}

		CGAL::Polygon_with_holes_2<CGALKernel> unionPolygon;
		if (CGAL::join(cgalPolygon1, cgalPolygon2, unionPolygon)) {
			Polygon2DWithHoles result;
			for (auto it = unionPolygon.outer_boundary().vertices_begin(); it != unionPolygon.outer_boundary().vertices_end(); it++) {
				result.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = unionPolygon.holes_begin(); it != unionPolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				result.holes.push_back(hole);
			}
			return{ result };
		}
		else {
			return{ { polygon1 }, { polygon2 } };
		}
	}

	std::vector<Polygon2DWithHoles> UnionPolygons(const std::vector<Polygon2D>& polygons) {
		// convert the input polygons to the CGAL polygon type
		std::vector<CGAL::Polygon_2<CGALKernel>> cgalPolygons;
		for (const auto& polygon : polygons) {
			CGAL::Polygon_2<CGALKernel> cgalPolygon;
			for (const auto& p : polygon) {
				cgalPolygon.push_back(CGALKernel::Point_2(p.x, p.y));
			}

			cgalPolygons.push_back(cgalPolygon);
		}

		// calculate the union of a list of polygons
		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> cgalJoinedPolygons;
		CGAL::join(cgalPolygons.begin(), cgalPolygons.end(), std::back_inserter(cgalJoinedPolygons));

		// convert the result to the return type
		std::vector<Polygon2DWithHoles> result;
		for (const auto& cgalPolygon : cgalJoinedPolygons) {
			Polygon2DWithHoles polygon;
			for (auto it = cgalPolygon.outer_boundary().vertices_begin(); it != cgalPolygon.outer_boundary().vertices_end(); it++) {
				polygon.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = cgalPolygon.holes_begin(); it != cgalPolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygon.holes.push_back(hole);
			}
			result.push_back(polygon);
		}

		return result;
	}

	std::vector<Polygon2DWithHoles> UnionPolygons(const std::vector<Polygon2DWithHoles>& polygons) {
		// convert the input polygons to the CGAL polygon type
		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> cgalPolygons;
		for (const auto& polygon : polygons) {
			CGAL::Polygon_2<CGALKernel> cgalPolygon;
			for (const auto& p : polygon.outerContour) {
				cgalPolygon.push_back(CGALKernel::Point_2(p.x, p.y));
			}
			std::vector<CGAL::Polygon_2<CGALKernel>> cgalHoles;
			for (const auto& hole : polygon.holes) {
				CGAL::Polygon_2<CGALKernel> cgalHole;
				for (const auto& p : hole) {
					cgalHole.push_back(CGALKernel::Point_2(p.x, p.y));
				}
				cgalHoles.push_back(cgalHole);
			}

			CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygonWithHoles(cgalPolygon, cgalHoles.begin(), cgalHoles.end());
			cgalPolygons.push_back(cgalPolygonWithHoles);
		}

		// calculate the union of a list of polygons
		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> cgalJoinedPolygons;
		CGAL::join(cgalPolygons.begin(), cgalPolygons.end(), std::back_inserter(cgalJoinedPolygons));

		// convert the result to the return type
		std::vector<Polygon2DWithHoles> result;
		for (const auto& cgalPolygon : cgalJoinedPolygons) {
			Polygon2DWithHoles polygon;
			for (auto it = cgalPolygon.outer_boundary().vertices_begin(); it != cgalPolygon.outer_boundary().vertices_end(); it++) {
				polygon.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			RemoveRedundantPoints(polygon.outerContour);
			for (auto it = cgalPolygon.holes_begin(); it != cgalPolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				RemoveRedundantPoints(hole);
				polygon.holes.push_back(hole);
			}
			result.push_back(polygon);
		}

		return result;
	}

	std::vector<Polygon2DWithHoles> DifferencePolygons(const Polygon2D& polygon1, const Polygon2D& polygon2) {
		// convert the input polygons to CGAL polygons
		CGAL::Polygon_2<CGALKernel> cgalPolygon1 = ConvertPolygonToCGALPolygon(polygon1);
		CGAL::Polygon_2<CGALKernel> cgalPolygon2 = ConvertPolygonToCGALPolygon(polygon2);

		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> differencePolygons;
		CGAL::difference(cgalPolygon1, cgalPolygon2, std::back_inserter(differencePolygons));

		std::vector<Polygon2DWithHoles> result;
		for (const auto& differencePolygon : differencePolygons) {
			Polygon2DWithHoles polygonWithHoles;

			for (auto it = differencePolygon.outer_boundary().vertices_begin(); it != differencePolygon.outer_boundary().vertices_end(); it++) {
				polygonWithHoles.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = differencePolygon.holes_begin(); it != differencePolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygonWithHoles.holes.push_back(hole);
			}

			result.push_back(polygonWithHoles);
		}
		return result;
	}

	std::vector<Polygon2DWithHoles> DifferencePolygons(const Polygon2DWithHoles& polygon1, const Polygon2D& polygon2) {
		// convert input polygons to CGAL polygon
		CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygonWithHoles1 = ConvertPolygonToCGALPolygonWithHoles(polygon1);
		CGAL::Polygon_2<CGALKernel> cgalPolygon2 = ConvertPolygonToCGALPolygon(polygon2);

		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> differencePolygons;
		CGAL::difference(cgalPolygonWithHoles1, cgalPolygon2, std::back_inserter(differencePolygons));

		std::vector<Polygon2DWithHoles> result;
		for (const auto& differencePolygon : differencePolygons) {
			Polygon2DWithHoles polygonWithHoles;

			for (auto it = differencePolygon.outer_boundary().vertices_begin(); it != differencePolygon.outer_boundary().vertices_end(); it++) {
				polygonWithHoles.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = differencePolygon.holes_begin(); it != differencePolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygonWithHoles.holes.push_back(hole);
			}

			result.push_back(polygonWithHoles);
		}
		return result;
	}

	std::vector<Polygon2DWithHoles> DifferencePolygons(const std::vector<Polygon2D>& polygons1, const std::vector<Polygon2D>& polygons2) {
		// convert the input polygons to the CGAL polygon type
		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> cgalPolygonWithHolesVec1;
		for (const auto& polygon : polygons1) {
			CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygonWithHoles = ConvertPolygonToCGALPolygonWithHoles(polygon);
			cgalPolygonWithHolesVec1.push_back(cgalPolygonWithHoles);
		}

		std::vector<CGAL::Polygon_2<CGALKernel>> cgalPolygons2;
		for (const auto& polygon : polygons2) {
			CGAL::Polygon_2<CGALKernel> cgalPolygon = ConvertPolygonToCGALPolygon(polygon);
			cgalPolygons2.push_back(cgalPolygon);
		}

		// subtract polygons
		for (const auto& cgalPolygon : cgalPolygons2) {
			std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> newCgalPolygonWithHolesVec1;

			for (const auto& cgalPolygonWithHoles : cgalPolygonWithHolesVec1) {
				std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> differencePolygons;
				CGAL::difference(cgalPolygonWithHoles, cgalPolygon, std::back_inserter(differencePolygons));

				newCgalPolygonWithHolesVec1.insert(newCgalPolygonWithHolesVec1.end(), std::make_move_iterator(differencePolygons.begin()), std::make_move_iterator(differencePolygons.end()));
			}

			cgalPolygonWithHolesVec1 = newCgalPolygonWithHolesVec1;
		}

		std::vector<Polygon2DWithHoles> result;
		for (const auto& cgalPolygonWithHoles : cgalPolygonWithHolesVec1) {
			Polygon2DWithHoles polygonWithHoles;

			for (auto it = cgalPolygonWithHoles.outer_boundary().vertices_begin(); it != cgalPolygonWithHoles.outer_boundary().vertices_end(); it++) {
				polygonWithHoles.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = cgalPolygonWithHoles.holes_begin(); it != cgalPolygonWithHoles.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygonWithHoles.holes.push_back(hole);
			}

			result.push_back(polygonWithHoles);
		}
		return result;
	}

	/**
	* Subtract a set of polygons, polygons2, from a set of polygons, polygons1.
	* Use the bounding box to quickly skip the case when two polygons do not apparently intersect with each other.
	*/
	std::vector<Polygon2DWithHoles> DifferencePolygons(const std::vector<Polygon2DWithHoles>& polygons1, const std::vector<Polygon2D>& polygons2) {
		// convert the input polygons to the CGAL polygon type
		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> cgalPolygonWithHolesVec1;
		std::vector<BoundingBox> boundingBoxes1;
		for (const auto& polygon : polygons1) {
			CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygonWithHoles = ConvertPolygonToCGALPolygonWithHoles(polygon);
			cgalPolygonWithHolesVec1.push_back(cgalPolygonWithHoles);
			boundingBoxes1.push_back(BoundingBox(polygon.outerContour));
		}

		std::vector<CGAL::Polygon_2<CGALKernel>> cgalPolygons2;
		std::vector<BoundingBox> boundingBoxes2;
		for (const auto& polygon : polygons2) {
			CGAL::Polygon_2<CGALKernel> cgalPolygon = ConvertPolygonToCGALPolygon(polygon);
			cgalPolygons2.push_back(cgalPolygon);
			boundingBoxes2.push_back(BoundingBox(polygon));
		}

		// subtract polygons
		for (size_t i = 0; i < cgalPolygons2.size(); i++) {
			std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> newCgalPolygonWithHolesVec1;
			std::vector<BoundingBox> newBoundingBoxes1;

			for (size_t j = 0; j < cgalPolygonWithHolesVec1.size(); j++) {
				if (boundingBoxes1[j].Intersect(boundingBoxes2[i])) {
					std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> differencePolygons;
					CGAL::difference(cgalPolygonWithHolesVec1[j], cgalPolygons2[i], std::back_inserter(differencePolygons));

					newCgalPolygonWithHolesVec1.insert(newCgalPolygonWithHolesVec1.end(), std::make_move_iterator(differencePolygons.begin()), std::make_move_iterator(differencePolygons.end()));
					for (size_t k = 0; k < differencePolygons.size(); k++) {
						newBoundingBoxes1.push_back(boundingBoxes1[j]);
					}
				}
				else {
					newCgalPolygonWithHolesVec1.push_back(cgalPolygonWithHolesVec1[j]);
					newBoundingBoxes1.push_back(boundingBoxes1[j]);
				}
			}

			cgalPolygonWithHolesVec1 = newCgalPolygonWithHolesVec1;
			boundingBoxes1 = newBoundingBoxes1;
		}

		std::vector<Polygon2DWithHoles> result;
		for (const auto& cgalPolygonWithHoles : cgalPolygonWithHolesVec1) {
			Polygon2DWithHoles polygonWithHoles;

			for (auto it = cgalPolygonWithHoles.outer_boundary().vertices_begin(); it != cgalPolygonWithHoles.outer_boundary().vertices_end(); it++) {
				polygonWithHoles.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = cgalPolygonWithHoles.holes_begin(); it != cgalPolygonWithHoles.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygonWithHoles.holes.push_back(hole);
			}

			result.push_back(polygonWithHoles);
		}
		return result;
	}

	std::vector<Polygon2DWithHoles> IntersectPolygons(const Polygon2D& polygon1, const Polygon2D& polygon2) {
		// convert the input polygons to CGAL polygons
		CGAL::Polygon_2<CGALKernel> cgalPolygon1 = ConvertPolygonToCGALPolygon(polygon1);
		CGAL::Polygon_2<CGALKernel> cgalPolygon2 = ConvertPolygonToCGALPolygon(polygon2);

		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> intersectionPolygons;
		CGAL::intersection(cgalPolygon1, cgalPolygon2, std::back_inserter(intersectionPolygons));

		std::vector<Polygon2DWithHoles> result;
		for (const auto& intersectionPolygon : intersectionPolygons) {
			Polygon2DWithHoles polygonWithHoles;

			for (auto it = intersectionPolygon.outer_boundary().vertices_begin(); it != intersectionPolygon.outer_boundary().vertices_end(); it++) {
				polygonWithHoles.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = intersectionPolygon.holes_begin(); it != intersectionPolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygonWithHoles.holes.push_back(hole);
			}

			result.push_back(polygonWithHoles);
		}
		return result;
	}

	std::vector<Polygon2DWithHoles> IntersectPolygons(const Polygon2DWithHoles& polygonWithHoles1, const Polygon2D& polygon2) {
		// convert the input polygons to CGAL polygons
		CGAL::Polygon_with_holes_2<CGALKernel> cgalPolygon1 = ConvertPolygonToCGALPolygonWithHoles(polygonWithHoles1);
		CGAL::Polygon_2<CGALKernel> cgalPolygon2 = ConvertPolygonToCGALPolygon(polygon2);

		std::vector<CGAL::Polygon_with_holes_2<CGALKernel>> intersectionPolygons;
		CGAL::intersection(cgalPolygon1, cgalPolygon2, std::back_inserter(intersectionPolygons));

		std::vector<Polygon2DWithHoles> result;
		for (const auto& intersectionPolygon : intersectionPolygons) {
			Polygon2DWithHoles polygonWithHoles;

			for (auto it = intersectionPolygon.outer_boundary().vertices_begin(); it != intersectionPolygon.outer_boundary().vertices_end(); it++) {
				polygonWithHoles.outerContour.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
			}
			for (auto it = intersectionPolygon.holes_begin(); it != intersectionPolygon.holes_end(); it++) {
				Polygon2D hole;
				for (auto it2 = it->vertices_begin(); it2 != it->vertices_end(); it2++) {
					hole.emplace_back(CGAL::to_double(it2->x()), CGAL::to_double(it2->y()));
				}
				polygonWithHoles.holes.push_back(hole);
			}

			result.push_back(polygonWithHoles);
		}
		return result;
	}

	bool IsIntersect(const Polygon2D& polygon1, const Polygon2D& polygon2) {
		BoundingBox bbox1{ polygon1 };
		BoundingBox bbox2{ polygon2 };
		if (!bbox1.Intersect(bbox2)) return false;

		// convert the input polygons to CGAL polygons
		CGAL::Polygon_2<CGALKernel> cgalPolygon1 = ConvertPolygonToCGALPolygon(polygon1);
		CGAL::Polygon_2<CGALKernel> cgalPolygon2 = ConvertPolygonToCGALPolygon(polygon2);

		return CGAL::do_intersect(cgalPolygon1, cgalPolygon2);
	}

	float DistanceFromPointToSegment(const glm::vec2& pt, const glm::vec2& a, const glm::vec2& b, glm::vec2& closestPt) {
		float r_numerator = (pt.x - a.x) * (b.x - a.x) + (pt.y - a.y) * (b.y - a.y);
		float r_denomenator = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);

		if (r_denomenator <= 0.0f) {
			return glm::length(a - pt);
		}

		float r = r_numerator / r_denomenator;

		if (r < 0 || r > 1) {
			float dist1 = glm::length(pt - a);
			float dist2 = glm::length(pt - b);
			if (dist1 < dist2) {
				closestPt = a;
				return dist1;
			}
			else {
				closestPt = b;
				return dist2;
			}
		}
		else {
			closestPt = a + (b - a) * r;
			return std::abs((a.y - pt.y) * (b.x - a.x) - (a.x - pt.x) * (b.y - a.y)) / std::sqrt(r_denomenator);
		}
	}

	float DistanceFromPointToPolygon(const glm::vec2& pt, const Polygon2D& polygon, glm::vec2& closestPt) {
		float min_dist = std::numeric_limits<float>::max();
		for (size_t i = 0; i < polygon.size(); i++) {
			size_t next = (i + 1) % polygon.size();
			glm::vec2 cpt;
			float dist = DistanceFromPointToSegment(pt, polygon[i], polygon[next], cpt);
			if (dist < min_dist) {
				min_dist = dist;
				closestPt = cpt;
			}
		}
		return min_dist;
	}

	bool IsClockwise(const Polygon2D& points) {
		float area = 0;
		for (size_t i = 0; i < points.size(); i++) {
			size_t next = (i + 1) % points.size();
			area += (points[next].x - points[i].x) * (points[next].y + points[i].y);
		}
		return area > 0;
	}

	void OrientPolygon(Polygon2D& points) {
		if (IsClockwise(points)) {
			std::reverse(points.begin(), points.end());
		}
	}

	void RemoveRedundantPoints(Polygon2D& points) {
		for (size_t i = 1; i < points.size();) {
			if (glm::length(points[i] - points[i - 1]) < 0.001f) {
				points.erase(points.begin() + i);
			}
			else {
				i++;
			}
		}
		if (points.size() >= 2 && glm::length(points.back() - points.front()) < 0.001f) {
			points.pop_back();
		}
	}

	void OrientedBoundingBox(const Polygon2D& polygon, glm::mat4& xformMat, BoundingBox& bbox) {
		float min_area = std::numeric_limits<float>::max();

		for (size_t i = 0; i < polygon.size(); i++) {
			size_t next = (i + 1) % polygon.size();
			glm::vec2 dir = polygon[next] - polygon[i];
			float theta = atan2f(dir.y, dir.x);

			glm::mat4 mat(1.0f);
			mat[0][0] = std::cos(-theta);
			mat[0][1] = std::sin(-theta);
			mat[1][0] = -mat[0][1];
			mat[1][1] = mat[0][0];

			// rotate all the points
			Polygon2D rotatedPolygon(polygon.size());
			for (size_t j = 0; j < polygon.size(); j++) {
				rotatedPolygon[j] = glm::vec2(mat * glm::vec4(polygon[j], 0, 1));
			}

			BoundingBox b(rotatedPolygon);
			float area = b.area();
			if (area < min_area) {
				min_area = area;
				xformMat = glm::inverse(mat);
				bbox = b;
			}
		}
	}

	bool WithinPolygon(const glm::vec2& pt, const Polygon2D& polygon) {
		BoundingBox bbox(polygon);
		if (!bbox.within(pt)) return false;

		CGAL::Polygon_2<CGALKernel> cgalPolygon = ConvertPolygonToCGALPolygon(polygon);
		if (CGAL::bounded_side_2(cgalPolygon.vertices_begin(), cgalPolygon.vertices_end(), CGALKernel::Point_2(pt.x, pt.y), CGALKernel()) == CGAL::ON_BOUNDED_SIDE) return true;
		else return false;
	}

	// CGAL documentation is here
	// https://doc.cgal.org/latest/Straight_skeleton_2/index.html

	void OffsetPolygon(const Polygon2D& points, float offsetDistance, OffsetPolygonType offsetPolygonType, std::vector<Polygon2D>& polygons) {
		if (offsetPolygonType == OffsetPolygonType::CGAL) {
			CGAL::Polygon_2<CGALKernel> poly;

			for (const auto& p : points) {
				poly.push_back(CGALConstrainedDelaunayTriangulation::Point(p.x, p.y));
			}

			if (offsetDistance >= 0) {
				CGALKernel::FT lOffset = offsetDistance;
				std::vector<boost::shared_ptr<CGAL::Polygon_2<CGALKernel>>> offsetPolygons = CGAL::create_exterior_skeleton_and_offset_polygons_2(lOffset, poly);

				Polygon2D offsetPolygon;
				if (offsetPolygons.size() >= 2) {
					for (auto it = offsetPolygons[1]->vertices_begin(); it != offsetPolygons[1]->vertices_end(); it++) {
						offsetPolygon.push_back(glm::vec2(CGAL::to_double(it->x()), CGAL::to_double(it->y())));
					}

					// since the erosion may cause flipping the orientation, we need to reorient the polygon.
					OrientPolygon(offsetPolygon);

					// since the erosion may cause some redundant points, we need to remove them.
					RemoveRedundantPoints(offsetPolygon);
				}
				else {
					// if CGAL skeleton fails, the input polygon will be returned.
					offsetPolygon = points;
				}
				polygons = { offsetPolygon };
			}
			else {
				CGALKernel::FT lOffset = -offsetDistance;
				std::vector<boost::shared_ptr<CGAL::Polygon_2<CGALKernel>>> offsetPolygons = CGAL::create_interior_skeleton_and_offset_polygons_2(lOffset, poly);

				polygons.clear();
				for (const auto& offsetPolygon : offsetPolygons) {
					Polygon2D polygon;
					for (auto it = offsetPolygon->vertices_begin(); it != offsetPolygon->vertices_end(); it++) {
						polygon.emplace_back(CGAL::to_double(it->x()), CGAL::to_double(it->y()));
					}
					polygons.push_back(polygon);
				}
			}
		}
		else {
			const float scale = 1000;

			ClipperLib::Path clipperPolygon;
			for (const auto& p : points) {
				clipperPolygon.emplace_back(p.x * scale, p.y * scale);
			}

			ClipperLib::ClipperOffset co;
			ClipperLib::JoinType joinType;
			if (offsetPolygonType == OffsetPolygonType::SQUARE) {
				joinType = ClipperLib::jtSquare;
			}
			else if (offsetPolygonType == OffsetPolygonType::ROUND) {
				joinType = ClipperLib::jtRound;
			}
			else {
				joinType = ClipperLib::jtMiter;
			}
			co.AddPath(clipperPolygon, joinType, ClipperLib::etClosedPolygon);
			ClipperLib::Paths results;
			co.Execute(results, offsetDistance * scale);

			polygons.clear();
			bool success = true;
			for (const auto& path : results) {
				Polygon2D polygon;
				for (const auto& p : path) {
					polygon.emplace_back((float)p.X / scale, (float)p.Y / scale);
				}

				// since the erosion may cause flipping the orientation, we need to reorient the polygon.
				OrientPolygon(polygon);

				// since the erosion may cause some redundant points, we need to remove them.
				RemoveRedundantPoints(polygon);

				if (!IsSimple(polygon)) {
					success = false;
					break;
				}

				polygons.push_back(polygon);
			}

			// if the Clipper library fails, we will try CGAL function as a backup option.
			if (!success) {
				OffsetPolygon(points, offsetDistance, OffsetPolygonType::CGAL, polygons);
			}
		}
	}

	bool IsSimple(const Polygon2D& polygon) {
		CGAL::Polygon_2<CGALKernel> cgalPolygon;

		for (const auto& p : polygon) {
			cgalPolygon.push_back(CGALKernel::Point_2(p.x, p.y));
		}

		return cgalPolygon.is_simple();
	}

	void GenerateRectangle(float width, float height, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces) {
		glm::vec3 v1(xformMat * glm::vec4(-width * 0.5, -height * 0.5, 0, 1));
		glm::vec3 v2(xformMat * glm::vec4(width * 0.5, -height * 0.5, 0, 1));
		glm::vec3 v3(xformMat * glm::vec4(width * 0.5, height * 0.5, 0, 1));
		glm::vec3 v4(xformMat * glm::vec4(-width * 0.5, height * 0.5, 0, 1));

		glm::vec3 normal(xformMat * glm::vec4(0, 0, 1, 0));

		Face face;
		face.vertices.emplace_back(v1, normal, color);
		face.vertices.emplace_back(v2, normal, color);
		face.vertices.emplace_back(v3, normal, color);
		face.vertices.emplace_back(v4, normal, color);
		faces.push_back(face);
	}

	void GenerateRectangle(float width, float height, const glm::vec4& color, const std::vector<glm::vec2>& texCoords, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces) {
		glm::vec3 v1(xformMat * glm::vec4(-width * 0.5, -height * 0.5, 0, 1));
		glm::vec3 v2(xformMat * glm::vec4(width * 0.5, -height * 0.5, 0, 1));
		glm::vec3 v3(xformMat * glm::vec4(width * 0.5, height * 0.5, 0, 1));
		glm::vec3 v4(xformMat * glm::vec4(-width * 0.5, height * 0.5, 0, 1));

		glm::vec3 normal(xformMat * glm::vec4(0, 0, 1, 0));

		Face face;
		face.vertices.emplace_back(v1, normal, color, texCoords[0]);
		face.vertices.emplace_back(v2, normal, color, texCoords[1]);
		face.vertices.emplace_back(v3, normal, color, texCoords[2]);
		face.vertices.emplace_back(v4, normal, color, texCoords[3]);
		face.texturePath = texturePath;
		faces.push_back(face);
	}

	void GeneratePolygon(const Polygon2D& points, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces) {
		if (points.size() < 3) return;

		glm::vec3 normal(xformMat * glm::vec4(0, 0, 1, 0));

		// triangulate
		std::vector<procedural_generation::Polygon2D> triangulatedPolygons = procedural_generation::Triangulate(points);

		for (auto triangulatedPolygon : triangulatedPolygons) {
			Face face;
			for (const auto& point : triangulatedPolygon) {
				glm::vec3 v(xformMat * glm::vec4(point, 0, 1));
				face.vertices.emplace_back(v, normal, color);
			}
			faces.push_back(face);
		}
	}

	void GeneratePolygon(const std::vector<glm::vec2>& points, const glm::vec4& color, const std::vector<glm::vec2>& texCoords, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces) {
		if (points.size() < 3) return;

		glm::vec3 normal(xformMat * glm::vec4(0, 0, 1, 0));

		Face face;
		for (size_t i = 0; i < points.size(); i++) {
			glm::vec3 v(xformMat * glm::vec4(points[i], 0, 1));
			face.vertices.emplace_back(v, normal, color, texCoords[i]);
		}
		face.texturePath = texturePath;
		faces.push_back(face);
	}

	void GeneratePolygon(const Polygon2D& points, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces) {
		if (points.size() < 3) return;

		glm::vec3 normal(xformMat * glm::vec4(0, 0, 1, 0));

		Face face;
		for (const auto& p : points) {
			glm::vec3 v(xformMat * glm::vec4(p, 0, 1));
			glm::vec2 texCoord;
			if (textureCoordinateCalculationType == TextureCoordinateCalculationType::LOCAL_COORDINATE) {
				texCoord = glm::vec2(p.x / textureWidth, p.y / textureHeight);
			}
			else {
				texCoord = glm::vec2(v.x / textureWidth, v.y / textureHeight);
			}
			face.vertices.emplace_back(v, normal, color, texCoord);
		}
		face.texturePath = texturePath;
		faces.push_back(face);
	}

	void GeneratePolygon(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces) {
		if (polygonWithHoles.outerContour.size() < 3) return;

		std::vector<Polygon2D> triangulatedPolygons = Triangulate(polygonWithHoles.outerContour, polygonWithHoles.holes);
		for (const auto& triangulatedPolygon : triangulatedPolygons) {
			GeneratePolygon(triangulatedPolygon, color, xformMat, faces);
		}
	}

	void GeneratePolygon(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces) {
		if (polygonWithHoles.outerContour.size() < 3) return;

		glm::vec3 normal(xformMat * glm::vec4(0, 0, 1, 0));

		std::vector<Polygon2D> triangulatedPolygons = Triangulate(polygonWithHoles.outerContour, polygonWithHoles.holes);
		for (const auto& triangulatedPolygon : triangulatedPolygons) {
			GeneratePolygon(triangulatedPolygon, color, textureWidth, textureHeight, textureCoordinateCalculationType, texturePath, xformMat, faces);
		}
	}

	void GeneratePolygon(const Polygon3D& points, const glm::vec4& color, const glm::mat4& xformMat, Faces& faces) {
		if (points.size() < 3) return;

		// calculate the normal from the points
		glm::vec3 normal;
		for (size_t i = 0; i < points.size(); i++) {
			int prev = (i - 1 + points.size()) % points.size();
			int next = (i + 1) % points.size();

			glm::vec3 prev_v(xformMat * glm::vec4(points[prev], 1));
			glm::vec3 v(xformMat * glm::vec4(points[i], 1));
			glm::vec3 next_v(xformMat * glm::vec4(points[next], 1));

			glm::vec3 n = glm::cross(v - prev_v, next_v - v);
			if (glm::length(n) > 0.0f) {
				normal = glm::normalize(n);
				break;
			}
		}

		Face face;
		for (const auto& point : points) {
			glm::vec3 v(xformMat * glm::vec4(point, 1));
			face.vertices.emplace_back(v, normal, color);
		}
		faces.push_back(face);
	}

	void GeneratePolygon(const Polygon3D& points, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, Faces& faces) {
		if (points.size() < 3) return;

		// calculate the normal from the points
		glm::vec3 normal;
		for (size_t i = 0; i < points.size(); i++) {
			int prev = (i - 1 + points.size()) % points.size();
			int next = (i + 1) % points.size();

			glm::vec3 prev_v(xformMat * glm::vec4(points[prev], 1));
			glm::vec3 v(xformMat * glm::vec4(points[i], 1));
			glm::vec3 next_v(xformMat * glm::vec4(points[next], 1));

			glm::vec3 n = glm::cross(v - prev_v, next_v - v);
			if (glm::length(n) > 0.0f) {
				normal = glm::normalize(n);
				break;
			}
		}

		Face face;
		for (const auto& point : points) {
			glm::vec3 v(xformMat * glm::vec4(point, 1));
			glm::vec2 texCoord;
			if (textureCoordinateCalculationType == TextureCoordinateCalculationType::LOCAL_COORDINATE) {
				texCoord = glm::vec2(point.x / textureWidth, point.y / textureHeight);
			}
			else {
				texCoord = glm::vec2(v.x / textureWidth, v.y / textureHeight);
			}
			face.vertices.emplace_back(v, normal, color, texCoord);
		}
		face.texturePath = texturePath;
		faces.push_back(face);
	}

	void GeneratePrism(const Polygon2D& points, const glm::vec4& color, const glm::mat4& xformMat, float height, Faces& faces) {
		// top
		GeneratePolygon(points, color, glm::translate(xformMat, glm::vec3(0, 0, height)), faces);

		// bottom
		{
			Polygon2D flipped_points;
			for (const auto& p : points) {
				flipped_points.push_back(glm::vec2(glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(1, 0, 0)) * glm::vec4(p, 0, 1)));
			}
			GeneratePolygon(flipped_points, color, glm::rotate(xformMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
		}

		// side
	{
		glm::vec3 p1(xformMat * glm::vec4(points.back(), 0, 1));
		glm::vec3 p2(xformMat * glm::vec4(points.back(), height, 1));

		for (size_t i = 0; i < points.size(); ++i) {
			glm::vec3 p3(xformMat * glm::vec4(points[i], 0, 1));
			glm::vec3 p4(xformMat * glm::vec4(points[i], height, 1));
			glm::vec3 normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));

			Face face;
			face.vertices.emplace_back(p1, normal, color);
			face.vertices.emplace_back(p3, normal, color);
			face.vertices.emplace_back(p4, normal, color);
			face.vertices.emplace_back(p2, normal, color);
			faces.push_back(face);

			p1 = p3;
			p2 = p4;
		}
	}
	}

	void GeneratePrism(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, const glm::mat4& xformMat, float height, Faces& faces) {
		// top
		GeneratePolygon(polygonWithHoles, color, glm::translate(xformMat, glm::vec3(0, 0, height)), faces);

		// bottom
		// ...
		{
			/*
			Polygon2D flipped_points;
			for (const auto& p : points) {
			flipped_points.push_back(glm::vec3(glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(1, 0, 0)) * glm::vec4(p, 0, 1)));
			}
			GeneratePolygon(flipped_points, color, glm::rotate(xformMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
			*/
		}

		// outer side
	{
		glm::vec3 p1(xformMat * glm::vec4(polygonWithHoles.outerContour.back(), 0, 1));
		glm::vec3 p2(xformMat * glm::vec4(polygonWithHoles.outerContour.back(), height, 1));

		for (const auto& point : polygonWithHoles.outerContour) {
			glm::vec3 p3(xformMat * glm::vec4(point, 0, 1));
			glm::vec3 p4(xformMat * glm::vec4(point, height, 1));
			glm::vec3 normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));

			Face face;
			face.vertices.emplace_back(p1, normal, color);
			face.vertices.emplace_back(p3, normal, color);
			face.vertices.emplace_back(p4, normal, color);
			face.vertices.emplace_back(p2, normal, color);
			faces.push_back(face);

			p1 = p3;
			p2 = p4;
		}
	}

	// inner side
	// ...
	}

	/**
	* Generate prism mesh with texture mapping.
	* The texture coordinates are automatically computed based on the local coordinates and the texture size.
	*/
	void GeneratePrism(const Polygon2D& points, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, float height, Faces& faces) {
		// top
		GeneratePolygon(points, color, textureWidth, textureHeight, textureCoordinateCalculationType, texturePath, glm::translate(xformMat, glm::vec3(0, 0, height)), faces);
		//GeneratePolygon(points, color, glm::translate(xformMat, glm::vec3(0, 0, height)), faces);

		// bottom
		{
			Polygon2D flipped_points;
			for (const auto& p : points) {
				flipped_points.push_back(glm::vec2(glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(1, 0, 0)) * glm::vec4(p, 0, 1)));
			}
			GeneratePolygon(flipped_points, color, textureWidth, textureHeight, textureCoordinateCalculationType, texturePath, glm::rotate(xformMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
			//GeneratePolygon(flipped_points, color, glm::rotate(xformMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
		}

		// side
	{
		glm::vec3 p1(xformMat * glm::vec4(points.back(), 0, 1));
		glm::vec3 p2(xformMat * glm::vec4(points.back(), height, 1));

		glm::vec2 texCoord1;
		if (textureCoordinateCalculationType == TextureCoordinateCalculationType::LOCAL_COORDINATE) {
			texCoord1 = glm::vec2(points.back().x / textureWidth, points.back().y / textureHeight);
		}
		else {
			texCoord1 = glm::vec2(p1.x / textureWidth, p1.y / textureHeight);
		}

		for (size_t i = 0; i < points.size(); ++i) {
			glm::vec3 p3(xformMat * glm::vec4(points[i], 0, 1));
			glm::vec3 p4(xformMat * glm::vec4(points[i], height, 1));
			glm::vec3 normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));

			glm::vec2 texCoord3;
			if (textureCoordinateCalculationType == TextureCoordinateCalculationType::LOCAL_COORDINATE) {
				texCoord3 = glm::vec2(points[i].x / textureWidth, points[i].y / textureHeight);
			}
			else {
				texCoord3 = glm::vec2(p3.x / textureWidth, p3.y / textureHeight);
			}

			Face face;
			face.vertices.emplace_back(p1, normal, color, texCoord1);
			face.vertices.emplace_back(p3, normal, color, texCoord3);
			face.vertices.emplace_back(p4, normal, color, texCoord3);
			face.vertices.emplace_back(p2, normal, color, texCoord1);
			face.texturePath = texturePath;
			faces.push_back(face);

			p1 = p3;
			p2 = p4;
			texCoord1 = texCoord3;
		}
	}
	}

	/**
	* Generate prism mesh with texture mapping.
	* The texture coordinates are automatically computed based on the local coordinates and the texture size.
	*/
	void GeneratePrism(const Polygon2DWithHoles& polygonWithHoles, const glm::vec4& color, float textureWidth, float textureHeight, TextureCoordinateCalculationType textureCoordinateCalculationType, const std::string& texturePath, const glm::mat4& xformMat, float height, Faces& faces) {
		// top
		GeneratePolygon(polygonWithHoles, color, textureWidth, textureHeight, textureCoordinateCalculationType, texturePath, glm::translate(xformMat, glm::vec3(0, 0, height)), faces);
		//GeneratePolygon(points, color, glm::translate(xformMat, glm::vec3(0, 0, height)), faces);

		// bottom
		// TODO...
		/*
		{
		Polygon2D flipped_points;
		for (const auto& p : polygonWithHoles.polygon) {
		flipped_points.push_back(glm::vec3(glm::rotate(glm::mat4(), glm::pi<float>(), glm::vec3(1, 0, 0)) * glm::vec4(p, 0, 1)));
		}
		GeneratePolygon(flipped_points, color, textureWidth, textureHeight, texturePath, glm::rotate(xformMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
		//GeneratePolygon(flipped_points, color, glm::rotate(xformMat, glm::pi<float>(), glm::vec3(1, 0, 0)), faces);
		}
		*/

		// side
		{
			glm::vec3 p1(xformMat * glm::vec4(polygonWithHoles.outerContour.back(), 0, 1));
			glm::vec3 p2(xformMat * glm::vec4(polygonWithHoles.outerContour.back(), height, 1));

			glm::vec2 texCoord1;
			if (textureCoordinateCalculationType == TextureCoordinateCalculationType::LOCAL_COORDINATE) {
				texCoord1 = glm::vec2(polygonWithHoles.outerContour.back().x / textureWidth, polygonWithHoles.outerContour.back().y / textureHeight);
			}
			else {
				texCoord1 = glm::vec2(p1.x / textureWidth, p1.y / textureHeight);
			}

			for (size_t i = 0; i < polygonWithHoles.outerContour.size(); ++i) {
				glm::vec3 p3(xformMat * glm::vec4(polygonWithHoles.outerContour[i], 0, 1));
				glm::vec3 p4(xformMat * glm::vec4(polygonWithHoles.outerContour[i], height, 1));
				glm::vec3 normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));

				glm::vec2 texCoord3(polygonWithHoles.outerContour[i].x / textureWidth, polygonWithHoles.outerContour[i].y / textureHeight);
				if (textureCoordinateCalculationType == TextureCoordinateCalculationType::LOCAL_COORDINATE) {
					texCoord3 = glm::vec2(polygonWithHoles.outerContour[i].x / textureWidth, polygonWithHoles.outerContour[i].y / textureHeight);
				}
				else {
					texCoord3 = glm::vec2(p3.x / textureWidth, p3.y / textureHeight);
				}

				Face face;
				face.vertices.emplace_back(p1, normal, color, texCoord1);
				face.vertices.emplace_back(p3, normal, color, texCoord3);
				face.vertices.emplace_back(p4, normal, color, texCoord3);
				face.vertices.emplace_back(p2, normal, color, texCoord1);
				face.texturePath = texturePath;
				faces.push_back(face);

				p1 = p3;
				p2 = p4;
				texCoord1 = texCoord3;
			}

			// holes
			// TODO...
		}
	}

}
