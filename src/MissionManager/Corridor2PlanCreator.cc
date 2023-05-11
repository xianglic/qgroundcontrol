/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "Corridor2PlanCreator.h"
#include "PlanMasterController.h"
#include "MissionSettingsItem.h"
#include "Corridor2.h"
Corridor2PlanCreator::Corridor2PlanCreator(PlanMasterController* planMasterController, QObject* parent)
    : PlanCreator(planMasterController, Corridor2::name, QStringLiteral("/qmlimages/PlanCreator/CorridorScanPlanCreator.png"), parent)
{

}

void Corridor2PlanCreator::createPlan(const QGeoCoordinate& mapCenterCoord)
{
    _planMasterController->removeAll();
    VisualMissionItem* takeoffItem = _missionController->insertTakeoffItem(mapCenterCoord, -1);
    _missionController->insertComplexMissionItem(Corridor2::name, mapCenterCoord, -1);
    _missionController->insertLandItem(mapCenterCoord, -1);
    _missionController->setCurrentPlanViewSeqNum(takeoffItem->sequenceNumber(), true);
}
