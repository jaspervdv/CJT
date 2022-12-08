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
		Edge(gp_Pnt sPoint, gp_Pnt ePoint);
		gp_Pnt getStart() { return sPoint_; }
		gp_Pnt getEnd() { return ePoint_; }
		bool isProcessed() { return processed_; }
		void setIsProcessed() { processed_ = true; }

		void reverse();
	};

	class EdgeCollection 
	{
	private:
		std::vector<Edge*> ring_;
		std::vector<EdgeCollection*>  innerRingList_;
		gp_Pnt normal_;

	public:
		std::vector<Edge*> getEdges() { return ring_; }
		std::vector<EdgeCollection*> getInnerRings() { return innerRingList_; }
		void setEdges(std::vector<Edge*> edgeList) { ring_ = edgeList; }
		void addEdge(Edge* edge) { ring_.emplace_back(edge); }
		std::vector<gp_Pnt> getStartPoints();
		void addInnerRing(EdgeCollection* innerRing) { innerRingList_.emplace_back(innerRing); }
		bool hasPositiveNormal();
		std::vector<Edge*> getAllEdges();

		void computeNormal();

		void orderEdges(int idx = 0);
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
		/// @brief initialize the Kernel before use
		Kernel() {};
		Kernel(CityCollection* cityCollection);


		TopoDS_Shape getShape(GeoObject geoObject);
		std::vector<TopoDS_Shape> getShape(CityObject cityObject);

		// places the shape into the internalized cityObject as geoObject, the geoobject itself is not placed
		GeoObject convertShape(const TopoDS_Shape& shape, std::string lod);
	};
}