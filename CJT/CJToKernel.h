#pragma once

#include "CJT.h"
#include <nlohmann/json.hpp>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <BRep_Tool.hxx>

namespace CJT {
	class Edge 
	{
	private:
		gp_Pnt sPoint_;
		gp_Pnt ePoint_;

		Edge* forwardNeighbour_;
		Edge* backwardNeighbour_;

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
		std::vector<Edge*> ring_;
		std::vector<EdgeCollection*>  innerRingList_;
		gp_Pnt normal_;

	public:
		// @brief returns the Edge objects from the outer ring of the Edgecollection
		std::vector<Edge*> getEdges() { return ring_; }
		/// @brief returns the Edgecollection objects make up the inner rings of the Edgecollection
		std::vector<EdgeCollection*> getInnerRings() { return innerRingList_; }
		/// @brief sets the Edge objects of the outer ring
		void setEdges(std::vector<Edge*> edgeList) { ring_ = edgeList; }
		/// @brief adds an Edge object to the outer ring
		void addEdge(Edge* edge) { ring_.emplace_back(edge); }
		/// @brief returns the start points of every Edge object in the outer ring
		std::vector<gp_Pnt> getStartPoints();
		/// @brief adds an Edgecollection representing an inner ring 
		void addInnerRing(EdgeCollection* innerRing) { innerRingList_.emplace_back(innerRing); }
		/// @brief returns true if the normal has a positive Z direction
		bool hasPositiveNormal();
		/// @brief gets the Edge object from all the rings of the collection
		std::vector<Edge*> getAllEdges();
		/// @bief computes the normal of the Edgecollection
		void computeNormal();
		/// @brief orders the edges by connecting the Endpoint to the Startpoint of the next edge, creating an continuous loop
		void orderEdges(int idx = 0);
		/// @brief flips the normal/Edge order
		void flipFace();
	};

	class Kernel 
	{
	private:
		int idCounter_ = 10000;
		std::map<int, TopoDS_Shape > internalizedObjectMap_;
		CityCollection* cityCollection_;

		int findTopEdgeCollection(std::vector<EdgeCollection*> edgeCollectionList);
		void correctFaceDirection(std::vector<EdgeCollection*> edgeCollectionList, int startingIndx);

	public:
		/// @brief constructs kernel and internaliz the cityCollection, required to convert.
		Kernel(CityCollection* cityCollection);

		// @brief create OpenCASCADE TopoDS_Shape from CityJSON GeoObject
		TopoDS_Shape getShape(GeoObject geoObject);
		// @brief create multiple OpenCASCADE TopoDS_shape objects from CityJSON CityObject
		std::vector<TopoDS_Shape> getShape(CityObject cityObject);

		// places the shape into the internalized cityObject as geoObject, the geoobject itself is not placed
		GeoObject convertShape(const TopoDS_Shape& shape, std::string lod);
	};
}