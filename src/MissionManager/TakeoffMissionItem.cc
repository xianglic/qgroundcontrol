/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include <QStringList>
#include <QDebug>

#include "TakeoffMissionItem.h"
#include "FirmwarePluginManager.h"
#include "QGCApplication.h"
#include "JsonHelper.h"
#include "MissionCommandTree.h"
#include "MissionCommandUIInfo.h"
#include "QGroundControlQmlGlobal.h"
#include "SettingsManager.h"
#include "PlanMasterController.h"


// detect task
const char* TakeoffMissionItem::detectTask_gimbal_pitchName           = "DetectTask_gimbal_pitch";
const char* TakeoffMissionItem::detectTask_drone_rotationName         = "DetectTask_drone_rotation";
const char* TakeoffMissionItem::detectTask_sample_rateName            = "DetectTask_sample_rate";
const char* TakeoffMissionItem::detectTask_hover_delayName            = "DetectTask_hover_delay";

// obstacle task
const char* TakeoffMissionItem::obstacleTask_speedName                = "ObstacleTask_speed";
const char* TakeoffMissionItem::obstacleTask_altitudeName             = "ObstacleTask_altitude";
const char* TakeoffMissionItem::obstacleTask_modelName                = "ObstacleTask_model";


// tracking task
const char* TakeoffMissionItem::trackingTask_gimbal_pitchName         = "TrackingTask_gimbal_pitch";
const char* TakeoffMissionItem::trackingTask_className                = "TrackingTask_class";

TakeoffMissionItem::TakeoffMissionItem(PlanMasterController* masterController, bool flyView, MissionSettingsItem* settingsItem, bool forLoad)
    : SimpleMissionItem (masterController, flyView, forLoad)
    , _settingsItem     (settingsItem)
{
    _init(forLoad);

    _initCustomizedFields();
}

TakeoffMissionItem::TakeoffMissionItem(MAV_CMD takeoffCmd, PlanMasterController* masterController, bool flyView, MissionSettingsItem* settingsItem, bool forLoad)
    : SimpleMissionItem (masterController, flyView, false /* forLoad */)
    , _settingsItem     (settingsItem)
    , _steelEagleMode   (masterController->missionController()->globalSteelEagleModeDefault())// detect task

{
    setCommand(takeoffCmd);
    _init(forLoad);

    _initCustomizedFields();

}

TakeoffMissionItem::TakeoffMissionItem(const MissionItem& missionItem, PlanMasterController* masterController, bool flyView, MissionSettingsItem* settingsItem, bool forLoad)
    : SimpleMissionItem (masterController, flyView, missionItem)
    , _settingsItem     (settingsItem)
{
    _init(forLoad);

    _initCustomizedFields();
}

TakeoffMissionItem::~TakeoffMissionItem()
{

}

void TakeoffMissionItem::_initCustomizedFields (){
    
    // detect task
    FactMetaData md1 = FactMetaData(FactMetaData::valueTypeDouble);
    md1.setRawMax(90.0);
    md1.setRawMin(-90.0);
    md1.setRawDefaultValue(-45.0);
    md1.setDecimalPlaces(1);
    md1.setRawUnits("deg");

    FactMetaData md2 = FactMetaData(FactMetaData::valueTypeDouble);
    md2.setRawMax(360.0);
    md2.setRawMin(0.0);
    md2.setRawDefaultValue(0.0);
    md2.setDecimalPlaces(1);
    md2.setRawUnits("deg");

    FactMetaData md3 = FactMetaData(FactMetaData::valueTypeUint32);
    md3.setRawMax(30);
    md3.setRawMin(1);
    md3.setRawDefaultValue(2);
    md3.setRawUnits("#fr/sec");


    FactMetaData md4 = FactMetaData(FactMetaData::valueTypeUint32);
    md4.setRawMax(10);
    md4.setRawMin(1);
    md4.setRawDefaultValue(5);
    md4.setRawUnits("sec");

    // Build the brand list from known model for detect tasks
    _detectTask_modellist.append("coco");
    _detectTask_modellist.append("oidv4");

    _detectTask_gimbal_pitchFact    = Fact("Gimbal Pitch",             &md1);
    _detectTask_drone_rotationFact   =Fact("Drone Rotation",             &md2);
    _detectTask_sample_rateFact      = Fact("Sample Rate",             &md3);
    _detectTask_hover_delayFact       = Fact("Hover Delay",             &md4);


    // obstacle task
    FactMetaData md5 = FactMetaData(FactMetaData::valueTypeDouble);
    md5.setRawDefaultValue(5.0);
    md5.setDecimalPlaces(1);

    FactMetaData md6 = FactMetaData(FactMetaData::valueTypeDouble);
    md6.setRawDefaultValue(qQNaN());
    md6.setDecimalPlaces(1);
    

    _obstacleTask_speedFact           = Fact("Speed",             &md5); // obstacle task
    _obstacleTask_altitudeFact        = Fact("Altitude",             &md6);
    _obstacleTask_modelFact           =Fact(0, "Model",             FactMetaData::valueTypeString);
    _obstacleTask_modelFact.setRawValue("DPT_Large");



    // tracking task
    FactMetaData md7 = FactMetaData(FactMetaData::valueTypeDouble);
    md7.setRawMax(90.0);
    md7.setRawMin(-90.0);
    md7.setRawDefaultValue(-30.0);
    md7.setRawUnits("deg");
    md7.setDecimalPlaces(1);

    _trackingTask_gimbal_pitchFact    =Fact("Gimbal Pitch",             &md7);// tracking task
    _trackingTask_classFact           =Fact(0, "Class",             FactMetaData::valueTypeString);
    _trackingTask_classFact.setRawValue("Person");


    // Build the brand list from known model for tracking tasks
    _trackingTask_modellist.append("coco");
    _trackingTask_modellist.append("robomaster");
}

void TakeoffMissionItem::_init(bool forLoad)
{
    _editorQml = QStringLiteral("qrc:/qml/TakeoffMissionItemEditor.qml");

    connect(_settingsItem, &MissionSettingsItem::coordinateChanged, this, &TakeoffMissionItem::launchCoordinateChanged);

    if (_flyView) {
        _initLaunchTakeoffAtSameLocation();
        return;
    }

    QGeoCoordinate homePosition = _settingsItem->coordinate();
    if (!homePosition.isValid()) {
        Vehicle* activeVehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
        if (activeVehicle) {
            homePosition = activeVehicle->homePosition();
            if (homePosition.isValid()) {
                _settingsItem->setCoordinate(homePosition);
            }
        }
    }

    if (forLoad) {
        // Load routines will set the rest up after load
        return;
    }

    _initLaunchTakeoffAtSameLocation();
    if (_launchTakeoffAtSameLocation && homePosition.isValid()) {
        SimpleMissionItem::setCoordinate(homePosition);
    }

    // Wizard mode is set if:
    //  - Launch position is missing - requires prompt to user to click to set launch
    //  - Fixed wing - warn about climb out position adjustment
    if (!homePosition.isValid() || _controllerVehicle->fixedWing()) {
        _wizardMode = true;
    }

    setDirty(false);
}

void TakeoffMissionItem::setLaunchTakeoffAtSameLocation(bool launchTakeoffAtSameLocation)
{
    if (launchTakeoffAtSameLocation != _launchTakeoffAtSameLocation) {
        _launchTakeoffAtSameLocation = launchTakeoffAtSameLocation;
        if (_launchTakeoffAtSameLocation) {
            setLaunchCoordinate(coordinate());
        }
        emit launchTakeoffAtSameLocationChanged(_launchTakeoffAtSameLocation);
        setDirty(true);
    }
}

void TakeoffMissionItem::setCoordinate(const QGeoCoordinate& coordinate)
{
    if (coordinate != this->coordinate()) {
        SimpleMissionItem::setCoordinate(coordinate);
        if (_launchTakeoffAtSameLocation) {
            _settingsItem->setCoordinate(coordinate);
        }
    }
}

bool TakeoffMissionItem::isTakeoffCommand(MAV_CMD command)
{
    return qgcApp()->toolbox()->missionCommandTree()->isTakeoffCommand(command);
}

void TakeoffMissionItem::_initLaunchTakeoffAtSameLocation(void)
{
    if (specifiesCoordinate()) {
        if (_controllerVehicle->fixedWing() || _controllerVehicle->vtol()) {
            setLaunchTakeoffAtSameLocation(false);
        } else {
            // PX4 specifies a coordinate for takeoff even for multi-rotor. But it makes more sense to not have a coordinate
            // from and end user standpoint. So even for PX4 we try to keep launch and takeoff at the same position. Unless the
            // user has moved/loaded launch at a different location than takeoff.
            if (coordinate().isValid() && _settingsItem->coordinate().isValid()) {
                setLaunchTakeoffAtSameLocation(coordinate().latitude() == _settingsItem->coordinate().latitude() && coordinate().longitude() == _settingsItem->coordinate().longitude());
            } else {
                setLaunchTakeoffAtSameLocation(true);
            }

        }
    } else {
        setLaunchTakeoffAtSameLocation(true);
    }
}

bool TakeoffMissionItem::load(QTextStream &loadStream)
{
    bool success = SimpleMissionItem::load(loadStream);
    if (success) {
        _initLaunchTakeoffAtSameLocation();
    }
    _wizardMode = false; // Always be off for loaded items
    return success;
}

bool TakeoffMissionItem::load(const QJsonObject& json, int sequenceNumber, QString& errorString)
{
    bool success = SimpleMissionItem::load(json, sequenceNumber, errorString);
    if (success) {
        _initLaunchTakeoffAtSameLocation();
    }
    _wizardMode = false; // Always be off for loaded items
    return success;
}

void TakeoffMissionItem::setLaunchCoordinate(const QGeoCoordinate& launchCoordinate)
{
    if (!launchCoordinate.isValid()) {
        return;
    }

    _settingsItem->setCoordinate(launchCoordinate);

    if (!coordinate().isValid()) {
        QGeoCoordinate takeoffCoordinate;
        if (_launchTakeoffAtSameLocation) {
            takeoffCoordinate = launchCoordinate;
        } else {
            double distance = qgcApp()->toolbox()->settingsManager()->planViewSettings()->vtolTransitionDistance()->rawValue().toDouble(); // Default distance is VTOL transition to takeoff point distance
            if (_controllerVehicle->fixedWing()) {
                double altitude = this->altitude()->rawValue().toDouble();

                if (altitudeMode() == QGroundControlQmlGlobal::AltitudeModeRelative) {
                    // Offset for fixed wing climb out of 30 degrees to specified altitude
                    if (altitude != 0.0) {
                        distance = altitude / tan(qDegreesToRadians(30.0));
                    }
                } else {
                    distance = altitude * 1.5;
                }
            }
            takeoffCoordinate = launchCoordinate.atDistanceAndAzimuth(distance, 0);
        }
        SimpleMissionItem::setCoordinate(takeoffCoordinate);
    }
}


void TakeoffMissionItem::setSteelEagleMode(QGroundControlQmlGlobal::SteelEagleMode seMode)
{
    if (seMode != _steelEagleMode) {
        _steelEagleMode = seMode;
        // std::cout<< "hi test, setting the mode: "<< seMode;
        emit steelEagleModeChanged(_steelEagleMode);
    }
}


void TakeoffMissionItem::setDetectTask_model(const QString& detectTask_model)
{
    if (detectTask_model != _detectTask_model_2) {
        _detectTask_model_2 = detectTask_model;
    }
}

void TakeoffMissionItem::setTrackingTask_model(const QString& trackingTask_model)
{
    if (trackingTask_model != _trackingTask_model_2) {
        _trackingTask_model_2 = trackingTask_model;
    }
}