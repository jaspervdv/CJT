#include "CJT.h"
#include "CJToKernel.h"

namespace CJT {

	void printPoint(gp_Pnt p) {
		std::cout << p.X() << ", " << p.Y() << ", " << p.Z() << ", " << std::endl;
	}


	bool isEqual(gp_Pnt p1, gp_Pnt p2) {
		if (p1.X() == p2.X() && p1.Y() == p2.Y() && p1.Z() == p2.Z()) // TODO add uncertainty 
		{
			return true;
		}
		return false;
	}


	bool isPointInList(gp_Pnt p, std::vector<gp_Pnt> pointList) {
		for (size_t i = 0; i < pointList.size(); i++)
		{
			gp_Pnt pC = pointList[i];
			if (pC.X() == p.X() && pC.Y() == p.Y() && pC.Z() == p.Z()) // TODO add uncertainty 
			{
				return true;
			}
		}
		return false;
	}


	std::vector<std::vector<int>> getSurfaceIdx(json* boundaries) {
		std::vector < std::vector<int> >collection;
		bool isInt = false;

		if (boundaries->begin().value().is_number_integer()) { isInt = true; }

		if (isInt)
		{
			std::vector<int> surfaceIdx;
			for (json::iterator obb = boundaries->begin(); obb != boundaries->end(); ++obb)
			{
				json* subvalue = &obb.value();
				surfaceIdx.emplace_back(*subvalue);
			}
			collection.emplace_back(surfaceIdx);
		}
		else {
			for (json::iterator obb = boundaries->begin(); obb != boundaries->end(); ++obb)
			{
				json* subvalue = &obb.value();
				std::vector<std::vector<int>> outputboundaries = getSurfaceIdx(subvalue);
				for (size_t i = 0; i < outputboundaries.size(); i++)
				{
					collection.emplace_back(outputboundaries[i]);
				}
			}
		}
		return collection;
	}

	int furtestPoint(int baseIdx, const std::vector<gp_Pnt>& pointList) {
		double distance = 0;
		int idx = 0;

		gp_Pnt basePoint = pointList[baseIdx];

		for (size_t i = 0; i < pointList.size(); i++)
		{
			if (i == idx){ continue; }
			double localDistance = basePoint.Distance(pointList.at(i));
			if (localDistance > distance)
			{
				distance = localDistance;
				idx = i;
			}
		}
		return idx;
	}

	int furtestPointLine(int p1Idx, int p2Idx, const std::vector<gp_Pnt>& pointList) {
		double distance = 0;
		int idx = 0;

		gp_Pnt p1 = pointList[p1Idx];
		gp_Pnt p2 = pointList[p2Idx];

		for (size_t i = 0; i < pointList.size(); i++)
		{
			if (i == p1Idx || i == p2Idx){ continue; }

			gp_Pnt currentPoint = pointList[i];
			double localDistance = abs(
				(p2.X() - p1.X()) * (p1.Y() - currentPoint.Y()) -
				(p1.X() - currentPoint.X()) * (p2.Y() - p1.Y())
			) / sqrt(
				pow(p2.X() - p1.X(), 2) + pow(p2.Y() - p1.Y(), 2)
			);
			if (localDistance > distance)
			{
				distance = localDistance;
				idx = i;
			}
		}
		return idx;
	}


	Edge::Edge(gp_Pnt sPoint, gp_Pnt ePoint)
	{
		sPoint_ = sPoint;
		ePoint_ = ePoint;
	}


	void Edge::reverse()
	{
		gp_Pnt tempPoint = sPoint_;
		sPoint_ = ePoint_;
		ePoint_ = tempPoint;
	}


	std::vector<gp_Pnt> EdgeCollection::getStartPoints()
	{
		std::vector<gp_Pnt> startPoints;
		for (size_t i = 0; i < ring_.size(); i++) { startPoints.emplace_back(ring_[i]->getStart()); }
		return startPoints;
	}

	bool EdgeCollection::hasPositiveNormal()
	{
		if (normal_.Z() > 0) { return true; }
		return false;
	}

	std::vector<Edge*> EdgeCollection::getAllEdges() {
		std::vector<Edge*> selfEdges = ring_;
		std::vector<EdgeCollection*> selfRings = innerRingList_;
		for (size_t j = 0; j < selfRings.size(); j++)
		{
			std::vector<Edge*> selfInnerEdges = selfRings[j]->getEdges();
			for (size_t k = 0; k < selfInnerEdges.size(); k++)
			{
				selfEdges.emplace_back(selfInnerEdges[k]);
			}
		}
		return selfEdges;
	}


	void EdgeCollection::computeNormal() // creates a unitvector for the face normal
	{
		std::vector<gp_Pnt> startPoints = getStartPoints();
		gp_Pnt p1 = startPoints[0];
		gp_Pnt p2 = startPoints[1];
		gp_Pnt p3 = startPoints[2];

		gp_Pnt pu = gp_Pnt(p2.X() - p1.X(), p2.Y() - p1.Y(), p2.Z() - p1.Z());
		gp_Pnt pv = gp_Pnt(p3.X() - p1.X(), p3.Y() - p1.Y(), p3.Z() - p1.Z());

		gp_Pnt normal =  gp_Pnt(
			pu.Y() * pv.Z() - pu.Z() * pv.Y(),
			pu.Z() * pv.X() - pu.X() * pv.Z(),
			pu.X() * pv.Y() - pu.Y() * pv.X()
		);

		double magnitude = std::pow(std::pow(normal.X(), 2) + std::pow(normal.Y(), 2) + std::pow(normal.Z(), 2), 0.5);
		normal_ = gp_Pnt(normal.X() / magnitude, normal.Y() / magnitude, normal.Z() / magnitude);
	}

	void EdgeCollection::orderEdges(int idx)
	{
		int falsePresent = ring_.size();
		Edge* startEdge = ring_[idx];
		Edge* currentEdge = ring_[idx];
		std::vector<Edge*> cleanedList;
		while (falsePresent != 0)
		{
			if (currentEdge->isProcessed())
			{
				EdgeCollection* innerRing = new EdgeCollection;
				for (size_t i = 0; i < ring_.size(); i++)
				{
					if (!ring_[i]->isProcessed())
					{
						Edge& tempEdge = *ring_[i];
						innerRing->addEdge(&tempEdge);
					}
				}
				innerRing->orderEdges();
				addInnerRing(innerRing);
				falsePresent = falsePresent - innerRing->getEdges().size();
			}

			if (!currentEdge->isProcessed())
			{
				currentEdge->setIsProcessed();
				falsePresent--;

				cleanedList.emplace_back(currentEdge);
				gp_Pnt p2 = currentEdge->getEnd();

				for (size_t i = 0; i < ring_.size(); i++)
				{
					if (ring_[i] == currentEdge) { continue; }

					if (isEqual(p2, ring_[i]->getStart())) // TODO add neighbours data
					{
						currentEdge = ring_[i];
						break;
					}
					else if (isEqual(p2, ring_[i]->getEnd()))
					{
						ring_[i]->reverse();
						currentEdge = ring_[i];
						break;
					}
				}
			}	
		}
		computeNormal();
		ring_ = cleanedList;

		if (innerRingList_.size() == 0) { return; }
		for (size_t i = 0; i < innerRingList_.size(); i++)
		{
			gp_Pnt otherNormal = innerRingList_[i]->normal_;
			if (otherNormal.X() == normal_.X() && otherNormal.Y() == normal_.Y() && otherNormal.Z() == normal_.Z())
			{
				innerRingList_[i]->flipFace();
			}
		}
	}

	void EdgeCollection::flipFace()
	{
		for (size_t i = 0; i < ring_.size(); i++) { ring_[i]->reverse(); }
		std::reverse(ring_.begin(), ring_.end());
		for (size_t i = 0; i < innerRingList_.size(); i++) { innerRingList_[i]->flipFace(); }
	}


	Kernel::Kernel(CityCollection* cityCollection)
	{
		cityCollection_ = cityCollection;
	}


	int Kernel::findTopEdgeCollection(std::vector<EdgeCollection*> edgeCollectionList)
	{
		int highestCollectionIdx = 0;
		double height = -999999999;
		for (size_t i = 0; i < edgeCollectionList.size(); i++)
		{
			std::vector<gp_Pnt> vertCollection = edgeCollectionList[i]->getStartPoints();
			double avHeight = 0;
			for (size_t j = 0; j < vertCollection.size(); j++)
			{
				avHeight += vertCollection[j].Z();
			}

			if (height < avHeight)
			{
				height = avHeight;
				highestCollectionIdx = i;
			}
		}
		return highestCollectionIdx;
	}

	void Kernel::correctFaceDirection(std::vector<EdgeCollection*> edgeCollectionList, int startingIndx)
	{
		// set starting info
		std::vector<int> processedIdx;
		std::vector<int> stepIdx;
		processedIdx.emplace_back(startingIndx);
		stepIdx.emplace_back(startingIndx);
		bool isFirst = true;

		while (true)
		{
			std::vector<int> nextStep;
			nextStep.clear();

			for (size_t st = 0; st < stepIdx.size(); st++)
			{
				// get all edges of self
				EdgeCollection* currentCollection = edgeCollectionList[stepIdx[st]];
				std::vector<Edge*> selfEdges = currentCollection->getAllEdges();

				// find neighbours
				for (size_t i = 0; i < edgeCollectionList.size(); i++)
				{
					if (std::count(processedIdx.begin(), processedIdx.end(), i)) { continue; } // pass if self or already processed

					// get all edges of the potental neighbour
					EdgeCollection* otherEdgeCollection = edgeCollectionList[i];
					std::vector<Edge*> otherEdges = otherEdgeCollection->getAllEdges();
					bool isFound = false;

					for (size_t j = 0; j < selfEdges.size(); j++)
					{
						for (size_t k = 0; k < otherEdges.size(); k++)
						{
							if (isEqual(selfEdges[j]->getStart(), otherEdges[k]->getStart()) && isEqual(selfEdges[j]->getEnd(), otherEdges[k]->getEnd()))
							{
								otherEdgeCollection->flipFace();
								isFound = true;
								nextStep.emplace_back(i);
								processedIdx.emplace_back(i);
							}
							else if (isEqual(selfEdges[j]->getStart(), otherEdges[k]->getEnd()) && isEqual(selfEdges[j]->getEnd(), otherEdges[k]->getStart())) {
								isFound = true;
								nextStep.emplace_back(i);
								processedIdx.emplace_back(i);
							}
							if (isFound) { break; }
						}
					}
				}
			}
			if (nextStep.size() == 0) { break; }
			stepIdx = nextStep;
		}
	}

	TopoDS_Shape Kernel::getShape(GeoObject geoObject) {
		TopoDS_Shape occtShape;

		if (cityCollection_ == nullptr)
		{
			std::cout << "Kernel has not been initialized, add cityCollection to kernel before retrieving a shape" << std::endl;
			return occtShape;
		}
		if (cityCollection_->getVerices().size() == 0)
		{
			std::cout << "Kernel has not been initialized, add pointlist to cityCollection before retrieving a shape" << std::endl;
			return occtShape;
		}

		// construct facelist 		
		std::vector<std::vector<int>> faceIntList = getSurfaceIdx(&geoObject.getBoundaries());
		std::vector<CJTPoint> cityVerts = cityCollection_->getVerices();

		BRep_Builder brepBuilder;
		BRepBuilderAPI_Sewing brepSewer;
		brepSewer.SetTolerance(0.001);

		for (size_t i = 0; i < faceIntList.size(); i++)
		{
			std::vector<gp_Pnt> oPointList;
			for (size_t j = 0; j < faceIntList[i].size(); j++)
			{
				CJTPoint currentPoint = cityVerts[faceIntList[i][j]];
				oPointList.emplace_back(gp_Pnt(currentPoint.getX(), currentPoint.getY(), currentPoint.getZ()));
			}
			BRepBuilderAPI_MakeWire mkwire;
			for (size_t j = 0; j < oPointList.size(); j++)
			{
				if (j + 1 < oPointList.size())
				{
					mkwire.Add(BRepBuilderAPI_MakeEdge(oPointList[j], oPointList[j + 1]));
				}
				else {
					mkwire.Add(BRepBuilderAPI_MakeEdge(oPointList[j], oPointList[0]));
				}
			}
			TopoDS_Wire topoWire = mkwire.Wire();
			TopoDS_Face topoFace = BRepBuilderAPI_MakeFace(topoWire).Face();

			if (topoFace.IsNull())
			{
				gp_Pnt basePoint = oPointList[0];
				int supportPointIdx = furtestPoint(0, oPointList);
				gp_Pnt supportPoint1 = oPointList[supportPointIdx];
				gp_Pnt supportPoint2 = oPointList[furtestPointLine(0, supportPointIdx, oPointList)];

				auto plane = GC_MakePlane(oPointList[0], supportPoint1, supportPoint2);
				topoFace = BRepBuilderAPI_MakeFace(plane.Value(), topoWire).Face();
			}
			brepSewer.Add(topoFace);
		}
		TopoDS_Solid solidShape;
		brepBuilder.MakeSolid(solidShape);
		brepSewer.Perform();
		brepBuilder.Add(solidShape, brepSewer.SewedShape());
		return solidShape;
	}


	std::vector<TopoDS_Shape> Kernel::getShape(CityObject cityObject) {
		std::vector<TopoDS_Shape> shapeList;
		
		if (cityCollection_ == nullptr)
		{
			std::cout << "Kernel has not been initialized, add cityCollection to kernel before retrieving a shape" << std::endl;
			return shapeList;
		}
		if (cityCollection_->getVerices().size() == 0)
		{
			std::cout << "Kernel has not been initialized, add pointlist to cityCollection before retrieving a shape" << std::endl;
			return shapeList;
		}

		std::vector<GeoObject> geoObjectList = 	cityObject.getGeoObjects();
		for (size_t i = 0; i < geoObjectList.size(); i++)
		{
			GeoObject currentObject = geoObjectList[i];
			int geoId = currentObject.getId();

			if (geoId == -1)
			{
				shapeList.emplace_back(getShape(currentObject));
			}
			else
			{
				if (internalizedObjectMap_.find(geoId) != internalizedObjectMap_.end())
				{
					shapeList.emplace_back(internalizedObjectMap_[geoId]);
					continue;
				}
				else
				{
					currentObject.setId(-1);
					shapeList.emplace_back(getShape(currentObject));
				}
			}
		}
		return shapeList;
	}


	GeoObject Kernel::convertShape(const TopoDS_Shape& shape, std::string lod)
	{
		std::string geomType = "";
		if (shape.ShapeType() == 2) { geomType = "Solid"; }
		else if (shape.ShapeType() == 3) { geomType = "MultiSurface"; }
		else 
		{ 
			std::cout << "Kernel does not regonize geo type" << std::endl;
			throw std::exception();
		}

		// mapping of the verts and get unique verts 
		std::vector<gp_Pnt> uniqueVerts;
		std::vector<TopoDS_Face> faceList;
		std::vector<EdgeCollection*> edgeCollectionList;
		TopExp_Explorer expl;
		for (expl.Init(shape, TopAbs_FACE); expl.More(); expl.Next()) { faceList.emplace_back(TopoDS::Face(expl.Current())); } // TODO try brep function

		for (size_t i = 0; i < faceList.size(); i++)
		{
			EdgeCollection* edgeCollection = new EdgeCollection;
			int c = 0;
			gp_Pnt lP;
			for (expl.Init(faceList[i], TopAbs_VERTEX); expl.More(); expl.Next())
			{	
				TopoDS_Vertex vertex = TopoDS::Vertex(expl.Current());
				gp_Pnt p = BRep_Tool::Pnt(vertex);
				if (!isPointInList(p, uniqueVerts)) { uniqueVerts.emplace_back(p); }
				if (c % 2 == 1) 
				{ 
					Edge* collectedEdge = new Edge(lP, p);
					edgeCollection->addEdge(collectedEdge);
				}
				lP = p;
				c++;
			}
			edgeCollection->orderEdges();
			edgeCollectionList.emplace_back(edgeCollection);
		}

		// find highest face
		int highestCollectionIdx = findTopEdgeCollection(edgeCollectionList);
		if (edgeCollectionList[highestCollectionIdx]->hasPositiveNormal()) { edgeCollectionList[highestCollectionIdx]->flipFace(); } // TODO find out why normal is reversed
		correctFaceDirection(edgeCollectionList, highestCollectionIdx);


		// find or add the unique verts to the collection
		std::vector<CJTPoint> cjtUniquePoints;
		std::vector<gp_Pnt> uniquePoints;
		for (size_t i = 0; i < uniqueVerts.size(); i++)
		{
			const auto& currectPoint = uniqueVerts[i];
			cjtUniquePoints.emplace_back(CJTPoint(currectPoint.X(), currectPoint.Y(), currectPoint.Z()));
			uniquePoints.emplace_back(currectPoint);
		}
		std::vector<int> pointLocation = cityCollection_->addVertex(cjtUniquePoints, true);

		// from opencascade data to json
		json boundaries;

		for (size_t i = 0; i < edgeCollectionList.size(); i++)
		{
			json ShapeCollection;
			EdgeCollection currentCollection = *edgeCollectionList[i];

			// get outer ring
			std::vector<gp_Pnt> startPointCollection = currentCollection.getStartPoints();
			std::vector<int> idxList;
			for (size_t j = 0; j < startPointCollection.size(); j++)
			{
				for (size_t k = 0; k < uniquePoints.size(); k++)
				{
					if (isEqual(startPointCollection[j], uniquePoints[k]))
					{
						idxList.emplace_back(pointLocation[k]);
					}
				}
				
			}
			ShapeCollection.emplace_back(idxList);

			// get inner rings
			std::vector<EdgeCollection*> innerRingsCollection = currentCollection.getInnerRings();

			for (size_t j = 0; j < innerRingsCollection.size(); j++)
			{
				EdgeCollection* currentInnerCollection = innerRingsCollection[j];
				std::vector<gp_Pnt> innerStartPointCollection = currentInnerCollection->getStartPoints();
				std::vector<int> innerIdxList;
				for (size_t k = 0; k < innerStartPointCollection.size(); k++)
				{
					for (size_t l = 0; l < uniquePoints.size(); l++)
					{
						if (isEqual(innerStartPointCollection[k], uniquePoints[l]))
						{
							innerIdxList.emplace_back(pointLocation[l]);
						}
					}
				}
				ShapeCollection.emplace_back(innerIdxList);
			}
			boundaries.emplace_back(ShapeCollection);
		}


		for (size_t i = 0; i < edgeCollectionList.size(); i++)
		{
			delete edgeCollectionList[i];
		}

		if (geomType == "Solid")
		{
			json solidCollection;
			solidCollection.emplace_back(boundaries);
			return GeoObject(solidCollection, lod, geomType);
		}
		if (geomType == "MultiSurface")
		{
			return GeoObject(boundaries, lod, geomType);
		}
	}
}