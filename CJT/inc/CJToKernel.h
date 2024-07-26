#pragma once

#include "CJT.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>

#include <nlohmann/json.hpp>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Vec.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRep_Builder.hxx>
#include <GC_MakePlane.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <IntCurvesFace_Intersector.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepBndLib.hxx>
#include <GeomLProp_SLProps.hxx>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 3, bg::cs::cartesian> BoostPoint3D;
typedef std::pair<bg::model::box<BoostPoint3D>, int> Value;

namespace CJT {
	class Edge 
	{
	private:
		gp_Pnt sPoint_;
		gp_Pnt ePoint_;
		bool processed_ = false;

	public:
		/// @brief Constructs an edge object via an start and end point
		Edge(gp_Pnt sPoint, gp_Pnt ePoint);
		/// @brief returns the starting point
		gp_Pnt getStart() { return sPoint_; }
		/// @brief returns the end point
		gp_Pnt getEnd() { return ePoint_; }
		/// @brief returns true if the edge has been processed 
		bool isProcessed() { return processed_; }
		/// @brief sets processed to true (do not use!)
		void setIsProcessed() { processed_ = true; }
		/// @brief flips the start and end point
		void reverse();
	};

	class EdgeCollection 
	{
	private:
		std::vector<std::shared_ptr<Edge>> ring_;
		std::vector<std::shared_ptr<EdgeCollection>>  innerRingList_;
		gp_Vec normal_ = gp_Vec(0,0,1);
		TopoDS_Face originalFace_;
		bool isInner_ = false;
	public:
		// @brief returns the Edge objects from the outer ring of the Edgecollection
		std::vector<std::shared_ptr<Edge>> getEdges() { return ring_; }
		/// @brief returns the Edgecollection objects make up the inner rings of the Edgecollection
		std::vector<std::shared_ptr<EdgeCollection>> getInnerRings() { return innerRingList_; }
		/// @brief sets the Edge objects of the outer ring
		void setEdges(const std::vector<std::shared_ptr<Edge>>& edgeList) { ring_ = edgeList; }
		/// @brief adds an Edge object to the outer ring
		void addEdge(std::shared_ptr<Edge> edge) { ring_.emplace_back(edge); }
		/// @brief returns the start points of every Edge object in the outer ring
		std::vector<gp_Pnt> getStartPoints() const;
		/// @brief adds an Edgecollection representing an inner ring 
		void addInnerRing(std::shared_ptr<EdgeCollection> innerRing) { innerRingList_.emplace_back(innerRing); }
		/// @brief returns true if the normal has a positive Z direction
		bool hasPositiveNormal() const;
		/// @brief gets the Edge object from all the rings of the collection
		std::vector<std::shared_ptr<Edge>> getAllEdges();
		/// @bief computes the normal of the Edgecollection
		void computeNormal();
		/// @brief get the normal
		gp_Vec getNormal() const { return normal_; }
		/// @brief set if ring is inner ring
		void setIsInner() { isInner_ = true; }
		/// @brief get if ring is inner ring
		bool isInner() const { return isInner_; }
		/// @brief sets the original face
		void setOriginalFace(const TopoDS_Face& face) { originalFace_ = face;}
		/// @brief get the original face geometry
		TopoDS_Face getOriginalFace() const { return originalFace_; }
		/// @brief orders the edges by connecting the Endpoint to the Startpoint of the next edge, creating an continuous loop
		void orderEdges();
		/// @brief flips the normal/Edge order
		void flipFace();
	};

	class Kernel 
	{
	private:
		double fprecision = 0;
		int idCounter_ = 10000;
		std::map<int, TopoDS_Shape* > internalizedObjectMap_;
		std::shared_ptr<CityCollection> cityCollection_;

		static const int treeDepth = 25;

		int findTopEdgeCollection(const std::vector<std::shared_ptr<EdgeCollection>>& edgeCollectionList);

		void correctFaceDirection(std::vector<std::shared_ptr<EdgeCollection>> edgeCollectionList);

		bool checkIfInit();

	public:
		/// @brief constructs kernel and internaliz the cityCollection, required to convert.
		Kernel(const std::shared_ptr<CityCollection>& cityCollection);

		/// @brief create OpenCASCADE TopoDS_Shape from CityJSON GeoObject
		TopoDS_Shape convertToCascade(GeoObject& geoObject);
		/// @brief create multiple OpenCASCADE TopoDS_shape objects from CityJSON CityObject
		std::vector<TopoDS_Shape> convertToCascade(CityObject& cityObject);

		/// @brief places the shape into the internalized cityObject as geoObject, the geoobject itself is not placed in the cityObject!
		GeoObject convertToJSON(const TopoDS_Shape& shape, std::string lod, bool trustedSolid = false);
	};
}