#include "inc/CJT.h"
#include "inc/CJToKernel.h"
#include <BRepGProp.hxx>
#include <gp_Lin.hxx>

namespace CJT {

	struct IntFace {
		std::vector<int> outerRing_;
		std::vector<std::vector<int>> innerRingList_;
		bool hasInner_ = false;
	};


	void printPoint(gp_Pnt p) {
		std::cout << p.X() << ", " << p.Y() << ", " << p.Z() << ", " << std::endl;
	}


	void printFaces(TopoDS_Shape shape)
	{
		//std::cout << "Shape:" << std::endl;
		std::vector<TopoDS_Face> faceList;

		TopExp_Explorer expl;
		for (expl.Init(shape, TopAbs_FACE); expl.More(); expl.Next())
		{
			faceList.emplace_back(TopoDS::Face(expl.Current()));
		}

		for (size_t i = 0; i < faceList.size(); i++)
		{
			std::cout << "new" << std::endl;
			for (expl.Init(faceList[i], TopAbs_VERTEX); expl.More(); expl.Next())
			{
				TopoDS_Vertex vertex = TopoDS::Vertex(expl.Current());
				gp_Pnt p = BRep_Tool::Pnt(vertex);
				printPoint(p);
			}
		}
		//std::cout << std::endl;
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


	std::vector<IntFace> getSurfaceIdx(const json& boundaries) {
		std::vector<IntFace> collection;
		bool isInt = false;

		if (boundaries.begin().value().begin().value().is_number_integer()) { isInt = true; }
		if (isInt)
		{
			IntFace intFace;

			int ringCount = 0;
			for (json ring : boundaries)
			{
				std::vector<int> surfaceIdx;
				surfaceIdx.clear();
				for (json subvalue : ring)
				{
					surfaceIdx.emplace_back(subvalue);
				}
				if (ringCount == 0)
				{
					intFace.outerRing_ = surfaceIdx;
					ringCount++;
				}
				else if (ringCount != 0)
				{
					intFace.innerRingList_.emplace_back(surfaceIdx);
					intFace.hasInner_ = true;
				}
			}
			collection.emplace_back(intFace);
		}
		else {
			for (json subvalue : boundaries)
			{
				std::vector<IntFace> outputboundaries = getSurfaceIdx(subvalue);
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

		for (int i = 0; i < pointList.size(); i++)
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

		gp_Lin lineF (p1, gp_Vec(p1, p2));
		gp_Lin lineB (p1, gp_Vec(p2, p1));

		TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(p1, p2);

		for (int i = 0; i < pointList.size(); i++)
		{
			if (i == p1Idx || i == p2Idx){ continue; }

			gp_Pnt currentPoint = pointList[i];
			double localDistanceF = lineF.Distance(currentPoint);
			double localDistanceB = lineB.Distance(currentPoint);
			double localDistance;

			if (localDistanceF < localDistanceB)
			{
				localDistance = localDistanceF;
			}
			else {
				localDistance = localDistanceB;
			}

			if (localDistance > distance)
			{
				distance = localDistance;
				idx = i;
			}
		}
		return idx;
	}

	gp_Vec calculateNormal(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, bool normalize = false) {
		gp_Vec v1(p2, p1);
		gp_Vec v2(p3, p1);
		gp_Vec normal = v1.Crossed(v2);

		if (normalize)
		{
			double magnitude = normal.Magnitude();
			return gp_Vec(normal.X() / magnitude, normal.Y() / magnitude, normal.Z() / magnitude);
		}
		else {
			return normal;
		}
	}


	std::vector<gp_Pnt> intPoint2GpPoint(const std::vector<int> &pointIdxList, const std::vector<CJTPoint> &cityVerts) {
		std::vector<gp_Pnt> oPointList;
		for (size_t i = 0; i < pointIdxList.size(); i++)
		{
			CJTPoint currentPoint = cityVerts[pointIdxList[i]];
			oPointList.emplace_back(gp_Pnt(currentPoint.getX(), currentPoint.getY(), currentPoint.getZ()));
		}
		return oPointList;
	}

	TopoDS_Wire makeRingWire(std::vector<gp_Pnt> pointList) {
		BRepBuilderAPI_MakeWire mkwire;
		for (size_t j = 0; j < pointList.size(); j++)
		{
			if (j + 1 < pointList.size())
			{
				if (pointList[j].IsEqual(pointList[j + 1], 0.001))
				{
					return TopoDS_Wire();
				}
				mkwire.Add(BRepBuilderAPI_MakeEdge(pointList[j], pointList[j + 1]));
			}
			else {
				if (pointList[j].IsEqual(pointList[0], 0.001))
				{
					return TopoDS_Wire();
				}
				mkwire.Add(BRepBuilderAPI_MakeEdge(pointList[j], pointList[0]));
			}
		}
		TopoDS_Wire topoWire = mkwire.Wire();
		return topoWire;
	}

	gp_Pnt getMiddlePoint(gp_Pnt p1, gp_Pnt p2) {
		return gp_Pnt(
			(p1.X() + p2.X()) / 2,
			(p1.Y() + p2.Y()) / 2,
			(p1.Z() + p2.Z()) / 2
		);
	}

	gp_Pnt getMiddlePoint(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3) {
		return gp_Pnt(
			(p1.X() + p2.X() + p3.X()) / 3,
			(p1.Y() + p2.Y() + p3.Y()) / 3,
			(p1.Z() + p2.Z() + p3.Z()) / 3
		);
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


	std::vector<gp_Pnt> EdgeCollection::getStartPoints() const
	{
		std::vector<gp_Pnt> startPoints;
		for (size_t i = 0; i < ring_.size(); i++) { startPoints.emplace_back(ring_[i]->getStart()); }
		return startPoints;
	}

	bool EdgeCollection::hasPositiveNormal() const
	{
		if (normal_.Z() > 0) { return true; }
		return false;
	}

	std::vector<std::shared_ptr<Edge>> EdgeCollection::getAllEdges() {
		std::vector<std::shared_ptr<Edge>> selfEdges = ring_;
		std::vector<std::shared_ptr<EdgeCollection>> selfRings = innerRingList_;
		for (size_t j = 0; j < selfRings.size(); j++)
		{
			std::vector<std::shared_ptr<Edge>> selfInnerEdges = selfRings[j]->getEdges();
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

		double bigArea = 0;
		int backupIdx1 = 0;
		int backupIdx2 = 0;
		int backupIdx3 = 0;
		bool found = false;

		if (isInner_ == false)
		{
			TopLoc_Location loc;
			Handle(Poly_Triangulation) tri = BRep_Tool::Triangulation(originalFace_, loc);

			if (tri.IsNull()) { return; }

			int counter = tri->NbTriangles();
			for (int i = 0; i < counter; i++)
			{
				int idx = i + 1;
				Poly_Triangle triangle = tri->Triangle(idx);

				int n1, n2, n3;
				triangle.Get(n1, n2, n3);

				gp_Pnt p1 = tri->Node(n1).Transformed(loc);
				gp_Pnt p2 = tri->Node(n2).Transformed(loc);
				gp_Pnt p3 = tri->Node(n3).Transformed(loc);

				gp_Pnt pc = getMiddlePoint(p1, p2, p3);

				double distance = BRepExtrema_DistShapeShape(originalFace_, BRepBuilderAPI_MakeVertex(pc)).Value();
				if (distance > 0.001) { continue; }
				for (size_t j = 0; j < ring_.size(); j++)
				{
					std::shared_ptr<Edge> edge = ring_[j];

					if (edge->getStart().IsEqual(p1, 0.01) && edge->getEnd().IsEqual(p2, 0.01) ||
						edge->getStart().IsEqual(p2, 0.01) && edge->getEnd().IsEqual(p3, 0.01) ||
						edge->getStart().IsEqual(p3, 0.01) && edge->getEnd().IsEqual(p1, 0.01) // correct orientation
						)
					{
						normal_ = calculateNormal(p1, p2, p3, true);
						return;
					}
					else if (edge->getStart().IsEqual(p2, 0.01) && edge->getEnd().IsEqual(p1, 0.01) ||
						edge->getStart().IsEqual(p3, 0.01) && edge->getEnd().IsEqual(p2, 0.01) ||
						edge->getStart().IsEqual(p1, 0.01) && edge->getEnd().IsEqual(p3, 0.01) // reversed orientation
						)
					{
						normal_ = calculateNormal(p1, p2, p3, true).Reversed();
						return;
					}
				}

			}
			std::cout << "not found" << std::endl;
			//TODO: find a fix when there is no normal found
		}
		else if (isInner_ == true)
		{
			double distance = 99999999;
			TopoDS_Vertex measurePoint = BRepBuilderAPI_MakeVertex(startPoints[0]);
			TopoDS_Wire originalWire;
			for (TopExp_Explorer explorer(originalFace_, TopAbs_WIRE); explorer.More(); explorer.Next())
			{
				const TopoDS_Wire& wire = TopoDS::Wire(explorer.Current());
				double distanceRing = BRepExtrema_DistShapeShape(wire, measurePoint).Value();

				if (distance > distanceRing)
				{
					distance = distanceRing;
					originalWire = wire;
				}
			}

			bool large = false;

			for (int i = 0; i < startPoints.size(); i++) //TODO change this to inner ring normal computation
			{
				p1 = startPoints[i];
				for (int j = 1; j < startPoints.size(); j++)
				{
					if (i >= j) { continue; }
					p2 = startPoints[j];
					for (int k = 2; k < startPoints.size(); k++)
					{
						if (j >= k) { continue; }
						p3 = startPoints[k];

						bool found = false;

						if (startPoints.size() >= 4)
						{	
							// compute distance from face (if not >0 triangle is presumably outside of inner ring)
							gp_Pnt pc = getMiddlePoint(p1, p2, p3);

							double distc = BRepExtrema_DistShapeShape(originalFace_, BRepBuilderAPI_MakeVertex(pc)).Value();
							if (distc < 0.0001) { continue; }
							// compute if any edge of triangle has overlap with edge to correctly find orientation

							int hits = 0;
							BRepExtrema_DistShapeShape h1(originalWire, BRepBuilderAPI_MakeVertex(p1));
							BRepExtrema_DistShapeShape h2(originalWire, BRepBuilderAPI_MakeVertex(p2));
							BRepExtrema_DistShapeShape h3(originalWire, BRepBuilderAPI_MakeVertex(p3));

							if (h1.Value() < 1e-6) { hits++;}
							if (h2.Value() < 1e-6) { hits++;}
							if (h3.Value() < 1e-6) { hits++;}
							if (hits <= 1)  { continue;  }
							found = true;
						}

						if (!found) { continue; }

						// compute area of the triangle
						gp_Vec v1(p2.X() - p1.X(), p2.Y() - p1.Y(), p2.Z() - p1.Z());
						gp_Vec v2(p3.X() - p1.X(), p3.Y() - p1.Y(), p3.Z() - p1.Z());
						gp_Vec normal = calculateNormal(p1, p2, p3);
						double area = normal.Magnitude() * 0.5;
						if (area > 0.01)
						{
							backupIdx1 = i;
							backupIdx2 = j;
							backupIdx3 = k;
							large = true;
							break;
						}

						if (area > bigArea)
						{
							bigArea = area;
							backupIdx1 = i;
							backupIdx2 = j;
							backupIdx3 = k;
						}
					}
					if (large) { break; }
				}
				if (large) { break; }
			}

			p1 = startPoints[backupIdx1];
			p2 = startPoints[backupIdx2];
			p3 = startPoints[backupIdx3];

			for (size_t j = 0; j < ring_.size(); j++)
			{
				std::shared_ptr<Edge> edge = ring_[j];

				if (edge->getStart().IsEqual(p1, 0.01) && edge->getEnd().IsEqual(p2, 0.01) ||
					edge->getStart().IsEqual(p2, 0.01) && edge->getEnd().IsEqual(p3, 0.01) ||
					edge->getStart().IsEqual(p3, 0.01) && edge->getEnd().IsEqual(p1, 0.01) // correct orientation
					)
				{
					normal_ = calculateNormal(p1, p2, p3, true);
					return;
				}
				else if (edge->getStart().IsEqual(p2, 0.01) && edge->getEnd().IsEqual(p1, 0.01) ||
					edge->getStart().IsEqual(p3, 0.01) && edge->getEnd().IsEqual(p2, 0.01) ||
					edge->getStart().IsEqual(p1, 0.01) && edge->getEnd().IsEqual(p3, 0.01) // reversed orientation
					)
				{
					normal_ = calculateNormal(p1, p2, p3, true).Reversed();
					return;
				}
			}
			return;
		}
	}

	void EdgeCollection::orderEdges()
	{
		int falsePresent = static_cast<int>(ring_.size());
		std::shared_ptr<Edge> startEdge = ring_[0];
		std::shared_ptr<Edge> currentEdge = ring_[0];
		std::vector<std::shared_ptr<Edge>> cleanedList;
		while (falsePresent != 0)
		{
			if (currentEdge->isProcessed())
			{
				if (!isInner_)
				{
					std::shared_ptr<EdgeCollection> innerRing = std::make_shared<EdgeCollection>(EdgeCollection());
					innerRing->setOriginalFace(originalFace_);
					for (size_t i = 0; i < ring_.size(); i++)
					{
						if (ring_[i]->isProcessed() == false)
						{
							std::shared_ptr<Edge> tempEdge = ring_[i];
							innerRing->addEdge(tempEdge);
						}
					}
					innerRing->setIsInner();
					innerRing->orderEdges();
					addInnerRing(innerRing);
					falsePresent = static_cast<int>(falsePresent - innerRing->getEdges().size());
				}
				else {
					break;
				}
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

					if (isEqual(p2, ring_[i]->getStart()))
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
		ring_ = cleanedList;
		computeNormal();

		if (innerRingList_.size() == 0) { return; }
		for (size_t i = 0; i < innerRingList_.size(); i++)
		{
			gp_Vec otherNormal = innerRingList_[i]->normal_;
			if (otherNormal.IsEqual(normal_, 1e-6, 1e-6))
			{
				innerRingList_[i]->flipFace();
			}
		}
	}

	void EdgeCollection::flipFace()
	{
		normal_.Reverse();
		for (size_t i = 0; i < ring_.size(); i++) { ring_[i]->reverse(); }
		std::reverse(ring_.begin(), ring_.end());
		for (size_t i = 0; i < innerRingList_.size(); i++) 
		{ 
			innerRingList_[i]->flipFace(); 
		}
	}


	Kernel::Kernel(const std::shared_ptr<CityCollection>& cityCollection)
	{
		fprecision = cityCollection->getTransformation().getScale()[0];
		cityCollection_ = cityCollection;
	}


	int Kernel::findTopEdgeCollection(const std::vector<std::shared_ptr<EdgeCollection>>& edgeCollectionList)
	{
		int highestCollectionIdx = 0;
		double height = -999999999;
		double count = 0;
		for (int i = 0; i < edgeCollectionList.size(); i++)
		{
			double tempCount = 0;
			double currentHeight = -999999999;
			std::vector<gp_Pnt> vertCollection = edgeCollectionList[i]->getStartPoints();
			for (size_t j = 0; j < vertCollection.size(); j++)
			{
				if (currentHeight < vertCollection[j].Z())
				{
					currentHeight = vertCollection[j].Z();
					tempCount = 0;
				}
				if (abs(currentHeight - vertCollection[j].Z()) < 1e-6)
				{
					tempCount++;
				}
			}

			if (currentHeight > height)
			{
				height = currentHeight;
				count = tempCount;
				highestCollectionIdx = i;
			}
			else if (abs(currentHeight - height) < 1e-6 && tempCount > count)
			{
				count = tempCount;
				highestCollectionIdx = i;
			}
		}

		std::vector<gp_Pnt> vertCollection = edgeCollectionList[highestCollectionIdx]->getStartPoints();
		return highestCollectionIdx;
	}

	void Kernel::correctFaceDirection(std::vector<std::shared_ptr<EdgeCollection>> edgeCollectionList)
	{
		double buffer = 0.05;
		// if single surface the surface should always be orentated to the positive Z
		if (edgeCollectionList.size() == 1)
		{
			std::shared_ptr<EdgeCollection> groundCollection = edgeCollectionList[0];
			if (groundCollection->getNormal().Z() > 0) { return; }

			groundCollection->flipFace();
			return;
		}

		// index surfaces and find highest surface
		bgi::rtree<Value, bgi::rstar<treeDepth>> spatialIndex;
		int topSurfaceIndx = 0;
		double highestZ = -999999;

		std::vector<bg::model::box<BoostPoint3D>> boundingBoxCollection;
		for (int i = 0; i < edgeCollectionList.size(); i++)
		{
			// make bbox of every surface
			Bnd_Box bbox;
			BRepBndLib::Add(edgeCollectionList[i]->getOriginalFace(), bbox);
			
			gp_Pnt lll = bbox.CornerMin();
			gp_Pnt urr = bbox.CornerMax();

			bg::model::box<BoostPoint3D> boostbbox(
				{ lll.X() - buffer, lll.Y() - buffer, lll.Z() - buffer }, { urr.X() + buffer, urr.Y() + buffer, urr.Z() + buffer }
			);
			spatialIndex.insert(std::make_pair(boostbbox, i));
			boundingBoxCollection.emplace_back(boostbbox);

			double currentZ = urr.Z();
			if (highestZ < currentZ)
			{
				double zNomalCompo = edgeCollectionList[i]->getNormal().Z();
				if (abs(zNomalCompo) < 0.05) { continue; }

				highestZ = currentZ;
				topSurfaceIndx = i;
			}
		}

		//orientate the highest surface
		std::shared_ptr<EdgeCollection> topSurface = edgeCollectionList[topSurfaceIndx];
		if (topSurface->getNormal().Z() < 0) { topSurface->flipFace(); }

		//correct rest of the normals by growing from this face
		std::vector evalList(edgeCollectionList.size(), 0);
		evalList[topSurfaceIndx] = 1;
		std::vector<int> bufferList;
		bufferList.emplace_back(topSurfaceIndx);

		while (true)
		{
			std::vector<int> tempBufferList;
			for (int edgeCollectionIdx : bufferList)
			{
				std::shared_ptr<EdgeCollection> currentEdgeCollection = edgeCollectionList[edgeCollectionIdx];
				bg::model::box<BoostPoint3D> currenBBox = boundingBoxCollection[edgeCollectionIdx];

				std::vector<Value> qResult;
				qResult.clear();
				spatialIndex.query(
					bgi::intersects(
						currenBBox
					),
					std::back_inserter(qResult)
				);

				for(Value neighbourCollectionValue : qResult)
				{
					int otherIdx = neighbourCollectionValue.second;
					if (evalList[otherIdx] == 1) { continue; }				

					std::shared_ptr<EdgeCollection> otherEdgeCollection = edgeCollectionList[otherIdx];

					//find matching edge
					bool found = false;
					for (std::shared_ptr<Edge> currentEdge : currentEdgeCollection->getEdges())
					{
						for (std::shared_ptr<Edge> otherEdge : otherEdgeCollection->getEdges())
						{
							// correct orientation
							if (currentEdge->getStart().IsEqual(otherEdge->getEnd(), 1e-6) &&
								currentEdge->getEnd().IsEqual(otherEdge->getStart(), 1e-6))
							{
								found = true;
								break;
							}
							// wrong orientation
							if (currentEdge->getStart().IsEqual(otherEdge->getStart(), 1e-6) &&
								currentEdge->getEnd().IsEqual(otherEdge->getEnd(), 1e-6))
							{
								otherEdgeCollection->flipFace();
								found = true;
								break;
							}
						}
						if (found) { break; }
					}
					if (found) 
					{
						evalList[otherIdx] = 1;
						tempBufferList.emplace_back(otherIdx);
					}
					 //Inner ring processing 1
					for (std::shared_ptr<EdgeCollection> currentInnerRing : currentEdgeCollection->getInnerRings())
					{
						for (std::shared_ptr<Edge> currentEdge : currentInnerRing->getEdges())
						{
							for (std::shared_ptr<Edge> otherEdge : otherEdgeCollection->getEdges())
							{
								// correct orientation
								if (currentEdge->getStart().IsEqual(otherEdge->getEnd(), 1e-6) &&
									currentEdge->getEnd().IsEqual(otherEdge->getStart(), 1e-6))
								{
									found = true;
									break;
								}
								// wrong orientation
								if (currentEdge->getStart().IsEqual(otherEdge->getStart(), 1e-6) &&
									currentEdge->getEnd().IsEqual(otherEdge->getEnd(), 1e-6))
								{
									otherEdgeCollection->flipFace();
									found = true;
									break;
								}
							}
							if (found) { break; }
						}
					}
					if (found)
					{
						evalList[otherIdx] = 1;
						tempBufferList.emplace_back(otherIdx);
					}

					// inner ring processing 2
					for (std::shared_ptr<Edge> currentEdge : currentEdgeCollection->getEdges())
					{
						for (std::shared_ptr<EdgeCollection> otherInnerRing : otherEdgeCollection->getInnerRings())
						{
							for (std::shared_ptr<Edge> otherEdge : otherInnerRing->getEdges())
							{
								// correct orientation
								if (currentEdge->getStart().IsEqual(otherEdge->getEnd(), 1e-6) &&
									currentEdge->getEnd().IsEqual(otherEdge->getStart(), 1e-6))
								{
									found = true;
									break;
								}
								// wrong orientation
								if (currentEdge->getStart().IsEqual(otherEdge->getStart(), 1e-6) &&
									currentEdge->getEnd().IsEqual(otherEdge->getEnd(), 1e-6))
								{
									otherEdgeCollection->flipFace();
									found = true;
									break;
								}
							}
							if (found) { break; }
						}
					}
					if (found)
					{
						evalList[otherIdx] = 1;
						tempBufferList.emplace_back(otherIdx);
					}
				}
			}
			bufferList.clear();
			bufferList = tempBufferList;
			tempBufferList.clear();

			if (!bufferList.size()) { break; }
		}
	}


	bool Kernel::checkIfInit() {
		if (cityCollection_ == nullptr)
		{
			std::cout << "Kernel has not been initialized, add cityCollection to kernel before retrieving a shape" << std::endl;
			return false;
		}
		if (cityCollection_->getVerices().size() == 0)
		{
			std::cout << "Kernel has not been initialized, add pointlist to cityCollection before retrieving a shape" << std::endl;
			return false;
		}
		return true;
	}

	TopoDS_Shape Kernel::convertToCascade(GeoObject& geoObject) {
		TopoDS_Shape occtShape;
		bool success = true;

		if (!checkIfInit()) { return occtShape; }

		// construct facelist
		std::vector<IntFace> faceIntList = getSurfaceIdx(geoObject.getBoundaries());
		std::vector<CJTPoint> cityVerts = cityCollection_->getVerices();

		BRep_Builder brepBuilder;
		BRepBuilderAPI_Sewing brepSewer;
		brepSewer.SetTolerance(0.001);

		for (size_t i = 0; i < faceIntList.size(); i++)
		{
			IntFace currentIntFace = faceIntList[i];
			std::vector<gp_Pnt> oPointList = intPoint2GpPoint(currentIntFace.outerRing_, cityVerts);
			TopoDS_Wire topoWire = makeRingWire(oPointList);

			if (topoWire.IsNull()) { 
				success = false;
				continue; 
			}

			TopoDS_Face topoFace = BRepBuilderAPI_MakeFace(topoWire).Face();
	
			if (topoFace.IsNull())
			{
				gp_Pnt basePoint = oPointList[0];
				int supportPointIdx = furtestPoint(0, oPointList);
				gp_Pnt supportPoint1 = oPointList[supportPointIdx];
				gp_Pnt supportPoint2 = oPointList[furtestPointLine(0, supportPointIdx, oPointList)];

				if (supportPoint1.IsEqual(basePoint, 1e-6) || supportPoint2.IsEqual(basePoint, 1e-6) || supportPoint1.IsEqual(supportPoint2, 1e-6))
				{
					continue;
				}

				auto plane = GC_MakePlane(oPointList[0], supportPoint1, supportPoint2);
				topoFace = BRepBuilderAPI_MakeFace(plane.Value(), topoWire).Face();
			}

			if (topoFace.IsNull())
			{
				success = false;
				break;
			}

			if (currentIntFace.hasInner_)
			{
				for (size_t j = 0; j < currentIntFace.innerRingList_.size(); j++)
				{
					std::vector<gp_Pnt> iPointList = intPoint2GpPoint(currentIntFace.innerRingList_[j], cityVerts);
					TopoDS_Wire innerWire = makeRingWire(iPointList);

					if (innerWire.IsNull()) { continue; }
					
					BRepBuilderAPI_MakeFace faceCreator(topoFace, innerWire);

					if (faceCreator.Error() != BRepBuilderAPI_FaceDone) { continue; }

					topoFace = faceCreator.Face();
				}
			}

			if (topoFace.IsNull()) 
			{
				success = false;
				break;
			}

			brepSewer.Add(topoFace);
		}

		if (geoObject.getType() == "Solid" && success)
		{
			TopoDS_Solid solidShape;
			brepBuilder.MakeSolid(solidShape);
			brepSewer.Perform();
			brepBuilder.Add(solidShape, brepSewer.SewedShape());
			return solidShape;
		}
		if (geoObject.getType() == "MultiSurface" || !success)
		{
			TopoDS_Compound shellShape;
			brepBuilder.MakeCompound(shellShape);
			brepSewer.Perform();
			brepBuilder.Add(shellShape, brepSewer.SewedShape());
			return shellShape;
		}

		TopoDS_Solid solidShape;
		brepBuilder.MakeSolid(solidShape);
		brepSewer.Perform();
		brepBuilder.Add(solidShape, brepSewer.SewedShape());
		return solidShape;
	}


	std::vector<TopoDS_Shape> Kernel::convertToCascade(CityObject& cityObject) {
		std::vector<TopoDS_Shape> shapeList;
		
		if (!checkIfInit()) { return shapeList; }

		std::vector<std::shared_ptr<GeoObject>> geoObjectList = cityObject.getGeoObjectsPtr();
		for (size_t i = 0; i < geoObjectList.size(); i++)
		{
			std::shared_ptr<GeoObject> currentObject = geoObjectList[i];
			int geoId = currentObject->getId();

			if (geoId == -1)
			{
				shapeList.emplace_back(convertToCascade(*currentObject));
			}
			else
			{
				if (internalizedObjectMap_.find(geoId) != internalizedObjectMap_.end())
				{
					//TODO: check for the need of this map 
					//shapeList.emplace_back(internalizedObjectMap_[geoId]);
					continue;
				}
				else
				{
					currentObject->setId(-1);
					shapeList.emplace_back(convertToCascade(*currentObject));
				}
			}
		}
		return shapeList;
	}


	GeoObject Kernel::convertToJSON(const TopoDS_Shape& shape, std::string lod, bool trustedSolid)
	{
		std::string geomType = "";

		if (trustedSolid) { geomType = "Solid"; }
		else if (shape.ShapeType() == 2) { geomType = "Solid"; }
		else if (shape.ShapeType() == 3 || shape.ShapeType() == 4) { geomType = "MultiSurface"; }
		else if (shape.ShapeType() == 0) { geomType = "MultiSurface"; }
		else 
		{ 
			std::cout << "Kernel does not regonize geo type" << std::endl;
			throw std::exception();
		}

		// mapping of the verts and get unique verts 
		std::vector<gp_Pnt> uniqueVerts;
		std::vector<TopoDS_Face> faceList;
		std::vector<std::shared_ptr<EdgeCollection>> edgeCollectionList;
		TopExp_Explorer expl;
		BRepMesh_IncrementalMesh(shape, 0.001);

		for (expl.Init(shape, TopAbs_FACE); expl.More(); expl.Next()) 
		{ 
			TopoDS_Face face = TopoDS::Face(expl.Current());
			faceList.emplace_back(face);
		} 
		
		for (size_t i = 0; i < faceList.size(); i++)
		{

			std::shared_ptr<EdgeCollection> edgeCollection = std::make_shared<EdgeCollection>(EdgeCollection());
			int c = 0;
			gp_Pnt lP;
			for (expl.Init(faceList[i], TopAbs_VERTEX); expl.More(); expl.Next())
			{	
				TopoDS_Vertex vertex = TopoDS::Vertex(expl.Current());
				gp_Pnt p = BRep_Tool::Pnt(vertex);
				
				if (c % 2 == 1) 
				{ 
					// TODO: fix the jump that is present here
					//if (p.Distance(lP) > fprecision)
					{
						if (!isPointInList(lP, uniqueVerts)) { uniqueVerts.emplace_back(lP); }
						if (!isPointInList(p, uniqueVerts)) { uniqueVerts.emplace_back(p); }

						std::shared_ptr<Edge> collectedEdge = std::make_shared<Edge>(Edge(lP, p));
						edgeCollection->addEdge(collectedEdge);
					}
				}
				lP = p;
				c++;
			}
			edgeCollection->setOriginalFace(faceList[i]);
			edgeCollection->orderEdges();
			edgeCollectionList.emplace_back(edgeCollection);
		}
		correctFaceDirection(edgeCollectionList);

		// find or add the unique verts to the collection
		std::vector<CJTPoint> cjtUniquePoints;
		std::vector<gp_Pnt> uniquePoints;
		for (size_t i = 0; i < uniqueVerts.size(); i++)
		{
			const auto& currectPoint = uniqueVerts[i];
			cjtUniquePoints.emplace_back( CJTPoint(currectPoint.X(), currectPoint.Y(), currectPoint.Z()));
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
			std::vector<std::shared_ptr<EdgeCollection>> innerRingsCollection = currentCollection.getInnerRings();

			for (size_t j = 0; j < innerRingsCollection.size(); j++)
			{
				std::shared_ptr<EdgeCollection> currentInnerCollection = innerRingsCollection[j];
				std::vector<gp_Pnt> innerStartPointCollection = currentInnerCollection->getStartPoints();
				std::vector<int> innerIdxList;
				for (size_t k = 0; k < innerStartPointCollection.size(); k++)
				{
					//printPoint(innerStartPointCollection[k]);
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

		return GeoObject(boundaries, lod, geomType); // TODO: add more types
	}
}