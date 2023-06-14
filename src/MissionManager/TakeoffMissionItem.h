/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "SimpleMissionItem.h"
#include "MissionSettingsItem.h"

class PlanMasterController;

/// Takeoff mission item is a special case of a SimpleMissionItem which supports Launch Location display/editing
/// which is tied to home position.
class TakeoffMissionItem : public SimpleMissionItem
{
    Q_OBJECT
    
public:
    // Note: forLoad = true indicates that TakeoffMissionItem::load will be called onthe item
    TakeoffMissionItem(PlanMasterController* masterController, bool flyView, MissionSettingsItem* settingsItem, bool forLoad);
    TakeoffMissionItem(MAV_CMD takeoffCmd, PlanMasterController* masterController, bool flyView, MissionSettingsItem* settingsItem, bool forLoad);
    TakeoffMissionItem(const MissionItem& missionItem,  PlanMasterController* masterController, bool flyView, MissionSettingsItem* settingsItem, bool forLoad);

    Q_PROPERTY(QGeoCoordinate   launchCoordinate            READ launchCoordinate               WRITE setLaunchCoordinate               NOTIFY launchCoordinateChanged)
    Q_PROPERTY(bool             launchTakeoffAtSameLocation READ launchTakeoffAtSameLocation    WRITE setLaunchTakeoffAtSameLocation    NOTIFY launchTakeoffAtSameLocationChanged)
    

    QGeoCoordinate  launchCoordinate            (void) const { return _settingsItem->coordinate(); }
    bool            launchTakeoffAtSameLocation (void) const { return _launchTakeoffAtSameLocation; }

    void setLaunchCoordinate            (const QGeoCoordinate& launchCoordinate);
    void setLaunchTakeoffAtSameLocation (bool launchTakeoffAtSameLocation);

    static bool isTakeoffCommand(MAV_CMD command);

    ~TakeoffMissionItem();

    // Overrides from VisualMissionItem
    void            setCoordinate           (const QGeoCoordinate& coordinate) override;
    bool            isTakeoffItem           (void) const final { return true; }
    double          specifiedFlightSpeed    (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    double          specifiedGimbalYaw      (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    double          specifiedGimbalPitch    (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    QString         mapVisualQML            (void) const override { return QStringLiteral("TakeoffItemMapVisual.qml"); }

    // Overrides from SimpleMissionItem
    bool load(QTextStream &loadStream) final;
    bool load(const QJsonObject& json, int sequenceNumber, QString& errorString) final;

    // void setDirty(bool dirty) final;

    // Steel Eagle Mode
    Q_PROPERTY(QGroundControlQmlGlobal::SteelEagleMode seMode READ steelEagleMode WRITE setSteelEagleMode NOTIFY steelEagleModeChanged)
    QGroundControlQmlGlobal::SteelEagleMode steelEagleMode(void) const { return _steelEagleMode; }
    void setSteelEagleMode               (QGroundControlQmlGlobal::SteelEagleMode seMode);

    // detect task
    Q_PROPERTY(Fact*            detectTask_gimbal_pitch     READ detectTask_gimbal_pitch                            CONSTANT)
    Q_PROPERTY(Fact*            detectTask_drone_rotation   READ detectTask_drone_rotation                          CONSTANT)
    Q_PROPERTY(Fact*            detectTask_sample_rate      READ detectTask_sample_rate                             CONSTANT) 
    Q_PROPERTY(Fact*            detectTask_hover_delay      READ detectTask_hover_delay                             CONSTANT)
    Q_PROPERTY(QStringList      detectTask_modellist        MEMBER _detectTask_modellist                            CONSTANT)
    Q_PROPERTY(QString          detectTask_model_2          MEMBER _detectTask_model_2                              WRITE setDetectTask_model)
    
    // obstacle task
    Q_PROPERTY(Fact*            obstacleTask_speed          READ obstacleTask_speed                                 CONSTANT)
    Q_PROPERTY(Fact*            obstacleTask_altitude       READ obstacleTask_altitude                              CONSTANT)

    // tracking task 
    Q_PROPERTY(Fact*            trackingTask_gimbal_pitch   READ trackingTask_gimbal_pitch                          CONSTANT)
    Q_PROPERTY(Fact*            trackingTask_class          READ trackingTask_class                                 CONSTANT)
    Q_PROPERTY(QStringList      trackingTask_modellist        MEMBER _trackingTask_modellist                        CONSTANT)
    Q_PROPERTY(QString          trackingTask_model_2          MEMBER _trackingTask_model_2                          WRITE setTrackingTask_model)
    
    //detect task
    Fact* detectTask_gimbal_pitch       (void) { return &_detectTask_gimbal_pitchFact; }
    Fact* detectTask_drone_rotation     (void) {return &_detectTask_drone_rotationFact;}
    Fact* detectTask_sample_rate        (void) {return &_detectTask_sample_rateFact;} 
    Fact* detectTask_hover_delay        (void) {return &_detectTask_hover_delayFact;}

    void  setDetectTask_model           (const QString& detectTask_model);
    QString  detectTask_model_2        (void) {return _detectTask_model_2;}
        
    // obstacle task
    Fact* obstacleTask_speed            (void) { return &_obstacleTask_speedFact; }
    Fact* obstacleTask_altitude         (void) {return &_obstacleTask_altitudeFact;}


    // tracking task
    Fact* trackingTask_gimbal_pitch     (void) { return &_trackingTask_gimbal_pitchFact; }
    Fact* trackingTask_class            (void) {return &_trackingTask_classFact;}

    void  setTrackingTask_model          (const QString& trackingTask_model);
    QString  trackingTask_model_2        (void) {return _trackingTask_model_2;}



signals:
    void launchCoordinateChanged            (const QGeoCoordinate& launchCoordinate);
    void launchTakeoffAtSameLocationChanged (bool launchTakeoffAtSameLocation);
    void steelEagleModeChanged      (int seMode);


private:
    void _init(bool forLoad);
    void _initLaunchTakeoffAtSameLocation(void);

    MissionSettingsItem*    _settingsItem;
    bool                    _launchTakeoffAtSameLocation = true;



    QGroundControlQmlGlobal::SteelEagleMode    _steelEagleMode               = QGroundControlQmlGlobal::DetectTask;
    // detect task
    Fact _detectTask_gimbal_pitchFact;
    Fact _detectTask_drone_rotationFact;
    Fact _detectTask_sample_rateFact;
    Fact _detectTask_hover_delayFact;
    QStringList  _detectTask_modellist;
    QString      _detectTask_model_2 = "coco";
    
    // obstacle task
    Fact _obstacleTask_speedFact;
    Fact _obstacleTask_altitudeFact; 

    // tracking task
    Fact _trackingTask_gimbal_pitchFact;
    Fact _trackingTask_classFact;
    QStringList  _trackingTask_modellist;
    QString      _trackingTask_model_2 = "coco";



    //detect task
    static const char* detectTask_gimbal_pitchName;
    static const char* detectTask_drone_rotationName;
    static const char* detectTask_sample_rateName;
    static const char* detectTask_hover_delayName;

    // obstacle task
    static const char* obstacleTask_speedName;
    static const char* obstacleTask_altitudeName;  


    // tracking task
    static const char* trackingTask_gimbal_pitchName;
    static const char* trackingTask_className;  

};
