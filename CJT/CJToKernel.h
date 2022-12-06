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

	public:
		Edge(gp_Pnt sPoint, gp_Pnt ePoint);
		gp_Pnt getStart() { return sPoint_; }
		gp_Pnt getEnd() { return ePoint_; }

		void reverse();
	};

	class EdgeCollection 
	{
	private:
		std::vector<Edge*> edgeList_;
	public:
		std::vector<Edge*> getEdges() { return edgeList_; }
		void setEdges(std::vector<Edge*> edgeList) { edgeList_ = edgeList; }
		void addEdge(Edge* edge) { edgeList_.emplace_back(edge); }

		void orderEdges();
		void reverseEdges();
	};

	class Kernel 
	{
	private:
		int idCounter_ = 10000;
		std::map<int, TopoDS_Shape > internalizedObjectMap_;
		CityCollection* cityCollection_;

	public:
		/// @brief initialize the Kernel before use
		Kernel() {};
		Kernel(CityCollection* cityCollection);


		TopoDS_Shape getShape(GeoObject geoObject);
		std::vector<TopoDS_Shape> getShape(CityObject cityObject);

		// places the shape into the internalized cityObject
		void convertShape(TopoDS_Shape& shape, std::string lod, std::string objectType);
	};
}