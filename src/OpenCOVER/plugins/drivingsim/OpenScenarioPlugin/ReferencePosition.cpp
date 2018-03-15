#include "ReferencePosition.h"


ReferencePosition::ReferencePosition():
    road(NULL),
    s(0.0),
    t(0.0)
{
}

ReferencePosition::~ReferencePosition()
{}

// copy constructor
ReferencePosition::ReferencePosition(const ReferencePosition* oldRefPos)
{
    // Road
    roadId = oldRefPos->roadId;
    road = oldRefPos->road;
    roadLength = oldRefPos->roadLength;
    system = oldRefPos->system;

    s = oldRefPos->s;
    t = oldRefPos->t;
    hdg = oldRefPos->hdg;

    // Lane
    laneId = oldRefPos->laneId;
    LS = oldRefPos->LS;

    // Absolute
    xyz = oldRefPos->xyz;
}

void ReferencePosition::init(std::string init_roadId, int init_laneId, double init_s, RoadSystem* init_system)
// init from Lane
{
    roadId = init_roadId;
    laneId = init_laneId;
    s = init_s;
    system = init_system;

    road = system->getRoad(roadId);

    LS = road->getLaneSection(s);
    Vector2D laneCenter = LS->getLaneCenter(laneId, s);

    t = laneCenter[0];

    Transform vtrans = road->getRoadTransform(s, t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

    roadLength = road->getLength();
}

void ReferencePosition::init(std::string init_roadId,double init_s,double init_t,RoadSystem* init_system)
// init from Road
{
    roadId = init_roadId;
    t = init_t;
    s = init_s;
    system = init_system;

    road = system->getRoad(roadId);

    LS = road->getLaneSection(s);
    laneId = LS->searchLane(s,t);

    Transform vtrans = road->getRoadTransform(s, t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

    roadLength = road->getLength();
}

void ReferencePosition::init(osg::Vec3 initPos, double init_hdg, RoadSystem* init_system)
// init from World
{
    hdg = init_hdg;
    xyz = initPos;
    system = init_system;

    //Vector2D searchHere = Vector2D(x,y); road->isOnRoad(searchHere);
    const Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
    Vector2D output = system->searchPosition(newPoint, road, s);

    if(road != NULL){

        Vector2D stNew = road->searchPosition(newPoint, s);

        s = stNew[0];
        t = stNew[1];

        LaneSection* newLS = road->getLaneSection(s);
        LS = newLS;

        laneId = LS->searchLane(s,t);

        Transform vtrans = road->getRoadTransform(s, t);
        xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

        roadLength = road->getLength();
    }


}

void ReferencePosition::move(double ds, double dt, float step)
{
    double ds1 = ds/sqrt(ds*ds+dt*dt);
    double dt1 = dt/sqrt(ds*ds+dt*dt);

    s = s+ds1*step;
    t = t+dt1*step;

    double phi = atan(dt1/ds1);
    hdg = phi + road->getHeading(s);

    if(s>roadLength)
    {
        this->getSuccessor();
    }
    else if(s<0)
    {
        this->getPredecessor();
    }

    LaneSection* newLS = road->getLaneSection(s);

    if (newLS != LS)
    {
        laneId =  road->searchLane(s,t);
        LS = newLS;
    }

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());
}

void ReferencePosition::move(osg::Vec3 dirVec,float step_distance)
{
    xyz = xyz+(dirVec*step_distance);
}


osg::Vec3 ReferencePosition::getPosition()
{
    return xyz;
}

void ReferencePosition::update(std::string init_roadId, double init_s, int init_laneId)
{
    s = init_s;
    laneId = init_laneId;

    if(init_roadId != roadId)
    {
        road = system->getRoad(init_roadId);
    }
    roadId = init_roadId;

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    Vector2D laneCenter = LS->getLaneCenter(laneId, s);
    t = laneCenter[0];

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

}

void ReferencePosition::update(int init_dLane, double init_ds)
{
    s += init_ds;
    laneId += init_dLane;

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    Vector2D laneCenter = LS->getLaneCenter(laneId, s);
    t = laneCenter[0];

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

}

void ReferencePosition::update(std::string init_roadId, double init_s, double init_t)
{
    s = init_s;
    t = init_t;

    if(init_roadId != roadId)
    {
        road = system->getRoad(init_roadId);
    }
    roadId = init_roadId;

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    laneId = LS->searchLane(s,t);

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

}

void ReferencePosition::update(double init_ds, double init_dt)
{
    s += init_ds;
    t += init_dt;

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    laneId = LS->searchLane(s,t);

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());


}

void ReferencePosition::update(double x, double y, double z)
{
    xyz = osg::Vec3(x,y,z);

    //Vector2D searchHere = Vector2D(x,y); road->isOnRoad(searchHere);
    if(road == NULL)
    {
        const Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
        Vector2D output = system->searchPosition(newPoint, road, s);

        if (road != NULL)
        {
            Vector3D newPoint = Vector3D(x,y,z);
            Vector2D stNew = road->searchPosition(newPoint, s);

            s = stNew[0];
            t = stNew[1];

            LaneSection* newLS = road->getLaneSection(s);
            LS = newLS;

            laneId = LS->searchLane(s,t);
        }

    }
    else
    {
        Vector3D newPoint = Vector3D(x,y,z);
        Vector2D stNew = road->searchPosition(newPoint, s);

        s = stNew[0];
        t = stNew[1];

        LaneSection* newLS = road->getLaneSection(s);
        LS = newLS;

        laneId = LS->searchLane(s,t);
    }


}

void ReferencePosition::update(double dx, double dy, double dz, bool dummy)
{
    xyz[0] +=dx;
    xyz[1] +=dy;
    xyz[2] +=dz;

    //xyz = osg::Vec3(x,y,z);

    //Vector2D searchHere = Vector2D(x,y); road->isOnRoad(searchHere);
    Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
    Vector2D stNew = road->searchPosition(newPoint, s);

    s = stNew[0];
    t = stNew[1];

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    laneId = LS->searchLane(s,t);

}

void ReferencePosition::getSuccessor()
{
    TarmacConnection* connection;

    connection = road->getSuccessorConnection();
    road = dynamic_cast<Road *>(connection->getConnectingTarmac());

    s = s-roadLength;
    roadId = system->getRoadId(road);
    roadLength = road->getLength();
}

void ReferencePosition::getPredecessor()
{
    TarmacConnection* connection;

    connection = road->getPredecessorConnection();
    road = dynamic_cast<Road *>(connection->getConnectingTarmac());

    roadId = system->getRoadId(road);
    roadLength = road->getLength();
    s = roadLength+s;
}
